classdef AutomateMask < handle
% AutomateMask accelerates high level mask operations like renaming and
% remasking

% Copyright 2021 Bose Corporation

%#ok<*AGROW>

methods (Static, Access=public)
    %----------------------------------------------------------------------
    function SwitchToUseClassdef(block)
        % SwitchToUseClassdef Destroy mask and recreate with classdef.
        % This routine is used internally for other processes, but under
        % specific circumstances it may be useful to use directly.
        % Usage:
        %    SwitchToUseClassdef(block)
        % See also: AutomateMask.CreateClassdefBlockAndWrapInsideLegacy
        if (nargin<1)
            block = gcb;
        end
        fprintf('Adjusting block [%s]\n',block);
        this = AutomateMask(block);
        
        this.InternalSwitchToUseClassdef();
    end
    %----------------------------------------------------------------------
    function RenameParameters(block)
        % RenameParameters Accelerates the effort involved in renaming mask
        % parameter(s) for a library block that is already release and
        % being used. This routine adds an entry in the library's
        % forwarding table to maintain backward compatibility.
        % Make sure to add the newly generated transform function file to
        % source control! It is created in the same folder as the library.
        % Usage:
        %    AutomateMask.RenameParameters(block)
        if (nargin<1)
            block = gcb;
        end
        fprintf('Adjusting block [%s]\n',block);
        this = AutomateMask(block);
        
        % get names, but not "Config" mode:
        if (false == this.GetNewNames(AutomateMask.renameInMask))
            % No changes - bail out
            disp('No changes requested.');
            return;
        end
        
        % ----- Change the actual mask parameter names themselves
        for nIdx=1:numel(this.oldNames)
            maskParam = this.theMask.getParameter(this.oldNames{nIdx});
            maskParam.Name = this.newNames{nIdx};
        end

        % ----- Perform search/replace in lots of other places
        this.ReplaceCallbacks();
        this.ReplaceSubBlocks();
        this.ReplaceInitCode();
        this.ReplaceDisplayCode();       
        
        % Go ahead and ensure that the library's window shows up now with the block
        open_system(this.modelName);
        hilite_system(this.blockPath,'none');

        % ----- Create a transform function
        functionName = this.CreateRenameTransformFunction();

        % ----- Update the library's forwarding table
        this.AddForwardingTableFunction(functionName);

        % ----- Dump out and display the difference report
        this.WriteReportFile(strrep(this.blockPath,'/','_'));
    end
    %----------------------------------------------------------------------
    function RenamePropertiesOfClassdef(block, maskParameterName)
        % RenamePropertiesOfClassdef Accelerates the effort involved in
        % renaming classdef properties for the classdef associated with the
        % specified mask parameter, or "Config" if not specified.
        % Usage:
        %    AutomateMask.RenamePropertiesOfClassdef(block)
        %    AutomateMask.RenamePropertiesOfClassdef(block, maskParameterName)
        if (nargin<1)
            block = gcb;
        end

        % Use Diagnostic Viewer for status:
        myStage = sldiagviewer.createStage('TalariaAutomation','ModelName',bdroot(block));
        sldiagviewer.reportInfo(sprintf('Adjusting block [%s]\n',block));
        this = AutomateMask(block);
        
        % Determine the name of the classdef
        if (nargin < 2)
            maskParameterName = 'Config';
        end
        this.classdefMaskParamName = maskParameterName;

        % Before proceeding, ensure this isn't a "locked" library link
        linkStatus = get_param(block,'LinkStatus');
        switch(linkStatus)
            case "none"
                % Good to go - this block has sovereignty
            case "inactive"
                warning('****** Selected block is a disabled link. Any changes made to the classdef WILL impact the library block ''%s'' as well, but without associate modelling changes!', ...
                    get_param(block,'AncestorBlock'));
            case "resolved"
                % Active link - suggest the one that should change
                error('Selected block is a library link. Try again with ''%s''', ...
                    get_param(block,'ReferenceBlock'));
            case "implicit"
                % Inside an active link - suggest the one that could change
                error('Selected block is inside a library link. Try again inside ''%s''', ...
                    MaskUtil.FindAncestorLibRef(block));
            otherwise
                % This is likely "unresolved" which means the library block
                % isn't found anyway. Expected that the search for a mask
                % will fail well before this case could be hit.
                error('Unresolved library link to ''%s''', ...
                    get_param(block,'SourceBlock'));
        end
        
        % get names in the "rename classdef properties" mode:
        if (false == this.GetNewNames(AutomateMask.renameInClassdef))
            % No changes - bail out
            sldiagviewer.reportInfo('No changes requested.');
            return;
        end
        
        % Unknown why the editor automatically SAVES on click away. That
        % kinda ruins our plan to make changes without saving. Change it!
        %s = settings;
        %s.matlab.editor.SaveFilesOnClickAway.TemporaryValue = 0;
        
        % ----- Perform search/replace in lots of places
        this.ReplaceCallbacks();
        this.ReplaceSubBlocks();
        this.ReplaceInitCode();
        this.ReplaceDisplayCode();       
        this.ReplaceInClassdef();
        
        % ----- Create the backwards-compatibility classdef properties
        this.CreatePropertyRenameCompatibility();
        
        % Go ahead and ensure that the library's window shows up now with the block
        open_system(this.modelName);
        hilite_system(this.blockPath,'none');

        % ----- Dump out and display the difference report
        this.WriteReportFile(strrep(this.blockPath,'/','_'));
        
        % Leave the "do not automatically save" behavior on for now.
        % Otherwise, we could clear it with this command:
        %clearTemporaryValue(s.matlab.editor.SaveFilesOnClickAway);
    end
    %----------------------------------------------------------------------
    function RenameLibraryBlock(blockPaths, newBlockNames)
        % RenameLibraryBlock Renames a library block including forwarding
        % table entries. Renaming only within the library is supported.
        % In other words, this automation does not support redirecting a
        % library block link into a different library diagram (slx file).
        % If no new name is specified in the call, a dialog box pops up to
        % request the new name.        
        % Usage:
        %    AutomateMask.RenameLibraryBlock(blockPath, [newBlockName])
        
        if (nargin<1 || isempty(blockPaths))
            % Need one or more blocks to start. This one does not
            % automatically select gcb anymore
            error('Aborted. No block paths specified')
        end
        if (nargin<2 || isempty(newBlockNames))
            newBlockNames = [];
        end
        blockPathCell = MaskUtil.MakeCell(getfullname(blockPaths));

        % Ensure all blocks are in the same diagram, which is a library
        theDiagram = unique(bdroot(blockPathCell));
        if (numel(theDiagram) > 1)
            error('Aborted. All blocks must be in the same library');
        end
        theDiagram = theDiagram{1};
        if (false == bdIsLibrary(theDiagram))
            error('Aborted. Diagram [%s] is not a library',theDiagram);
        end

        % Now that a model name is known, create diagnostic viewer stage to
        % receive any additional messsages:
        myStage = sldiagviewer.createStage('RenameLibraryBlock','ModelName', ...
            theDiagram); %#ok<NASGU>

        % ... and an AutomateMask object to hold information
        this = AutomateMask(theDiagram,false,true);

        %  Figure out new names
        if (isempty(newBlockNames))
            % No new names specified - prompt for them
            oldNamesCell = MaskUtil.MakeCell(get_param(blockPathCell,'Name'));
            input = {};
            MaskUtil.ReportInfo('Creating prompt dialog      ...');
            input = this.NamesDialog(input, oldNamesCell, ...
                'Choose new name(s) for library block(s)');
            if (isempty(input))
                MaskUtil.ReportInfo('ABORTED');
                return;
            end            
            newBlockNamesCell = MaskUtil.MakeCell(input);
        else
            % Name(s) are specified- Ensure their count matches with blocks
            newBlockNamesCell = MaskUtil.MakeCell(newBlockNames);
            if (numel(blockPathCell) ~= numel(newBlockNamesCell))
                error(['Provided new block names must either be empty, ' ...
                    'or match the number of block paths']);
            end
        end


        % Ready to proceed - perform the renames:
        for i=1:numel(blockPathCell)
            this.blockPath = blockPathCell{i};
            oldBlockName = get_param(this.blockPath,'Name');
            newBlockName = newBlockNamesCell{i};
            
            % Check for no change            
            if (isempty(newBlockName) || strcmp(newBlockName,oldBlockName))
                MaskUtil.ReportInfo('No change in name for [%s]',this.blockPath);
                continue;
            end
            MaskUtil.ReportInfo('Adjusting block [%s]\n',this.blockPath);
            
            % Validate block name
            [validName,cleanName,newErrs] = MaskUtil.ExtractValidName(newBlockName,'new name');
            if (isempty(validName) || false == strcmp(newBlockName,validName))
                MaskUtil.ReportError('Invalid name [%s] specified [%s]. Without invalid characters:[%s]', ...
                    newBlockName,strjoin(newErrs),cleanName);
                % This one failed... but perhaps there are more
                continue;
            end
    
            % Update the forwarding table
            this.AddForwardingTableBlockNameChange(newBlockName);
            
            % Rename the block
            handle = getSimulinkBlockHandle(this.blockPath);
            set_param(this.blockPath, 'Name', newBlockName);
            this.blockPath = getfullname(handle);
            
            % Go ahead and ensure that the library's window shows up now with the block
            open_system(this.modelName);
            hilite_system(this.blockPath,'none');
            MaskUtil.ReportInfo('Rename complete [%s]-->[%s]\n', ...
                oldBlockName,newBlockName);
        end
        MaskUtil.ReportInfo('Save library if everything is good.\n');
    end
    %----------------------------------------------------------------------
    function CreateClassdefBlockAndWrapInsideLegacy(block, newBlockPath)
        % CreateClassdefBlockAndWrapInsideLegacy Creates a new library
        % block that uses classdef-based configuration. Also hollows-out
        % the specified legacy block to contain only a link to the new
        % classdef-based block. New classdef-based block has the same name
        % but is located in a new library, enabling re-use of the block
        % name. Make sure to add the newly created library file to source
        % control. It is found in the same folder as the original library.
        % If no new block path is specified, WikiName versions of the
        % current library file and block name will be automatically used.
        % Usage:
        %    AutomateMask.CreateClassdefBlockAndWrapInsideLegacy(block,newBlockPath)
        % (note that all arguments are optional - will just use gcb)
        if (nargin<1)
            block = gcb;
        end
        fprintf('Adjusting block [%s]\n',block);
        this = AutomateMask(block);
        
        legacyBlockPath = this.blockPath;
        legacyBlockName = get_param(block,'Name');

        % Get the new library and block
        if (nargin < 2)
            % Repair the new block name if it's not class-ready
            newBlockName = CreatePascalName(legacyBlockName);
            newLibraryName = CreatePascalName(this.libObj.Name);
            newBlockPath = sprintf("%sBtc/%s",newLibraryName,newBlockName);
        end
        
        [newLibraryName,newBlockName] = fileparts(newBlockPath);
        
        % Create a new copy of the block to become the "classdef" version
        classdefBlockH = this.CopyBlockToLibrary(legacyBlockPath, newLibraryName);
        % Ensure the new name is in place
        set(classdefBlockH,'Name',newBlockName);
        
        classdefBlockPath = getfullname(classdefBlockH);
        
        % Redirect focus on new block just to remask it with classdef
        this.blockPath = classdefBlockPath;
        this.theMask = Simulink.Mask.get(this.blockPath);
        this.InternalSwitchToUseClassdef();
        
        % Re-focus on the legacy block
        this.blockPath = legacyBlockPath;
        this.theMask = Simulink.Mask.get(legacyBlockPath);
        
        %  ... Create initialization code to copy legacy mask parameters
        %  into an instance of the classdef
        this.UpdateInitCodeToCopyDiscreteToObject();
        
        % ... Unpromote any mask parameters in the legacy mask which are
        % being converted to classdef-based properties
        this.ConvertPromoteToNormal();
        
        %  ... Replace content of legacy copy to be a link to the original
        %  block with the configuration set to the Config.
        Simulink.SubSystem.deleteContents(legacyBlockPath);
        innerLinkH = add_block(classdefBlockPath,[legacyBlockPath '/InnerLink']);
        set_param(innerLinkH,'Config','InnerLinkConfig');
        
        % Connect surface all the ports of the inner block up to subsystem
        this.ConnectAllPorts(innerLinkH);
        
        %  ... Promote parameters from classdef block up into legacy mask
        this.ReplaceDesignTimeParametersWithPromotion(innerLinkH);
    end

    %----------------------------------------------------------------------
    function ManageMfbParameterMask(block, cleanNonParams)
        % ManageMfbParameterMask will create or update a block mask for a
        % MATLAB Function Block (MFB) to have a mask-parameter that matches
        % each MFB function parameter. In addition, if the function script
        % itself has a comment in the form %[Param:Name], a parameter of
        % the specified "Name" will be added to both the MFB and the mask.
        % An optional second argument, when true, will remove any mask
        % parameters which are not registered as MFB parameters.
        % (False by default: no deletions)
        % Usage:
        %    AutomateMask.ManageMfbParameterMask(blockPath,cleanNonParams);
        if (nargin<1)
            block = gcb;
        end

        % Use Diagnostic Viewer for status:
        myStage = sldiagviewer.createStage('TalariaAutomation','ModelName',bdroot(block));
        sldiagviewer.reportInfo(sprintf('Adjusting block [%s]\n',block));

        % First attempt to retrieve the MFB's chart object. Get fancy and
        % handle running this from inside a library linked block!
        reference = get_param(block,'ReferenceBlock');
        if (~isempty(reference))
            path = reference;
        else
            path = block;
        end        
        sf = sfroot();
        theMfb = sf.find('-isa','Stateflow.EMChart','-regexp','Path',path);
        if (isempty(theMfb))
            error('Block [%s] does not appear to be a MATLAB Function Block', ...
                block);
        end

        % Do not support directly modifying from a locked library link
        if (~isempty(reference))
            error('Selected block [%s] is linked to a library. Try again with ''%s''\n', ...
                block,reference);
        end

        % This method is designed to add a mask if one is not already
        % present:
        if (isempty(Simulink.Mask.get(block)))
            % Add a mask!
            MaskUtil.ReportInfo('Adding mask to [%s]\n',block);
            Simulink.Mask.create(block);
        end
        this = AutomateMask(block,true); % modifications OK
        if (nargin<2)
            cleanNonParams = false;
        end

        % Source #1 Discover [Param:Name] annotations in the script code
        % Parse the script itself and look for special parameter comments
        % in the form %[Param:Name]
        theScript = theMfb.Script;
        pattern = '%\s*\[\s*Param\s*:\s*(\w*)\s*\]';
        scriptParamsCellCell = regexp(theScript,pattern,'tokens');
        scriptParams = string([scriptParamsCellCell{:}]);
        scriptParams = unique(scriptParams);
        numScriptParams = numel(scriptParams);
        MaskUtil.ReportInfo('Found %d unique script %%[Param:Name] entries\n', ...
            numScriptParams);
        if (false == isempty(scriptParams))
            MaskUtil.ReportInfo('   %%[Param:{%s}]\n', scriptParams.join(','));
        end

        % Souce #2 Discover MFB Parameters already registered to Ports&Data
        args = theMfb.getChildren();
        mfbParams = findobj(args,'Scope','Parameter');
        numMfbParams = numel(mfbParams);
        mfbParamNames = string(get(mfbParams,'Name'));
        MaskUtil.ReportInfo('Found %d MFB parameters: %s\n', ...
            numMfbParams,mfbParamNames.join(','));

        % Propagate script-annotated parameters into the Ports&Data Dialog
        addedSomeParameters = false;
        for i=1:numel(scriptParams)
            % Check if parameter is already present in MFB registrations
            aName = scriptParams(i);
            foundParam = findobj(mfbParams,'Name',aName);
            if (isempty(foundParam))
                % Not found! Add it:    
                addedSomeParameters = true;
                MaskUtil.ReportInfo('...Adding [%s] to MFB as parameter\n', aName);
                newElem = Stateflow.Data(theMfb);
                newElem.Name = aName;
                newElem.Scope = "Parameter";
                newElem.Tunable = 0;
            end
        end

        % Refresh our discovery of MFB parameters after possible additions
        if (addedSomeParameters)
            args = theMfb.getChildren();
            mfbParams = findobj(args,'Scope','Parameter');
            numMfbParams = numel(mfbParams);
            mfbParamNames = string(get(mfbParams,'Name'));
            MaskUtil.ReportInfo('Now there are %d MFB parameters: %s\n', ...
                numMfbParams,mfbParamNames.join(','));
        end
        if (0 == numMfbParams)
            MaskUtil.ReportInfo('No MFB parameters found\n');
        end
        
        % TODO - it would be grand to detect other parameters used by the
        % MFB in the ports&data dialog such as expressions for dimensions
        % for input and output signals and such. The strings are available
        % for these, but they are unparsed in terms of parameter usage -
        % and so more difficult to reliably detect BTC variables
        % e.g. theMfb.Inputs(1).Props.Array.Size
        
        % Next get the mask and ensure all parameters are present there
        maskParams = this.theMask.Parameters;
        if (isempty(maskParams))
            MaskUtil.ReportInfo('No mask parameters found yet\n');
        else
            maskParamNames = string({maskParams.Name});
            MaskUtil.ReportInfo('Found %d mask parameters: %s\n', ...
                numel(maskParamNames),maskParamNames.join(','));
        end
        
        % Add any missing parameters to the mask.
        for i=1:numMfbParams
            scriptParam = mfbParams(i);
            theName = scriptParam.Name;
            if (scriptParam.Tunable)
                MaskUtil.ReportInfo('MFB param [%s] was tunable - turning off\n',theName);
                scriptParam.Tunable = 0;
            end
            maskParam = findobj(maskParams,'Name',theName);
            if (isempty(maskParam))
                MaskUtil.ReportInfo('...Adding [%s] as mask parameter\n',theName);
                p = this.theMask.addParameter('Type','edit','Name',theName, ...
                    'Tunable','off', ...
                    'Prompt',theName, ...
                    'Value',['Config.' theName], ...
                    'Evaluate', 'on', ...
                    'Container', 'ParameterGroupVar');
                p.DialogControl.PromptLocation = 'left';
            end
        end
        
        % If so empowered, destroy any mask parameters without matching MFB
        % parameters. Find the names to delete first, then delete
        % afterwards by name to avoid modifying the list as we iterate
        % through it. Deleting a mask parameter causes parameter lists to
        % become invalid;
        maskParams = this.theMask.Parameters;
        numMaskParams = numel(maskParams);
        deadMaskParamNames = {};
        for i=1:numMaskParams
           maskParam = maskParams(i);
           theName = maskParam.Name;
           scriptParam = findobj(mfbParams,'Name',theName);
           if (isempty(scriptParam))
               deadMaskParamNames{end+1} = theName;
           end
        end
        % Now perform deletions / print notifications
        resetPromptLocations = false;
        for i=1:numel(deadMaskParamNames)
           theName = deadMaskParamNames{i};
            if (cleanNonParams)
                % Sadly, removing a mask parameter RESETS all the survivors
                % to have prompt location of TOP (and perhaps other things)
                MaskUtil.ReportInfo('Deleting mask parameter [%s]\n',theName);
                this.theMask.removeParameter(theName);
                resetPromptLocations = true;
            else
                MaskUtil.ReportInfo('Mask parameter [%s] not registered with script (add true arg to remove)\n', ...
                    theName);
            end
        end
        if (resetPromptLocations)
            % Reset the prompt locations to left. This is blind, but
            % otherwise they always get reset to Top
            maskParams = this.theMask.Parameters;
            numMaskParams = numel(maskParams);
            for i=1:numMaskParams
                maskParams(i).DialogControl.PromptLocation = 'left';
            end
        end

        % If there are no mask parameters left, delete the whole mask!
        if (cleanNonParams && 0 == numel(this.theMask.Parameters))
            MaskUtil.ReportInfo('Removing mask that has no parameters');
            this.theMask.delete;
        end
    end
   %----------------------------------------------------------------------
    function DeprecatedLegacyWrapInClassdef(block, newLibraryName)
        % DeprecatedLegacyWrapInClassdef Creates a new library block
        % that uses classdef-based configuration. The specified original
        % legacy block is left untouched. Instead, the new classdef-based
        % block contains only a reference to the legacy block. Note that
        % some dialog controls cannot be transfered from design-time to
        % build-time using this method (e.g. checkboxes). The new
        % classdef-based block is added to the specified library, or if
        % none specified, a library based on the current one + "Temp".
        % Usage:
        %    AutomateMask.DeprecatedLegacyWrapInClassdef(block)
        if (nargin<1)
            block = gcb;
        end
        fprintf('Adjusting block [%s]\n',block);
        this = AutomateMask(block);
        
        legacyBlockPath = this.blockPath;
        
        % Get the new library
        if (nargin < 2)
            newLibraryName = [this.libObj.Name 'Temp'];
        end
        
        % Get names for innerLegacy mode from legacy mask
        if (false == this.GetNewNames(AutomateMask.innerLegacy))
            % Abort
            error('Aborted by user request in rename dialog');
        end
        
        % Create a new copy of the block to become the "classdef" version
        classdefBlockH = this.CopyBlockToLibrary(legacyBlockPath, newLibraryName);
        
        % Repair the new block name if it's not class-ready
        newName = CreatePascalName(get_param(classdefBlockH,'Name'));
        set_param(classdefBlockH,'Name',newName);
        classdefBlockPath = getfullname(classdefBlockH);
        
        % Redirect focus on new block - this is where all the remaining
        % action will take place:
        this.blockPath = classdefBlockPath;
        this.theMask = Simulink.Mask.get(this.blockPath);
        
        % ----- Extract the default values
        this.GetPropDefs();

        % ----- Create the new classdef
        this.CreateClassdef();
        
        % ----- Destroy the eligible mask parameters
        for nIdx=1:numel(this.oldNames)
            this.theMask.removeParameter(this.oldNames{nIdx});        
        end
        
        %  ... Replace content of classdef copy to be a link to the legacy
        Simulink.SubSystem.deleteContents(classdefBlockPath);
        innerLinkH = add_block(legacyBlockPath,[classdefBlockPath '/InnerLink']);
        
        % Connect surface all the ports of the inner block up to subsystem
        this.ConnectAllPorts(innerLinkH);
        
        % Clear initialization code to prepare for promotion work
        this.theMask.Initialization = '';

        %  ... Promote parameters from legacy block up into classdef mask
        this.ReplaceDesignTimeParametersWithPromotion(innerLinkH);
        
        % Add the Config parameter
        BTConfig.AutomateCreateMaskParameter(classdefBlockPath)
        
        % Update innerLink to use Config-based variables
        innerMask = Simulink.Mask.get(innerLinkH);
        for i=1:numel(this.oldNames)
            aParam = innerMask.getParameter(this.oldNames{i});
            aParam.Value = this.newNames{i};
        end
    end    

end


properties (Access = protected, Constant)
    renameInMask = 0
    innerClassdef = 1
    innerLegacy = 2
    renameInClassdef = 3
end

properties (Access = protected)
    oldNames
    newNames
    propDefaults
    propTypes
    diffReport = ""
    theMask
    blockPath
    modelName
    libObj
    configMode
    classdefName
    classdefMaskParamName
end

methods (Access = protected)

    %----------------------------------------------------------------------
    function this = AutomateMask(block, modificationsOk, noMaskOk)
        if (nargin < 2)
            modificationsOk = false;
        end
        if (nargin < 3)
            noMaskOk = false;
        end
        
        % Load the diagram that contains the blockpath
        if (ischar(block))
            this.modelName = fileparts(block);
            if (isempty(this.modelName))
                % the model itself was specified
                this.modelName = block;
            end
            load_system(this.modelName);
        end

        this.blockPath = getfullname(block);
        this.modelName = bdroot(this.blockPath);
        this.libObj = get_param(this.modelName,'Object');
        
        if (false == modificationsOk)
            if (strcmp('on',this.libObj.Dirty))
                error('Diagram [%s] already has modifications. Save or close first', ...
                    this.modelName)
            end
        end
        this.libObj.Lock = 'off';

        % Acquire the mask
        this.theMask = Simulink.Mask.get(this.blockPath);
        if (false == noMaskOk && isempty(this.theMask))
            error('Block [%s] has no mask!',this.blockPath);
        end
       
    end

    %----------------------------------------------------------------------
    function InternalSwitchToUseClassdef(this)

        % ----- Get names in "Config" mode:
        if (false == this.GetNewNames(AutomateMask.innerClassdef))
            % Abort
            error('Aborted by user request in rename dialog');
        end

        % ----- Extract the property definitions
        this.GetPropDefs();

        % ----- Create the new classdef
        this.CreateClassdef();

        % ----- Workaround Part 1: Simulink messes up prompt locations for
        % remaining parameters when some mask parameters are removed. Try
        % to compensate by gathering the current prompt locations
        locMap = containers.Map;
        dlgCtrls = [this.theMask.Parameters.DialogControl];
        for i=1:numel(dlgCtrls)
            aCtrl = dlgCtrls(i);
            if isprop(aCtrl,'PromptLocation')
                locMap(aCtrl.Name) = aCtrl.PromptLocation;
            end
        end
        
        % ----- Destroy the eligible mask parameters
        % First undo the promotion so that the source parameters will still
        % reference the soon-to-be-gone mask parameters.
        this.ConvertPromoteToNormal();
        for nIdx=1:numel(this.oldNames)
            this.theMask.removeParameter(this.oldNames{nIdx});  
        end
        
        % ----- Workaround Part 2: Return parameter prompt locations
        for nIdx=1:numel(this.theMask.Parameters)
            ctrl = this.theMask.Parameters(nIdx).DialogControl;
            if (false == isKey(locMap,ctrl.Name))
                % no prompt location for this one so move on
                continue;
            end
            if (false == strcmp(ctrl.PromptLocation, locMap(ctrl.Name)))
                fprintf('Prompt location workaround [%s] %s->%s\n', ...
                    ctrl.Name,ctrl.PromptLocation,locMap(ctrl.Name));
                ctrl.PromptLocation = locMap(ctrl.Name);
            end
        end
        
        % ----- Perform search/replace in lots of places
        this.ReplaceCallbacks();
        this.ReplaceSubBlocks();
        this.ReplaceInitCode();
        this.ReplaceDisplayCode();        

        % ----- Add the classdef-based configuration mask parameter
        % We do this *after* the rename intentionally so that the variable
        % does not exist yet. Sometimes the newly made default values are bad.
        BTConfig.AutomateCreateMaskParameter(this.blockPath);

        % ----- Dump out and display the difference report
        this.WriteReportFile(strrep(this.blockPath,'/','_'));
    end
    %----------------------------------------------------------------------
    function ConvertPromoteToNormal(this)
        % Finds any mask parameters in oldNames that are promotions and
        % transforms them into normal parameters (not promotions)
        
        for idx=1:numel(this.oldNames)
            paramName = this.oldNames{idx};
            aParam = this.theMask.getParameter(paramName);
            if ("promote" ~= aParam.Type)
                continue;
            end
            % Found one: convert to "normal" parameter
            % Step 1: Discover all the promoted parameters
            typeOptions = aParam.TypeOptions;
            % Step 2: Find deepest source parameter to discover the type
            [srcParam] = MaskUtil.FindPromotedParameter(this.theMask,paramName);
            % Step 3: Convert the promote to a "normal" parameter
            aParam.Type = srcParam.Type;
            % Step 4: Replace all the promoted parameters' values with the
            % name of the now "normal" parent mask parameter.
            for promotedIdx=1:numel(typeOptions)
                [subPath,subParam] = fileparts(typeOptions{promotedIdx});
                subBlock = MaskUtil.SubBlockName(this.blockPath,subPath);
                set_param(subBlock,subParam,aParam.Name);
            end
        end
    end
    %--------------------------------------------------------------------------
    function [newValue, isChanged] = PerformReplace(this, origValue, description, indexes)

        % If no indexes specified, do them all by default
        if (nargin < 4)
            indexes = 1:numel(this.oldNames);
        end
        isChanged = false;
        newValue = origValue;
        oldDiff = MaskUtil.EscapeTextForHtml(origValue);
        newDiff = MaskUtil.EscapeTextForHtml(origValue);
        for changeIdx=indexes
            nameFindExp = ['\<' this.oldNames{changeIdx} '\>'];
            replaceNormal = this.newNames{changeIdx};
            replaceOldDiff = ['<span style="background-color:yellow;">' ...
                this.oldNames{changeIdx} '</span>'];
            replaceNewDiff = ['<span style="background-color:yellow;">' ...
                this.newNames{changeIdx} '</span>'];

            % First check for "error" of new name already present in old value
            newNameFindExp = ['\<' this.newNames{changeIdx} '\>'];
            replaceErrNewInOld = ['<span style="background-color:#ff9999;">' ...
                this.newNames{changeIdx} '</span>'];
            maybeBad  = regexprep(oldDiff,newNameFindExp,replaceErrNewInOld);
            if (false == strcmp(maybeBad,oldDiff))
                % Found a new name in the old value - show in report
                isChanged = true;
                oldDiff = maybeBad;
            end            

            newValue = regexprep(newValue,nameFindExp,replaceNormal);
            oldDiff  = regexprep(oldDiff,nameFindExp,replaceOldDiff);
            newDiff  = regexprep(newDiff,nameFindExp,replaceNewDiff);
        end

        % If something changed, add to report
        if (false == strcmp(newValue, origValue) || isChanged)
            isChanged = true;
            this.diffReport = this.diffReport + ...
                '<tr><td colspan="2" bgcolor="#eeeeee">' + ...
                description + ...
                '</td></tr>' + ...
                ['<tr><td>' oldDiff '</td><td>' newDiff '</td></tr>'] + ...
                '<tr height="10px"></tr>';
        end
    end
    %--------------------------------------------------------------------------
    function WriteReportFile(this,fileName)
        reportPath = fullfile(tempdir,['Diff_' fileName '.html']);
        fidReport = fopen(reportPath,'w+');

        % Doc header
        fprintf(fidReport, ...
        ['<!DOCTYPE html><html><head><style>td{border: 1px solid #CCCCCC; }' ...
        '</style></head><body style="font-family:monospace;">' ...
        '<h2>Mask Rename Report</h2>']);

        % Mask changes summary
        fprintf(fidReport,'<table><tr><td><b>Original Mask Name</b></td>');
        fprintf(fidReport,'<td><b>New Mask Name</b></td></tr>');
        for nIdx=1:numel(this.oldNames)
            fprintf(fidReport,'<tr><td style="border:none;">%s</td>', ...
                this.oldNames{nIdx});
            fprintf(fidReport,'<td style="border:none;">%s</td></tr>', ...
                this.newNames{nIdx});
        end
        fprintf(fidReport, '</tr></table>\n');

        % Differences content
        fprintf(fidReport, ...
        ['<table style="width:100%%">' ...
        '<tr><th>Original</th><th>Renamed</th></tr>']);
        fprintf(fidReport,'%s',this.diffReport);

        % Doc ending
        fprintf(fidReport,'\n</table></body></html>\n');
        fclose(fidReport);

        winopen(reportPath);
    end
    %--------------------------------------------------------------------------
    function inputCell = NamesDialog(this, inputCell, oldNamesCell, dlgText)
        % NamesDialog opens dialog box to enable choice of new names
        namesCount = numel(oldNamesCell);

        % Create the figure dialog and a grid layout inside to manage pieces
        fig = uifigure('Name','Rename');
        grid = uigridlayout(fig,'Scrollable','on');
        extraRows = 3;
        grid.RowHeight = repmat({'fit'},1,namesCount+extraRows);
        grid.RowHeight{1} = 50; % First row is a title label
        grid.ColumnWidth = {'1x','1x'};
        
        % Add a "title" label inside the dialog
        theLabel = uilabel(grid,'Text',dlgText);
        theLabel.FontSize = 14;
        theLabel.HorizontalAlignment = 'center';
        theLabel.FontWeight = 'bold';
        theLabel.Layout.Column = 1:2;

        % ... and an "auto-fill" button
        btn = uibutton(grid,'Text','Auto-Fill', ...
           'ButtonPushedFcn', @(btn,event) CB_ButtonAutoFill(btn,event,grid,namesCount));
        btn.Layout.Column = 2;
        
        % Remember the edit fields so we do not have to find them again
        editFieldCtrlsCell = cell(1,namesCount);

        % Loop through oldNamesCell and create labels & edit boxes
        for i=1:namesCount
            if (numel(inputCell) < i)
                inputCell{i} = '';
            end
            theLabel = uilabel(grid,'Tag',string(i),'Text',oldNamesCell{i});
            theLabel.HorizontalAlignment = 'right';
            editFieldCtrlsCell{i} = uieditfield(grid,'Tag',string(i),'Value',inputCell{i});
        end

        % Add buttons on the bottom for OK and Abort
        btn = uibutton(grid,'Text','OK', ...
           'ButtonPushedFcn', @(btn,event) CB_ButtonOkPush(btn,event,fig));
        btn = uibutton(grid,'Text','Abort', ...
           'ButtonPushedFcn', @(btn,event) CB_ButtonAbortPush(btn,event,fig));

        % Wait for dialog to be closed, or for OK button
        uiwait(fig);

        % Check if figure/button all deleted, otherwise return names
        if (ishandle(btn))
            % Return names
            for i=1:namesCount
                editField = editFieldCtrlsCell{i};
                inputCell{i} = get(editField,'Value');
            end
            close(fig);
        else
            % Cancelled. Clear out returned cell array
            inputCell = [];
        end
    end

    %--------------------------------------------------------------------------
    function goodToGo = GetNewNames(this, isConfig)
        % Get all parameter names - but filter out just the ones we want
        this.configMode = isConfig;
        theBlock = this.theMask.getOwner.Handle;
        switch (this.configMode)
            case AutomateMask.innerClassdef
                dlgText = sprintf(['Convert mask parameters to classdef properties\n' ...
                    'as part of REMASKING to create new block']);
                eligibleParams = findobj(this.theMask.Parameters,'Hidden','off','Evaluate','on');
            case AutomateMask.innerLegacy
                dlgText = sprintf(['Convert mask parameters to classdef properties\n' ...
                    '(just creating wrapper block)']);
                eligibleParams = findobj(this.theMask.Parameters,'Hidden','off','Evaluate','on','Type','edit');
                % Add promoted edits to the mix
                promotedParams = findobj(this.theMask.Parameters,'Hidden','off','Evaluate','on','Type','promote');
                for p=1:numel(promotedParams)
                    promotedParam = MaskUtil.FindPromotedParameter(this.theMask,promotedParams(p).Name);
                    if (false == isempty(promotedParam) && promotedParam.Type == "edit")
                        eligibleParams(end+1) = promotedParams(p);
                    end
                end
            case AutomateMask.renameInClassdef
                % Get the mask classdef param from the mask workspace to
                % determine the classdef name
                theParamValue = MaskUtil.GetEvaluatedParameter(...
                    theBlock,this.classdefMaskParamName);
                this.classdefName = class(theParamValue);
                theDoc = matlab.desktop.editor.findOpenDocument(this.classdefName);
                if (false == isempty(theDoc) && theDoc.Modified)
                    error('Text document [%s] has unsaved changes. Aborting.', ...
                        theDoc.Filename);
                end
                dlgText = sprintf('Rename classdef properties\nin file [%s.m] and in\nsubsystem [%s])', ...
                    this.classdefName, getfullname(theBlock));
                mco = meta.class.fromName(this.classdefName);
                % LOL pretend the parameters are properties. They both have
                % the "Name" property that is needed below.
                eligibleParams = findobj(mco.PropertyList,'Hidden',0);
            otherwise
                dlgText = sprintf('Rename mask parameters\nin subsystem [%s]', ...
                    getfullname(theBlock));
                eligibleParams = findobj(this.theMask.Parameters,'Hidden','off');
        end
        paramNameCell = {eligibleParams.Name};
        numParams = numel(paramNameCell);
        input(1:numParams) = deal({''});

        goodToGo = false;
        while (goodToGo == false)
            errCell ={};
            input = this.NamesDialog(input, paramNameCell,dlgText);
            if (isempty(input))
                disp('ABORTED');
                return;
            end

            % Get list of changed old names and new names
            changed = (input ~= "");
            this.oldNames = paramNameCell(changed);
            this.newNames = input(changed);
            this.diffReport = "";
            changeCount = numel(this.newNames);
            if (changeCount == 0)
                switch (this.configMode)
                    case {AutomateMask.innerClassdef AutomateMask.innerLegacy}
                        % No rename changes! No problem for conversions
                        goodToGo = true;
                    case {AutomateMask.renameInMask AutomateMask.renameInClassdef}
                        % No renames in renaming mode means not goodToGo
                        goodToGo = false;
                        return;
                end
            end

            % Validate that new names are good
            for nIdx=1:changeCount
                [validName,cleanName,newErrs] = MaskUtil.ExtractValidName(this.newNames{nIdx},'new name');
                if (isempty(validName) || false == strcmp(this.newNames{nIdx},validName))
                    errCell{end+1} = ...
                        sprintf('Invalid name [%s] specified', this.newNames{nIdx});
                    errCell = [errCell newErrs];
                    if (false == isempty(cleanName))
                        input(strcmp(paramNameCell,this.oldNames{nIdx})) = {cleanName};
                    end
                end
                if (sum(strcmp(validName,this.newNames)) > 1)
                    errCell{end+1} = sprintf('Duplicate new name [%s] specified for [%s]', ...
                        validName, this.oldNames{nIdx});
                end
            end

            if (isempty(errCell))
                goodToGo = true;
            else
                errMsg = strjoin(errCell,newline);
                waitfor(errordlg(errMsg,'Problems encountered','modal'));
            end
        end
        
        % Adjust the rename operation based on mode
        switch (this.configMode)
            case {AutomateMask.innerClassdef AutomateMask.innerLegacy}
                % Conversion from mask to classdef:
                % Copy over all the old names that were not changed.
                notChanged = setdiff(paramNameCell,this.oldNames);
                this.oldNames = horzcat(this.oldNames,notChanged);
                this.newNames = horzcat(this.newNames,notChanged);
                % Add the "Config." prefix to all the new names
                this.newNames = strcat('Config.',this.newNames);
            case {AutomateMask.renameInClassdef}
                % Get rid of any names that have not actually changed
                matches = ismember(this.newNames,this.oldNames);
                this.newNames = this.newNames(~matches);
                this.oldNames = this.oldNames(~matches);
                
                % Classdef rename is all about the "Config"
                prefix = [this.classdefMaskParamName '.'];
                this.newNames = strcat(prefix,this.newNames);
                this.oldNames = strcat(prefix,this.oldNames);
                return;
            otherwise
                % No adjustments needed
        end
    end
    %--------------------------------------------------------------------------
    function ReplaceCallbacks(this)
        numParams = numel(this.theMask.Parameters);
        for i=1:numParams
            aParam = this.theMask.Parameters(i);
            description = sprintf('Callback for mask parameter [%s]', ...
                aParam.Name);
            [newCode,isChanged] = this.PerformReplace(aParam.Callback, description);
            if (isChanged)
                aParam.Callback = newCode;
            end
        end
    end
    %--------------------------------------------------------------------------
    function ReplaceSubBlocks(this)

        % Acquire a list of blocks inside the specified block but without
        % looking under any *additional* masks other than the one from the
        % specified block.
        if (false == isempty(get_param(this.blockPath,'Parent')))
            % blockPath is not a block, but is it a subsystem?
            if ("SubSystem" ~= get_param(this.blockPath,"BlockType"))
                % Not a subsystem, so cannot look "inside" it
                return;
            end
        end
        F = Simulink.FindOptions("FollowLinks",true,"LookUnderMasks","Graphical");
        allBlocks = Simulink.findBlocks(this.blockPath,F);
        numBlocks = numel(allBlocks);

        % Use the mask-replacing-block object to ward off promotion issues
        maskStyles = get_param(this.blockPath,'MaskStyles');
        
        % Since we cannot determine whether the parameters of built-in
        % blocks are evaluated or not, we'll just search and give it a try.
        % However, don't try with these as they are *not* evaluated!
        builtIn = containers.Map;
        builtIn('DataStoreMemory')  = {'DataStoreName'};
        builtIn('DataStoreWrite')   = {'DataStoreName'};
        builtIn('DataStoreRead')    = {'DataStoreName'};
        builtIn('Goto')             = {'GotoTag'};
        builtIn('From')             = {'GotoTag'};
        builtIn('GotoTagVisibility')= {'GotoTag'};

        % For each block, search all dialog parameters and do replace
        for blockIdx=1:numBlocks
            % Block loop
            aBlock = getfullname(allBlocks(blockIdx));
            
            theBlockType = get_param(aBlock,'BlockType');

            % Special MATLAB Function Block parameter handling: Attempt to
            % perform rename operations inside the MATLAB function script
            isMFB = strcmp('SubSystem',theBlockType) && ...
                strcmp('MATLAB Function',get_param(aBlock,'SFBlockType'));
            if (isMFB)
                this.SearchMatlabFunctionBlockParameters(aBlock);
            end

            % Block Dialog Parameters
            dialogParams = get_param(aBlock,'DialogParameters');
            if (false == isstruct(dialogParams))
                % No parameters. Move on.
                continue;
            end
            paramNames = fields(dialogParams);
            numParams = numel(paramNames);
            relativeBlockPath = regexprep(aBlock,['^' this.blockPath '/'],'');
            aBlockMask = get_param(aBlock,'MaskObject');

            % For each parameter, search/replace for new names
            for blockParamIdx=1:numParams
                % Block's parameter loop
                paramName = paramNames{blockParamIdx};

                % Check if this parameter has been promoted into renamed mask
                ppathExp = sprintf('\\<%s/%s\\>',relativeBlockPath,paramName);
                if (false==isempty(cell2mat(regexp(maskStyles,ppathExp,'once'))))
                    % Found it! This means the current parameter is promoted
                    % into the mask we're renaming. No need to search here.
                    continue;
                end

                % Check if this is a user mask with an evaluated parameter
                notEvaluated = false;
                if (isempty(aBlockMask))
                    evalDesc = '<unknown if evaluated>';
                else
                    theMaskParam = aBlockMask.getParameter(paramName);
                    if isempty(theMaskParam)
                        evalDesc = '<unknown if evaluated>';
                    elseif (strcmp(theMaskParam.Evaluate,'on'))
                        evalDesc = '<b>Evaluated</b>';
                    else
                        evalDesc = sprintf('%sNot-Evaluated : Rename <b>not</b> performed%s', ...
                            '<span style="color:red;">', ...
                            '</span>');
                        % Too many troubles from these renames so don't do
                        % them... but might be helpful to include in report
                        % so keep track of this situation for use below
                        notEvaluated = true;
                    end
                end

                blockParam = dialogParams.(paramName);
                isExpr = strcmp('string',blockParam.Type);
                if (false == isExpr)
                    % This parameter isn't even a string. Move on.
                    continue;
                end
                
                % Bail if we've got a problematic built-in
                if (builtIn.isKey(theBlockType) && ...
                    ismember(paramName,builtIn(theBlockType)))
                    % Yep- just go ahead and move along
                    continue;
                end
                
                oldValue = get_param(aBlock,paramName);
                % For each new name, perform the search/replace (iteratively)
                description = sprintf('Block:[%s] Parameter:[%s]        %s', ...
                    aBlock,paramName,evalDesc);
                [newValue,isChanged] = this.PerformReplace(oldValue, description);
                if (isChanged)
                    if (notEvaluated)
                        % Do not make any changes. Ignore the rename.
                    else
                        set_param(aBlock,paramName,newValue);
                    end
                end
            end % parameter loop
        end % block loop
    end
    %--------------------------------------------------------------------------
    function ReplaceInitCode(this)
        % ----- Perform search/replace in initialization code
        [newValue,isChanged] = this.PerformReplace( ...
            this.theMask.Initialization, 'Mask Initialization Code');
        if (isChanged)
            this.theMask.Initialization = newValue;
        end
    end
    %--------------------------------------------------------------------------
    function ReplaceDisplayCode(this)
        % ----- Perform search/replace in icon display code
        [newValue,isChanged] = this.PerformReplace(this.theMask.Display, 'Mask Display Code');
        if (isChanged)
            this.theMask.Display = newValue;
        end
    end    
    %--------------------------------------------------------------------------
    function ReplaceInClassdef(this)
        % Open the classdef in the editor
        edit(this.classdefName);
        theDoc = matlab.desktop.editor.getActive;
        
        % Temporarily change the name-arrays to eliminate the parameter
        saveNew = this.newNames;
        saveOld = this.oldNames;
        this.newNames = extractAfter(this.newNames,'.');
        this.oldNames = extractAfter(this.oldNames,'.');

        % Replace the property names *in editor* but do not save
        description = sprintf('classdef [%s] "%s"', this.classdefName, ...
            theDoc.Filename);
        [newScript,isChanged] = this.PerformReplace(theDoc.Text, description);
        if (isChanged)
            theDoc.Text= newScript;
        end        
        
        % Revert rename lists
        this.newNames = saveNew;
        this.oldNames = saveOld;
    end

    %--------------------------------------------------------------------------
    function functionName = CreateRenameTransformFunction(this)
        % Create the text of the function
        namePieceBlock = strrep(this.blockPath,'/','_');
        % Use the current (soon to be old) library version number in the
        % function name to indicate which version (and earlier) gets
        % transformed. Replace "." with underscore for function/filename
        namePieceVer = strrep(this.libObj.ModelVersion,'.','_');
        functionName = sprintf('%s_Transform%s',namePieceBlock,namePieceVer);
        funcLines = { ...
            sprintf('function outData = %s(inData)',functionName) ...
            sprintf('%% Transform function automatically created by %s', mfilename) ...
            '   outData = MaskUtil.TransformFunctionRename(inData, ...' ...
            sprintf('      %s, ...', CellArrayToString(this.oldNames)) ...
            sprintf('      %s);', CellArrayToString(this.newNames)) ...
            'end' ...
            ''};
        funcText = strjoin(funcLines, newline);

        % Write out the function file to disk
        modelFileName = get_param(this.modelName,'FileName');
        modelFolder = fileparts(modelFileName);
        newFilePath = fullfile(modelFolder, [functionName '.m']);
        fid = fopen(newFilePath,'w+');
        fprintf(fid,'%s',funcText);
        fclose(fid);

        % Open it in the editor also so everyone "sees" it
        edit(newFilePath);
    end
    %--------------------------------------------------------------------------
    function AddForwardingTableFunction(this,functionName)

        % Forwarding table cell format is:
        % 1-oldBlock 2-newBlock 3-oldVer 4-newVer 5-functName
        origForwardingTable = this.libObj.ForwardingTable;
        lastVer = '0.0';
        for i=1:numel(origForwardingTable)
            tableLine = origForwardingTable{i};
            if (strcmp(tableLine{1},this.blockPath) && strcmp(tableLine{2},this.blockPath))
                lastVer = tableLine{4};
            end
        end

        % Acquire the likely next version number if possible.
        versionFormat = this.libObj.ModelVersionFormat;
        autoInc = regexp(versionFormat,'%<AutoIncrement:([0-9]*)','tokens');
        if (numel(autoInc) ~= 1)
            warning(['Unable to calculate next library version. You must edit ' ...
                'the library forwarding table manually to replace current version.']);
            newVersion = this.libObj.ModelVersion;
        else
            % This is an auto-increment situation. For format may have
            % changed since 2019. Now increment by tenths is expected.
            incremented = str2double(autoInc{1}) + 0.1;
            newVersion = num2str(incremented);
        end

        newForwardLine = { this.blockPath this.blockPath lastVer newVersion functionName };
        this.libObj.ForwardingTable = [origForwardingTable {newForwardLine}];
    end
    %--------------------------------------------------------------------------
    function AddForwardingTableBlockNameChange(this,newBlockName)

        % Swap out the block name in the blockPath
        oldBlockPath = this.blockPath;
        newBlockPath = regexprep(oldBlockPath,'[^/]*$', newBlockName);
        
        % Forwarding table cell format is:
        % 1-oldBlock 2-newBlock 3-oldVer 4-newVer 5-functName
        origForwardingTable = this.libObj.ForwardingTable;
        
        % In the table, rename all instances of the previous name to this
        % new name. The only entry mentioning the previous name will be the
        % one we add below mapping it to the new rename.
        origEntryCount = numel(origForwardingTable);
        for i=1:origEntryCount
            entryOldPath = origForwardingTable{i}{1};
            entryNewPath = origForwardingTable{i}{2};
            if (strcmp(entryOldPath,oldBlockPath))
                % old path matches. Update to renamed-new
                origForwardingTable{i}{1} = newBlockPath;
            end
            if (strcmp(entryNewPath,oldBlockPath))
                % new path matches. Update to renamed-new
                origForwardingTable{i}{2} = newBlockPath;
            end
        end
        % Now that the table is updated to only contain the renamed-new
        % path instead of the previous block path, insert an entry at the
        % top of the table that renames from old to renamed-new.
        newForwardLine = { oldBlockPath newBlockPath };
        this.libObj.ForwardingTable = [{newForwardLine} origForwardingTable ];
    end
    %--------------------------------------------------------------------------
    function SearchMatlabFunctionBlockParameters(this,aBlock)
        path = aBlock;
        % Get from inside a library linked block!
        reference = get_param(path,'ReferenceBlock');
        if (~isempty(reference))
            path = reference;
        end

        sf = sfroot();
        sfBlock = sf.find('Path',path,'-isa','Stateflow.EMChart');

        if isempty(sfBlock)
            % we failed to find the block. Give up.
            return;
        end

        % Check if each argument is a parameter that is found in this.oldNames. If
        % so, add to search/replace indexes for this block's script:
        args = sfBlock.getChildren();
        numArgs = numel(args);
        foundIndexes = [];
        for i=1:numArgs
            arg = args(i);
            if (strcmp('Parameter',arg.Scope))
                [~,foundIdx] = ismember(arg.Name,this.oldNames);
                if (foundIdx > 0)
                    % This parameter is being renamed! Add to replacement list
                    foundIndexes(end+1) = foundIdx;
                end
            end
        end

        % Get outta here if no names were found
        if (isempty(foundIndexes))
            return;
        end
        
        % Names *were* found.
        if (this.configMode)
            warning(['MATLAB Function Block [%s] receives mask parameters [%s/%s].' ...
                'Not automated yet. Try Simulink.Mask.create(''%s'');\nAutomateMask.ManageMfbParameterMask(''%s'')'], ...
                aBlock, ...
                strjoin(this.oldNames(foundIndexes)), ...
                strjoin(this.newNames(foundIndexes)), ...
                aBlock,aBlock);
        else
            description = sprintf('Block:[%s] "%s"', aBlock, ...
                'Script inside MATLAB Function Block');
            [newScript,isChanged] = this.PerformReplace(sfBlock.Script, description, ...
                foundIndexes);
           if (isChanged)
               sfBlock.Script = newScript;
           end
        end
    end
    %--------------------------------------------------------------------------
    function GetPropDefs(this)
        numNames = numel(this.oldNames);
        this.propDefaults = cell(1,numNames);
        for i=1:numNames
            paramName = this.oldNames{i};
            aParam = this.theMask.getParameter(paramName);
            
            % Find the originating parameter just to discover the type
            origParam = MaskUtil.FindPromotedParameter(this.theMask,paramName);
            
            % Assume properties have no types
            this.propTypes{i} = '';
            
            switch origParam.Type
            case 'checkbox'
                this.propDefaults{i} = num2str(strcmp('on',aParam.Value));
            case {'popup' 'radiobutton'}
                propName = extractAfter(this.newNames{i},'.');
                [this.propTypes{i},this.propDefaults{i}] = ...
                    this.CreateEnumClassdef(propName,aParam.Value,aParam.TypeOptions);
            otherwise
                this.propDefaults{i} = aParam.Value;
            end

        end
    end
    %--------------------------------------------------------------------------
    function CreateClassdef(this)
        propertyLines = {};
        for i=1:numel(this.newNames)
            propertyName = regexprep(this.newNames{i},'^Config.','');
            if (false == isempty(this.propDefaults{i}))
                % Check if the default value is something that makes sense
                try
                    eval([this.propDefaults{i} ';']);
                    % Looks good - let's use it
                    propertyLines{end+1} = sprintf('    %s %s = %s', ...
                        propertyName,this.propTypes{i},this.propDefaults{i});
                catch
                    % Whatever this is (probably references a variable), it
                    % does not compute in global space. Comment it out.
                    propertyLines{end+1} = sprintf('    %s %s %% = %s', ...
                        propertyName,this.propTypes{i},this.propDefaults{i});
                end
            else
                propertyLines{end+1} = sprintf('    %s', ...
                    propertyName);
            end
        end
        
        % Sort the property definitions in the classdef file
        propertyLines = sort(propertyLines);

        % Create brand-new classdef file, read it in, and add the properties
        fileName = HandyTextFileMask.RecreateFile(this.blockPath);
        classdefFilePath = which(fileName);
        % Remember the classdef name for other purposes
        [~,this.classdefName] = fileparts(classdefFilePath);
        classdefText = fileread(classdefFilePath);
        addText = strjoin(propertyLines, newline);

        % Insert text at the top of the properties section
        insertionIndex = regexp(classdefText,'(?<=^\s*properties).*','lineanchors');
        newClassdefText = insertAfter(classdefText,insertionIndex(1), ...
            addText);

        % Now write it out
        fid = fopen(classdefFilePath,'w+');
        fprintf(fid,'%s',newClassdefText);
        fclose(fid);

        % Open it in the editor also so everyone "sees" it
        edit(classdefFilePath);
    end
    %--------------------------------------------------------------------------
    function [className,defVal] = CreateEnumClassdef(this,propName,propVal,membersCell)
        % Creates a new enumerated class based on an originating property
        % name and a cell array of enumeration members
        className = sprintf('%sEnum',propName);
        
        defVal = char("'" + string(propVal).replace(' ','') + "'");
        enumMembers = string(membersCell).replace(' ','');
        for i=1:numel(enumMembers)
            enumMembers(i) = sprintf('      %s (%d)', enumMembers(i), i);
        end
        
        fileName = HandyTextFileMask.RecreateFile(this.blockPath,'Enum',[propName 'Enum']);
        classdefFilePath = which(fileName);
        classdefText = fileread(classdefFilePath);
        addText = strjoin(enumMembers, newline);

        % Insert text at the top of the properties section
        insertionIndex = regexp(classdefText,'(?<=^\s*enumeration).*','lineanchors');
        newClassdefText = insertAfter(classdefText,insertionIndex(1), ...
            addText);

        % Now write it out
        fid = fopen(classdefFilePath,'w+');
        fprintf(fid,'%s',newClassdefText);
        fclose(fid);

        % Open it in the editor also so everyone "sees" it
        edit(classdefFilePath);
    end
    %--------------------------------------------------------------------------
    function UpdateInitCodeToCopyDiscreteToObject(this)
        % Create initialization code that will copy discrete mask
        % parameters listed in newNames to a classdef object
        
        objectName = 'InnerLinkConfig';
        
        initCode = { 'blkh = gcbh;'; ...
            '% Copy discrete mask parameters into a classdef object'; ...
            sprintf('%s = %s;',objectName,this.classdefName) };
        
        for i=1:numel(this.oldNames)
            propertyName = strrep(this.newNames{i},'Config.','');
            newCode = sprintf('%s.%s = %s;', ...
                objectName, propertyName, this.oldNames{i});
            initCode = vertcat(initCode, newCode);
        end
        initCode = strjoin(initCode, newline);
        this.theMask.Initialization = initCode;        
        
    end
    %--------------------------------------------------------------------------
    function ReplaceDesignTimeParametersWithPromotion(this,innerLink)
        % Loop through design-time parameters of innerLink and
        % promote/replace matching parameters in parent mask

        % But first: Handle the common big red X code. It's kinda like a
        % promotion but for a calculated mask workspace variable.
        findThis = 'if (exist(''ok'',''var'') && ok)';
        if (contains(this.theMask.Display,findThis))
            insertThis = ["% Support icon display use of ok" ...
                "[ok,found] = MaskUtil.GetEvaluatedParameter(..." ...
                "    MaskUtil.SubBlockName(blkh,'InnerLink'),'ok');" ...
                "if (found == false)" ...
                "    ok = true;" ...
                "end"];
            insertThis = char(strjoin(insertThis, newline));
            this.theMask.Initialization  = [this.theMask.Initialization ...
                newline newline insertThis];
        end
        
        innerMask = Simulink.Mask.get(innerLink);

        % Acquire array of parameters that are DESIGN TIME
        designParams = findobj(innerMask.Parameters,'Hidden','off','Evaluate','off');

        % Loop through parameter taking one of two actions:
        % 1- For parameters are ARE already promotions, update typeoptions
        %    to be promotions from innerlink instead of from original block
        % 2- For parameters that are not promoted in the legacy mask,
        %    promote now from new classdef-block up to the legacy block
        numParams = numel(designParams);
        for i=1:numParams
            innerParam = designParams(i);
            paramName = innerParam.Name;
            % ... but do not include the new Config param if present
            if ("Config" == paramName)
                continue;
            end
            outerParam = this.theMask.getParameter(paramName);
            innerPromotedName = ['InnerLink/' paramName];
            
            if (outerParam.Type == "promote")
                % This parameter is already a promotion. Switch to promote
                % up from the new classdef block instead of from the
                % original source(s).
                outerParam.TypeOptions = {innerPromotedName};
            else
                % This parameter was not already a promotion. Try to make
                % it one now.
                % Unknown why some control types loose their prompt
                % location when switching to become a promote control.
                % Also some controls (e.g. checkbox) do not have prompt loc
                % Workaround for prompt location
                if (isprop(innerParam.DialogControl,'PromptLocation'))
                    savePromptLoc = innerParam.DialogControl.PromptLocation;
                end
                outerParam.Type = 'promote';
                outerParam.TypeOptions = {innerPromotedName};
                if (isprop(outerParam.DialogControl,'PromptLocation'))
                    outerParam.DialogControl.PromptLocation = savePromptLoc;
                end
            end
        end
    end
    %----------------------------------------------------------------------
    function ConnectAllPorts(this,block)
        portHs = get_param(block,'PortHandles');
        
        F = Simulink.FindOptions("FollowLinks",true,"SearchDepth",1);

        % Handle outports
        if (numel(portHs.Outport) > 0)
            portBlocks = Simulink.findBlocksOfType(block,'Outport',F);
            portBlockNames = get_param(portBlocks,'Name');
            portBlockNumbers = get_param(portBlocks,'Port');
            nameMap = containers.Map(portBlockNumbers,portBlockNames);
            for i=1:numel(portHs.Outport)
                portH = portHs.Outport(i);
                portName = nameMap(num2str(i));
                MaskUtil.ConnectPort(portH, '', portName, i,'');
            end
        end
        % Handle inports
        if (numel(portHs.Inport) > 0)
            portBlocks = Simulink.findBlocksOfType(block,'Inport',F);
            portBlockNames = get_param(portBlocks,'Name');
            portBlockNumbers = get_param(portBlocks,'Port');
            nameMap = containers.Map(portBlockNumbers,portBlockNames);
            for i=1:numel(portHs.Inport)
                portH = portHs.Inport(i);
                portName = nameMap(num2str(i));
                MaskUtil.ConnectPort(portH, '', portName, i,'');
            end
        end
    end
    %----------------------------------------------------------------------
    function newBlockH = CopyBlockToLibrary(this, sourceBlockPath, libraryName)

        % get the library
        foundIt = contains(which(libraryName),'.slx');
        if (false == foundIt)
            % Create it and save it
            close_system(libraryName,0);
            new_system(libraryName,'Library');
            % We will save in same folder as this.libObj
            fullPath = fullfile(fileparts(this.libObj.FileName),libraryName);
            save_system(libraryName,fullPath);
        end
        open_system(libraryName);
        
        % Ensure new libary is ready for action
        newLibObj = get_param(libraryName,'Object');
        if (strcmp('on',newLibObj.Dirty))
            error('Diagram [%s] already has modifications. Save or close first', ...
                newLibObj.Name)
        end
        newLibObj.Lock = 'off';
        
        % Copy the EnableLBRepository property from old to new library
        MaskUtil.safe_setparam(libraryName,'EnableLBRepository', ...
            this.libObj.EnableLBRepository);
        
        % Calculate new block path (blocks placed in root of library)
        blockName = get_param(sourceBlockPath,'Name');
        newBlockPath = MaskUtil.SubBlockName(libraryName,blockName);
        % Make sure it's not already there
        MaskUtil.EnsureBlocksDoNotExist(libraryName,{blockName});

        % Make the copy
        % My Experimental Workaround for MathWorks Case#04724331
        % (try to copy from library to model back to library)
%         add_block(sourceBlockPath,newBlockPath,'CopyOption','nolink');
        tempSysName = 'Temp04724331'; 
        close_system(tempSysName,0);
        new_system(tempSysName);
        % Setup ert target file to eliminate warnings for code settings
        set_param(tempSysName,'SystemTargetFile','ert_shrlib.tlc');
        tempBlockPath = [tempSysName '/' blockName];
        add_block(sourceBlockPath,tempBlockPath);
        set_param(tempBlockPath,'LinkStatus','none');
        newBlockH = add_block(tempBlockPath,newBlockPath);
        close_system(tempSysName,0);
    end
    %----------------------------------------------------------------------
    function CreatePropertyRenameCompatibility(this)
        % See the following reference page for how hidden dependent
        % properties can be used to maintain compatibility with existing
        % scripts after classdef properties have been renamed
        %https://www.mathworks.com/help/matlab/matlab_oop/example-maintaining-class-compatibility.html

        % Open the classdef in the editor - should already be there and
        % already with modifications is OK.
        edit(this.classdefName);
        theDoc = matlab.desktop.editor.getActive;

        % Determine if renaming has already been performed.
        renNumSearch = '\[Rename-Compatibility:(?<NUM>\d*)\]';
        prevNumsCell = regexp(theDoc.Text,renNumSearch,'tokens');
        thisRenNum = 1;
        if (false == isempty(prevNumsCell))
            prevNums = cellfun(@str2double,prevNumsCell);
            highestPrev = max(prevNums);
            thisRenNum = highestPrev + 1;
        end
        
        % Acquire new and old names without the "Config"
        newPropNames = extractAfter(this.newNames,'.');
        oldPropNames = extractAfter(this.oldNames,'.');
        
        % Collect the new classdef backwards compatibility code
        compatibilityCode = {''};
        compatibilityCode{end+1} = sprintf('properties (Dependent, Hidden) %% [Rename-Compatibility:%d]',thisRenNum);
        for i=1:numel(oldPropNames)
            compatibilityCode{end+1} = sprintf('   %s',oldPropNames{i});
        end
        compatibilityCode{end+1} = 'end';
        % ... next fill in the property access methods
        compatibilityCode{end+1} = 'methods';
        for i=1:numel(oldPropNames)
            % set
            compatibilityCode{end+1} = sprintf( ...
                '   function set.%s(obj,val)', oldPropNames{i}); 
            compatibilityCode{end+1} = sprintf( ...
                '      obj.%s = val;', newPropNames{i});
            compatibilityCode{end+1} = sprintf( ...
                '      warning(''Deprecated %s property [%s] used. Update to use [%s]'')', ...
                this.classdefName,oldPropNames{i}, newPropNames{i});
            compatibilityCode{end+1} = sprintf( ...
                '   end');
            % get
            compatibilityCode{end+1} = sprintf( ...
                '   function val = get.%s(obj)', oldPropNames{i});
            compatibilityCode{end+1} = sprintf( ...
                '      val = obj.%s;', newPropNames{i});
            compatibilityCode{end+1} = sprintf( ...
                '      warning(''Deprecated %s property [%s] used. Update to use [%s]'')', ...
                this.classdefName,oldPropNames{i}, newPropNames{i});
            compatibilityCode{end+1} = sprintf( ...
                '   end');
        end
        compatibilityCode{end+1} = sprintf('end %% [Rename-Compatibility:%d]',thisRenNum);
        compatibilityCode{end+1} = '';
        newCode = strjoin(compatibilityCode,newline);

        % Now make the addition in two places: the real document and the
        % diff report. First, grab the unaltered document text.
        % Grab text and prepare for HTML reporting
        escapeText = MaskUtil.EscapeTextForHtml(theDoc.Text);
        escapeTextLines = strsplit(escapeText,'<br/>','CollapseDelimiters',false);
        escapeNewText = MaskUtil.EscapeTextForHtml(newCode);
        escapeNewTextLines = strsplit(escapeNewText,'<br/>','CollapseDelimiters',false);
        
        % Find insertion spot for compatibility properties block
        insertSearchExp = '^\s*properties\s*\n';
        insertionIndexes = regexp(theDoc.Text,insertSearchExp,'lineanchors');
        if (isempty(insertionIndexes))
            % Could not find. Just dump to top of document
            warning('Unable to find the first plain "properties" block in class [%s]. Adding to top of file', ...
                this.classdefName);
            insertionLine = 1;
        else
            % Yes - found the spot. Convert to line number
            inLine = matlab.desktop.editor.indexToPositionInLine(theDoc,insertionIndexes(1));
            insertionLine = inLine;
        end

        % "new" code is before-text, then yellow new-text, then after-text
        newDiffLines = horzcat( ...
            escapeTextLines(1:insertionLine-1), ...
            {'<span style="background-color:yellow;">'}, ...
            escapeNewTextLines, ...
            {'</span>'}, ...
            escapeTextLines(insertionLine:end) );
        % "old" code is before-text, then empty lines, then after-text
        oldDiffLines = horzcat( ...
            escapeTextLines(1:insertionLine-1), ...
            repmat({newline},1,numel(compatibilityCode) ), ...
            escapeTextLines(insertionLine:end) );
        newDiff = strjoin(newDiffLines,'<br/>');
        oldDiff = strjoin(oldDiffLines,'<br/>');

        description = ['Additional backward-compatibility code added to ' theDoc.Filename];
        this.diffReport = this.diffReport + ...
            '<tr><td colspan="2" bgcolor="#eeeeee">' + ...
            description + ...
            '</td></tr>' + ...
            ['<tr><td>' oldDiff '</td><td>' newDiff '</td></tr>'] + ...
            '<tr height="10px"></tr>';
        
        % Insert the new code to the document in the editor (with undo!)
        % Unfortunately the previous rename operations do not have undo.
        theDoc.insertTextAtPositionInLine(newCode,insertionLine,1);        
    end

end % methods protected
end % classdef

%--------------------------------------------------------------------------
% Local utility functions
%--------------------------------------------------------------------------
function theString = CellArrayToString(cellArray)
    strCell = string(cellArray);
    elements = "'" + strCell + "'";
    list = join(elements,',');
    theString = sprintf('{%s}',list);
end
%--------------------------------------------------------------------------
function newName = CreatePascalName(origName)
% CreatePascalName replaces non-alphanumeric characters with next-character
% capitalization
    newName = MaskUtil.CreateWikiName(origName);
end
%--------------------------------------------------------------------------
function CB_ButtonOkPush(btn,event,fig)
    uiresume(fig);
end
%--------------------------------------------------------------------------
function CB_ButtonAbortPush(btn,event,fig)
    close(fig);
end
%--------------------------------------------------------------------------
function CB_ButtonAutoFill(btn,event,grid,namesCount)
    % Loop through grid and automatically fill in new names based on old
    % names but with capital first letters
    controls = grid.Children;
    for i=1:namesCount
        label = findobj(controls,'Tag',string(i),'Type','uilabel');
        edit = findobj(controls,'Tag',string(i),'Type','uieditfield');
        if (isempty(label) || isempty(edit))
            continue;
        end
        origName = label.Text;
        newName = MaskUtil.CreateWikiName(origName);

        % Now remove invalid chars
        [~,cleanName] = MaskUtil.ExtractValidName(char(newName),'new name');        
        
        edit.Value = cleanName;
    end
end



classdef BTConfig
    %BTCONFIG Utilities for Simulink mask initialization code to administer
    %build-time configuration.
    %   These utilities allow mask initialization code to define and
    %   control the use of MATLAB classdef objects to hold build-time
    %   configuration. In addition to the critical "Assert" method that
    %   defines a classdef type for a mask parameter, other methods provide
    %   automation to create a default "Config" mask parameter complete
    %   with mechanisms to align FeaturePath names with received component
    %   names.
    
    % Copyright 2021 Bose Corporation
    %#ok<*AGROW>

    methods (Static)
    %----------------------------------------------------------------------
    function Config = Assert(blockHndlOrClassdefName, Config)
        % Assert Ensures a parameter is an instance of the correct classdef
        % type, replacing empty values with a default classdef instance.
        % Use BTConfig.AutomateCreateMaskParameter during block mask design
        % to create a default parameter named Config with a classdef-named
        % prompt and initialization code that calls this Assert method and
        % handles FeaturePath alignment if relevant.
        % Usage:
        %    Config = BTConfig.Assert(gcb, Config);
        %      or
        %    Config = BTConfig.Assert('MyConfigClassName', Config)
        %
        % Note that this function has TWO modes of operation depending on
        % whether the first argument is a block handle or a classdef name.
        % If using a block handle, the following must be true:
        %     1 - The specified block must be masked and have a mask
        %         parameter named "Config"
        %     2 - This Config parameter has a prompt which is the exact 
        %         name of the MATLAB classdef containing the build-time
        %         variables used under the mask.
        %
        % In all cases, the configuration class must offer a default
        % constructor with no arguments required to get a class instance.
        %
        % To work as intended, the value of the evaluated Config parameter
        % must be both passed in and accepted as return value. From the
        % mask's "initialization" code pane.
        %
        % See also BTConfig.AutomateCreateMaskParameter

        % Retrieve BTC classdef name: mask prompt or directly specified
        configClassName = BTConfig.GetConfigClassName(blockHndlOrClassdefName);
        % Replace empties with defaults or ensure non-empties correct type
        if (isempty(Config))
            % Empty configuration - use defaults
            constructor = str2func(configClassName);
            Config = constructor();
        elseif false == isa(Config,configClassName)
            error('Supplied configuration must be of type [%s], not [%s]', ...
                configClassName, class(Config));
        end
    end
    %----------------------------------------------------------------------
    function [theModelSettings,varName] = GetModelSettings(aBlockURL,optionalReset)
        % GetModelSettings Returns the model-level configuration object.
        % Ensures the <model-root-name>Settings variable exists, verifying
        % expected classdef type <model-root-name>Config. The variable is
        % defined/replaced if necessary to the correct classdef instance.
        % Returns a "copy" of the variable that is found/created as well as
        % a second output value containing the workspace variable name.
        % Although this call could be used to generically access the model
        % configuration from block-masks inside the model, it's not
        % recommended to call anywhere except in the model InitFcn callback
        % due to the overhead involved (i.e. it's slower than just naming
        % the variable explicitly and trusting that the InitFcn set it up).
        % To get started, use BTConfig.AutomateCreateModelConfig on a model
        % to automate the setup of the model-level build-time-configuration
        % class and model InitFcn variable creation.
        % Note: this model-level configuration technique uses the same
        % settings for every instance of a model if using model references.
        % Usage:
        %   [theModelSettings,varName] = BTConfig.GetModelSettings(aBlockURL,optionalReset);
        %   BTConfig.GetModelSettings(); % typical usage
        %
        % aBlockURL: any Simulink block or model reference. If not provided
        %   bdroot will be used.
        % optionalReset: boolean value that is false by default. When true,
        %   the model's BTC settings are wiped clean with a newly
        %   constructed configuration object instance. This feature is
        %   intended for use by test scripts to both reset and acquire
        %   model settings to begin setup of a test case.
        % 
        % See also BTConfig.AutomateCreateModelConfig
        if (nargin < 1 || isempty(aBlockURL))
            % By default, use the current diagram
            aBlockURL = bdroot;
        end
        if (nargin < 2 || isempty(optionalReset))
            % By default, definitely do *not* reset all the model settings!
            optionalReset = false;
        end
        createIt = true;
        % Get the model name
        modelName = ModelFullName(aBlockURL);
        rootName = ModelRootName(modelName);
        varName = [rootName BTConfig.ModelVarNameEnd];
        className = [rootName 'Config'];

        if (false == exist(className,'class'))
            error('Class [%s] is not found',className);
        end

        % If the variable is already there, grab it and check type. We have
        % to load the model in order to understand where it keeps its
        % global data (e.g. base workspace... but could be data dictionary)
        load_system(modelName);
        if (optionalReset == false && Simulink.data.existsInGlobal(modelName,varName))
            theModelSettings = Simulink.data.evalinGlobal(modelName,varName);
            if (isa(theModelSettings,className))
                % Already exists and is correct type. All done!
                createIt = false;
            end
        end
        if (createIt)
            % The variable was missing or bad- create a new one now
            constructor = str2func(className);
            theModelSettings = constructor();
            Simulink.data.assigninGlobal(modelName,varName,theModelSettings);
        end
    end
    %----------------------------------------------------------------------
    function Expand(Config,optionalPrefix)
        % Expand Expands object properties into workspace as individual variables
        % with an optional prefix on each variable name.
        % Since MATLAB Function Blocks can accept only numeric, char, or
        % string parameters, this method assists by expanding object
        % parameters into individual variables. Best to call this method
        % *after* calling any Validate methods that might adjust properties
        % a bit.
        % NOTE: Talaria now recommends adding parameter masks instead.        
        % Usage Examples:
        %    BTConfig.Expand(Config);
        %    BTConfig.Expand(Config.SubObj,'sub_');
        %
        % See also BTConfig.Assert, BTConfig.AutomateCreateMaskParameter, AutomateMask.ManageMfbParameterMask
        
        % Get metaclass
        mco = metaclass(Config);
        
        if nargin<2
            optionalPrefix = '';
        end
        
        % Find properties with public read access
        pubProps = findobj(mco.PropertyList,'GetAccess','public');
        
        numProps = numel(pubProps);
        for i=1:numProps
            aProp = mco.PropertyList(i);
            propName = aProp.Name;
            propValue = Config.(aProp.Name);
            % Assign value in caller
            assignin('caller',[optionalPrefix propName], propValue);
        end
        
    end
    %----------------------------------------------------------------------
    function isPresent = NodeIsPresent(blockURL)
        % NodeIsPresent Returns true if non-empty "Config" mask parameter
        % is present whose value would represent aggregation into a
        % parental configuration object.
        theMask = get_param(blockURL,'MaskObject');
        if (isempty(theMask.getParameter('Config')))
            % There *is* no Config parameter!
            isPresent = false;
        else
            configStrValue = strrep(get_param(blockURL,'Config'),' ','');
            isNotEmpty = ("[]" ~= configStrValue);
            isNotReservedWord = false == ismember(configStrValue, ...
                {'Config','InnerLinkConfig'});
            isNotConvertLegacy = (false == startsWith(configStrValue,'ConvertLegacy'));
            isPresent = isNotEmpty && isNotReservedWord && isNotConvertLegacy;
        end
    end
    %----------------------------------------------------------------------
    function CallbackHideIfPresent(blockURL,paramNameCell)
        % CallbackHideIfPresent Hides the specified mask parameters if the
        % "Config" mask parameter is present (i.e. non empty). However, if
        % the Config parameter is empty then the parameters are shown.
        % Returns boolean true if Config was non-empty and parameters were
        % hidden. Actually it just disables parameter but remains visible.
        % Note: Please avoid attempts to set design-time parameters with
        % build-time data as this will cause models to "dirty" themselves
        % when external build-time data changes. Setting design-time
        % parameters derived from other design-time parameters as done here
        % does not have this same concern.
        % Usage:
        %   << In the mask callback for the "Config" parameter >>
        %   BTConfig.CallbackHideIfPresent(gcbh,{'FeatureName'});
        %
        % See also: BTConfig.InitSetFeatureIfPresent

        % Check if non-aggregated non-default config is found
        isPresent = BTConfig.NodeIsPresent(blockURL);
        if (isPresent)
            enabled = 'off';
        else
            enabled = 'on';
        end

        theMask = Simulink.Mask.get(blockURL);
        if (false == iscell(paramNameCell))
            paramNameCell = {paramNameCell};
        end
        
        % Loop through and show/hide parameters
        for i=1:numel(paramNameCell)
            aParam = theMask.getParameter(paramNameCell{i});
            aParam.Enabled = enabled;
            aParam.Visible = 'on';
        end
    end
    %----------------------------------------------------------------------
    function InitSetFeatureIfPresent(blockURL,featureParamName)
        % InitSetFeatureIfPresent Sets the value of the specified feature
        % name parameter if the "Config" mask parameter is present
        % (i.e. non-empty). This method extracts the last whole word of the
        % specified Config variable expression ("Mute" from "Config.Mute").
        % Note that this method does *not* hide the feature name parameter 
        % as that job is for the Config parameter's callback.
        % Usage:
        %    << During mask initialization: >>
        %    BTConfig.InitSetFeatureIfPresent(gcbh,'FeatureName');
        %
        % See also: BTConfig.CallbackHideIfPresent

        % Check is non-aggregated non-default config is found
        isPresent = BTConfig.NodeIsPresent(blockURL);
        
        if (isPresent)
            % Set the parameter
            configValue = get_param(blockURL,'Config');
            theName = regexp(configValue,'(\w*)$','match','once');
            MaskUtil.SetPromotedParameter(blockURL,featureParamName,theName);
        end
    end
    %----------------------------------------------------------------------
    function AutomateCreateMaskParameter(blockURL, force)
        % AutomateCreateMaskParameter creates Config mask parameter set up
        % to work with build-time configuration routines. The name of the
        % configuration class is automatically the block name plus "Config"
        % Usage:
        %    BTConfig.AutomateCreateMaskParameter(gcb,[force]);
        
        % If the block's current name does not meet WikiCase guidelines,
        % this routine exits with an error. Supply optional second argument
        % force as 'true' to attempt to proceed with current name anyway.
        if (nargin < 2)
            force = false;
        end
        modelName = bdroot(getfullname(blockURL));
        myStage = sldiagviewer.createStage('TalariaMaskConfig','ModelName', ...
            modelName); %#ok<NASGU> 
        
        refBlock = get_param(blockURL,'ReferenceBlock');
        if (false == isempty(refBlock))
            error('No action taken on library link [%s]. Try again with [%s]', ...
                MaskUtil.GetBlockLinkString(blockURL), ...
                MaskUtil.GetBlockLinkString(refBlock));
        end
        
        % ----- Automate #0: Ensure the block name is a good start
        blockName = get_param(blockURL,'Name');
        wikiName = MaskUtil.CreateWikiName(blockName);
        if (false == strcmp(blockName, wikiName))
            blockNameExpression = MaskUtil.GetFullNameStringExpression(blockURL);
            if (bdIsLibrary(bdroot(blockURL)))
                fixItLink = sprintf("<a href=""matlab:AutomateMask.RenameLibraryBlock(%s)"">FIX IT</a>", ...
                    blockNameExpression);
            else
                fixItLink = sprintf("<a href=""matlab:set_param(%s,'Name','%s');disp('Fixed it! Try again.')"">FIX IT</a>", ...
                    blockNameExpression, wikiName);
            end
            errMsg = sprintf('Block name violation [%s]. Try [%s] %s\n', ...
                blockName, wikiName, ...
                fixItLink);
            if (force)
                MaskUtil.ReportWarning(errMsg); 
            else
                error(errMsg); %#ok<SPERR>
            end
        end

        MaskUtil.ReportInfo('Automating Config mask for block [%s]...', ...
            getfullname(blockURL));
        
        % ----- Automate #1: Create the Config mask parameter
        configClassdefName = [blockName 'Config'];        
        theMask = Simulink.Mask.get(blockURL);
        if (isempty(theMask))
            theMask = MaskUtil.CreateEmptyMask(blockURL);
        end
        configParam = theMask.getParameter('Config');
        if (isempty(configParam))
            paramGroup = theMask.getDialogControl('ParameterGroupVar');
            if (isempty(paramGroup))
                theMask.addDialogControl('Type','group','Name','ParameterGroupVar');
            end            
            configParam = theMask.addParameter('Type','edit','Name','Config', ...
            'Tunable','off', ...
            'Evaluate','on', ...
            'Container','ParameterGroupVar', ...
            'Prompt',configClassdefName);
            configParam.DialogControl.PromptLocation = 'left';
            MaskUtil.ReportInfo('Added Config parameter\n');
        else
            configParam.Tunable = 'off';
            configParam.Evaluate = 'on';
            configParam.Prompt = configClassdefName;
            configParam.DialogControl.PromptLocation = 'left';
            % Not sure it's good to move:Just leave it where it's at
            MaskUtil.ReportInfo('... Config parameter already present\n');
        end
        
        % ----- Automate #2: Add a handy-text-file config block, and create
        % the class text file (but only if not already found)
        if ("SubSystem" == get_param(blockURL,'BlockType'))
            htfBlock = HandyTextFileMask.AddHandyBlock(blockURL);
            [filename,wasCreated] = HandyTextFileMask.CreateFileIfMissing(htfBlock);
            if (wasCreated)
                MaskUtil.ReportInfo('Created default classdef file [%s]\n',filename);
                % Rehash to refresh the caches of found classdefs
                rehash;
            else
                MaskUtil.ReportInfo('... class definition file already exists. (OK)\n');
            end
        end
        
        % ----- Automate #3: Remove out-of-date versions of the code
        for i=1:numel(BTConfig.OldAssertCode)
            code = BTConfig.OldAssertCode{i};
            [theMask.Initialization, wasRemoved] = ...
                BTConfig.RemoveCode(theMask.Initialization,code);
            if (wasRemoved)
                MaskUtil.ReportInfo('Removed old Assertion [%s]\n',code);
            end
        end
        
        % ----- Automate #4: Add code to the top (list in reverse order)
        iCode = theMask.Initialization;
        iCode = BTConfig.AnnounceCodeAdd(iCode,BTConfig.AssertLine,BTConfig.AddToStart,'classdef-assert');
        iCode = BTConfig.AnnounceCodeAdd(iCode,BTConfig.FirstLine,BTConfig.AddToFirst,'block-handle');
        theMask.Initialization = iCode;
        
        % ----- Automate #5: FeaturePath/Config-path alignment
        % Setup the FeaturePath / Hierarchical-Config-Path alignment code
        % (aka TALARIA-269). Start by examining the current block mask to
        % see if any promoted feature path names are present.
        for i=1:numel(theMask.Parameters)
            aParam = theMask.Parameters(i);
            [initCode,callbackCode] = BTConfig.IsFeaturePathParam(theMask, aParam);
            if (false == isempty(initCode))
                % Step 1: FeaturePath alignment Initialization code
                % ... remove for old gcb-based alignment code
                [theMask.Initialization,removeMsgsCell] = BTConfig.RemoveOldCodeVersions( ...
                    theMask.Initialization, initCode, ...
                    {'blkh' 'blkh'},{'gcb','gcbh'},['Old mask initialization ' ...
                    'code for FeaturePath alignment removed']);
                for msgIdx=1:numel(removeMsgsCell)
                    MaskUtil.ReportInfo('%s\n',removeMsgsCell{msgIdx});
                end
                % ... add new blkh-based alignment code
                theMask.Initialization=BTConfig.AnnounceCodeAdd(theMask.Initialization, ...
                    initCode,BTConfig.AddToEnd,'FeaturePath-alignment-init');
                % Step 2: FeaturePath alignment Callback code
                % ... remove for old gcb-based alignment code
                [configParam.Callback,removeMsgsCell] = BTConfig.RemoveOldCodeVersions( ...
                    configParam.Callback, callbackCode, ...
                    {'gcbh'},{'gcb'},['Old Config param callback code for ' ...
                    'FeaturePath alignment removed']);
                for msgIdx=1:numel(removeMsgsCell)
                    MaskUtil.ReportInfo('%s\n',removeMsgsCell{msgIdx});
                end
                % ... add new blkh-based alignment code
                configParam.Callback=BTConfig.AnnounceCodeAdd(configParam.Callback, ...
                    callbackCode, BTConfig.AddToStart,'FeaturePath-alignment-callback');
            end            
        end
        
        mco = meta.class.fromName(configClassdefName);        
        if (isempty(mco))
            MaskUtil.ReportWarning(['Configuration class [%s] does ' ...
                    'not exist yet (or not on path)!'], configClassdefName);
        else
            % ----- Automate #6: Call Validate
            % Setup a call to the Validate method in mask initialization
            code = BTConfig.ValidateLine;
            if (false == isempty(findobj(mco.MethodList,'Name','Validate')))
                theMask.Initialization=BTConfig.AnnounceCodeAdd(theMask.Initialization, ...
                    code, BTConfig.AddToEnd,'Validate-classdef');
            else
                MaskUtil.ReportWarning(['Configuration class [%s] does ' ...
                    'not have a Validate method.'], configClassdefName);
            end
            % ----- Automate #7: Call DoMaskInit
            % Setup a call to the DoMaskInit method in mask initialization
            code = BTConfig.DoMaskInitLine;
            if (false == isempty(findobj(mco.MethodList,'Name','DoMaskInit')))
                theMask.Initialization=BTConfig.AnnounceCodeAdd(theMask.Initialization, ...
                    code, BTConfig.AddToEnd,'DoMaskInit-classdef');
            else
                MaskUtil.ReportWarning(['Configuration class [%s] does ' ...
                    'not have a DoMaskInit method.'], configClassdefName);
            end
        end

        MaskUtil.ReportInfo('Done.');        
    end
    %----------------------------------------------------------------------
    function AutomateCreateMaskParameterWithVariants(blockURL)
        % AutomateCreateMaskParameterWithVariants works like
        % AutomateCreateMaskParameter except the created class is made to
        % inherit from BtcBaseClassWithVariants instead of BtcBaseClass.
        % Usage:
        %    BTConfig.AutomateCreateMaskParameterWithVariants(gcb);

        % First, setup everything like the normal non-variant case
        modelName = bdroot(getfullname(blockURL));
        myStage = sldiagviewer.createStage('TalariaMaskVariantConfig','ModelName', ...
            modelName); %#ok<NASGU>         
        BTConfig.AutomateCreateMaskParameter(blockURL);

        % Discover the classname
        theMask = Simulink.Mask.get(blockURL);
        if (isempty(theMask))
            % No mask: bail!
            return;
        end
        configParam = theMask.getParameter('Config');
        if (isempty(configParam))
            % No config parameter: bail!
            return;
        end
        className = configParam.Prompt;        
        MaskUtil.ReportInfo('... Adjusting class [%s] to support variant configurations...', className);

        % ----- Adjust #1: Add a handy-text-file enum block, and create
        % the class text file (but only if not already found)
        if ("SubSystem" == get_param(blockURL,'BlockType'))
            htfBlock = HandyTextFileMask.AddHandyBlock(blockURL,'VariantEnum');
            [filename,wasCreated] = HandyTextFileMask.CreateFileIfMissing(htfBlock);
            if (wasCreated)
                MaskUtil.ReportInfo('Created Variant Enum file [%s]\n',filename);
                % Rehash to refresh the caches of found classdefs
                rehash;
            else
                MaskUtil.ReportInfo('... Variant Enum classdef file already exists. (OK)\n');
            end
        end

        % Adjust #2: Edit the config classdef to ensure correct base class
        edit(className);
        theDoc = matlab.desktop.editor.findOpenDocument(className);
        originalText = theDoc.Text;
        replaceExp = '\<BtcBaseClass\>';
        newText = regexprep(originalText, replaceExp,'BtcBaseClassWithVariants');
        if (strcmp(originalText,newText))
            MaskUtil.ReportInfo('... Class [%s] already derives from BtcBaseClassWithVariants (OK)', ...
                className);
        else
            theDoc.Text = newText;
            MaskUtil.ReportInfo('Class [%s] adjusted to derive from BtcBaseClassWithVariants.', ...
                className);
        end

        % Adjust #3: add the constructor specification of the enum        
        [insertionIndex,thisName] = BTConfig.GetConstructorInsertionSpot(theDoc.Text,className);
        searchExp = BTConfig.CreateRegexCodeSearch([thisName '@BtcBaseClassWithVariants']);
        foundIndex = regexp(theDoc.Text,searchExp, 'once');
        if (false == isempty(foundIndex))
            % The call to the baseclass constructor was found
            MaskUtil.ReportInfo('... Class [%s] already calls BtcBaseClassWithVariants constructor (OK)', ...
                className);
        else
            % Call to baseclass constructor missing: add it now
            enumClassName = regexprep(className,'Config$','VariantEnum');
            lineNumber = numel(regexp(theDoc.Text(1:insertionIndex),'\n'))+1;
            newCode = sprintf('        %s@BtcBaseClassWithVariants(''%s'');\n', ...
                thisName,enumClassName);
            theDoc.insertTextAtPositionInLine(newCode,lineNumber,1);
            % Announce what we just did:
            hereLink = sprintf('<a href="matlab:edit(''%s'');goToLine(matlab.desktop.editor.getActive,%d);">HERE</a>', ...
                className,lineNumber);
            MaskUtil.ReportInfo('Inserted baseclass constructor enumeration class declaration on line %d [%s]', ...
                lineNumber,hereLink);
        end
    end
    %----------------------------------------------------------------------
    function AutomateCreateMaskParameterWithBreeds(blockURL)
        % AutomateCreateMaskParameterWithBreeds works like
        % AutomateCreateMaskParameter except the created class is made to
        % inherit from BtcBaseClassWithBreeds instead of BtcBaseClass.
        % Usage:
        %    BTConfig.AutomateCreateMaskParameterWithBreeds(gcb);

        % First, setup everything like the normal non-variant case
        modelName = bdroot(getfullname(blockURL));
        myStage = sldiagviewer.createStage('TalariaMaskBreedConfig','ModelName', ...
            modelName); %#ok<NASGU>         
        BTConfig.AutomateCreateMaskParameter(blockURL);

        % Discover the classname
        theMask = Simulink.Mask.get(blockURL);
        if (isempty(theMask))
            % No mask: bail!
            return;
        end
        configParam = theMask.getParameter('Config');
        if (isempty(configParam))
            % No config parameter: bail!
            return;
        end
        className = configParam.Prompt;        
        MaskUtil.ReportInfo('... Adjusting class [%s] to support breed configurations...', className);

        % Open the class definition file for editing
        edit(className);
        theDoc = matlab.desktop.editor.findOpenDocument(className);
        originalText = theDoc.Text;
        
        % Adjust #1: Edit the config classdef to ensure correct base class.
        % Since this operation involves removing text, it has to be done by
        % swapping out the entire file contents. MATLAB provides only
        % provides methods to insert text... nothing to replace/delete
        replaceExp = '\<BtcBaseClass\>';
        newText = regexprep(originalText, replaceExp,'BtcBaseClassWithBreeds');
        if (strcmp(originalText,newText))
            MaskUtil.ReportInfo('... Class [%s] already derives from BtcBaseClassWithBreeds (OK)', ...
                className);
        else
            theDoc.Text = newText;
            MaskUtil.ReportInfo('Class [%s] adjusted to derive from BtcBaseClassWithBreeds.', ...
                className);
        end

        % ----- Adjust #2: Add the BreedClassMap property
        searchExp = '^\s*BreedClassMap\>';
        foundIndex = regexp(theDoc.Text,searchExp,"once","lineanchors");
        if isempty(foundIndex)
            % Not found yet: Find "top of class" insertion spot Just under
            % the Bose Corporation copyright
            searchExp = '(?<=Bose Corporation.$).';
            insertionIndex = regexp(theDoc.Text,searchExp,"once","lineanchors");
            if isempty(insertionIndex)
                MaskUtil.ReportError("Cannot find Bose Corporation copyright " + ...
                    "at the top of class [%s] as insertion spot for BreedClassMap", ...
                    className);
            else
                newCodeLines = {
                    '  properties (Constant,Hidden)'
                    '      % Map from an enum element to the name of a BreedConfig class'
                    '      %. Must be dictionary object mapping from Simulink.IntEnumType-derived'
                    '      %. enumeration to string. For example (note multi-line constructs can'
                    '      %. comment out one of the lines by starting it with elipsis) :'
                    '      %. BreedClassMap = dictionary( ...'
                    '      %.     AsdHwEnum.Generic, "MyBlockGenericBreed", ...'
                    '      %.     ... AsdHwEnum.SharcPlusCore, "MyBlockGenericSharcPlusCoreBreed", ...'
                    '      %.     AsdHwEnum.SharcPlusAccel, "MyBlockGenericSharcPlusAccelBreed")'
                    '      BreedClassMap = dictionary() %TODO! Add mappings!'
                    '  end'
                    ''};
                newCode = strjoin(newCodeLines,newline);
                lineNumber = numel(regexp(theDoc.Text(1:insertionIndex),'\n'))+1;
                theDoc.insertTextAtPositionInLine(newCode,lineNumber,1);
                % Announce what we just did:
                hereLink = sprintf('<a href="matlab:edit(''%s'');goToLine(matlab.desktop.editor.getActive,%d);">HERE</a>', ...
                    className,lineNumber);
                MaskUtil.ReportInfo('Inserted BreedClassMap property on line %d [%s]', ...
                    lineNumber,hereLink);
            end
        else
            MaskUtil.ReportInfo('... Class [%s] already defines BreedClassMap (OK)', ...
                className);
        end
        
        % Adjust #3: add the constructor's call to super class
        [insertionIndex,thisName] = BTConfig.GetConstructorInsertionSpot(theDoc.Text,className);
        searchExp = BTConfig.CreateRegexCodeSearch([thisName '@BtcBaseClassWithBreeds']);
        foundIndex = regexp(theDoc.Text,searchExp, 'once');
        if (false == isempty(foundIndex))
            % The call to the baseclass constructor was found
            MaskUtil.ReportInfo('... Class [%s] already calls BtcBaseClassWithBreeds constructor (OK)', ...
                className);
        else
            % Call to baseclass constructor missing: add it now
            lineNumber = numel(regexp(theDoc.Text(1:insertionIndex),'\n'))+1;
            newCode = sprintf('        %s@BtcBaseClassWithBreeds();\n', ...
                thisName);
            theDoc.insertTextAtPositionInLine(newCode,lineNumber,1);
            % Announce what we just did:
            hereLink = sprintf('<a href="matlab:edit(''%s'');goToLine(matlab.desktop.editor.getActive,%d);">HERE</a>', ...
                className,lineNumber);
            MaskUtil.ReportInfo('Inserted baseclass constructor call on line %d [%s]', ...
                lineNumber,hereLink);
        end

        % Adjust #4: add a handy text block for the shared enum
        handyEnumBlock = HandyTextFileMask.AddHandyBlock(blockURL,'enum','RenameToSharedEnum');
        MaskUtil.safe_setparam(handyEnumBlock,"UseThisBlockName","on");
    end
    %----------------------------------------------------------------------
    function checkCell = CheckConfigParam(blockURL)
        % CheckConfigParam Checks if the "Config" mask parameter is all set
        % Currently This method only works for blocks with Config mask
        % parameters whose prompt is the BTC classdef name.
        % Usage:
        %    issuesCell = BTConfig.CheckConfigParam(blockURL);
        
        checkCell = {};
        
        % ----- Check #1: Check the Config mask parameter
        theMask = Simulink.Mask.get(blockURL);
        if (isempty(theMask))
            % No mask: bail!
            return;
        end
        configParam = theMask.getParameter('Config');
        if (isempty(configParam))
            % No config parameter: bail!
            return;
        end
        % Check various properties of the parameter
        if ("on" == configParam.Tunable)
            checkCell{end+1} = sprintf('Config mask param should be non-tunable');
        end
        if ("off" == configParam.Evaluate)
            checkCell{end+1} = sprintf('Config mask param must be evaluated');
        end
        % Note: Not a problem if prompt location is not on left

        % Discover the classdef name from the Config prompt
        configClassdefName = configParam.Prompt;
        
        % ----- Check #2: Add a handy-text-file config block
        % Note: Not a problem if handy block missing
        
        % ----- Check #3:  out-of-date versions of the code
        for i=1:numel(BTConfig.OldAssertCode)
            code = BTConfig.OldAssertCode{i};
            [~, wasRemoved] = ...
                BTConfig.RemoveCode(theMask.Initialization,code);
            if (wasRemoved)
                checkCell{end+1} = sprintf(['Old mask initialization assertion ' ...
                    'code could be removed [%s]'],code);
            end
        end
        
        % ----- Check #4:  code to the top (list in reverse order)
        [~, wasAdded] = BTConfig.AddCode(theMask.Initialization,BTConfig.AssertLine,BTConfig.AddToStart);
        if (wasAdded)
            checkCell{end+1} = sprintf(['Mask initialization code ' ...
                'should be added [%s]'],BTConfig.AssertLine);
        end
        [~, wasAdded] = BTConfig.AddCode(theMask.Initialization,BTConfig.FirstLine,BTConfig.AddToFirst);
        if (wasAdded)
            checkCell{end+1} = sprintf(['Mask initialization code ' ...
                'should be added [%s]'],BTConfig.FirstLine);
        end
        
        % ----- Check #5: FeaturePath/Config-path alignment
        for i=1:numel(theMask.Parameters)
            aParam = theMask.Parameters(i);
            [initCode,callbackCode] = BTConfig.IsFeaturePathParam(theMask, aParam);
            if (false == isempty(initCode))
                % Step 1: Init code
                % (First look for old gcb code)
                [~,removeMsgsCell] = BTConfig.RemoveOldCodeVersions( ...
                    theMask.Initialization, initCode, ...
                    {'blkh' 'blkh'},{'gcb','gcbh'},['Old mask initialization ' ...
                    'code for FeaturePath alignment should be removed']);
                checkCell = horzcat(checkCell,removeMsgsCell);
                % (Then for new blkh code)
                [~, wasAdded] = BTConfig.AddCode( ...
                    theMask.Initialization, initCode, BTConfig.AddToEnd);
                if (wasAdded)
                    checkCell{end+1} = sprintf(['Mask initialization code ' ...
                        'for FeaturePath alignment is missing [%s]'], initCode);
                end
                % Step 2: Callback code
                % (First look for old gcb code)
                [~,removeMsgsCell] = BTConfig.RemoveOldCodeVersions( ...
                    configParam.Callback, callbackCode, ...
                    {'gcbh'},{'gcb'},['Old Config param callback code for ' ...
                    'FeaturePath alignment should be removed']);
                checkCell = horzcat(checkCell,removeMsgsCell);
                % (Then check new code)
                [~,wasAdded] = BTConfig.AddCode(configParam.Callback, callbackCode, BTConfig.AddToEnd);
                if (wasAdded)
                    checkCell{end+1} = sprintf(['Config param callback FeaturePath ' ...
                        'alignment code is missing [%s]'], callbackCode);
                end
            end
        end
        
        mco = meta.class.fromName(configClassdefName);        
        if (isempty(mco))
            checkCell{end+1} = sprintf(['Configuration class [%s] does ' ...
                'not exist yet (or not on path)!'], configClassdefName);
        else
            % ----- Check #6: Call Validate
            % Check call to the Validate method in mask initialization
            code = BTConfig.ValidateLine;
            if false == isempty(findobj(mco.MethodList,'Name','Validate'))
                [~, wasAdded] = BTConfig.AddCode(theMask.Initialization, code, false);
                if (wasAdded)
                    checkCell{end+1} = sprintf(['Mask initialization call to Validate ' ...
                        'must be added [%s].'], code);
                end
            else
                    checkCell{end+1} = sprintf(['Configuration class [%s] does ' ...
                        'not have a Validate method.'], configClassdefName);
            end
            % ----- Check #7: Call DoMaskInit
            % Check call to the DoMaskInit method in mask initialization
            code = BTConfig.DoMaskInitLine;
            if false == isempty(findobj(mco.MethodList,'Name','DoMaskInit'))
                [~, wasAdded] = BTConfig.AddCode(theMask.Initialization, code, false);
                if (wasAdded)
                    checkCell{end+1} = sprintf(['Mask initialization call to DoMaskInit ' ...
                        'must be added [%s].'], code);
                end
            else
                    checkCell{end+1} = sprintf(['Configuration class [%s] does ' ...
                        'not have a DoMaskInit method.'], configClassdefName);
            end
        end
    end
    %----------------------------------------------------------------------
    function AutomateCreateModelConfig(blockHandle)
        % AutomateCreateModelConfig creates model-level BTC.
        % Creates <root-model-name>Config classdef and sets up model's 
        % InitFcn to create a variable in the base workspace
        % Usage:
        %    BTConfig.AutomateCreateModelConfig(gcb);
        %
        % See also BTConfig.GetModelSettings
        
        modelName = bdroot(getfullname(blockHandle));
        myStage = sldiagviewer.createStage('TalariaModelConfig','ModelName', ...
            modelName); %#ok<NASGU> 

        if (bdIsLibrary(modelName))
            error(['Diagram [%s] is a library and so unable to support model-' ...
                'level build-time configuration. Instead, create mask-level ' ...
                'configurations for the block(s) inside the library.'], modelName);
        end
        
        % ----- Automate #0: Ensure the model name is a good start
        % If the models's root name does not meet WikiCase guidelines,
        % this routine exits with an error.
        rootName = ModelRootName(modelName);
        wikiName = MaskUtil.CreateWikiName(rootName);
        if (false == strcmp(rootName, wikiName))
            error('Model name violation [%s]. Try [%s]\n', modelName, wikiName);
        end
        % ----- Automate #1: Create the Config mask parameter
        % Alas, no can do. Model-level parameters must be inherent numeric
        % types and cannot support classdef objects used for BTC.
        
        % ----- Automate #2: Add a handy-text-file config block, and create
        % the class text file (but only if not already found)
        htfBlock = HandyTextFileMask.AddHandyBlock(modelName,'Config',wikiName);
        set(htfBlock,'ShowName','off');
        [filename,wasCreated] = HandyTextFileMask.CreateFileIfMissing(htfBlock);
        if (wasCreated)
            MaskUtil.ReportInfo('Created default classdef file [%s]\n',filename);
            % Rehash to refresh the caches of found classdefs
            rehash;
        else
            MaskUtil.ReportInfo('... class definition file already exists. (OK)\n');
        end
        
        % ----- Automate #3: Remove out-of-date versions of the code
        % TODO. There are none so far...
        
        % ----- Automate #4: Add code to the top (list in reverse order)
        initialCode = get_param(modelName,'InitFcn');
        configVarName = [rootName BTConfig.ModelVarNameEnd];
        assertCode = sprintf(BTConfig.ModelAssertLine,configVarName);
        newCode = BTConfig.AnnounceCodeAdd(initialCode,assertCode,BTConfig.AddToStart,'Model InitFcn Assert');
        MaskUtil.safe_setparam (modelName,'InitFcn',newCode);
        
        % ----- Automate #5: FeaturePath/Config-path alignment
        % Not done for models

        configClassdefName = [rootName 'Config'];
        mco = meta.class.fromName(configClassdefName);        
        if (isempty(mco))
            MaskUtil.ReportWarning(['Configuration class [%s] does ' ...
                    'not exist yet (or not on path)!'], configClassdefName);
        else
            % ----- Automate #6: Call Validate
            % Setup a call to the Validate method in model initialization
            if (false == isempty(findobj(mco.MethodList,'Name','Validate')))
                initialCode = get_param(modelName,'InitFcn');
                insertCode = sprintf(BTConfig.ModelValidateLine,configVarName);
                newCode = BTConfig.AnnounceCodeAdd(initialCode,insertCode,BTConfig.AddToEnd,'Model InitFcn Validate');
                MaskUtil.safe_setparam (modelName,'InitFcn',newCode);
            else
                MaskUtil.ReportWarning(['Configuration class [%s] does ' ...
                    'not have a Validate method.'], configClassdefName);
            end
            % ----- Automate #7: Call DoMaskInit
            % Setup a call to the DoMaskInit method in mask initialization
            if (false == isempty(findobj(mco.MethodList,'Name','DoMaskInit')))
                initialCode = get_param(modelName,'InitFcn');
                insertCode = sprintf(BTConfig.ModelDoMaskInitLine,configVarName,configVarName);
                newCode = BTConfig.AnnounceCodeAdd(initialCode,insertCode,BTConfig.AddToEnd,'Model InitFcn DoMaskInit');
                MaskUtil.safe_setparam (modelName,'InitFcn',newCode);
            else
                MaskUtil.ReportWarning(['Configuration class [%s] does ' ...
                    'not have a DoMaskInit method.'], configClassdefName);
            end
        end

        % ----- Final step for model-level settings : initialize them now!
        BTConfig.GetModelSettings(modelName);

        MaskUtil.ReportInfo('Done.');
    end
    %----------------------------------------------------------------------
    function [hasConfig, configVarName] = HasModelLevelConfig(aBlockURL)
        % HasModelLevelConfig returns true if it looks like the model is
        % set up for model-level BTC as evidenced by InitFcn code
        % Usage:
        %   [hasConfig, configVarName] = HasModelLevelConfig(aBlockURL);
        modelName = bdroot(getfullname(aBlockURL));

        % Previously, this code only looked for what it thought was the
        % only "correct" code. Instead now is searches for typical code and
        % learns the variable chosen by the model instead of enforcing only
        % one possible variable name.
        requiredCode = sprintf(BTConfig.ModelAssertLine,'');
        requiredCodeExp = BTConfig.CreateRegexCodeSearch(requiredCode);
        searchExp = ['(?<VARNAME>\w*)(?<ASSIGNMENT>' requiredCodeExp ')'];

        currentCode = get_param(modelName,'InitFcn');
        results = regexp(currentCode,searchExp,'names');

        hasConfig = (false == isempty(results));
        if (hasConfig)
            configVarName = results(1).VARNAME;
        else
            configVarName = [];
        end
    end
    %----------------------------------------------------------------------
    function AddChildBlockConfig(theChildBlockHandle)
        % AddChildBlockConfig Attempts to add a component parameter of the
        % specified child block's config class to the config class of the
        % parent block found up the subsystem hierarchy.

        % Use the diagnostic viewer for status messages
        modelName = bdroot(getfullname(theChildBlockHandle));
        myStage = sldiagviewer.createStage('TalariaAddChild','ModelName', ...
            modelName); %#ok<NASGU> 

        childName = getfullname(theChildBlockHandle);
        rootName = ModelRootName(childName);

        % Acquire masks
        childMask = Simulink.Mask.get(theChildBlockHandle);
        parentMask = MaskUtil.FindParentMask(theChildBlockHandle);

        % Verify child block
        if (isempty(childMask))
            error('Block ''%s'' does not have a mask. Unable to proceed', ...
                childName);
        end
        % Child mask found... ensure it has Config parameter
        childBlockObj = childMask.getOwner;
        if (false == isfield(childBlockObj.DialogParameters,'Config'))
            error('Block ''%s'' does not have a Config parameter. Unable to proceed', ...
                childName)
        end
        childConfigClass = childBlockObj.DialogParameters.Config.Prompt;

        % Verify parent block
        if (isempty(parentMask) || (false == parentMask.isMaskWithDialog))
            % No parent mask - Check if the model is setup for model-level
            [hasModelLevel, configVarName] = BTConfig.HasModelLevelConfig(theChildBlockHandle);
            if (false == hasModelLevel)
                error('Unable to find parent BTC mask or model-level configuration setup');
            end
            % Special handling for model-level configuration. Check if
            % there's a configuration class based on the model name
            parentConfigClass = [rootName 'Config'];
            if (8 ~= exist(parentConfigClass,'class'))
                error('Unable to find model-level configuration class [%s]', ...
                    parentConfigClass);
            end
            MaskUtil.ReportInfo('Using model-level configuration class [%s]', ...
                parentConfigClass);
        else
            % Parent mask found... ensure it has Config parameter
            parentBlockObj = parentMask.getOwner;
            parentConfigClass = parentBlockObj.DialogParameters.Config.Prompt;
            configVarName = 'Config';
            if (false == isfield(parentBlockObj.DialogParameters,'Config'))
                error('Masked parent block ''%s'' does not have a Config parameter. Unable to proceed', ...
                    parentBlockObj.getFullName)
            end
            MaskUtil.ReportInfo('Using block-mask configuration class [%s]', ...
                parentConfigClass);
        end

        % Report out the plans so far:
        componentPropertyName = MaskUtil.CreateWikiName(childBlockObj.Name);
        MaskUtil.ReportInfo('Intending to add property [%s] with type [%s]', ...
            componentPropertyName, childConfigClass);

        % The child block about the configuration property
        configValue = sprintf('%s.%s',configVarName,componentPropertyName);
        [changed,oldValue] = MaskUtil.safe_setparam(childName,'Config',configValue);
        if (changed)
            MaskUtil.ReportInfo('Updated %s from [%s] to [%s]', ...
                MaskUtil.GetBlockLinkString(childName,'child Config'), ...
                oldValue,configValue);
        end

        % Add the component property to the parent class
        if (BTConfig.AddClassdefProperty(parentConfigClass, ...
            componentPropertyName,childConfigClass))
            % Remind that these edits are in the MATLAB editor undo stack
            MaskUtil.ReportInfo('While the editor remains open, the "undo" buffer is able to reverse these edits if desired...');
            MaskUtil.ReportInfo('Done.');        
        end
    end
    %----------------------------------------------------------------------
    function AddAllTuneVars(blockHandle, useSeparateTuneConfigClass)
        % AddAllTuneVars adds BTC for default values of internal TuneVars.
        % This method discovers TuneVars inside the specified block and
        % adds default values for them as properties in a classdef. There 
        % are two modes of operation: same class and separate class.
        % Main class mode adds "TuneVar" prefixed properties to the same 
        % main BTC classdef for the block.
        % Separate class mode adds properties inside a separate classdef 
        % just for TuneVar defaults entitled <block>TuneConfig. A component
        % property inside the block's main BTC classdef called TuneDefaults
        % is also set up to construct this separate class.
        % Usage:
        %    BTConfig.AddAllTuneVars(blockHandle, useSeparateTuneConfigClass)
        % useSeparateTuneConfigClass when true uses separate class mode.
        % Otherwise when false, this method uses main class mode.
        if (nargin < 2)
            useSeparateTuneConfigClass = false;
        end

        % Use the diagnostic viewer for status messages
        modelName = bdroot(getfullname(blockHandle));
        myStage = sldiagviewer.createStage('TalariaAddAllTuneVars','ModelName', ...
            modelName); %#ok<NASGU> 
        
        if ("SubSystem" ~= get_param(blockHandle,'BlockType'))
            error('This method only works on SubSystem blocks.');
        end
        
        % Verify block's Config class
        configClassName = BTConfig.GetConfigClassName(blockHandle);

        btcVariable = 'Config';
        if (false == useSeparateTuneConfigClass)
            % Use the block's config class to hold TuneVar values
            tuneClassName = configClassName;
            btcVariableContainer = '';
        else
            % Use a seprate TuneConfig class via a component property.
            % First, create/verify the TuneConfig class (via HandyTextFile)
            htfBlock = HandyTextFileMask.AddHandyBlock(blockHandle,'TuneConfig');
            [filename,wasCreated] = HandyTextFileMask.CreateFileIfMissing(htfBlock);
            if (wasCreated)
                MaskUtil.ReportInfo('Created TuneConfig classdef file [%s]\n',filename);
                % Rehash to refresh the caches of found classdefs
                rehash;
            else
                MaskUtil.ReportInfo('... class definition file already exists. (OK)\n');
            end
    
            % Then insert TuneDefaults property into the block Config class
            tuneClassName = extractBefore(filename,'.');
            btcVariableContainer = '.TuneDefaults';
            % Use string substitution for local variable "thisName" present
            % in the AddClassdefProperty call (because we know it... this
            % is all part of the same class)
            wasAdded = BTConfig.AddClassdefProperty(configClassName,'TuneDefaults',tuneClassName,'($(thisName))');
        end

        % Get all the TuneVars - but not inside links. Assume that links
        % have already created classdefs for their settings.
        F = Simulink.FindOptions();
        tuneVarBlocks = Simulink.findBlocks(blockHandle,'MaskType',TuneVarMask.MaskType,F);
        tuneTopBlocks = Simulink.findBlocks(blockHandle,'MaskType',TuneTOP_Mask.MaskType,F);
        allTuneBlocks = vertcat(tuneVarBlocks,tuneTopBlocks);

        % Find the block's feature path - as witnessed by any block INSIDE
        % of its subsystem.
        F = Simulink.FindOptions("SearchDepth",1);
        insideBlocks = Simulink.findBlocks(blockHandle,F);
        blockFeaturePathInfo = FeaturePathUtil.GetPathInfo(insideBlocks(1));
        blockFeaturePath = blockFeaturePathInfo.featurePath;

        % For each TuneVar, add a component property for its default
        % Leave the closing parenthesis off the property attributes in case
        % the GetObservable attributes are present.
        propertiesLine = 'properties(Description=''TuneDefault''';
        for i=1:numel(allTuneBlocks)
            % Get the Tune block's featurePath
            aTuneBlock = allTuneBlocks(i);
            featurePath = TuneVarMask.GetActiveTunePath(aTuneBlock);
            if (isempty(featurePath))
                error('FeaturePath values have not resolved yet');
            else
                withinBlockFeaturePath = extractAfter(featurePath,blockFeaturePath);
                internalPath = MaskUtil.CreateWikiName(withinBlockFeaturePath);
            end
            % If adding to the main Config class, prefix with "TuneVar"
            if (false == useSeparateTuneConfigClass)
                internalPath = ['TuneVar' internalPath];
            end
            % Attempt to retrieve the current TuneVar default for use in
            % property declaration
            [currDefault,wasFound] = MaskUtil.GetEvaluatedParameter(aTuneBlock,'MaskValue');
            if (wasFound && false == isempty(currDefault))
                defaultValueStr = MaskUtil.ValueToStr(currDefault);
            else
                defaultValueStr = [];
            end

            wasAdded = BTConfig.AddClassdefProperty(tuneClassName,internalPath,[],defaultValueStr,propertiesLine);            

            % Regardless of whether the property was added, ensure the
            % TuneVar's default value now receives from the property
            configValue = sprintf('%s%s.%s',btcVariable,btcVariableContainer,internalPath);
            [changed,oldValue] = MaskUtil.safe_setparam(aTuneBlock,'MaskValue',configValue);
            if (changed)
                aBlockUrl = getfullname(aTuneBlock);
                linkText = sprintf('TuneVar %s', featurePath);
                MaskUtil.ReportInfo('Updated %s from [%s] to [%s]', ...
                    MaskUtil.GetBlockLinkString(aBlockUrl,linkText), ...
                    oldValue,configValue);
            end
        end

        % Remind that these edits are in the MATLAB editor undo stack
        if (wasAdded)
            rehash;
            MaskUtil.ReportInfo('While the editor remains open, the "undo" buffer is able to reverse these edits if desired...');
        end
        
    end
    %----------------------------------------------------------------------
    function [fullVariableName,isConfigurable] = DiscoverSourceVariable(aBlock,maskParamName)
        % DiscoverSourceVariable Returns variable name providing BTC
        % Given a block and initial mask-parameter name, searches up
        % through the subsystem hierarchy to find the full property path
        % providing the build-time-configuration data.
        % Usage:
        %  [fullVariableName,isConfigurable] = BTConfig.DiscoverSourceVariable(aBlock,maskParamName)
        % fullVariableName: string name of the controlling variable
        % isConfigurable: boolean true if the search landed in global data
        % aBlock: block-handle or Simulink-block-path of the block
        % maskParamName: name of block's mask parameter accepting BTC (e.g.
        %   'Config' or 'MaskValue')
        %
        % See also: BTConfig.PrintDiscoveredBtc
        modelName = ModelFullName(aBlock);
        isConfigurable = false;
        varNamePath = cell.empty;
        maximumDepth = 1000; % protect against infinite loop
        % Loop up through Mask layers until reaching the top
        for i=1:maximumDepth
            % Get the value from this block and break on the first "."
            maskParamNames = get_param(aBlock,'MaskNames');
            if (false == ismember(maskParamName,maskParamNames))
                if (i==1)
                    % The specified mask variable isn't found in the
                    % specified block. This is an error
                    error('Parameter [%s] not found in block [%s]', ...
                        maskParamName,aBlock);
                else
                    % Oops - this mask does not contain this parameter! We are
                    % stumped and so consider this TuneVar non-configurable
                    varNamePath{end+1} = maskParamName;
                    break;
                end
            end
            theValue = get_param(aBlock,maskParamName);
            if (contains(theValue,'@'))
                % This BTC property is using the function handle naming for the
                % propertyPath. Pull out just what's inside the parens
                searchExp ='(?<Call>BtcGet|\w*\.Get)\(@(?<Path>.*)\)';
                results = regexp(theValue,searchExp,'names');
                if (isempty(results))
                    error('Failed to parse TuneVar BTC propertyPath expression [%s] found in [%s]', ...
                        theValue,MaskUtil.GetBlockErrorLink(aBlock));
                else
                    theValue = results.Path;
                end
            end
            maskParamName = extractBefore(theValue,'.');
            newPathPiece = extractAfter(theValue,'.');

            % Check if for a mask above the current block
            [parentMask,~,parentName] = MaskUtil.FindParentMask(aBlock);

            % Check if all done
            % Do not be fooled by model-level system masks. These are
            % identified by isMaskWithDialog being false, and can be ignored
            % for TuneVar defaults with this simple search.
            if (isempty(parentMask) || (false == parentMask.isMaskWithDialog))
                % We've reached the top. All done
                if (isempty(maskParamName))
                    varNamePath{end+1} = theValue;
                    maskParamName = theValue;
                else
                    varNamePath{end+1} = newPathPiece;
                    varNamePath{end+1} = maskParamName;
                end
                % Check if the expression is a variable that exists
                if (Simulink.data.existsInGlobal(modelName,maskParamName))
                    isConfigurable = true;
                end
                break;
            end

            % Not all done- gather parts and loop again
            if (isempty(maskParamName))
                % This level was not a component property - take it all
                maskParamName = theValue;
            else
                % This level *is* component property - add piece to path
                varNamePath{end+1} = newPathPiece;
            end
            aBlock = parentName;
        end
        fullVariableName = strjoin(flip(varNamePath),'.');
    end
    %----------------------------------------------------------------------
    function PrintDiscoveredBtc(aBlock)
        % PrintDiscoveredBtc attempts to print the BTC variable for a block
        % Usage:
        %   BTConfig.PrintDiscoveredBtc(aBlock);
        %
        % See also: BTConfig.DiscoverSourceVariable

        % Setup diagnostic stage for printouts
        modelName = ModelFullName(aBlock);        
        myStage = sldiagviewer.createStage('TalariaSeeBtc','ModelName', ...
            modelName); %#ok<NASGU> 
        
        % Step One: Figure out what type of block has been provided
        % Start by acquiring a mask
        maskedBlock = aBlock;
        theMask = Simulink.Mask.get(maskedBlock);
        if (false == isempty(theMask))
            maskType = theMask.Type;
        else
            maskType = '';
        end

        % Based on the block type (or lack thereof)
        switch (maskType)
            case {TuneTOP_Mask.MaskType,TuneVarMask.MaskType}
                % TuneVar wants to know about their MaskValue
                maskParamName = 'MaskValue';
            otherwise
                % Everything else is just Config variables
                maskParamName = 'Config';
        end

        % Search up the hierarchy to find a mask with the parameter
        theParameter = [];
        while (false == isempty(maskedBlock) && isempty(theParameter))
            % Try to find the parameter is we have a mask
            if (false == isempty(theMask))
                theParameter = theMask.getParameter(maskParamName);
            end
            % Move up the hierarchy if we haven't found the parameter
            if (isempty(theParameter))
                [theMask,~,parentName] = MaskUtil.FindParentMask(maskedBlock);
                maskedBlock = parentName;
            end
        end

        % Acquire information about model-level configuration - might want.
        [hasModelBtc,modelVar] = BTConfig.HasModelLevelConfig(aBlock);
        if (hasModelBtc)
            % Clicked hypertext links execute in a callback function
            % context, so they cannot set base workspace variables unless
            % explicitly enabled to access the base workspace. Here just
            % the simple assignin 'base' does it.
            mulliganLink = sprintf(['<a href="matlab:assignin(''base'',''%s'',BTConfig.GetModelSettings(''%s''));' ...
                'BTConfig.PrintDiscoveredBtc(''%s'')">%s</a>'], ...
                modelVar, aBlock, aBlock,...
                '(Create model settings and try again)');
        else
            mulliganLink = '';
        end

        % Ensure we have a mask
        if (isempty(theMask) || (false == theMask.isMaskWithDialog))
            % Could not find BTC from this block. Graceful fallback to
            % model-level or just give error.
            if (hasModelBtc)
                % Since this model has model-level config, just show that
                MaskUtil.ReportWarning('Could not find [%s] at or above block [%s].', ...
                    maskParamName,MaskUtil.GetBlockLinkString(aBlock));
                if (false == Simulink.data.existsInGlobal(bdroot(aBlock),modelVar))
                    MaskUtil.ReportInfo(mulliganLink);
                else
                    MaskUtil.ReportInfo('Opening tree view for entire model');
                    BtcTreeView.CreateOrUpdateTree(modelVar);
                end
                return; % Bail out here. We've done all that we can.
            else
                error('Could not find a mask on or above [%s] with parameter [%s]', ...
                    MaskUtil.GetBlockLinkString(aBlock),maskParamName);
            end
        end

        % Child mask found... try to figure out which mask parameter is the
        % one the user intends to print out

        % Acquire the block handle and discover the btc variable name
        maskedBlockObj = theMask.getOwner;  
        maskedBlockH = maskedBlockObj.Handle;
        maskBlockPath = getfullname(maskedBlockH);        
        [btcVarName,isGlobal] = BTConfig.DiscoverSourceVariable(maskedBlockH,maskParamName);

        % Create a clickable link to evaluate the BTC variable
        viewVarLink = sprintf('<a href="matlab:openvar(''%s'');">%s</a>', ...
            btcVarName,btcVarName);
        cmdLineDumpVarLink = sprintf('<a href="matlab:disp(''%s'');disp(''%s'');disp(Simulink.data.evalinGlobal(''%s'',''%s''));">%s</a>', ...
            '------------------------------------------',btcVarName, ...
            modelName,btcVarName,'Display in Command Window');
        treeViewLink = sprintf('<a href="matlab:BtcTreeView.CreateOrUpdateTree(''%s'');">%s</a>', ...
            btcVarName,'Display in tree view');
        if (isGlobal)
            % Normal good case: the variable can be set
            accessibilityText = '';
            % Since the variable is found, open it up in tree view
            BtcTreeView.CreateOrUpdateTree(btcVarName);
        else
            % Legacy wrap or not-yet-initialized case: no global view of it
            accessibilityText = ' (currently inaccessible)';
        end

        % Print it out
        MaskUtil.ReportInfo('----------- BTC Block Report ----------\n')
        MaskUtil.ReportInfo('Masked Block   : %s',MaskUtil.GetBlockLinkString(maskBlockPath));
        MaskUtil.ReportInfo('Mask Parameter : %s',maskParamName);
        MaskUtil.ReportInfo('Source Variable: %s%s',viewVarLink,accessibilityText);
        MaskUtil.ReportInfo([cmdLineDumpVarLink '     ' treeViewLink]);
        % Dump the control variable's value - beware it might not exist yet
        MaskUtil.ReportInfo('Current Value:');
        try
            specifiedValue = Simulink.data.evalinGlobal(modelName,btcVarName);
            if (isempty(specifiedValue))
                % For empty values, go the extra step to show any
                % calculated default:
                MaskUtil.ReportInfo('    [ ]');
                MaskUtil.ReportInfo('Calculated Default:');
                calculatedValue = Simulink.data.evalinGlobal(modelName,['BtcGet(@' btcVarName ')']);
                MaskUtil.ReportInfo(formattedDisplayText(calculatedValue));
            else
                MaskUtil.ReportInfo(formattedDisplayText(specifiedValue));
            end
        catch
            % The variable is not found, but if model-level BTC is enabled,
            % supply a convenient "do it now" link.
            MaskUtil.ReportWarning('Variable not available in global data %s', mulliganLink);
        end
    end
    %----------------------------------------------------------------------
    function PreValidate(btcObject,propertyPath)
        % PreValidate runs through the provided BTC object and calls
        % Validate on every component that includes a function called
        % "Validate". This routine is for debugging only and is NOT part of
        % the normal flow. The Simulink block mask initialization code is
        % the expected caller of Validate. This routine just attempts to
        % save time by finding any problems faster without updating the
        % entire model.
        % Usage:
        %   BTConfig.PreValidate(aBtcSettingsObject)
        arguments
            btcObject BtcBaseClass
            propertyPath string = string.empty
        end
        % If there is no property path yet, start with the name of the
        % variable that is passed in
        if (isempty(propertyPath))
            propertyPath = inputname(1);
        end
        
        % Determine the class and acquire its meta class information
        theClassName = class(btcObject);
        mco = metaclass(btcObject);
        if (isempty(mco))
            MaskUtil.ReportError('Unable to analyze class [%s].', theClassName);
            return;
        end

        % Call Validate. Since btcObject is a BtcBaseClass derivative, it
        % is known that Validate is a member function
        fprintf('Validating %s (%s)...\n',theClassName,propertyPath);
        btcObject.Validate();
        
        % Loop through public get properties and look at them
        propList = mco.PropertyList;
        for i=1:numel(propList)
            % Investigate only public-viewable properties (canGet)
            aMetaPropertyObject = mco.PropertyList(i);
            canGet = BtcBaseClass.GetPropertyStats(aMetaPropertyObject);
            if (canGet)
                propName = aMetaPropertyObject.Name;
                childPropPath = propertyPath + "." + propName;
                aPropValue = btcObject.(propName);
                if (isa(aPropValue,"BtcBaseClass"))
                    BTConfig.PreValidate(aPropValue,childPropPath);
                end
            end
        end
    end

    end % methods Static public
    %----------------------------------------------------------------------
    %----------------------------------------------------------------------
    methods (Static, Access=protected)
    %----------------------------------------------------------------------
    %----------------------------------------------------------------------
    function [newCode,wasAdded] = AddCode(currentCode,addedCode,addTo)
        % AddCode Adds a snippet of code into an existing code string if
        % the snippet is not already present. 
        % Usage: newCode = BTConfig.AddCode(currentCode,addedCode,addTo);
        %  addTo : one of the following options:
        %       BTConfig.AddToEnd,BTConfig.AddToStart,BTConfig.AddToFirst
        
        % Ensure inputs are converted to char. Strings behave differently
        currentCode = char(currentCode);
        addedCode = char(addedCode);
        
        newCode = currentCode;
        wasAdded = false;
        
        % Try to find the code that should be added
        addSearchExp = BTConfig.CreateRegexCodeSearch(addedCode);
        foundStartIdx = regexp(currentCode,addSearchExp,'once');
        notFound = isempty(foundStartIdx);
        
        % Take action based on where the code should be added
        switch (addTo)
            case BTConfig.AddToEnd
                % If not found, add code to the end of current code
                if (notFound)
                    newCode = [currentCode newline addedCode];
                    wasAdded = true;
                end
            case BTConfig.AddToStart
                % If not found, add code to the top of current code
                if (notFound)
                    newCode = [addedCode newline currentCode];
                    wasAdded = true;
                end
            case BTConfig.AddToFirst
                % If found somewhere other than the first line, remove it!
                if (foundStartIdx > 1)
                    currentCode = BTConfig.RemoveCode(currentCode,addedCode);
                    % Not found anymore ;-)
                    notFound = true;
                end
                % If not found (or removed just above) add it to the top
                if (notFound)
                    newCode = [addedCode newline currentCode];
                    wasAdded = true;
                end
            otherwise
                error('Unknown AddCode option %d',addTo);
        end

    end
    %----------------------------------------------------------------------
    function [newCode, wasRemoved] = RemoveCode(currentCode, codeExpression)
        % Searches current code for provided code and removes it if found
        % Usage: BTConfig.RemoveCode(currentCode, codeExpression)
        currentCode = char(currentCode);
        searchExp = BTConfig.CreateRegexCodeSearch(codeExpression);
        % Get the ending newline also
        searchExp = strcat(searchExp,'\s*[\n\r]+');
        newCode = regexprep(currentCode,searchExp,'');
        wasRemoved = (false == strcmp(newCode,currentCode));
    end
    %----------------------------------------------------------------------
    function searchExp = CreateRegexCodeSearch(codeExpression)
        %CreateRegexCodeSearch Create regex search expression for code
        % Adds possible whitespace to the provided code expression and
        % escapes characters that cause problems.
        codeExpression = char(codeExpression);
        
        % Create a regex expression that matches the following four
        % whitespace-accepting delimiters outside quotes: = , ( )
        % It looks funny because we have to escape the parenthesis "\{"
        % and the single quotes "''"
        unquotedDelimeters = '[=,\(\)](?=([^'']*''[^'']*'')*[^'']*$)';

        % Decorate the code expression with possible whitespace
        searchExp = regexprep(codeExpression,unquotedDelimeters,'\\s\*$0\\s\*');
        
        % Also escape any parenthesis and periods in the code expression
        searchExp = regexprep(searchExp,'([\.\(\)])','\\$0');

        % Add a negative lookback to ensure the line is not commented
        searchExp = ['(?<!%[^\n]*)' searchExp];
    end
    %----------------------------------------------------------------------
    function [newCode,removeMsgsCell] = RemoveOldCodeVersions(currentCode,goodCodeExpression, ...
            goodWordsCell,badWordsCell,msgText)
        % RemoveOldCodeVersions looks for older versions of code where
        % goodWords used to be badWords
        removeMsgsCell = {};
        newCode = char(currentCode);
        for i=1:numel(goodWordsCell)
            % Create the old code expression to be removed
            goodSearch = ['\<' goodWordsCell{i} '\>'];
            badReplace = badWordsCell{i};
            badExpression = regexprep(goodCodeExpression,goodSearch,badReplace);
            % Do the removal
            [newCode,wasRemoved] = BTConfig.RemoveCode(newCode,badExpression);
            if (wasRemoved)
                removeMsgsCell{end+1} = sprintf('%s [%s]', ...
                    msgText, badExpression);
            end
        end
    end
    %----------------------------------------------------------------------
    function [initCode,callbackCode] = IsFeaturePathParam(theMask, paramObject)
        % IsFeaturePathParam returns code for insertion if the provided
        % parameter object is a promoted feature path, empties otherwise.
        % [initCode,callbackCode] = BTCOnfig.IsFeaturePathParam(theMask, paramObject)
        initCode = [];
        callbackCode = [];
        if (paramObject.Type == "promote")
            % Search down to the original block and parameter
            [origParam,origBlock] = MaskUtil.FindPromotedParameter( ...
                theMask,paramObject.Name);
            [~,origBlockName] = fileparts(origBlock);
            origParamName = origParam.Name;
            % If it's a promoted FeaturePath name, then setup alignment
            if (strcmp(origBlockName,'FeaturePath') && ...
                strcmp(origParamName,'FeatureName'))
                % [TALARIA-269] alignment between config-data-path and feature-path
                % Step 1: Snap the feature path into alignment if config specified
                % by adding to the mask initialization code
                initCode = sprintf("BTConfig.InitSetFeatureIfPresent(blkh,'%s');", ...
                    paramObject.Name);
                % Step 2: Hide the feature path entry box when config specified
                callbackCode = sprintf("BTConfig.CallbackHideIfPresent(gcbh,{'%s'});", ...
                    paramObject.Name);        
            end
        end
    end
    %----------------------------------------------------------------------
    function theCode = AnnounceCodeAdd(theCode,lineOfCodeToAdd,addTo,description)
        % Adds code and announces (MaskUtil.ReportInfo) what happens
        [theCode,wasAdded]=BTConfig.AddCode(theCode,lineOfCodeToAdd,addTo);
        if (wasAdded)
            MaskUtil.ReportInfo('Added %s code [%s]\n',description,lineOfCodeToAdd);
        else
            MaskUtil.ReportInfo('... %s code already present [%s]\n',description,lineOfCodeToAdd);
        end
    end
    %----------------------------------------------------------------------
    function configClassName = GetConfigClassName(blockHndlOrClassdefName)
        % GetConfigClassName returns the classdef name or emits error.
        % This method does not create or assume the name, but rather grabs
        % it from the block mask's Config parameter or passes through
        % inputs that are not blocks paths/handles.
        % Usage:
        %   BTConfig.GetConfigClassName(blockHndlOrClassdefName)
        % If blockHndlOrClassdefName is a block handle or path, the prompt
        % of the "Config" mask parameter is returned. Otherwise, the
        % assumption is that the specified value is itself a classdef name
        if ((isnumeric(blockHndlOrClassdefName) && ...
            ishandle(blockHndlOrClassdefName)) || ...
            (ischar(blockHndlOrClassdefName) && ...
            ishandle(getSimulinkBlockHandle(blockHndlOrClassdefName))))
            % Block specified: Use prompt of mask's "Config" parameter
            dlgParams = get_param(blockHndlOrClassdefName,'DialogParameters');
            if (false == isfield(dlgParams,'Config'))
                error('Block [%s] must have a "Config" parameter to proceed.', ...
                    MaskUtil.GetBlockErrorLink(blockHndlOrClassdefName));
            end    
            % Parameter found. Use the prompt as a class name
            configClassName = dlgParams.Config.Prompt;
        else
            % Direct classdef name mode:
            configClassName = char(blockHndlOrClassdefName);
            % In this mode, the name must exist already
            if (8~= exist(configClassName, "class"))
                warning('Class [%s] does not exist',configClassName);
            end
        end
    end
    %----------------------------------------------------------------------
    function addedIt = AddClassdefProperty(className,newPropertyName,componentConfigClass,defaultValueStr,propertiesLine)
        % AddClassdefProperty adds a property to a classdef.
        % Usage:
        %  addedIt = AddClassdefProperty(className,newPropertyName,componentConfigClass,defaultValueStr,propertiesLine)
        % className is the name of the classdef to receive the new property
        % newPropertyName is... well you know
        % componentConfigClass is the classdef type of the new property -
        % if not specified or empty, then no class specification or 
        % constructor initialization will be inserted.
        % Returns addedIt = true if modifications to the className are mode
        if (nargin < 3)
            componentConfigClass = [];
        end
        if (nargin < 4)
            defaultValueStr = [];
        end
        if (nargin < 5)
            propertiesLine = 'properties';
        end
        addedIt = true;

        % Check if the classdef already has the property
        mco = meta.class.fromName(className);
        existingProperty = findobj(mco.PropertyList,'Name',newPropertyName);
        if (false == isempty(existingProperty))
            MaskUtil.ReportWarning('Propery [%s] already exists for class [%s] so classdef is unmodified.', ...
                newPropertyName,MaskUtil.GetFileEditLink(className));
            % Since the property is already present, do nothing. Return.
            addedIt = false;
            return;
        end
        
        % Ready to proceed: Load the parent class and find insertion spots
        edit(className);
        theDoc = matlab.desktop.editor.findOpenDocument(className + ".m");
        
        % Find insertion for declaration of property
        % Functional comments signal where to insert into the code. The
        % comments look like %[category:type] so for example for the
        % TuneVar properties it would be %[Properties:TuneVar]
        if isempty(componentConfigClass)
            % No class: this is a TuneVar
            searchCategory = "Properties";
            searchType = "TuneVar";
        else
            % There's a class: this is a component
            searchCategory = "Properties";
            searchType = "Component";
        end
        searchExp = "(?<=%\[" + searchCategory + ":" + searchType + ...
            ".+?)\n(?=^\s*(end|%\[" + searchCategory + "))";
        functionalComment = "%[" + searchCategory + ":" + searchType + "]";
        insertionIndex = regexp(theDoc.Text,searchExp,'lineanchors','once');
        if (isempty(insertionIndex))
            % Try to insert
            BTConfig.InsertPropertiesBlock(theDoc,className,functionalComment);
            % Try to find again
            insertionIndex = regexp(theDoc.Text,searchExp,'lineanchors','once');
            if (isempty(insertionIndex))
                error(['Unable to find the "%s" section in class [%s]. Unable to proceed.\n' ...
                    'Add this classdef section yourself and try again.'], ...
                    functionalComment,MaskUtil.GetFileEditLink(className));
            end
        end
        lineNumber = 1+count(theDoc.Text(1:insertionIndex),newline);
        % Format the property declaration <name> <type> <= default>
        if (false == isempty(componentConfigClass))
            % Add a space before non-empty property class
            componentConfigClass = sprintf(' %s',componentConfigClass);
        end
        constructorArgument = [];
        if (false == isempty(defaultValueStr))
            if (startsWith(defaultValueStr,"("))
                % This default value is a constructor argument list - do
                % not use it as a property default, but instead as argument
                % to construction call below
                constructorArgument = defaultValueStr;
                defaultValueStr = [];
            else
                % This default value is a value expression, add the 
                % "equals" sign to make an assignment upon class load
                defaultValueStr = sprintf(' = %s', defaultValueStr);
            end
        end
        newCode = sprintf('    %s%s%s\n',newPropertyName,componentConfigClass,defaultValueStr);
        theDoc.insertTextAtPositionInLine(newCode,lineNumber,1);
        % Announce what we just did:
        hereLink = sprintf('<a href="matlab:edit(''%s'');goToLine(matlab.desktop.editor.getActive,%d);">HERE</a>', ...
            className,lineNumber);
        MaskUtil.ReportInfo('Inserted [%s.%s] property definition line %d [%s]', ...
            className,newPropertyName,lineNumber,hereLink);

        if (false == isempty(componentConfigClass))
            % Find insertion for initialization in constructor
            [insertionIndex,thisName] = BTConfig.GetConstructorInsertionSpot(theDoc.Text,className);
            constructorArgument = MaskUtil.StringSubstitution(constructorArgument);
            lineNumber = numel(regexp(theDoc.Text(1:insertionIndex),'\n'))+1;
            newCode = sprintf('        %s.%s = %s%s;\n', ...
                thisName,newPropertyName,componentConfigClass,constructorArgument);
            theDoc.insertTextAtPositionInLine(newCode,lineNumber,1);
            % Announce what we just did:
            hereLink = sprintf('<a href="matlab:edit(''%s'');goToLine(matlab.desktop.editor.getActive,%d);">HERE</a>', ...
                className,lineNumber);
            MaskUtil.ReportInfo('Inserted [%s.%s] property initialization line %d [%s]', ...
                className,newPropertyName,lineNumber,hereLink);
        end
    end
    %----------------------------------------------------------------------
    function [insertionIndex,thisName] = GetConstructorInsertionSpot(text,className)
        % Find insertion for initialization in constructor
        functionLine = ['^[^\n%]*function[^=]*=\s*',className,'[^\n]*\n'];
        commentLines = '(\s*%[^\n]*\n)*';
        baseConstruct = '(\s*\w+@\w+[^\n]*\n)*';
        % Find first character after preconditions are met
        insertSearchExp = sprintf('(?<=%s%s%s).',functionLine,commentLines,baseConstruct);
        insertionIndexes = regexp(text,insertSearchExp,'lineanchors');
        if (isempty(insertionIndexes))
            error('Unable to find the construction function in class [%s]. Unable to proceed.', ...
                className);
        end
        % ... and discover what the "this" variable is called:
        thisTokenExp = ['^.*function\s*([^=\s]*)\s*=\s*',className];
        thisToken = regexp(text,thisTokenExp,'lineanchors','tokens');
        if (isempty(thisToken))
            error('Unable to discover what the "this" variables in called during construction in class [%s]. Unable to proceed.', ...
                className);
        end
        % Insert at the last place identified:
        insertionIndex = insertionIndexes(end)-1;
        thisName = thisToken{1}{1};
    end
    %----------------------------------------------------------------------
    function InsertPropertiesBlock(theDoc,className,functionalComment)
        % Find insertion for a new properties block. Put it below the first
        % existing properties block
        theCode = theDoc.Text;
        xPropBlock = '(?<=^\s*)properties';
        xAttribs = '(?<ATTR>\s*\([^\)]*\))?';
        xPreText = '(?<PRET>.*?)';
        xTheEnd  = '(?<END>\<end\>)';
        superExp = ['(?<=' xPropBlock xAttribs xPreText xTheEnd ').'];
        spots = regexp(theCode,superExp,'lineanchors');
        if (isempty(spots))
            fprintf('Failed to find automatic insertion spot\n');
            return;
        end
        propInsertIdx = spots(1);
        lineNumber = numel(regexp(theDoc.Text(1:propInsertIdx),'\n'))+1;
        newCode = sprintf('  properties(GetObservable,SetObservable)\n    %s\n  end\n', ...
            functionalComment);
        theDoc.insertTextAtPositionInLine(newCode,lineNumber,1);
        hereLink = sprintf('<a href="matlab:edit(''%s'');goToLine(matlab.desktop.editor.getActive,%d);">HERE</a>', ...
            className,lineNumber);
        MaskUtil.ReportInfo('Inserted property block on line %d [%s]', ...
            lineNumber,hereLink);        
    end    

    end %protected methods
    
    properties (Constant, Access=protected)
        FirstLine = 'blkh=gcbh;'
        AssertLine = 'Config=BTConfig.Assert(blkh,Config);'
        ValidateLine = 'Config.Validate();'
        DoMaskInitLine = 'Config.DoMaskInit(blkh);'
        OldAssertCode = {
            'Config=BTConfig.Assert(gcb,Config);'
            'Config=BTConfig.Assert(gcbh,Config);'
            'Config=MaskUtil.ConfigAssert(gcb,Config);'
            'Config=MaskUtil.ConfigAssert(gcbh,Config);'}
        AddToEnd = 0
        AddToStart = 1
        AddToFirst = 2
        % Model-level ---------
        ModelVarNameEnd = 'Settings'
        ModelAssertLine = '%s=BTConfig.GetModelSettings();'
        ModelValidateLine = '%s.Validate();'
        ModelDoMaskInitLine = '%s.DoMaskInit(''%s'');'
    end
end


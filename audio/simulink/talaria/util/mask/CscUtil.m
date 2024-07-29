classdef CscUtil
% CscUtil class to assist mask initialization code in setting up code
% generation settings for signals and states.

methods (Static)
    
    %----------------------------------------------------------------------
    function [didIt] = SetOutportCsc(H,portIndex,package,storageClass,varargin)
        % SetLineStorageClass sets the storage class for a provided signal
        % line if it's OK to do so.
        % Usage:
        %    CscUtil.SetOutportCsc(H,portIndex,package,storageClass,...)
        % Where:
        % * H is handle or URL of block
        % * portIndex is numeric index of block H's output signal
        % * package is name of package (e.g. 'Simulink' or user-package-name)
        % * storageClass is name of storage class
        % * ... is zero or more property/value pairs to set for storage class
        didIt = false;

        % Ensure the block is present (and account for "Engine" models)
        blockIsFound = CscUtil.BlockExists(H,'Block');
        if (false == blockIsFound)
            return;
        end

        % Give up now if indeed now is not the time for code-gen settings
        [ok] = CscUtil.OkToPlay(H);
        if (false == ok)
            return;
        end

        % Make sure the package is loaded.
        CscUtil.LoadCsc(H,package,storageClass);

        % Grab mapping object and set the specified properties
        mObj = CscUtil.GetMappingObj(H);
        
        ph = get_param(H,'PortHandles');

        %**************************************************************************
        % Workaround - try/catch an mObj.getSignal - on catch, do mObj.removeSignal
        %**************************************************************************
        try
            % There appears to be something wrong in the MathWorks mapping object
            % where more than one entry for the signal can make its way
            % into their mapping object.
            mObj.addSignal(ph.Outport(portIndex),'StorageClass',storageClass, varargin{:});
        catch ME
            % Bingo - this one is having a problem
            switch (ME.identifier)
                case 'sl_services_mi:mcos:NotScalarInConversion'
                    % THIS is the problem we are worried about
                    workAroundMsg = sprintf('CscUtil caught the more-than-one-map-entry problem [%s][port:%d].',getfullname(H),portIndex);
                    % Need to remove the signal multiple unknown number of times
                    for i = 1:50
                        mObj.removeSignal(ph.Outport(portIndex));
                    end
                    try
                        mObj.addSignal(ph.Outport(portIndex),'StorageClass',storageClass, varargin{:});
%                         fprintf('%s \n', workAroundMsg);
                    catch ME2
                        % If still failed, don't know what to do, error out
                        causeException = MException('CscUtil:Workaround',workAroundMsg);
                        ME2 = addCause(ME2,causeException);
                        rethrow(ME2)
                    end
                case 'coderdictionary:api:invalidPropertyName'
                    % First attempt failed above, so switch away to "Auto" and back. If it still fails, 
                    % then first refresh the dictionary then switch away and back again.
                    % Nested exception is here so that we refresh the code dictionary only once since it's a slow multi-second operation
                    mObj.addSignal(ph.Outport(portIndex),'StorageClass','Auto');
                    try
                        mObj.addSignal(ph.Outport(portIndex),'StorageClass',storageClass,varargin{:});
                    catch ME2
                        switch (ME2.identifier)
                            case 'coderdictionary:api:invalidPropertyName'
                                dObj = CscUtil.GetCoderDictionary(ph.Outport(portIndex));
                                dObj.refreshPackage;
                                mObj.addSignal(ph.Outport(portIndex),'StorageClass','Auto');
                                fprintf('CscUtil.SetOutportCsc caught the out of date storage class problem [%s][%s]\n',package,storageClass);
                                mObj.addSignal(ph.Outport(portIndex),'StorageClass',storageClass,varargin{:});
                        otherwise
                            rethrow(ME2);
                        end
                    end
                case ''
                    the2023bLoadingError =  'Only one signal mapping is expected for port-handle';
                    the2023bCharCount = numel(the2023bLoadingError);
                    if (numel(ME.message) >= the2023bCharCount && ...
                            strncmp(ME.message,the2023bLoadingError,the2023bCharCount))
                        % Caught this new error. Seems to only happen
                        % during model loading in 2023b pre-release?
                        return; % silently give up
                    end
                    %(otherwise)
                    rethrow(ME);
                otherwise
                    % otherwise, we don't know what this is
                    rethrow(ME);
            end
        end

        % Now this port object has a non-empty SignalObject with CoderInfo
        didIt = true;
    end
    %----------------------------------------------------------------------
    function [didIt] = SetDatastoreCsc(H,package,storageClass,varargin)
        % SetDatastoreCsc sets the storage class for a provided data store
        % memory if it's OK to do so.
        % Usage:
        %    CscUtil.SetDatastoreCsc(dsmH,package,storageClass,...)
        % Where:
        % * H is handle or URL of a data store memory block
        % * package is name of package (e.g. 'Simulink' or user-package-name)
        % * storageClass is name of storage class
        % * ... is zero or more property/value pairs to set for storage class
        
        didIt = false;

        % Ensure the DSM is present (and account for "Engine" models)
        blockIsFound = CscUtil.BlockExists(H,'DataStoreMemory block');
        if (false == blockIsFound)
            return;
        end

        % Give up now if indeed now is not the time for code-gen settings
        [ok] = CscUtil.OkToPlay(H);
        if (false == ok)
            return;
        end
        
        % Make sure the package is loaded.
        CscUtil.LoadCsc(H,package,storageClass);
        
        % Grab the mapping object and set the specified properties
        mObj = CscUtil.GetMappingObj(H);
        try
            mObj.setDataStore(H,'StorageClass',storageClass,varargin{:});
        catch ME
            switch (ME.identifier)
                case 'coderdictionary:api:invalidPropertyName'
                    % First attempt failed above, so switch away to "Auto" and back. If it still fails, 
                    % then first refresh the dictionary then switch away and back again.
                    % Nested exception is here so that we refresh the code dictionary only once since it's a slow multi-second operation
                    mObj.setDataStore(H,'StorageClass','Auto');
                    try
                        mObj.setDataStore(H,'StorageClass',storageClass,varargin{:});
                    catch ME2
                        switch (ME2.identifier)
                            case 'coderdictionary:api:invalidPropertyName'
                                dObj = CscUtil.GetCoderDictionary(H);
                                dObj.refreshPackage;
                                mObj.setDataStore(H,'StorageClass','Auto');
                                fprintf('CscUtil.SetDatastoreCsc caught the out of date storage class problem [%s][%s]\n',package,storageClass);
                                mObj.setDataStore(H,'StorageClass',storageClass,varargin{:});
                        otherwise
                            rethrow(ME2);
                        end
                    end
            otherwise
                rethrow(ME);
            end
        end
        
        % Now this DSM has a non-empty StateSignalObject with CoderInfo!
        didIt = true;
    end
    
    %----------------------------------------------------------------------
    function SetupDsmsForStaticInit(H,dsmSubpathCell,varargin)
        % SetupDsmsForStaticInit Given a reference parent/container block
        % and a cell array of DataStoreMemory sub-block names/paths, sets
        % DSMs to use StaticStructInit Csc with names based on FeaturePath,
        % container-block-name, and DSM-name. Make sure to pass in a cell
        % array of the DSM block names, not the DSM "data store names"
        % Usage:
        %   CscUtil.SetupDsmsForStaticInit(parentBlock,{DsmBlock});
        dsmSubpathCell = MaskUtil.MakeCell(dsmSubpathCell);
        for i=1:numel(dsmSubpathCell)
            % Get the block URL
            aDsm = MaskUtil.SubBlockName(H,dsmSubpathCell{i});

            % Before looking at the block to get its name, make sure it's
            % actuall there:
            blockIsFound = CscUtil.BlockExists(aDsm,'DataStoreMemory block');
            if (false == blockIsFound)
                % We must be in an engine model. Not found is no problem.
                continue;
            end

            % Make unique C Identifier name that will hold the DSMs
            dsmName = get_param(aDsm,'DataStoreName');
            dsmName = MaskUtil.CreateWikiName(dsmName);
            pathInfo = FeaturePathUtil.GetPathInfo(aDsm,dsmName);

            % Make model-reference-friendly struct name
            structName = sprintf('%sControlDsms',bdroot(getfullname(H)));

            % Setup the code generation mapping
            CscUtil.SetDatastoreCsc(aDsm,'Bose','StructStaticInit', ...
                'Identifier',pathInfo.featurePath, ...
                'StructName',structName, ...
                varargin{:});
        end
    end
    
    % These functions are mostly used internally, but still useful in
    % situations "out there" in other script code.
    %----------------------------------------------------------------------
    function dObj = GetCoderDictionary(H)
        % GetCoderDictionary Returns embedded coder dictionary for the
        % model that contains Simulink reference H
        model = bdroot(H);        
        if (coder.dictionary.exist(model))
            dObj = coder.dictionary.open(model);
        else
                dObj = coder.dictionary.create(model);
        end        
    end
    %----------------------------------------------------------------------
    function mObj = GetMappingObj(H)
        % GetMappingObj returns the coder mappings object
        model = bdroot(H);
        try
            mObj = coder.mapping.utils.create(model);
        catch ME
            if (ME.identifier == "Simulink:mds:BlockDiagramCompiledErr")
                modelNameStr = ['''' getfullname(model) ''''];
                fixItLink = sprintf('<a href="matlab:open_system(%s);coder.mapping.utils.create(%s);disp(''FIXED. Now Save'')">', ...
                    modelNameStr,modelNameStr);
                fprintf('\n************************************************');
                fprintf('\n**    NO MAPPING OBJECT YET. %sCLICK HERE</a> TO FIX THEN SAVE MODEL', ...
                    fixItLink);
                fprintf('\n************************************************\n');
            else
                rethrow(ME);
            end
        end
    end
    
    %----------------------------------------------------------------------
    function CleanMultiMappingProblem(H)
        % CleanMultiMappingProblem prints out statistics about the signals
        % found in the mapping object and attempts to remove repeats.
        % This routine uses undocumented interfaces that may disappear.
        
        modelName = bdroot(getfullname(H));

        % Use naughty internal access undocumented and likely to change
        modelMapping = Simulink.CodeMapping.get(modelName, 'CoderDictionary');
        
        % Find unique port handle mappings
        [uniqueOnes,~,indexIntoIniqueOnes] = unique([modelMapping.Signals.PortHandle]);
        
        % Count mappings
        numSignalMappings = numel([modelMapping.Signals.PortHandle]);
        numUninqueSignalMappings = numel(uniqueOnes);
        
        % Count how many of each porthandle are present
        gcounts = groupcounts(indexIntoIniqueOnes,1:numSignalMappings+1);
        maxCount = max(gcounts);
        maxCountOccurances = numel(gcounts(gcounts == maxCount));
        
        fprintf('-------------------------------------------------\n');
        fprintf('Summary of [%s] BAD signal mappings:\n',modelName);
        fprintf('-------------------------------------------------\n');
        fprintf('Total mappings: %d\n', numSignalMappings);
        fprintf('Unique mappings: %d\n', numUninqueSignalMappings);
        fprintf('Suspected bad mappings: %d\n', numSignalMappings - numUninqueSignalMappings);
        fprintf('Maximum mapping repeat: %d (happens %d times)\n', maxCount, maxCountOccurances);
        fprintf('Bad mappings from worst case repeaters: %d\n', (maxCount-1) * maxCountOccurances);
        fprintf('Attempting to remove all repeated signal mappings.\n');
        
        % Remove all the repeats
        mObj = CscUtil.GetMappingObj(modelName);
        for i=1:numUninqueSignalMappings
            count = gcounts(i);
            portHandle = uniqueOnes(i);
            if (ishandle(portHandle))
                % Handle is valid - remove it (count-1) times
                for c=1:count-1
                        mObj.removeSignal(portHandle);
                end
            else
                % Nothing to be done! Cannot remove invalid handle...
                fprintf('Ignoring bad handle %f (%d instances)\n',portHandle,count);
            end
        end    
    end

    %----------------------------------------------------------------------
    function blockIsFound = BlockExists(H,description)
        % BlockExists checks if provided block path is valid.
        % This function has three possible results:
        % 1- block is found: returns true
        % 2- block is not found but should be: generates an error
        % 3- block is not found, but in an "Engine" model: returns false
        %
        % Note that if the provided block locator "H" is a handle (double)
        % then the block MUST exist: otherwise, how did you get the handle?
        % Creation of Translation and Interpretation models will only keep
        % the relevant blocks in the model and delete all non-relevant
        % blocks. However, if a subsystem contains both relevant and
        % non-relevant blocks, the subsystem is kept, but only with the
        % relevant blocks still present. If the subsystem has mask
        % initialization code that attempts to adjust settings for blocks
        % within the subsystem which were not relevant and so no longer are
        % present, the code will fail! This function helps by allowing code
        % to check whether the block that should be there is there, or if
        % the block is gone because it was intentionally deleted to create
        % an "Engine" model.
        %
        % Usage:
        %   blockIsFound = CscUtil.BlockExists(blockPath,description);
        %
        % If not provided, description defaults to 'Block'
        %
        % Example:
        %    blockIsFound = CscUtil.BlockExists(aBlockPath,'DataStoreMemory');

        blockIsFound = true;
        if false == exist('description','var')
            description = 'Block';
        end
        if (isa(H,'double'))
            blockHandle = H;
            handleWasProvided = true;
        else
            blockHandle = getSimulinkBlockHandle(H);
            handleWasProvided = false;
        end
        if (false == ishandle(blockHandle))
            % The block was not found. If in an "engine" model it's OK, but
            % otherwise it should not happen!
            myCaller = MaskUtil.NotThisFileCallerInfo('CscUtil');
            if (handleWasProvided)
                % HOWEVER, we can only determine which model is at play if
                % the originally provided locator was a block path.
                % Otherwise, it's just an error no matter what:
                error('Provided %s handle [%d] is not valid\ncalled from %s', ...
                    description,H,myCaller);
            else
                coreIndexRole = CoreIndexMask.GetCoreIndex(H);
                if (coreIndexRole <= CoreIndexMask.SourceModel)
                    % Block should be there. Generate an error.
                    error('%s [%s] is not found,\ncalled from %s', ...
                        description,H,myCaller);
                end
            end
            % We must be in an engine model. Not found is no problem.
            blockIsFound = false;
        end
    end


    %----------------------------------------------------------------------
    %function SetInstanceSpecificDatastore(H, headerFilename, sourceFilename, ...
    %        memSectionName, optionalMemSectionSource)
    %    % SetInstanceSpecificDatastore Sets up specified datastore memory
    %    % block for code generation with instance-specific properties.
    %    
    %end
    %----------------------------------------------------------------------
    %function CreateLibraryBasedMemSection(H, memSectionName, memSectionConfig)
    %    % CreateLibraryBasedMemSection Creates a memory section in the
    %    % model's embedded dictionary based on the path of the library
    %    % block instance.
    %end
    %----------------------------------------------------------------------
    
end % methods (Static)

methods (Static, Access=public)
    %----------------------------------------------------------------------
    function [ok,model] = OkToPlay(H)
        % Internal function that checks the diagram is not a library and
        % custom storage classes are OK to use in given context "H"
        ok = false;
        
        % Not OK if H is in a library
        model = bdroot(H);
        if bdIsLibrary(model)
            return;
        end
        
        % Give a warning, but keep trying if model is not "ert" based
        sysTargFile = get_param(model,'SystemTargetFile');
        if (false == strncmp(sysTargFile,'ert',3))
            warning('%s\nSystemTargetFile of [%s] is [%s]. Need embedded coder!', ...
                '---------------------------------------------------------', ...
                getfullname(model),sysTargFile);
        end
        
        % Made it here... so we're OK to play
        ok = true;
    end
    %----------------------------------------------------------------------
    function ok = LoadCsc(H,package,storageClass)
        % LoadCsc ensures the context H has a coder dictionary and loads
        % the specified package if the package-storageClass is not found.
        % If the output boolean is checked (i.e. anybody is listening) then
        % an additional status check is performed to ensure the specified
        % storage class is actually found.
        ok = false;
        % Grab the coder dictionary to ensure the package is present
        dObj = CscUtil.GetCoderDictionary(H);
        cscSec = dObj.getSection('StorageClasses');
        ssi = cscSec.find('DataSource',package,'Name',storageClass);
        if (false == isempty(ssi))
            % The CSC was found!
            ok = true;
        else
            % CSC was not found, try loading the package
            dObj.loadPackage(package);
            if (nargout > 0)
                % somebody is listening to the output. We'd better check if
                % the storage class is found now that the package is loaded
                ssi = cscSec.find('DataSource',package,'Name',storageClass);
                ok = (false == isempty(ssi));
            end
        end
    end
    
end % methods (Static, Access=protected)

end
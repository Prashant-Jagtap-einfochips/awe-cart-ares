classdef DataMapUtil
%DATAMAPUTIL Utility class for storing values in a base workspace data map
%   Using the block's full name as a key (with adjustments to support
%   using ModelRootName, saves a single value into a base workspace
%   containers.Map variable. 

%   Copyright 2018-2019 Bose Corporation
    properties
        MapNameRoot
        UseModelRootName
    end

    methods (Static)
    end
    methods
        %--------------------------------------------------
        function this = DataMapUtil(mapNameRoot,useModelRootName)
            this.MapNameRoot = mapNameRoot;
            if (nargin >= 2)
                this.UseModelRootName = useModelRootName;
            else
                % By default, we share among partiitoned models via use of
                % the model root name
                this.UseModelRootName = true;
            end
        end
        %--------------------------------------------------
        function blockKey = GetKey(this,H)
            fullName = getfullname(H);
            if (this.UseModelRootName)
                % This map is shared among partitions (i.e. uses a model's
                % "root" name instead of the full model name). Do it:
                modelName = bdroot(fullName);
                rootModelName = ModelRootName(H);
                blockKey = strrep(fullName,modelName,rootModelName);
            else
                % This map is specific to the specific model file. Do not
                % share the map among paritioned models.
                blockKey = fullName;
            end
        end
        %--------------------------------------------------
        function [mapName] = GetDataMapName(this,H)
            if (this.UseModelRootName)
                % This map is shared among partitions
                mapName = [this.MapNameRoot, ModelRootName(H)];
            else
                % This map is specific to the specific model file
                modelName = bdroot(getfullname(H));
                mapName = [this.MapNameRoot, modelName];
            end
        end
        %--------------------------------------------------
        function [theMap] = GetDataMap(this,H)
            wsName = 'base';
            mapName = this.GetDataMapName(H);
            try
                theMap = evalin(wsName,mapName);
            catch
                assignin(wsName,mapName,containers.Map);
                theMap = evalin(wsName,mapName);
            end
        end
        %--------------------------------------------------
        function isTheSame = CompareToMap(this,H,inMap)
            % Acquires the map for model of H and compares to inputMap
            isTheSame = false;
            myMap = this.GetDataMap(H);
            myMapKeys = myMap.keys;
            inMapKeys = inMap.keys;
            myCount = numel(myMapKeys);
            inCount = numel(inMapKeys);
            if (myCount ~= inCount)
                % Not the same! Different number of keys.
                return;
            end
            for i=1:numel(myMapKeys)
                aKey = myMapKeys{i};
                myMapValue = myMap(aKey);
                inMapValue = inMap(aKey);
                if (false == isequal(myMapValue, inMapValue))
                    % Not the same! This value did not match.
                    fprintf('Key [%s] mismatch\n',aKey);
                    disp(myMapValue);
                    disp(inMapValue);
                    return;
                end
            end
            % Nothing failed to compare. The maps are the same
            isTheSame = true;
        end
        %--------------------------------------------------
        function [filePath,H,mapName] = GetFileName(this,H,directory,fileNameOverride)
            arguments
                this DataMapUtil
                H = []
                directory char = []
                % non-empty fileName values override the default filename
                fileNameOverride char = []
            end
            if (isnumeric(H))
                if (false == ishandle(H))
                    error('Invalid Simulink Block handle provided %f',H);
                end
                if (isempty(H))
                    H = bdroot;
                end
                if (isempty(H))
                    error('No current model')
                end
            else
                % It's a block path or model name. Model might not be open
            end
            % If directory wasn't provided, use the model location
            if (isempty(directory))
                % Saves datamap to file in same directory as model
                modelRoot = bdroot(H);
                modelFilePath = get_param(modelRoot,'FileName');
                directory = fileparts(modelFilePath);
            end
            mapName = this.GetDataMapName(H);
            if (isempty(fileNameOverride))
                filePath = fullfile(directory,strcat(mapName,'.mat'));
            else
                filePath = fullfile(directory,strcat(fileNameOverride,'.mat'));
            end
        end
        %--------------------------------------------------
        function SaveDataMap(this,H,directory,fileNameOverride)
            arguments
                this DataMapUtil
                H = []
                directory char = []
                fileNameOverride char = []
            end
            [newFilePath,H,mapName] = this.GetFileName(H,directory,fileNameOverride);
            % Ensure that the map exists (even if just empty)
            this.GetDataMap(H);
            saveCmd = sprintf('save(''%s'',''%s'');',newFilePath,mapName);
            evalin('base',saveCmd);
        end
        %--------------------------------------------------
        function redirectedOutput = LoadDataMap(this,H,directory,fileNameOverride)
            % Loads data map from file. If nargout is 1, directly returns
            % the map. Otherwise (normal case) loads into base workspace.
            arguments
                this DataMapUtil
                H = []
                directory char = []
                fileNameOverride char = []
            end
            [theFilePath,~,mapName] = this.GetFileName(H,directory,fileNameOverride);

            if (nargout > 0)
                if (isfile(theFilePath))
                    % found it!
                    loadedStruct = load(theFilePath);
                    redirectedOutput = loadedStruct.(mapName);
                else
                    % File is not found. For this usage, instead of an
                    % error, we return empty
                    redirectedOutput = [];
                end
            else
                loadCmd = sprintf('load(''%s'');',theFilePath);
                evalin('base',loadCmd);
            end
        end
        %--------------------------------------------------
        function [theValue,wasFound] = GetStoredValue(this,H)
            % Do not load values for libraries - just models
            diagramType = get_param(bdroot(H),'BlockDiagramType');
            if (strcmp(diagramType,'library'))
                theValue=[];
                wasFound=false;
                return;
            end

            % Retrieve the numeric version of the value from storage. This
            % is the original inport value with no alteration.
            theMap = this.GetDataMap(H);
            blockKey = this.GetKey(H);
            if (theMap.isKey(blockKey))
                theValue = theMap(blockKey);
                wasFound = true;
            else
                theValue = [];
                wasFound = false;
            end
        end
        %--------------------------------------------------
        function SetStoredValue(this,H,theValue)
            % SetStoredValue Set the value stored in the data map for the
            % specified block.
            % Usage:
            %       map.SetStoredValue(blockH,value);
            
            % Do not save values for libraries - just models
            diagramType = get_param(bdroot(H),'BlockDiagramType');
            if (strcmp(diagramType,'library'))
                return;
            end

            % Save the numeric version of the value (received via inport)
            % into storage. Save the original inport value with no alteration.
            theMap = this.GetDataMap(H);
            blockKey = this.GetKey(H);
            theMap(blockKey) = theValue;
        end
        %--------------------------------------------------
        function ClearMap(this,H)
            wsName = 'base';
            mapName = this.GetDataMapName(H);
            cmmdExpr = sprintf('clear %s', mapName);
            evalin(wsName,cmmdExpr);
        end
    end
end


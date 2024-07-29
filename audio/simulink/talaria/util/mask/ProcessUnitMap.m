classdef ProcessUnitMap
    % ProcessUnitMap is a class of static methods to manage shared storage
    % that tracks assigned use of processing units (e.g. HW accelerators)
    % across different DSP cores and chips.
    
    % Challenge is that the built-up map does not know when to "reset"
    % itself between differing model updates. When the build-time settings
    % change, the "old" map must be either cleared - or re-validated to
    % ensure that it still represents what the blocks want. Passing in the
    % workspace variable as part of the assignment allows the assignment to
    % be re-validated by methods of this class by asking the block's
    % workspace what the *current* unit id assignment is. It's a sneaky way
    % to cause the blocks to ask each other what they want when conflicts
    % are detected.

    % Note that this class provides two services:
    % 1 - looks up the current chip and core assigned by partitioning
    % 2 - verifies that the same unit is not used by multiple cores of the
    %     *same* chip. Each chip has its own set of processing units.
    
    % Copyright 2021 Bose Corporation
    
    methods (Static)
        function [chipIndex,coreIndex] = AssignBlockToUnit(H,propertyPathArray,unitType,estimatedWorkload)
            % AssignBlockToUnit Verifies unit assignment by the specified 
            % block with the estimated additional workload. Note that the
            % actual assignment is discovered in the block's mask workspace
            % via the provided propertyPathArray. The rules are that within
            % a single chip, only one core is allowed access to a unit of a
            % particular type (e.g. iir, fir, etc).
            % Usage:
            %   [chipIndex,coreIndex] = ProcessUnitMap.AssignBlockToUnit(H,propertyPathArray,unitType,estimatedWorkload)
            %
            % H : Handle or Simulink-URL of the block to be assigned to the
            %   processing unit
            % propertyPathArray : array of string names to describe where
            %   the block configures its designed unitId starting from the
            %   block's mask workspace. For example BTC properties would
            %   look like ["Config","AccelDeviceId"]. For breeds, it would
            %   be a deeper path like ["Config","BreedObj","AccelId"].
            % unitType : description of unit type to differentiate units. A
            %   single block might have more than one type, or different
            %   collections of blocks might coordinate their own types
            %   (e.g. "iir" or "fir"). The shared data storage uses this
            %   name.
            % estimatedWorkload : OPTIONAL numeric quantity estimating how
            %   much work the block will ask of the processing unit
            % chipIndex : returned numeric index indicating block H's
            %   currently assigned processor chip
            % coreIndex : returned numeric index indicating block H's
            %   currently assigned processor core
            %
            % Example:
            % [chip,core]=ProcessUnitMap.AssignBlockToUnit(blkh,'AccelId','iir');
            arguments
                H
                propertyPathArray string
                unitType string
                estimatedWorkload = 0
            end
            
            % Retrieve the data - MATLAB table objects are not handles...
            % so any changes made here must be written back to global data.
            theT = ProcessUnitMap.GetSharedData(H,unitType);

            % Gather the variables of the table: {'BlockName','Unit','CoreTag','Workload'}
            theBlockName = string(getfullname(H));
            theUnit = ProcessUnitMap.GetAssignedUnit(H,propertyPathArray);
            [theCoreTag,chipIndex,coreIndex] = ProcessUnitMap.GetCoreTag(H);
            theWorkload = estimatedWorkload;
            
            % Build a table row with the gathered data. Place the
            % propertyPathArray in a separate cell since its dimensions can
            % vary from row to row.
            tableRow = {theBlockName,theUnit,chipIndex,coreIndex,theWorkload,{propertyPathArray}};
            
            % Update the table with this row
            rowMatch = ismember(theT.BlockName,theBlockName);
            switch (sum(rowMatch))
                case 0
                    % Not present yet, add it to the end
                    rowMatch(end+1) = true;                    
                case 1
                    % Found existing entry, it will be replaced
                otherwise
                    % Unexpected error! The row is found more than once!
                    warning('Block [%s] has more than one entry!', theBlockName);
                    % ... leave the rows in place - they will ALL be
                    % updated below
            end
            if (isempty(theT))
                theT = [theT; tableRow];
            else
                theT(rowMatch,:) = deal(tableRow);
            end
            
            % Retrieve the current table size
            numRows = size(theT,1);
            
            % Find rows that are assigned to the same unit
            unitMatch = ismember(theT.Unit,theUnit);

            % ... and investigate any that do not match
            coreMatch = ismember(theT.Core,coreIndex);
            chipMatch = ismember(theT.Chip,chipIndex);
            % Rows are trouble if the unit matches and the chip matches but
            % the core does not match. Two cores on same chip cannot access
            troubleRows = find(unitMatch & chipMatch & (false == coreMatch));
            invalidRows = false(numRows,1);
            for t=1:numel(troubleRows)
                i=troubleRows(t);
                % Check if this troubling row is still entirely valid
                peerBlock = theT.BlockName{i};
                peerExists = ishandle(getSimulinkBlockHandle(peerBlock));
                % Check #1: see if peer block still exists
                if (false == peerExists || MaskUtil.IsCommented(peerBlock))
                    % Row is bad... no block found here anymore!
                    % This row will be removed after the loop. Move on.
                    invalidRows(i) = true;
                    continue;
                end
                % Row has valid block... but re-check the assigned unit
                % Check #2: see if assigned unit still conflicts
                % Note that if this peer us using a breed class, the
                % selected Breed may have changed so the BTC property is no
                % longer available. Trap this and mark the row invalid if
                % it happens.
                try
                    peerPropPath = theT.PropertyPath{i};
                    peerUnit = ProcessUnitMap.GetAssignedUnit( ...
                        peerBlock,peerPropPath);
                    if (false == isequal(peerUnit,theUnit))
                        % Unit is not longer the same so it's out of date!
                        % This row will be removed after the loop. Move on.
                        invalidRows(i) = true;
                        continue;
                    end
                catch ME
                    % This row no longer has a valid property path.
                    if ("MATLAB:noSuchMethodOrField" == ME.identifier)
                        invalidRows(i) = true;
                        continue;
                    else
                        rethrow(ME);
                    end
                end
                % Assigned unit is the same, but re-check the chip & core
                % Check #3: assigned Chip & Core still trouble
                [peerTag,peerChip,peerCore] = ProcessUnitMap.GetCoreTag(peerBlock);
                if ((peerCore ~= coreIndex) && ...
                    (peerChip == chipIndex))
                    % Different cores on the same chip use same unit!
                    % This is the situation that is not tolerated
                    % Update the shared table so it is available to review before
                    % the error exits this function
                    ProcessUnitMap.SetSharedData(H,unitType,theT);
                    error('ProcessUnitMap:Mismatch', ...
                    'Encountered differing core assignments on chip %d for [%s] Unit:[%s].\n [%s]:[''%s''][%s] vs.\n [%s]:[''%s''][%s]', ...
                        chipIndex,unitType,string(theUnit), ...
                        theCoreTag,MaskUtil.GetBlockLinkString(GetSourceModelUrls(theBlockName)), propertyPathArray.join('.'), ...
                        peerTag,MaskUtil.GetBlockLinkString(GetSourceModelUrls(peerBlock)), peerPropPath.join('.'));
                    % Note that there could be more rows to look at but
                    % once an error is found, the search ends above.
                end
            end

            % Delete the invalid rows
            theT(invalidRows,:) = [];
            
            % All done. Write this updated table back out to global storage
            ProcessUnitMap.SetSharedData(H,unitType,theT);
        end
        
        function ClearSharedData(H,unitType)
            % ClearSharedData wipes out any shared data for this map
            [variableName,theModel,itExists] = ProcessUnitMap.GetVariableName(H,unitType);
            if (itExists)
                % Got it! Remove it!
                Simulink.data.evalinGlobal(theModel,['clear ' variableName]);
            end
        end
        
        function SetSharedData(H,unitType,theData)
            % SetSharedData replaces any shared data with the provided data
            [variableName,theModel] = ProcessUnitMap.GetVariableName(H,unitType);
            Simulink.data.assigninGlobal(theModel,variableName,theData);
        end

        function theData = GetSharedData(H,unitType)
            % GetSharedData retrieves shared data, or creates if not there
            [variableName,theModel,itExists] = ProcessUnitMap.GetVariableName(H,unitType);
            if (itExists)
                % Got it! Pass it along!
                theData = Simulink.data.evalinGlobal(theModel,variableName);
            else
                % Aint nobody got it. Create it now!
                theData = table(string.empty,[],[],[],[],cell.empty, ...
                    'VariableNames',{'BlockName','Unit','Chip','Core','Workload', ...
                    'PropertyPath'});
                Simulink.data.assigninGlobal(theModel,variableName,theData);
            end
        end

        function theUnit = GetAssignedUnit(H,propertyPath)
            % GetAssignedUnit retrieves the configured unit number from the
            % provided block and propertyPath.
            % Examples
            %    theUnit = ProcessUnitMap.GetAssignedUnit
            %                (blkh,["Config" "AccelId"]);
            %    theUnit = ProcessUnitMap.GetAssignedUnit
            %                (blkh,["Config","BreedObj","AccelDeviceId"]);
            arguments
                H
                propertyPath string
            end
            % The first element of the propertyPath is a mask workspace
            % variable or block mask parameter (e.g. "Config")
            theUnit = MaskUtil.GetEvaluatedParameter(H,propertyPath(1));
            for i=2:numel(propertyPath)
                pathElement = propertyPath(i);
                theUnit = theUnit.(pathElement);
            end
        end
    end
    methods (Static, Access=private)
        function [variableName,theModel,itExists] = GetVariableName(H,unitType)
            % GetVariableName returns shared-data-related values
            theModel = getfullname(bdroot(H));
            variableName = sprintf('%s_%sUnitMap',theModel,unitType);
            itExists = Simulink.data.existsInGlobal(theModel,variableName);
        end
        function [coreTag,chipIndex,coreIndex] = GetCoreTag(H)
            % GetCoreTag returns the assigned core tag for the given block
            % Note that it could be empty or whatever other weird value is
            % found in the tag. Empty tags are considered chip 1 core 1.
            chipIndex = 1;
            coreIndex = 1;
            topAtomic = GetTopAtomic(H,true);
            coreTag = string(get_param(topAtomic,'Tag'));
            foundTokensCell = regexp(coreTag,'(\d*)_(\d*)','tokens');
            if (false == isempty(foundTokensCell))
                chipIndex = str2double(foundTokensCell{1}{1});
                coreIndex = str2double(foundTokensCell{1}{2});
            end            
        end
    end
    
end
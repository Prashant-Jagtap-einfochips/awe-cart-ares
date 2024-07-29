classdef interpretation_engine < handle
%interpretation_engine Loads a interpretation engine into MATLAB.
%   Given the name of the original model, this routine seaches the code
%   generation folder for the "<modelName>_InterpretationEngine64" dll. If
%   found, the resulting object can be used to access the interpretation
%   engine.
% Note: The interpretation engine must be built with the same
% machine architecture as the host (i.e. 64 vs. 32 bit)

%   Copyright 2018-2019 Bose Corporation
    properties
        ModelName
        DllName
    end
    
    properties (Constant)
        SubDir32 = 'engines32';
        SubDir64 = 'engines64';
    end
    
    methods (Static)
        function dllName = GetDllName(modelName)
            codeGenPath = Simulink.fileGenControl('get','CodeGenFolder');
            searchBase = fullfile(codeGenPath,[modelName '_InterpretationEngine64']);
            searchString = [searchBase '*.dll'];
            namesStruct = dir(searchString);
            dllCandidates = string({namesStruct(:).name});
            [~,winnerIdx] = min(dllCandidates.strlength);
            [~,dllName] = fileparts(dllCandidates(winnerIdx).char);
        end
        function UnloadDll(modelName)
            dllName = interpretation_engine.GetDllName(modelName);
            if (false == isempty(dllName) && libisloaded(dllName))
                fprintf('%s unloaded\n',dllName);
                unloadlibrary(dllName);
            end
        end
    end
    
    methods
        function this = interpretation_engine(modelName)
            this.ModelName = modelName;
            
            this.DllName = interpretation_engine.GetDllName(modelName);
            if (isempty(this.DllName))
                error('Could not find DLL for %s',modelName);
            end
            
            % Now find the folder where this DLL's code headers are found
            codeGenPath = Simulink.fileGenControl('get','CodeGenFolder');
            folderName = [modelName '_InterpretationEngine64_ert_shrlib_rtw'];
            folderPath = fullfile(codeGenPath,translation_engine.SubDir64,folderName);
            dllHeader = fullfile(folderPath,'interpret_dll.h');
            
            % Try to load the library
            [notfound,warnings] = loadlibrary(this.DllName,dllHeader);

            % Cold start of the code:
            calllib(this.DllName,'InitializeInterpretationEngine');
        end
        
        function [stateVars] = List(this)
            % List Returns a string array of recognized StateVars
            stateVars = string( calllib(this.DllName,'FirstStateVar') );
            aName = calllib(this.DllName,'NextStateVar');
            while (false == isempty(aName))
                stateVars(end+1) = string(aName);
                aName = calllib(this.DllName,'NextStateVar');
            end
        end
        
        function [tsps] = Ask(this,stateVarName)
            % Ask Returns TSPs needed to calculate StateVar
            tsps = calllib(this.DllName,'GetTspsForStateVar',stateVarName);
            if isempty(tsps)
                error('TIE:Ask:error','Unrecognized StateVar [%s]',stateVarName);
            end
        end
        
        function Set(this,tspName,tspValues)
            % Set Sets a TSP with provided space-delimited values
            % Arguments: Set(this, tspName, tspValues)
            % Returns 0 for OK, 1 for unrecognized TSP, 2 for too many elements
            retVal = calllib(this.DllName,'SetTsp',tspName,tspValues);
            strings = string(calllib(this.DllName,'GetTSPErrorStrings'));
            switch(retVal)
                case 0 
                case 1
                    error('TIE:Set:error', strings);
                case 2
                    warning('TIE:Set:warning', strings);
            end
        end
        
        function [stateVar] = Get(this)
            % Get Returns the requested StateVar based on provided TSPs
            this.ClearInterpretationErrorState;
            stateVar = string(calllib(this.DllName,'CalculateStateVar'));
            EmitErrors(this, 'Get');
        end
        
        function [errorState] = GetInterpretationErrorState(this)
            % GetInterpretationErrorState Returns the current error state
            % for the engine. A value of "0" implies no errors.
            errorState = calllib(this.DllName,'GetInterpretationErrorState');
        end            

        function [errors] = GetErrorStrings(this)
            % GetErrorStrings Returns the current error strings
            % for the engine.
            errors = string(calllib(this.DllName,'GetInterpretationErrorStrings'));
        end            

        function ClearInterpretationErrorState(this)
            % ClearTranslationErrorState Resets the engine's error state
            % back to "0" (no errors).
            calllib(this.DllName,'ClearInterpretationErrorState');
        end            
        
        function [compatibilityHash] = GetTspCompat(this,overlayGroupNameHash)
            % GetTspCompat Returns the compatibility hash code for the TSPs
            % represented within this interpretation engine. Provide
            % the crc32 hash of the overlay group name.
            compatibilityHash = calllib(this.DllName,'GetTspCompat',overlayGroupNameHash);    
            compatibilityHash = sprintf('%08x',uint32(compatibilityHash));
        end

        function [compatibilityHash] = GetTspCompatByName(this,overlayGroupName)
            % GetTspCompatByName Returns the compatibility hash code for the TSPs
            % represented within this interpretation engine. Provide
            % the exact name of the overlay group.
            compatibilityHash = calllib(this.DllName,'GetTspCompatByName',overlayGroupName);            
            compatibilityHash = sprintf('%08x',uint32(compatibilityHash));
        end
        
        function Reset(this)
            % Reset Clears the engine's error state and all other states
            % such as the tracking of which StateVars have been seen, and
            % the flags which indicate that a interpretation is out-of-date
            % and needs to be run.
            calllib(this.DllName,'ClearAllStateRequests');
            calllib(this.DllName,'ClearAllInterpretationRequiredFlags');
            this.ClearInterpretationErrorState;
        end

        % Make sure to unload the library when object dies
        function delete(this)
            if (false == isempty(this.DllName) && libisloaded(this.DllName))
                unloadlibrary(this.DllName);
            end
        end
    end
    methods (Access = protected)
        function EmitErrors(this, functionName)
            if (this.GetInterpretationErrorState)
                errors = this.GetErrorStrings();
                messageId = sprintf('TIE:%s:error',functionName);
                error(messageId, errors);
            end
        end
    end
    
end


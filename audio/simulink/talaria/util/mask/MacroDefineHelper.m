classdef MacroDefineHelper
% MacroDefineHelper: Routines to assist with management of a #define macro

%   Copyright 2021 Bose Corporation

properties
    AddModelNameFilePrefix = false;
    FileName = 'MacroDefineHelper.h'
    DataType = 'uint32'
    DataParameterName = 'ExampleMacro'
    MacroDefineName = 'EXAMPLE_MACRO'
end

methods
    %-------------------------------------------------------------------------
    function theValue = EnsureDefine(this, H, defineValue, enforceValue)
        % EnsureDefine makes sure the Simulink.Parameter is found, creating
        % as necessary.
        % If enforceValue is true, then the provided value is set,
        % otherwise whatever might be already present is not altered.
        if (nargin < 4)
            enforceValue = false;
        end
        if (nargin < 3)
            defineValue = 0;
        end

        % Note that Simulink.Parameter objects are handle-based, so if the
        % object is present, changes made here will reflect everywhere
        theModel = bdroot(H);
        itExists = Simulink.data.existsInGlobal(theModel,this.DataParameterName);
        if (false == itExists)
            % Doesn't exist yet! Create it and assign it into global
            dataParam = Simulink.Parameter;
            Simulink.data.assigninGlobal(theModel,this.DataParameterName,dataParam);
            dataParam.Value = defineValue;
        else
            dataParam = Simulink.data.evalinGlobal(theModel,this.DataParameterName);
        end        
        
        % Now that we have a handle to the data object, snap its properties
        % into alignment with our desires:
        if (true == this.AddModelNameFilePrefix)
            hdrFileName = [ModelRootName(H) this.FileName];
        else
            hdrFileName = this.FileName;
        end
        if (enforceValue)
            dataParam.Value = defineValue;
        end
        dataParam.CoderInfo.StorageClass = 'Custom';
        dataParam.CoderInfo.Alias = this.MacroDefineName;
        dataParam.CoderInfo.Alignment = -1;
        dataParam.CoderInfo.CustomStorageClass = 'Define';
        dataParam.CoderInfo.CustomAttributes.HeaderFile = hdrFileName;
        dataParam.Description = '';
        dataParam.DataType = this.DataType;
        dataParam.Min = 0;
        dataParam.Max = pow2(32) - 1;
        dataParam.DocUnits = '';
        
        % Retrieve the value from the data object
        theValue = dataParam.Value;
    end
end % methods

end
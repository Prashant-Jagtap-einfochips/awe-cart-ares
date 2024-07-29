classdef PoolIirConfig < PoolIirWithResetConfig
% PoolIirConfig Build-time configuration class for the PoolIir block
% Usage:
%    aConfig = PoolIirConfig
%

%   Copyright 2023 Bose Corporation
  properties (GetObservable,SetObservable)
      MaxNumOfThreads = 4;
  end
  methods
    %----------------------------------------------------------------------
    function this = PoolIirConfig()
        % PoolIirConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok

        %.End with a call to our base class to continue validation checks
        %.including any violations of property locking.
        Validate@PoolIirWithResetConfig(this);
    end
    %----------------------------------------------------------------------
    function DoMaskInit(this,blockHandle)
        % DoMaskInit called from mask initialization to perform automation
        % of code generation settings and other canvas interactions.

        blockType = get_param(blockHandle, 'MaskType');
        if (blockType == "Bose Pool IIR With Reset")
            DoMaskInit@PoolIirWithResetConfig(this,blockHandle);
        elseif (blockType == "Bose Pool IIR")
            DoMaskInit@BtcBaseClass(this,blockHandle);
        else
            error("Unsupported Block %s" , blockType);
        end
    end

  end
end

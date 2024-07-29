classdef FirTunableConfig < FirBtcConfig
% FirTunableConfig Build-time configuration class for the FirTunable block
% Usage:
%    aConfig = FirTunableConfig
%

%   Copyright 2023 Bose Corporation

  properties (GetObservable,SetObservable)

    %[Properties:Component]
    
    %[Properties:TuneVar]

    %[Properties:General]
    
  end
  methods
    %----------------------------------------------------------------------
    function this = FirTunableConfig()
        % FirTunableConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        Validate@FirBtcConfig(this);
    end
    %----------------------------------------------------------------------
    function DoMaskInit(this,blockHandle)
        % DoMaskInit called from mask initialization to perform automation
        % of code generation settings and other canvas interactions.
        blockType = get_param(blockHandle, 'MaskType');
        if (blockType == "Blocklib Fir")
            DoMaskInit@FirConfig(this,blockHandle);
        elseif (blockType == "Blocklib FirTunable")
            DoMaskInit@BtcBaseClass(this,blockHandle);
        else
            error("Unsupported Block %s" , blockType);
        end    
    end
  end
end

classdef RfftWindowingConfig < RfftConfig
% Blocklib RfftWindowing---------------------------------------------------
% This block performs in-place windowing followed by an optimized FFT 
% computation for real multi-channel inputs. 
% 
% The SharcPlus and Hexagon breed only supports an even number of 
% channels and an FFT size of 128 or 256.
%--------------------------------------------------------------------------
% Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)
    % Properties use this syntax:
    %    PropertyName size class {validation functions} = DefaultValue
    % About Build-Time Configuration: https://wiki.bose.com/x/dqPoB
  end
  methods
    %----------------------------------------------------------------------
    function this = RfftWindowingConfig()
        % RfftWindowingConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@RfftConfig(this);

        % If there are no cross-property interactions, then this entire
        % method can be deleted!
    end
    %----------------------------------------------------------------------
    function DoMaskInit(this,blockHandle)
        % DoMaskInit called from mask initialization to perform automation
        % of code generation settings and other canvas interactions.
        
        % MaskInit Depending on which blockType uses this Config
        blockType = get_param(blockHandle, 'MaskType');
        if (blockType == "Blocklib Rfft")
            DoMaskInit@RfftConfig(this,blockHandle);
        elseif (blockType == "Blocklib RfftWindowing")
            DoMaskInit@BtcBaseClass(this,blockHandle);
        else
            error("Unsupported Block %s" , blockType);
        end
    end
    %----------------------------------------------------------------------
  end
end

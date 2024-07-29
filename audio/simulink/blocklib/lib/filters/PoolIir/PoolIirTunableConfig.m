classdef PoolIirTunableConfig < PoolIirTunableWithResetConfig
% PoolIirTunableConfig Build-time configuration class for the PoolIirTunable block
% Usage:
%    aConfig = PoolIirTunableConfig
%

%   Copyright 2023 Bose Corporation
methods
    %----------------------------------------------------------------------
    function DoMaskInit(this,blockHandle)
        % DoMaskInit called from mask initialization to perform automation
        % of code generation settings and other canvas interactions.

        blockType = get_param(blockHandle, 'MaskType');
        if (blockType == "Bose Pool IIR With Reset")
            DoMaskInit@PoolIirWithResetConfig(this,blockHandle);
        elseif (blockType == "Bose Pool IIR Tunable With Reset")
            DoMaskInit@PoolIirTunableWithResetConfig(this,blockHandle);
        elseif (blockType == "Bose Pool IIR Tunable")
            DoMaskInit@BtcBaseClass(this,blockHandle);
        else
            error("Unsupported Block %s" , blockType);
        end
    end
end
end

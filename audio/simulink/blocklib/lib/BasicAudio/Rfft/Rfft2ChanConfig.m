classdef Rfft2ChanConfig < RfftConfig
% Blocklib Rfft2Chan-------------------------------------------------------
% This block computes the Fourier Transform using an optimized FFT for real
% 2-channel input.
% Inputs:
%   x1[n]: [FftSize, 1] Real valued time domain input signal 1
%   x2[n]: [FftSize, 1] Real valued time domain input signal 2
% Outputs: 
%   X1[nL,k]: [(FftSize/2 +1) x 1] Complex half-spectra of the input 1
%   X2[nL,k]: [(FftSize/2 +1) x 1] Complex half-spectra of the input 2
%
% The SharcPlus and Hexagon breed currently only supports an even number 
% of channels and an FFT size of 128 or 256.
%
% This block adds a wrapper around the Blocklib Rfft block. This block is
% to be deprecated.
% 
%--------------------------------------------------------------------------
%
%   Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)
    % Properties use this syntax:
    %    PropertyName size class {validation functions} = DefaultValue
    % About Build-Time Configuration: https://wiki.bose.com/x/dqPoB
  end
  methods
    %----------------------------------------------------------------------
    function this = Rfft2ChanConfig()
        % RfftTwoChannelConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
        
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@RfftConfig(this);
        warning("Rfft2Chan is now deprecated. Use Rfft instead");

    end
    %----------------------------------------------------------------------
    function DoMaskInit(this, blockHandle)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        blockType = get_param(blockHandle, 'MaskType');
        if (blockType == "Blocklib Rfft")
            DoMaskInit@RfftConfig(this, blockHandle);
        elseif (blockType == "Blocklib Rfft2Chan")
            DoMaskInit@BtcBaseClass(this, blockHandle);
        else
            error("Unsupported Block %s", blockType);
        end

    end
    %----------------------------------------------------------------------
  end
end

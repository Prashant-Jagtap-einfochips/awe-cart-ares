classdef PoolIirExternalTranslationConfig < BtcBaseClassWithBreeds
% Blocklib PoolIir-----------------------------------------------------
%   Inputs:
%       AudioIn         [FrameSize x NumChannels] Input Signal
%
%   This block only supports Generic variant of Pool IIR. It shall not
%   support any hardware-specific variant as this block is planned to be
%   deprecated soon. For any active instance of this block, please swap it
%   out with one of the other Pool IIR lib blocks ASAP!!

%   Copyright 2022 Bose Corporation

properties (GetObservable, SetObservable)
    % Default tune values
    DefaultNumStages = 1
    DefaultSosCoefficients = [1 0 0 1 0 0]

    % MaxNumStages defines the cumulative count of all SOS stages supported
    % by the filter, regardless of whether all stages are being used in the
    % current configuration. By default at least one stage must be present,
    % otherwise there's no filter!
    MaxNumStages  = 1

    % PoolIir Object
    PoolIir PoolIirWithResetConfig

  end
  methods
    %----------------------------------------------------------------------
    function this = PoolIirExternalTranslationConfig()
        % PoolIirExternalTranslationConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values

        this.PoolIir = PoolIirWithResetConfig;
        this.Lock(@this.PoolIir.MaxNumStages, @()this.MaxNumStages);
        
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClass(this);
    end

  end
end

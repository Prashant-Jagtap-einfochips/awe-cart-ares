classdef InputMixerConfig < BtcBaseClass
% InputMixerConfig Build-time configuration class for the InputMixer block
% Usage:
%    aConfig = InputMixerConfig
%

%   Copyright 2021 Bose Corporation

  properties (GetObservable,SetObservable)
    % Choose 1 of the 10 types of mixing types, as follows (Type 8 is default):
    % 0: DEFER to Tuning to determine mixer type
    % 1: (C) to (L, R)
    % 2: (C, LFE) to (L, R)
    % 3: (L, R) to (L, R)
    % 4: (L, R, C) to (L, R)
    % 5: (L, R, LFE) to (L, R)
    % 6: (L, R, C, LFE) to (L, R)
    % 7: (L, R, Ls) to (L, R, Ls, Rs)
    % 8: (L, R, Ls, Rs) to (L, R, Ls, Rs)
    % 9: (L, R, Ls, Rs, Lrs) to (L, R, Ls, Rs)
    % 10: (L, R, Ls, Rs, Lrs, Rrs) to (L, R, Ls, Rs)      
    NumInputChans  = 6
    NumOutputChans  = 6

    DefaultWeights = ...
       [0,0.5,0,0,0,0,0,0,0,0,0;
        0,0.5,0.5,0,0,0,0,0,0,0,0;
        1.0,0,0,0,0,0,0,0,0,0,0;
        0.66666666,0.33333333,0,0,0,0,0,0,0,0,0;
        0.66666666,0,0.33333333,0,0,0,0,0,0,0,0;
        0.5,0.25,0.25,0,0,0,0,0,0,0,0;
        1.0,0,0,0.5,0,0,0.5,0,0,0,0;
        1.0,0,0,1.0,1.0,0,0,0,0,0,0;
        1.0,0,0,0.5,0.5,0,0,0.5,0,0,0.5;
        1.0,0,0,0.5,0.5,0,0,0.5,0.5,0,0]
    DefaultMixerType = 8
  end
  methods
    %----------------------------------------------------------------------
    function this = InputMixerConfig()
        % InputMixerConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClass(this);
    end

  end
end

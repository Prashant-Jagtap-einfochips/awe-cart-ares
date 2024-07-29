classdef MultiSourceEqConfig < BtcBaseClass
% MultiSourceEqConfig Build-time configuration class for the MultiSourceEq block
% Parts of this logic have been copied from legacy MultiSrcEQ_init.m
% Usage:
%    aConfig = MultiSourceEqConfig
%

%   Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)

    % Number of input channels per audio source
    NumEntChannel % = MultiSourceEQConfig.num_ent_channel
    
    % Number of audio sources from which to select (via RTC) and tune
    NumEntAudioSources = 7 % = MultiSourceEQConfig.num_ent_audio_sources
    
    % Number of stages per audio source
    NumStagesPerAudioSource = 12 % = MultiSourceEQConfig.num_stages_per_audio_source
  end
  properties (Constant)
      PassthruRawSos = [1 0 0 1 0 0]
  end
  properties (SetAccess=private) % Set during Validate
    InitialPoolNumStages % Initial value for DSM, determined via translation
    InitialPoolSosCoeff  % Initial value for DSM, determined via translation
  end
  properties (Dependent)
    % (num-sources * stages-per-source)
    MaxNumEntInputStages % = MultiSourceEQConfig.max_num_ent_input_eq_stages
    % a passthru stage for each (num-channels * num-sources)
    DefaultMultiSrcRawCoeffs% =repmat(passthru_raw_sos,this.NumEntChannel*num_ent_audio_sources,1);
    
    DefaultMultiSourceNumStages %= ones(1,num_ent_audio_sources*num_ent_channel);    
    
    PoolIir PoolIirConfig
  end
  methods
    %----------------------------------------------------------------------
    function this = MultiSourceEqConfig()
        % MultiSourceEqConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClass(this);

        % Check non-optional values have been defined.
        if (isempty(this.NumEntChannel))
            error('NumEntChannel must be defined.');
        end
        
        % Create initial values for state memories using the pooliir translation function.        
        % Set the default coeff set to passthrugh for pooliir per channel
        DefaultRawSosCoeff = struct('Value',repmat(this.PassthruRawSos,this.NumEntChannel,1));
        DefaultRawNumStages = struct('Value',ones(1,this.NumEntChannel));
        % call Pooliir translation fucntion for translating the rawSOS
        [this.InitialPoolNumStages,this.InitialPoolSosCoeff] = ...
            pooliirTranslationFcn(DefaultRawSosCoeff, DefaultRawNumStages, ...
                                  this.NumStagesPerAudioSource);
    end
    
    % For TuneVar SOSCoeff : passthrough SOS based on chans and sources
    function value = get.MaxNumEntInputStages(this)
        value = this.NumEntAudioSources * this.NumStagesPerAudioSource;
    end
    function value = get.DefaultMultiSrcRawCoeffs(this)
        value = repmat(this.PassthruRawSos,this.NumEntChannel*this.NumEntAudioSources,1);
    end
    
    % For TuneVar NumStages : 1 stage for channels-sources
    function value = get.DefaultMultiSourceNumStages(this)
        value = ones(1,this.NumEntChannel*this.NumEntAudioSources);
    end
    
    function value = get.PoolIir(this)
        value = PoolIirConfig;
        value.MaxNumStages = this.NumStagesPerAudioSource;
    end
  end
end

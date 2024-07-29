classdef AnnVolumeConfig < BtcBaseClass
% AnnVolumeConfig Build-time configuration class for the AnnVolume block
% Usage:
%    aConfig = AnnVolumeConfig
%

%   Copyright 2021 Bose Corporation

  properties (GetObservable,SetObservable)
    BankNumber  = 0
    InitialLinearGain = 1 % = ones(1,num_inp_chs)
    NumInputChannels = 1 % = num_inp_chs
  end
  properties (Dependent)
      Rgainy RgainyConfig
  end
  methods
    %----------------------------------------------------------------------
    function this = AnnVolumeConfig()
        % AnnVolumeConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClass(this);
    end

    %----------------------------------------------------------------------
    function value = get.Rgainy(this)
        value = RgainyConfig;
        value.NumRampers = this.NumInputChannels;
        value.InitialLinearGains = this.InitialLinearGain;
        value.Chan2RamperMap = 1:this.NumInputChannels;
    end
    
  end
end

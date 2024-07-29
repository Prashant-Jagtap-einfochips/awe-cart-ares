classdef SleepingBeautyConfig < BtcBaseClass
% SleepingBeautyConfig Build-time configuration class for the SleepingBeauty block
% Usage:
%    aConfig = SleepingBeautyConfig
%

%   Copyright 2021 Bose Corporation

  properties (GetObservable,SetObservable)
    ChanToRampMap  = [1 2 1 2 3 4 -1]
    DefaultOffset  = 128
    DefaultSilentExtremeMutesBass  = 0
    DefaultTaperGainTable = [[0,10,31,52,74,95,116,128,138,159,180,202,223,244,255];[-40,-30,-20,-10,0,0,0,0,0,0,0,-10,-20,-30,-40]]'
  end
  properties (Constant)
      NumRampers = 4 % Total of 4 rampers: left, right, center, mono
  end
  properties (Dependent)
      DlgRamperInit
  end
  methods
    %----------------------------------------------------------------------
    function this = SleepingBeautyConfig()
        % SleepingBeautyConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClass(this);

    end
    %----------------------------------------------------------------------
    function value = get.DlgRamperInit(this)
        % Use the InitalLinearGains and number of rampers to create an
        % initial structure value for the rampers
        initialGains = ones(1,this.NumRampers);
        value = RgainBusUtil.GetInitStruct(initialGains);
    end
  end
end

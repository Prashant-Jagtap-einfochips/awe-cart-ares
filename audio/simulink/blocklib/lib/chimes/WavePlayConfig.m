classdef WavePlayConfig < BtcBaseClass
% WavePlayConfig Build-time configuration class for the WavePlay block
% Usage:
%    aConfig = WavePlayConfig
%

%   Copyright 2022 Bose Corporation


  properties (GetObservable,SetObservable)
    ControlState  % = p.controlState
    FrameSize  % = frameSize
    Fs  % = Fs
    MinVol  % = minVol
    % RTC cold-start request values
    InitialPlayControl  % = p.playControl
    InitialClickClackSelect  % = selectclickclack
    InitialTotalPeriodMsec  % = TotalPeriod
    % TuneVar defaults
    DefaultTable1  % = p.table1
    DefaultTable2  % = p.table2
    DefaultTable3  % = p.table3
    DefaultTable4  % = p.table4

    % These are not used!?!
%     Len  % = p.tableLength
%     TableIndex  % = p.tableIndex

    % MaxTableSamples sets the storage size for each of the four tunable
    % wave signals. Mentioned in chimes_init.m and added as part of BTC
    MaxTableSamples = 1120
    
  end
  methods
    %----------------------------------------------------------------------
    function this = WavePlayConfig()
        % WavePlayConfig constructor: assign component property objects
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

classdef CalibratedSignalConfig < BtcBaseClass
% CalibratedSignalConfig Build-time configuration class for the CalibratedSignal block
% Usage:
%    aConfig = CalibratedSignalConfig
%

%   Copyright 2021 Bose Corporation

  properties (GetObservable,SetObservable)
    BankNumber  = 0
    DefaultOffset  = single(0)
    DefaultScale  = single(1)
    SignalDataTypeName = 'uint32'
  end
  methods
    %----------------------------------------------------------------------
    function this = CalibratedSignalConfig()
        % CalibratedSignalConfig constructor: assign component property objects
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

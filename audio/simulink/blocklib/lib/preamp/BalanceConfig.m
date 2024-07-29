classdef BalanceConfig < BtcBaseClass
% balanceConfig Build-time configuration class for the Balance block
% Usage:
%    aConfig = BalanceConfig
%

%   Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)
    BankNumber  = 0
    DefaultMaskBoostDisable  = 0
    DefaultMaskChanRampMap  = [1 2 3 1 2 -1]
    DefaultMaskOffset  = 128
    DefaultMaskSilentExtreme  = 1
    DefaultMaskSilentExtremeBass  = 1
  end
  properties (Constant)
      NumRampers = 4 % Total of 4 rampers: left, right, center, mono
  end
  properties (Dependent)
      DlgRamperInit
  end
  methods
    %----------------------------------------------------------------------
    function this = BalanceConfig()
        % balanceConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClass(this);

        % If there are no cross-property interactions, then this entire
        % method can be deleted!
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

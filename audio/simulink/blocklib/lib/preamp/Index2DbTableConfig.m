classdef Index2DbTableConfig < BtcBaseClass
% Index2DbTableConfig Build-time configuration class for the Index2DbTable block
% Usage:
%    aConfig = Index2DbTableConfig
%

%   Copyright 2021 Bose Corporation

  properties (GetObservable,SetObservable)
    BankNumber = 0 % Memory bank for TOP storage
    DefaultTable (:,2) {mustBeNumeric} = [0 -100; 0.5 -30; 1 0] % Each row represents a point in the lookup table as [linear-value,dB-value]
    MaximumTablePoints (1,1) {mustBeGreaterThanOrEqual(MaximumTablePoints,2)} = 8 % Max points allowed in tuning table
    DefaultMaxDbOffset (1,1) {mustBeNumeric} = 0 % Inexplicably called "maxgain" as a TuneVar, this is the default maximum db-offset allowed
  end
  methods
    %----------------------------------------------------------------------
    function this = Index2DbTableConfig()
        % Index2DbTableConfig constructor: assign component property objects
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

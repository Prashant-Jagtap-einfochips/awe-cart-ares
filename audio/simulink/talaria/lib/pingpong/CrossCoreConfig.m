classdef CrossCoreConfig < BtcBaseClass
% CoreCoreConfig Build-time configuration class for the CoreCore block
% Usage:
%    aConfig = CoreCoreConfig
%

%   Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)
      Api CrossCoreApiEnum = "FourListChipManager"
      Allocation CrossCoreAllocationEnum = "LocalCore"
      ManagerCoreNumber {mustBeScalarOrEmpty} = 2 % Ignored unless API includes LazySister
      CrossCoreSampleTime = -1 % Sample time of cross-core communication, -1 for automatic selection
  end
  methods
    %----------------------------------------------------------------------
    function this = CrossCoreConfig()
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        
        % Ensure that everybody knows "no api" also means "no allocation"
        % as it's the api that handles allocations
        if (this.Api == CrossCoreApiEnum.None)
            if (this.Allocation ~= CrossCoreAllocationEnum.None)
                warning(['Resetting cross-core buffer allocation from [%s] ' ...
                    'to [None] because the API is [None]'],this.Allocation);
                this.Allocation = CrossCoreAllocationEnum.None;
            end
        end
        
        % Continue onward
        Validate@BtcBaseClass(this);
    end

  end
end

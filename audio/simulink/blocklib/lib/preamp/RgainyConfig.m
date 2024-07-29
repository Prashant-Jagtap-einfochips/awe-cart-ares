classdef RgainyConfig < BtcBaseClass
% RgainyConfig Build-time configuration class for the Rgainy block
% Usage:
%    aConfig = RgainyConfig
%

%   Copyright 2021 Bose Corporation

  properties (GetObservable,SetObservable)
    EnableStrictSizeMatch (1,1) logical  = 0 % Non-zero requires Chan2RamperMap size to exactly match the number of discovered channels and InitialLinearGains to exactly match the number of rampers.
    Chan2RamperMap = 1 % For each channel specify which ramper to use. If EnableStrictSizeMatch is not enabled, extra mappings are ignored and missing mappings just copy the last value.
    InitialLinearGains = 1 % For each ramper specify initial linear gain. If EnableStrictSizeMatch is not enabled, extra gains are ignored and missing gains repeat the last value.
    NumRampers (1,1) {mustBeNumeric} = 1 % Number of rampers to allocate. Note that the number of channels present is dynamically discovered
  end
  properties (Dependent)
      DlgInitialLinearGains
      DlgRamperInit
  end
  methods
    %----------------------------------------------------------------------
    function this = RgainyConfig()
        % RgainyConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClass(this);
        
        % Ensure sizes match if so required
        if (this.EnableStrictSizeMatch)
            % Check that initial gains exactly match number of rampers
            if (numel(this.InitialLinearGains) ~= this.NumRampers)
                error('StrictSizeMatch validation failure: NumRampers [%d] does not match number of provided InitialLinearGains [%d]', ...
                    this.NumRampers,numel(this.InitialLinearGains));
            end
            % Note that the size checking for the Chan2RamperMap requires
            % knowledge of how many channels are present, which is
            % dynamically discovered by this block. See the mask code for
            % that size checking.
        end
    end
    %----------------------------------------------------------------------
    function value = get.DlgInitialLinearGains(this)
        % Use the specified gains and the number of rampers to create a
        % ramper-sized array
        numSpecified = numel(this.InitialLinearGains);
        numRequired = this.NumRampers;
        if (numSpecified >= numRequired)
            % Only take what is needed
            value = this.InitialLinearGains(1:numRequired);
        else
            % Pad the array by repeating the last value
            padSize = numRequired - numSpecified;
            value = [this.InitialLinearGains, ...
                repmat(this.InitialLinearGains(end),1,padSize)];
        end
    end
    %----------------------------------------------------------------------
    function value = get.DlgRamperInit(this)
        % Use the InitalLinearGains and number of rampers to create an
        % initial structure value for the rampers
        value = RgainBusUtil.GetInitStruct(this.DlgInitialLinearGains);
    end
  end
end

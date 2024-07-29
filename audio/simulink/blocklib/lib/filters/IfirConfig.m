classdef IfirConfig < BtcBaseClass
% IfirConfig Build-time configuration class for the Ifir block
% Usage:
%    aConfig = IfirConfig
%

%   Copyright 2023 Bose Corporation

  properties (GetObservable,SetObservable)
    % Properties use this syntax:
    %    PropertyName size class {validation functions} = DefaultValue
    % About Build-Time Configuration: https://wiki.bose.com/x/dqPoB

    Coefficients single     % See constructor for default value.
    InterpolationFactor int32 = 2
    NumChannels int32 {mustBePositive} = 2
  end

  properties(Dependent=true)
      FlippedCoefficients
      InitialStates
  end

  methods
    %----------------------------------------------------------------------
    function this = IfirConfig()
        % Default to unit impulse for all channels, this can be overridden
        % by a parent block or model. It's up to them to ensure that the
        % coefficient matrix has the correct number of columns, which will
        % be checked during Validate().
        this.Coefficients = ones(1, this.NumChannels);
    end

    function value = get.FlippedCoefficients(this)
        % Coefficients are fliped column-wise (i.e. each column represents
        % the filter coefficients for that input channel)
        value = flipud(this.Coefficients);
    end

    function value = get.InitialStates(this)
        value = zeros( ...
            size(this.FlippedCoefficients, 1) * this.InterpolationFactor, ...
            this.NumChannels);
    end

    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClass(this);

        % For now we're just validating that the settings are correct. It's
        % unclear if we should automatically invalidate these when the user
        % tries to set the values, or automatically update one when the
        % other is set?
        if (this.NumChannels ~= size(this.Coefficients, 2))
            error(['IfirConfig: The number of columns in coefficient matrix' ...
                ' must be equal to NumChannels.']);
        end
    end
  end
end

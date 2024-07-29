classdef FasGainComputationConfig < BtcBaseClass
% FasGainComputationConfig Build-time configuration class for the FasGainComputation block
% Configuration Parameters for Fresh Air Speaker (FAS)- Gain computation
% The constructor allows for changing the default values of 
% the properties during initialization. For example,
% Usage:
%    aConfig = FasGainComputationConfig('SpeedGainSlope',0.1);
% all constructor arguments are optional, but must come in name/value pairs

%   Copyright 2022 Bose Corporation

    properties(GetAccess = public, SetAccess = public,GetObservable,SetObservable)
        % These are the settable properties that will be used directly by
        % the FasGainComputation block
        % (mostly as default values for TuneVars)

        % Speed-Gain curve slope and threshold
        DefaultSpeedGainSlope       single  {mustBeNonnegative}     = 0.05
        DefaultSpeedGainThreshold   single  {mustBeNumeric}         = 120
        
        % Minimum value for the gain factor to avoid a log10(0)
        DefaultGainMinValue         single  {mustBePositive}        = 1e-5
        
        % Slew parameters
        DefaultSlewAttackDbPerSec   single  {mustBePositive}        = 3.0
        DefaulySlewDecayDbPerSec    single  {mustBeNegative}        = -12.0
        DefaultSlewSlowDbPerSec     single  {mustBeNonnegative}     = 0.5
        DefaultSlewThreshold        single  {mustBeNonnegative}     = 2
    end
   
    methods
        function obj = FASComputationConfig(varargin)
            % Constructor that creates an instance of the
            % FASComputationConfig class.
            % Default values are used, unless otherwise provided
            % (see introduction)

            % Process the user-specified property initializations
            if mod(nargin,2) ~= 0
                error(['The FAS-computation configuration ' ...
                    'constructor requires ' ...
                    'arguments that are property-value pairs']);
            else
                for argIndex = 1:2:(nargin-1)
                    obj.(varargin{argIndex}) = varargin{argIndex+1};
                end
            end
        end
    end
end




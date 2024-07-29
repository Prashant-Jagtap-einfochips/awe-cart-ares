classdef FASComputationConfig
    % FASComputationConfig

    
    
    
    % THIS CLASS IS DEPRECATED - Replaced by FasGainComputationConfig for
    % use with BTC configuration scheme

    
    
    
    
    properties(GetAccess = public, SetAccess = public)
        % These are the settable properties that will be used directly by
        % the FAS block.

        % Speed-Gain curve slope and threshold
        SpeedGainSlope              single  {mustBeNonnegative}     = 0.05;
        SpeedGainThreshold          single  {mustBeNumeric}         = 120;
        
        % Minimum value for the gain factor to avoid a log10(0)
        GainMinValue                single  {mustBePositive}        = 1e-5;
        
        % Slew parameters
        slew_attackdBperSec         single  {mustBePositive}        = 3.0;
        slew_decaydBperSec          single  {mustBeNegative}        = -12.0;
        slew_slowSlewdBperSec       single  {mustBeNonnegative}     = 0.5;
        slew_threshold              single  {mustBeNonnegative}     = 2;
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




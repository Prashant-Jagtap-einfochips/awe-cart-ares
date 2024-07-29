classdef FASApplicationConfig
    % FASApplicationConfig

    
    

    
    % THIS CLASS IS DEPRECATED - Replaced by FasGainApplicationConfig for
    % use with BTC configuration scheme

    
    
    
    
    
    properties(GetAccess = public, SetAccess = public)
        % These are the settable properties that will be used directly by
        % the FAS block.

        % LPF ramp filter
        lpfTimeConstMsec        single  {mustBePositive}        = 10;
        
        end
   
    methods
        function obj = FASApplicationConfig(varargin)
            % Constructor that creates an instance of the
            % FASApplicationConfig class.
            % Default values are used, unless otherwise provided
            % (see introduction)

            % Process the user-specified property initializations
            if mod(nargin,2) ~= 0
                error(['The FAS-Application configuration' ...
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




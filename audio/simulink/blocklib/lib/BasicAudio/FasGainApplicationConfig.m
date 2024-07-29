classdef FasGainApplicationConfig < BtcBaseClass
% FasGainApplicationConfig Build-time configuration class for the FasGainApplication block
% Configuration Parameters for Fresh Air Speaker (FAS)- Gain
% application
% The constructor allows for changing the default values of 
% the properties during initialization. For example,
% Usage:
%   aConfig = FasGainApplicationConfig('lpfTimeConstMsec',100);
% all constructor arguments are optional, but must come in name/value pairs

%   Copyright 2022 Bose Corporation

    properties(GetAccess = public, SetAccess = public,GetObservable,SetObservable)
        % These are the settable properties that will be used directly by
        % the FAS block.

        % LPF ramp filter
        DefaultLpfTimeConstMsec single  {mustBePositive}        = 10
        
        % Previously promoted mask parameter. Dimensions must match audio
        DefautFasEnable = [ones(10,1)' zeros(6,1)']        
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

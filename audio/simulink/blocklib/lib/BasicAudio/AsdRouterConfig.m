classdef AsdRouterConfig < BtcBaseClass
% AsdRouterConfig Build-time configuration class for the AsdRouter block
% Usage:
%    aConfig = AsdRouterConfig
%

%   Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)
    AuxGain  % Aux gain for each output channel for each Aux source
    EnableAux  % Enable Aux input sources
    FrameSize  % = L
    MainGain  % Main gain for each output channel
    MainSelect (:,1)  % Main Channel Select Vector
    NumAux  % Number of Aux Channels (length/size of vector important)
    NumChanOut  % Number of Output Channels
    NumMainCh  % Number of Main Channels
  end
  methods
    %----------------------------------------------------------------------
    function this = AsdRouterConfig()
        % AsdRouterConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClass(this);

        % Configure this.EnableAux parameter if it is not done already
        if isempty(this.EnableAux)
            for i=1:length(this.NumAux)
               if(this.NumAux(i) > 0)
                 this.EnableAux(i)   = 1;
               else                     
                 this.EnableAux(i)   = 0;
               end                 
            end
        end

    end

  end
end

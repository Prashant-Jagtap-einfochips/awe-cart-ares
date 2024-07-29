classdef ChimeRouterConfig < BtcBaseClass
% ChimeRouterConfig Build-time configuration class for the ChimeRouter block
% Usage:
%    aConfig = ChimeRouterConfig
%

%   Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)
    EnableAux  % = enableAux
    FrameSize  % = L
    Fs  % = Fs
    InputOffset  % = input_offset
    MainGain  % = MainGain
    MainSelect (:,1) % = MainSelect
    NChanOut  % = nChanOut
    Nch  % = NInpChs
    NumAux  % = numAux
    NumMainCh  % = numMainCh
    PAuxGain  % = pAuxGain
    PAuxSelect  % = pAuxSelect
  end
  methods
    %----------------------------------------------------------------------
    function this = ChimeRouterConfig()
        % ChimeRouterConfig constructor: assign component property objects
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

  end
end

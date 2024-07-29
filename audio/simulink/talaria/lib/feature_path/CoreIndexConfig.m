classdef CoreIndexConfig < BtcBaseClass
% CoreIndexConfig Build-time configuration class for the CoreIndex block
% Usage:
%    aConfig = CoreIndexConfig
%

%   Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)
    CrossCore CrossCoreConfig
    AddressingMode CoreIndexAddressingModeEnum = "DirectAddressing"
  end
  methods
    %----------------------------------------------------------------------
    function this = CoreIndexConfig()
        this.CrossCore = CrossCoreConfig;
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions

        % Our sub-component objects are lies - not really blocks (yet) so
        % we need to validate them ourselves because nobody else will.
        this.CrossCore.Validate();

        Validate@BtcBaseClass(this);
    end

  end
end

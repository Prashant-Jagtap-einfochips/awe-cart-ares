classdef ModelRootOutportConfig < TalariaApiBufferBTConfig & BtcBaseClass
% ModelRootOutportConfig Class definition to contain build-time configuration for ModelRootOutport
% Usage:
%    aConfig = ModelRootOutportConfig
%

%   Copyright 2020 Bose Corporation

  properties
    % Properties use this syntax:
    %    PropertyName size class {validation functions} = DefaultValue
    % About Build-Time Configuration: https://wiki.bose.com/x/dqPoB

    % All properties are inherited from other classes
  end
  methods
      function this = ModelRootOutportConfig()
          this.UpdatePropertyDefault('BufferName','SYSOUT');
      end
  end
end
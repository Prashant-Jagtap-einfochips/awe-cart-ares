classdef TalariaApiBufferBuildTimeConfig < BtcBaseClass
% TalariaApiBufferBuildTimeConfig Class definition to contain build-time configuration for TalariaApiBuffer
% Usage:
%    aConfig = TalariaApiBufferBuildTimeConfig
%

%   Copyright 2020 Bose Corporation

  properties (GetObservable,SetObservable)
    % Properties use this syntax:
    %    PropertyName size class {validation functions} = DefaultValue
    % About Build-Time Configuration: https://wiki.bose.com/x/dqPoB

    BufferName char = 'SYSIN'
    AccessType TalariaApiBufferAccess = 1
    
  end
end
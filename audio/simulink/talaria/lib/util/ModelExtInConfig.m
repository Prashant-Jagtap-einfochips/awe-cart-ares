classdef ModelExtInConfig < InportBTConfig
% ModelExtInConfig Class definition to contain build-time configuration for ModelExtIn
% Usage:
%    aConfig = ModelExtInConfig
%

%   Copyright 2020 Bose Corporation

  properties (GetObservable,SetObservable)
    % Properties use this syntax:
    %    PropertyName size class {validation functions} = DefaultValue
    % About Build-Time Configuration: https://wiki.bose.com/x/dqPoB

    % Inherits InputBTConfig properties, plus these:
    
    BufferName(1,:) char = 'In1'
    
  end
end
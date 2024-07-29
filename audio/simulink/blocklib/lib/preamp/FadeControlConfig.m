classdef FadeControlConfig < FadeRamperConfig
% FadeControlConfig Build-time configuration class for the FadeControl block
% Usage:
%    aConfig = FadeControlConfig
%

%   Copyright 2022 Bose Corporation


% Inherit from FadeRamperConfig to get NumRampers intended to control
  properties (GetObservable,SetObservable)
    BankNumber  = 0
    DefaultBoostDisable  = 0
    DefaultIndexOffset  = 128
    DefaultRampTimeMs = 30
    DefaultSilentExtreme  = 1
    DefaultIndex2DbTable = transpose([0,10,31,52,74,95,116,128,138,159,180,202,223,244,255;-30,-27,-14,-7,-5,-3,-1,0,-1,-3,-5,-7,-14,-27,-30])
  end
end

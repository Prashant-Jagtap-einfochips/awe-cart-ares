classdef VolumeConfig < Index2DbTableConfig
% volumeConfig Build-time configuration class for the volume block
% Usage:
%    aConfig = volumeConfig
%

%   Copyright 2021 Bose Corporation

  properties (GetObservable,SetObservable)
    InitialLinearGain (1,1) {mustBeNumeric} = 0
    MaskEnableGainOutput logical = 0

    %Inherited:BankNumber
    %Inherited:MaximumTablePoints
    %Inherited:DefaultTable

  end
  properties (Dependent)
      Rgainy RgainyConfig
  end
  methods
    %----------------------------------------------------------------------
    function this = VolumeConfig()
        % VolumeConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClass(this);
    end
    function value = get.Rgainy(this)
        % By deliverying the configuration this way, it's not only locked
        % in, but also invisible to the BTC hierarchy:
        value = RgainyConfig;
        value.EnableStrictSizeMatch = 0;
        value.Chan2RamperMap = 1;
        value.InitialLinearGains = this.InitialLinearGain;
        value.NumRampers = 1;
    end
  end
  
  
% Original BTC upgrade work used "Mask" names - the below lets these names
% still work if anybody was already using them. (Delete this
% backwards-compatibility code to expose those old names wherever they are
% used so they can be removed/updated)
properties (Dependent, Hidden) % [Rename-Compatibility]
   MaskRows
   MaskVolumeTable
end
methods
   function set.MaskRows(obj,val)
      obj.MaximumTablePoints = val;
   end
   function val = get.MaskRows(obj)
      val = obj.MaximumTablePoints;
   end
   function set.MaskVolumeTable(obj,val)
      obj.DefaultTable = val;
   end
   function val = get.MaskVolumeTable(obj)
      val = obj.DefaultTable;
   end
end % [Rename-Compatibility]
  
end

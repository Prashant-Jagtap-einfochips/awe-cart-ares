classdef ToneConfig < BtcBaseClass
% ToneConfig Build-time configuration class for the Tone block
% Set the Offset and Scale to normalize the input value between -1 (maximum
% cut) and 1 (maximum boost) so the following equation becomes true:
%     normalized_float_value = (integer_RTC_index - Offset) / Scale
% Usage:
%    aConfig = ToneConfig
%

%   Copyright 2021 Bose Corporation

  properties (GetObservable,SetObservable)
    BankNumber  = 0
    DefaultBoostDb  = 20
    DefaultBoostHz  = 200
    DefaultCutDb  = 20
    DefaultCutHz  = 200
    DefaultOffset  = 128
    DefaultScale  = 127
    FilterType ToneFilterTypeEnum = 'lowpass'
    rampRateBounds = [5, 50000]
    rampTimeBounds = [1, 65535]
  end
  properties (Constant)
      NumRampers = 2 % Two rampers in case gains need to cross zero
  end
  properties (Dependent)
      DlgRamperInit
  end
  methods
    %----------------------------------------------------------------------
    function this = ToneConfig()
        % ToneConfig constructor: assign component property objects
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
    %----------------------------------------------------------------------
    function value = get.DlgRamperInit(this)
        % Use the InitalLinearGains and number of rampers to create an
        % initial structure value for the rampers
        initialGains = ones(1,this.NumRampers);
        value = RgainBusUtil.GetInitStruct(initialGains);
    end
  end






properties (Dependent, Hidden) % [Rename-Compatibility]
   DefaultMaskBoostDB
   DefaultMaskBoostHz
   DefaultMaskCutDB
   DefaultMaskCutHz
   DefaultMaskOffset
   DefaultMaskScale
   MaskFilterType
end
methods
   function set.DefaultMaskBoostDB(obj,val)
      obj.DefaultBoostDb = val;
   end
   function val = get.DefaultMaskBoostDB(obj)
      val = obj.DefaultBoostDb;
   end
   function set.DefaultMaskBoostHz(obj,val)
      obj.DefaultBoostHz = val;
   end
   function val = get.DefaultMaskBoostHz(obj)
      val = obj.DefaultBoostHz;
   end
   function set.DefaultMaskCutDB(obj,val)
      obj.DefaultCutDb = val;
   end
   function val = get.DefaultMaskCutDB(obj)
      val = obj.DefaultCutDb;
   end
   function set.DefaultMaskCutHz(obj,val)
      obj.DefaultCutHz = val;
   end
   function val = get.DefaultMaskCutHz(obj)
      val = obj.DefaultCutHz;
   end
   function set.DefaultMaskOffset(obj,val)
      obj.DefaultOffset = val;
   end
   function val = get.DefaultMaskOffset(obj)
      val = obj.DefaultOffset;
   end
   function set.DefaultMaskScale(obj,val)
      obj.DefaultScale = val;
   end
   function val = get.DefaultMaskScale(obj)
      val = obj.DefaultScale;
   end
   
   function set.MaskFilterType(obj,val)
      obj.FilterType = val;
   end
   function val = get.MaskFilterType(obj)
      val = obj.FilterType;
   end
   
end % [Rename-Compatibility]

end
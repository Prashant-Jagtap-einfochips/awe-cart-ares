classdef MidrangeConfig < BtcBaseClass
% MidrangeConfig Build-time configuration class for the Midrange block
% Set the Offset and Scale to normalize the input value between -1 (maximum
% cut) and 1 (maximum boost) so the following equation becomes true:
%     normalized_float_value = (integer_RTC_index - Offset) / Scale
% Usage:
%    aConfig = MidrangeConfig
%

%   Copyright 2021 Bose Corporation

  properties (GetObservable,SetObservable)
    BankNumber  = 0
    DefaultBoostHz  = 775
    DefaultBoostq  = 0.9
    DefaultBoostDb  = 20
    DefaultCutHz  = 775
    DefaultCutq  = 0.9
    DefaultCutDb  = 20
    DefaultDefaultOffset  = 128
    DefaultDefaultScale  = 127
  end
  properties (Constant)
      NumRampers = 2
  end
  properties (Dependent)
      DlgRamperInit
  end
  methods
    %----------------------------------------------------------------------
    function this = MidrangeConfig()
        % MidrangeConfig constructor: assign component property objects
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
  

  
  
  %------------------------------------------------------------------------
  % Old names with "Mask" from initial BTC conversion work.
properties (Dependent, Hidden) % [Rename-Compatibility]
   DefaultMaskBoostHz
   DefaultMaskBoostQ
   DefaultMaskBoostdB
   DefaultMaskCutHz
   DefaultMaskCutQ
   DefaultMaskCutdB
   DefaultMaskDefaultOffset
   DefaultMaskDefaultScale
end
methods
   function set.DefaultMaskBoostHz(obj,val)
      obj.DefaultBoostHz = val;
   end
   function val = get.DefaultMaskBoostHz(obj)
      val = obj.DefaultBoostHz;
   end
   function set.DefaultMaskBoostQ(obj,val)
      obj.DefaultBoostq = val;
   end
   function val = get.DefaultMaskBoostQ(obj)
      val = obj.DefaultBoostq;
   end
   function set.DefaultMaskBoostdB(obj,val)
      obj.DefaultBoostDb = val;
   end
   function val = get.DefaultMaskBoostdB(obj)
      val = obj.DefaultBoostDb;
   end
   function set.DefaultMaskCutHz(obj,val)
      obj.DefaultCutHz = val;
   end
   function val = get.DefaultMaskCutHz(obj)
      val = obj.DefaultCutHz;
   end
   function set.DefaultMaskCutQ(obj,val)
      obj.DefaultCutq = val;
   end
   function val = get.DefaultMaskCutQ(obj)
      val = obj.DefaultCutq;
   end
   function set.DefaultMaskCutdB(obj,val)
      obj.DefaultCutDb = val;
   end
   function val = get.DefaultMaskCutdB(obj)
      val = obj.DefaultCutDb;
   end
   function set.DefaultMaskDefaultOffset(obj,val)
      obj.DefaultDefaultOffset = val;
   end
   function val = get.DefaultMaskDefaultOffset(obj)
      val = obj.DefaultDefaultOffset;
   end
   function set.DefaultMaskDefaultScale(obj,val)
      obj.DefaultDefaultScale = val;
   end
   function val = get.DefaultMaskDefaultScale(obj)
      val = obj.DefaultDefaultScale;
   end
end % [Rename-Compatibility]
  
end

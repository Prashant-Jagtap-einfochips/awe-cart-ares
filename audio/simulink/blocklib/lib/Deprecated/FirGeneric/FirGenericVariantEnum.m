classdef FirGenericVariantEnum < BtcVariantEnum
% FirGEnericVariantEnum Enumerated listing of BTC variants for FirGEneric.
% This class collaborates with the BtcBassClassWithVariants and BtcVariant
% classes to enable variant configuration. https://wiki.bose.com/x/Uyo9Cg
% Specify all supported variant choices for this block and the name of the
% classdef (inheriting from BtcVariant) associated with each choice. 

%   Copyright 2023 Bose Corporation

   enumeration
        Generic             (1, 'FirGenericGenericVariantConfig')
        SharcPlusCore       (2, 'FirGenericSharcPlusCoreVariantConfig')
        SharcPlusAccel      (3, 'FirGenericSharcPlusAccelVariantConfig')
        Hexagon             (4, 'FirGenericHexagonVariantConfig')
        ArmNeonV7V8         (5, 'FirGenericArmNeonV7V8VariantConfig')
   end

end
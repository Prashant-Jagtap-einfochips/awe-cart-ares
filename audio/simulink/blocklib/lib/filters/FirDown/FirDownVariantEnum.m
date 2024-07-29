classdef FirDownVariantEnum < BtcVariantEnum
% FirDownVariantEnum Enumerated listing of BTC variants for FirDown.
% This class collaborates with the BtcBassClassWithVariants and BtcVariant
% classes to enable variant configuration. https://wiki.bose.com/x/Uyo9Cg
% Specify all supported variant choices for this block and the name of the
% classdef (inheriting from BtcVariant) associated with each choice. 

%   Copyright 2023 Bose Corporation

   enumeration
        Simulink    (1, 'BtcVariant') % Simulink Fir Decimaton Block
        WithFir     (6, 'FirDownWithFirVariantConfig') % FirDown Implemented with Blocklib Fir block
%         Hexagon     (3, 'BtcVaraint')                % TODO   
%         SharcPlus   (5, 'BtcVariant')                % TODO
   end

end
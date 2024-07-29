classdef DelayTypeEnum < Simulink.IntEnumType
% DelayTypeEnum Enumerated class definition for DelayType
% Usage:
%    value = DelayTypeEnum.<enumeration-member>;
%    value = "<enumeration-member>";
%    value = <uint32 equivalent of enumeration-member>;
%

%   Copyright 2022 Bose Corporation

   enumeration
      InSamples (1)
      InMilliseconds (2)

   end

   % Ensure globally unique names for generated C-language enumerations 
   methods (Static = true)
        function doIt = addClassNameToEnumNames()
            doIt = true;
        end
   end
end
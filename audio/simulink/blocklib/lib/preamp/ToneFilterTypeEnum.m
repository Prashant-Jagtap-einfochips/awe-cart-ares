classdef ToneFilterTypeEnum < Simulink.IntEnumType
% ToneFilterTypeEnum Enumerated class definition for MaskFilterType
% Usage:
%    value = ToneFilterTypeEnum.<enumeration-member>;
%    value = "<enumeration-member>";
%    value = <uint32 equivalent of enumeration-member>;
%

%   Copyright 2021 Bose Corporation

   enumeration
      % Reference: https://www.mathworks.com/help/matlab/matlab_oop/enumerations.html
      lowpass (1)
      highpass (2)
   end
   
   % Ensure globally unique names for generated C-language enumerations 
   methods (Static = true)
        function doIt = addClassNameToEnumNames()
            doIt = true;
        end
   end
   
end
classdef RateSelectorEnum < Simulink.IntEnumType
% RateSelectorEnum Enumerated class definition for RateSelector
% Usage:
%    value = RateSelectorEnum.<enumeration-member>;
%    value = "<enumeration-member>";
%    value = <uint32 equivalent of enumeration-member>;
%

%   Copyright 2021 Bose Corporation

   enumeration
      OutputSampleRate (2)
      % Reference: https://www.mathworks.com/help/matlab/matlab_oop/enumerations.html
      % North (0)
      % East  (90)
      % South (180)
      % West  (270)
   end

   % Ensure globally unique names for generated C-language enumerations 
   methods (Static = true)
        function doIt = addClassNameToEnumNames()
            doIt = true;
        end
   end
end
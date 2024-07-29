classdef PoolDelayVariantSelectEnum < int32
% PoolDelayVariantSelectEnum Enumerated class definition for 
% PoolDelayVariantSelect
% Usage:
%    value = PoolDelayVariantSelectEnum.<enumeration-member>;
%    value = "<enumeration-member>";
%    value = <uint32 equivalent of enumeration-member>;
%

%   Copyright 2021 Bose Corporation

   enumeration
      % Reference: https://www.mathworks.com/help/matlab/matlab_oop/enumerations.html
      
      L1Memory (1)
      L2Memory (2)
      Generic (3)     
   end
   
   methods (Static = true)
      function doIt = addClassNameToEnumNames()
         doIt = true;
      end
   end      
end
classdef TrivialInterpretationMask
% TrivialInterpretationMask 
%    Copies state value without interpretation from TSP to StateVar.
%
%    Documentation for TrivialInterpretationMask
%       doc TrivialInterpretationMask
%
%
methods
function CB_TriggerInit
   %  If the location has changed, force the block to run its init by
   %  changing a parameter. This in turn causes the icon to refresh.
end
function Init
   %  TODO: Verify that the attached StateVar and TSP are appropriate
   %  (may not be necessary to fuss with such things...)
end
function TrivialInterpretationMask
   % TrivialInterpretationMask 
   %    Copies state value without interpretation from TSP to StateVar.
   %
   %    Documentation for TrivialInterpretationMask/TrivialInterpretationMask
   %       doc TrivialInterpretationMask
   %
end
end
end

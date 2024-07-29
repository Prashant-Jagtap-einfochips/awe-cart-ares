classdef InterpretTriggerMask
% InterpretTriggerMask 
%    This block collaborates with StateVar and TSP blocks to accomplish
%    the interpretation of Target State Probe Data into State Variable
%    data --- but only for code generation and usually in a partitioned
%    Interpretation Engine model.
%
%    Documentation for InterpretTriggerMask
%       doc InterpretTriggerMask
%
%
methods
function CreateEmptyInterpretation
   %  Create and hookup a blank example interpretation unless there's
   %  already something connected.
end
function CB_TriggerInit
   %  Hide the Error box
end
function InterpretTriggerMask
   % InterpretTriggerMask 
   %    This block collaborates with StateVar and TSP blocks to accomplish
   %    the interpretation of Target State Probe Data into State Variable
   %    data --- but only for code generation and usually in a partitioned
   %    Interpretation Engine model.
   %
   %    Documentation for InterpretTriggerMask/InterpretTriggerMask
   %       doc InterpretTriggerMask
   %
end
end
end

classdef TargetStateVarMask
% TargetStateVarMask StateVar with trivial interpretation from Target State
%  Probe
%    This block encapsultes a StateVar, a trivial interpretation, and a TSP
%
%    Documentation for TargetStateVarMask
%       doc TargetStateVarMask
%
%
methods
function CB_ConvertToFullInterpretation
   %  This function takes a one-way trip for the specified TargetStateVar to
   %  convert it into a full interpretation with a MATLAB function block.
end
function CB_UseSameName
   %  Check if state variable block is still with us
end
function CB_TriggerInit
   %  If the role is anything other than automatic, make sure it's
   %  visible, otherwise hide it
end
function UpdateDialogTSPText
   %  copy text from the child TSP mask to our mask
end
function TargetStateVarMask
   % TargetStateVarMask StateVar with trivial interpretation from Target State
   %  Probe
   %    This block encapsultes a StateVar, a trivial interpretation, and a TSP
   %
   %    Documentation for TargetStateVarMask/TargetStateVarMask
   %       doc TargetStateVarMask
   %
end
end
end

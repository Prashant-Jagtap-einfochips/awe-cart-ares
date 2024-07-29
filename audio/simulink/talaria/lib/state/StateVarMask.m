classdef StateVarMask
% StateVarMask State variable mask
%  Receives interpreted target state probe data through triggered
%  interpretation logic. This block is only present in Source models and
%  the Talaria State Interpretation Engine
%  Go ahead and save the inport value to a shared workspace parameter
%  for convenience in debugging interpretations, but this value is never
%  used for code generation. The Interpretation Engine does not have any
%  "startup" state variable values.
%  Also - I'm not sure why anybody would want to cast the interpreted
%  state we receive into another type, but the capability exists, so why
%  not.
%
%    Documentation for StateVarMask
%       doc StateVarMask
%
%
methods
function IsStateVar
   %  IsStateVar returns true if block provided (the end of signal trace)
   %  is a StateVar block... specifically the S-Function inside a StateVar. This
   %  method is called externally from the InterpretTrigger mask
end
function CB_TriggerInit
   %  Since this callback is "the block itself", this is a safe place
   %  to modify the mask text without raising any modification flags
end
function GetStoredValue
   %  Do not load values for libraries - just models
   %
   %Help for StateVarMask.GetStoredValue is inherited from superclass DataMapUtil
end
function SetStoredValue
   %  SetStoredValue Set the value stored in the data map for the
   %  specified block.
   %  Usage:
   %        map.SetStoredValue(blockH,value);
   %
   %Help for StateVarMask.SetStoredValue is inherited from superclass DataMapUtil
end
function ReceiveInportValue
   %  Called from our S-Func counterpart, this routine saves the
   %  inport value to workspace storage for debug display
end
function GetTrigger
   %  GetTrigger returns the StateVar's interpretation trigger block.
end
function StateVarMask
   % StateVarMask State variable mask
   %  Receives interpreted target state probe data through triggered
   %  interpretation logic. This block is only present in Source models and
   %  the Talaria State Interpretation Engine
   %  Go ahead and save the inport value to a shared workspace parameter
   %  for convenience in debugging interpretations, but this value is never
   %  used for code generation. The Interpretation Engine does not have any
   %  "startup" state variable values.
   %  Also - I'm not sure why anybody would want to cast the interpreted
   %  state we receive into another type, but the capability exists, so why
   %  not.
   %
   %    Documentation for StateVarMask/StateVarMask
   %       doc StateVarMask
   %
end
end
end

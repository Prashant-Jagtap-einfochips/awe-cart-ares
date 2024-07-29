classdef TOP_Mask
% TOP_Mask Target Overlay Parameter mask
%    This block modifies its behavior to support three environments:
%    source model : input and output ports
%    partitioned target core : output port only
%    translation engine : input port only
%  When an inport is present, its value is saved to a shared workspace
%  variable based on the ModelRootName() version of the model name.
%
%    Documentation for TOP_Mask
%       doc TOP_Mask
%
%
methods
function Transform7_0
   %  Transform function automatically created by AutomateMask
end
function Transform125
   %  Gracefully drop the a bunch of package parametera from existence
end
function Transform117
   %  Gracefully drop the "Value" parameter from existence
end
function Transform103
   %  Set backward-compatible "inherit" value for new DataType param.
end
function IsTOP
   %  IsTOP returns true if block provided (the end of signal trace)
   %  is a TOP block... specifically the S-Function inside a TOP. This
   %  method is called externally from the Translate Trigger mask
end
function CB_EditValueDialog
   %  retrieve the value from storage
end
function CB_TriggerInit
   %  If the role is anything other than automatic, make sure it's
   %  visible, otherwise hide it
end
function MexMaskEval
   %  Call Mask.GetEvaluatedParameter - through here for breakpoints
end
function SetupMapLoad
   %  sets up model to load the data map in its InitFcn callback
end
function GetStoredValue
   %  Do not load values for libraries - just models
   %
   %Help for TOP_Mask.GetStoredValue is inherited from superclass DataMapUtil
end
function SetStoredValue
   %  SetStoredValue Set the value stored in the data map for the
   %  specified block.
   %  Usage:
   %        map.SetStoredValue(blockH,value);
   %
   %Help for TOP_Mask.SetStoredValue is inherited from superclass DataMapUtil
end
function GetOutputSignalData
   %  Called from our C MEX counterpart, this routine retrieves data
   %  appropriate to copy into the output signal buffer. Note that for
   %  fixed-point types this method makes sure to grab the stored
   %  integer values instead of the embedded.fi object itself. The
   %  intialization values used in code generation come from here.
end
function ReceiveInportValue
   %  Called from our C MEX counterpart, this routine both saves the
   %  inport value to storage as well as casts/sets the outport value.
end
function SetOutputValue
   %  Cast the provided value into the desired datatype and stuff it
   %  into UserData to be retrieved by the C MEX block responsible for
   %  sending it.
end
function CastValue
   %  accepts dataType as returned from evaluated datatype mask
   %  parameter (i.e. unidt mask parameter)
end
function ValidateBank
   %  Ensure bank number is valid, returning back a string version
end
function CheckTrigger
   %  GetTriggers Follow the TOP's inbound signal to ensure it connects
   %  to triggered translation logic
end
function TOP_Mask
   % TOP_Mask Target Overlay Parameter mask
   %    This block modifies its behavior to support three environments:
   %    source model : input and output ports
   %    partitioned target core : output port only
   %    translation engine : input port only
   %  When an inport is present, its value is saved to a shared workspace
   %  variable based on the ModelRootName() version of the model name.
   %
   %    Documentation for TOP_Mask/TOP_Mask
   %       doc TOP_Mask
   %
end
end
end

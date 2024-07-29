classdef TuneVarMask
% TuneVarMask Tune Variable mask
%    Tune Variables feed tuning data to translation blocks which in turn
%    feed translated data into target overlay parameters (TOPs).
%    This block collaborates with Translation Trigger blocks to accomplish
%    automatic triggering of translation logic as tuning variables
%    change.
%
%    Documentation for TuneVarMask
%       doc TuneVarMask
%
%
properties
  % BusElemSize -  NOTE: Must match TuneVarMEX.c enum
BusElemSize
  % BusElemValue -  NOTE: Must match TuneVarMEX.c enum
BusElemValue
end
methods
function Transform126
   %  Rename a parameter
end
function Transform104
   %  Gracefully drop the a bunch of package parameters from existence
end
function GetOutValue
   %  Retrieve the default TuneVar value from the mask
end
function IsTuneVar
   %  IsTuneVar returns true if block provided (the end of signal trace)
   %  is a IsTuneVar block... specifically the S-Function inside a IsTuneVar. This
   %  method is called externally from the Translate Trigger mask
end
function MaxDim
   %  MaxDim use function to return this value since FEVAL from TLC
   %  does not appear to be able to read properties
end
function UpdateDialogText
   %  Update text
end
function GetActiveTunePath
   %  Attempt to retrieve, but otherwise return empty with no error
end
function CB_TriggerInit
   %  Close the panel
end
function GetBus
   %  play the boolean/logical game again. Only "boolean" allowed here
   %  or otherwise Simulink.Bus.createMATLABStruct fails with ugly error
end
function GetTriggers
   %  GetTriggers Follow the TuneVar's outbound signal to find all
   %  associated translation trigger blocks.
end
function PadDimension
   %  Get the current size
end
function TuneVarMask
   % TuneVarMask Tune Variable mask
   %    Tune Variables feed tuning data to translation blocks which in turn
   %    feed translated data into target overlay parameters (TOPs).
   %    This block collaborates with Translation Trigger blocks to accomplish
   %    automatic triggering of translation logic as tuning variables
   %    change.
   %
   %    Documentation for TuneVarMask/TuneVarMask
   %       doc TuneVarMask
   %
end
end
end

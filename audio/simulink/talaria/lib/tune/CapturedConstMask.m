classdef CapturedConstMask
% CapturedConstMask Target Overlay Parameter mask
%    This block modifies its behavior to support three environments:
%    source model : input and output ports
%    engine : output port only
%
%    Documentation for CapturedConstMask
%       doc CapturedConstMask
%
%
methods
function SetOutputValue
   %  Stuff into UserData to be retrieved by the C MEX block
end
function SetupMapLoad
   %  sets up model to load the data map in its InitFcn callback
end
function GetMaskValue
   %  Called by our C MEX block to retrieve output (when no in port)
end
function SetMaskValue
   %  Called from our C MEX counterpart, this routine both saves the
   %  inport value to storage as well as sets the outport value.
end
function IsCaptConst
   %  IsCaptConst returns true if block provided (the end of signal trace)
   %  is a CapturedConstant block... specifically the S-Function inside.
   %  This method is called externally from the Translate Trigger mask
end
function CapturedConstMask
   % CapturedConstMask Target Overlay Parameter mask
   %    This block modifies its behavior to support three environments:
   %    source model : input and output ports
   %    engine : output port only
   %
   %    Documentation for CapturedConstMask/CapturedConstMask
   %       doc CapturedConstMask
   %
end
end
end

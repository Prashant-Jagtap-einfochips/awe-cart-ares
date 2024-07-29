classdef TuneTOP_Mask
% TuneTOP_Mask TuneVar with trivial translation to Target Overlay Parameter
%    This block encapsultes a TuneVar, a trivial translation, and a TOP
%
%    Documentation for TuneTOP_Mask
%       doc TuneTOP_Mask
%
%
methods
function Transform104
   %  Gracefully drop the a bunch of package parameters from existence
end
function CreateMaskParametersAndDialog
   %  Acquire an empty mask ready to accept goodies
end
function CB_ConvertToFullTranslation
   %  This function takes a one-way trip for the specified TuneTOP to
   %  convert it into a full translation with a MATLAB function block.
end
function CB_EditDescription
   %  Invoke the description editor code from the TuneVar mask... but
   %  execute it on OUR block instead:
end
function CB_UseSameName
   %  Check if tune variable block is still with us
end
function CB_TriggerInit
   %  If the role is anything other than automatic, make sure it's
   %  visible, otherwise hide it
end
function TuneTOP_Mask
   % TuneTOP_Mask TuneVar with trivial translation to Target Overlay Parameter
   %    This block encapsultes a TuneVar, a trivial translation, and a TOP
   %
   %    Documentation for TuneTOP_Mask/TuneTOP_Mask
   %       doc TuneTOP_Mask
   %
end
end
end

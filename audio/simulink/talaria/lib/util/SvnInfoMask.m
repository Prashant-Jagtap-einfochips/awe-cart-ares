classdef SvnInfoMask
%  SvnInfoMask: Mask logic for the SvnInfo block mask
%
%    Documentation for SvnInfoMask
%       doc SvnInfoMask
%
%
methods
function CB_TriggerInit
   %  If the location has changed, force the block to run its init by
   %  changing a parameter. This in turn causes the icon to refresh.
end
function GetPaddedInt
   %  Returns a ASCIIZ string of provided strIn, but with enough
   %  terminators to always return padCount characters.
   %  Usage: SvnInfoMask.GetPaddedInt(strIn,padCount);
end
function SvnInfoMask
   %  SvnInfoMask: Mask logic for the SvnInfo block mask
   %
   %    Documentation for SvnInfoMask/SvnInfoMask
   %       doc SvnInfoMask
   %
end
end
end

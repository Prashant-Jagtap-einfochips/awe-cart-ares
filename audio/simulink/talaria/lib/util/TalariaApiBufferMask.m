classdef TalariaApiBufferMask
% TalariaApiBufferMask Talaria API Buffer Mask
%    This block sets up the storage class and declaration of a signal line
%
%    Documentation for TalariaApiBufferMask
%       doc TalariaApiBufferMask
%
%
methods
function CB_TriggerInit
   %  If the location has changed, force the block to run its init by
   %  changing a parameter. This in turn causes the icon to refresh.
end
function SetupStorage
   %  Headerfiles must be blank in the presence of any model references
   %  Happily the ApiBuffer storage class TLC will explicitly choose
   %  the headerfile intended for these signals (<model>_TalariaAPI.h)
end
function TalariaApiBufferMask
   % TalariaApiBufferMask Talaria API Buffer Mask
   %    This block sets up the storage class and declaration of a signal line
   %
   %    Documentation for TalariaApiBufferMask/TalariaApiBufferMask
   %       doc TalariaApiBufferMask
   %
end
end
end

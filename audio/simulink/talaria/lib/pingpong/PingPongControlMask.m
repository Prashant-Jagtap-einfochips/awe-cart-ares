classdef PingPongControlMask
% PingPongControlMask Ping Pong Buffer Control Mask
%    This block is part of the multi-core partitioning script mechanism.
%    It collaborates with the multi-core partitioning scripts and the
%    custom storage class 'PingPongStruct' to generate C source
%    code that allocates, accesses, and coordinates the operation of
%    cross-core signal buffers.
%
%    Documentation for PingPongControlMask
%       doc PingPongControlMask
%
%
methods
function Transform5_2
   %  Gracefully eliminate mask parameter
end
function Transform1_80
   %  Gracefully eliminate mask parameter
end
function CB_TriggerInit
   %  If the location has changed, force the block to run its init by
   %  changing a parameter. This in turn causes the icon to refresh.
end
function SetupControl
   %  Ensure there is a PingPongControl block in the model root system
end
function UpdateDialogText
   %  Update dialog informational text
   %  ... current overlay group
end
function ParseBufferNames
   %  Return information about the buffers by parsing their names.
   %  Buffer numbers start at 0 but of course our arrays start at 1
end
function PingPongControlMask
   % PingPongControlMask Ping Pong Buffer Control Mask
   %    This block is part of the multi-core partitioning script mechanism.
   %    It collaborates with the multi-core partitioning scripts and the
   %    custom storage class 'PingPongStruct' to generate C source
   %    code that allocates, accesses, and coordinates the operation of
   %    cross-core signal buffers.
   %
   %    Documentation for PingPongControlMask/PingPongControlMask
   %       doc PingPongControlMask
   %
end
end
end

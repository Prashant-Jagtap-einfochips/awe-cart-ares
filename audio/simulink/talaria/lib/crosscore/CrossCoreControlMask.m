classdef CrossCoreControlMask
% CrossCoreControlMask Ping Pong Buffer Control Mask
%    This block is part of the multi-core partitioning script mechanism.
%    It collaborates with the multi-core partitioning scripts and the
%    custom storage class 'PingPongStruct' to generate C source
%    code that allocates, accesses, and coordinates the operation of
%    cross-core signal buffers.
%
%    Documentation for CrossCoreControlMask
%       doc CrossCoreControlMask
%
%
methods
function CB_TriggerInit
   %  If the location has changed, force the block to run its init by
   %  changing a parameter. This in turn causes the icon to refresh.
end
function SetupControl
   %  Ensure there is a CrossCoreControl block in the model root system
end
function UpdateDialogText
   %  Update dialog informational text
   %  ... current overlay group
end
function ParseBufferNames
   %  Return information about the buffers by parsing their names.
   %  Buffer numbers start at 0 but of course our arrays start at 1
end
function WireSystem
   %  Wire up terminators to the useless outputs of our internal
   %  control block. These outputs are here only to exist in a
   %  particular sample time (so all rate groups are present in model)
end
function CrossCoreControlMask
   % CrossCoreControlMask Ping Pong Buffer Control Mask
   %    This block is part of the multi-core partitioning script mechanism.
   %    It collaborates with the multi-core partitioning scripts and the
   %    custom storage class 'PingPongStruct' to generate C source
   %    code that allocates, accesses, and coordinates the operation of
   %    cross-core signal buffers.
   %
   %    Documentation for CrossCoreControlMask/CrossCoreControlMask
   %       doc CrossCoreControlMask
   %
end
end
end

classdef PingPongXmlMask
% PingPongXmlMask Control Mask for direct exposure of cross-core buffers.
%    This block is part of the multi-core partitioning script mechanism.
%    It collaborates with the multi-core partitioning scripts and the
%    custom storage class 'PingPongStruct' to generate C source
%    code that embodies the buffers intended for cross-core communication.
%    Additionally, this block collaborates with the following elements:
%    -> ModelSyncApi block to dump into the same XML generation.
%    -> aggregateSyncApi script pulls the XML file together from all cores
%    -> parition tool base rate: manifests a base-rate signal on canvas to
%    ensure that all partitioned cores have the same base rate.
%
%    Documentation for PingPongXmlMask
%       doc PingPongXmlMask
%
%
methods
function Transform1_79
   %  Gracefully eliminate mask parameter
end
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
function PingPongXmlMask
   % PingPongXmlMask Control Mask for direct exposure of cross-core buffers.
   %    This block is part of the multi-core partitioning script mechanism.
   %    It collaborates with the multi-core partitioning scripts and the
   %    custom storage class 'PingPongStruct' to generate C source
   %    code that embodies the buffers intended for cross-core communication.
   %    Additionally, this block collaborates with the following elements:
   %    -> ModelSyncApi block to dump into the same XML generation.
   %    -> aggregateSyncApi script pulls the XML file together from all cores
   %    -> parition tool base rate: manifests a base-rate signal on canvas to
   %    ensure that all partitioned cores have the same base rate.
   %
   %    Documentation for PingPongXmlMask/PingPongXmlMask
   %       doc PingPongXmlMask
   %
end
end
end

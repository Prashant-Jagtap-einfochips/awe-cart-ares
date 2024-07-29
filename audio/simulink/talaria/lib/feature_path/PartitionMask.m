classdef PartitionMask
%  Mask logic for Partition block
%
%    Documentation for PartitionMask
%       doc PartitionMask
%
%
methods
function CB_TriggerInit
   %  If the location has changed, force the block to run its init by
   %  changing a parameter. This in turn causes the icon to refresh.
end
function CB_FindFreeNumber
   %  Find a free number by validating "-1" which will perform a search
end
function GetPartitionNumber
   %  Search up the hierarchy to look for partition blocks
   %  Usage: [partitionNumber,partitionBlockH] = GetPartitionNumber(H)
end
function PartitionMask
   %  Mask logic for Partition block
   %
   %    Documentation for PartitionMask/PartitionMask
   %       doc PartitionMask
   %
end
end
end

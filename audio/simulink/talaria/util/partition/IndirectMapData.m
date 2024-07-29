classdef IndirectMapData
% INDIRECTMAPDATA Holds data about indirect addressing group/partitions
%    Holds all block paths and assigned numbers for overlay groups and
%    partitions.
%
%    Documentation for IndirectMapData
%       doc IndirectMapData
%
%
properties
  % coreMap - Array of core numbers by partitionIndex
coreMap
  % partitionBlockPath - Array of partition block by partitionIndex#
partitionBlockPath
  % groupNameArray - Array of FeaturePaths by groupNumber
groupNameArray
  % groupNumberLookup - FeaturePath -> group number
groupNumberLookup
  % groupBlockLookup - FeaturePath -> first partition block
groupBlockLookup
end
methods
function AddPartitionBlock
   %IndirectMapData/AddPartitionBlock is a function.
   %    AddPartitionBlock(this, partitionBlockPathIn, groupFeaturePath, groupNumber, partitionNumber, coreNumber)
end
function GetGroupPartitionIndex
   %IndirectMapData/GetGroupPartitionIndex is a function.
   %    groupPartitionIndex = GetGroupPartitionIndex(~, groupNumber, partitionNumber)
end
function IndirectMapData
   % INDIRECTMAPDATA Construct IndirectMapData instance
   %    Sets up blank instance
   %
   %    Documentation for IndirectMapData/IndirectMapData
   %       doc IndirectMapData
   %
end
end
end

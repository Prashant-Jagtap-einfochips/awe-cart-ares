function CreateIndirectAddressingMap
% CreateIndirectAddressingMap Generates code for indirect addressing mode to
% map token group & partition numbers to partitioned cores.
%    Creates C source code that maps token to core number
%  Usage:
%   mapData = CreateIndirectAddressingMap(tagModelName,tagList,[mapData],[assignedCore])
%   If mapData provided, processes model as a referenced model - assigning
%   all partitions to the tagList and assignedCore provided.
%   Otherwise, performs top-level model map creation. If tagList is empty,
%   performs single-core processing which just checks that all partition and
%   group numbers are compatible.

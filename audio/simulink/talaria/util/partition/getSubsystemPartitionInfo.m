function getSubsystemPartitionInfo
% getSubsystemPartitionInfo Collects information from model in preparation
% of partitioning
%    [P, T, S, M] = getSubsystemPartitioningInfo(M) searches through model M
%    and returns a structure array of properties for each atomic subsystem
%    P, list of unique subsystem tags T, list of sample times S, and model
%    checksum map M.
%    The subsystems in P are listed in their sorted execution order.
% 
%    Note: The model is put in compiled mode in this function to pull out
%    all the necessary information.

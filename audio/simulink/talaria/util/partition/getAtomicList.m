function getAtomicList
% --------------------------------------------------------------------------
%  Function getAtomicList
% 
%  This function will create a file <modelName>_atomicList.m which is used
%  to assign each atomic subystem to a given CPU / Core.  
% 
%  Inputs:
%    modelName : The name of the current root level system (i.e. model)
%    numCPU: the number of CPUs in the system
%    numCores: The number of cores per CPU.  Entered as an array.  
%      e.g. if numCPU = 2 then could be...
%              numCores = [4 2]

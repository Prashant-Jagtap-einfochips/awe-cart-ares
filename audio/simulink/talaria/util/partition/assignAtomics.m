function assignAtomics
% assignAtomics Assigns top level atomic subsystems to its CPU / Core.
%    This function uses the CPU / Core assignment file to assign each atomic
%    subsystem to a CPU / Core pair. The named model is not modified.
%    Instead, a new model named <modelName>_TAGGED is created and tagged
%    with the assignments. Returns the name of this newly created model.
% 
%    T = assignAtomics(M) creates new model T with tagged atomic subsystems
%    indicating their CPU / Core based on model M and its atomic list.

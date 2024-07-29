function GetTopAtomic
%  GetTopAtomic attempts to return a Simulink block URL for the top-most
%  atomic subsystem containing the indicated block "H" unless "H" is a
%  root-level diagram, in which case ALL top-level atomic subsystems are
%  returned.
%  Note that for the purpose of listing all top-level atomics, model 
%  reference blocks are considered to be atomic subsystems.

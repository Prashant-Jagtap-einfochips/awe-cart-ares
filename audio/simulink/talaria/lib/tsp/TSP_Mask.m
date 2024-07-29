classdef TSP_Mask
% TSP_Mask Target State Probe: Retrieve target state (code gen on target)
%  and feed state into interpreter (code gen interpretation engine)
%    This block modifies its behavior to support three environments:
%    source model : input and output ports, simulation value passthrough
%    partitioned target core : input port only, no simulation function
%    interpretation engine : output port only, no simulation function
% 
%  To suppport the interpretation engine, the block needs to detect and
%  remember the datatype and dimensions of the input signal - so it saves
%  the value in the same manner as the TOP block (i.e. to base workspace).
%  Unlike a TOP though the value itself is not needed - just need type and
%  size.
%
%    Documentation for TSP_Mask
%       doc TSP_Mask
%
%
properties
  %  Since this is a static class but workspace-stored values need a
  %  unique variable name, all methods that get or set the stored value
  %  must reference this static constant INSTANCE of the DataMap class
DataMapType
end
methods
function IsTSP
   %  IsTSP returns true if block provided
   %  is a TSP block... specifically the S-Function inside a TSP. This
   %  method is called externally from the Interpret Trigger mask
end
function CB_TriggerInit
   %  If the role is anything other than automatic, make sure it's
   %  visible, otherwise hide it
end
function SetupMapLoad
   %  sets up model to load the data map in its InitFcn callback
end
function UpdateDialogText
   %  Update dialog informational text
   %  ... current overlay group
end
function TSP_Mask
   % TSP_Mask Target State Probe: Retrieve target state (code gen on target)
   %  and feed state into interpreter (code gen interpretation engine)
   %    This block modifies its behavior to support three environments:
   %    source model : input and output ports, simulation value passthrough
   %    partitioned target core : input port only, no simulation function
   %    interpretation engine : output port only, no simulation function
   % 
   %  To suppport the interpretation engine, the block needs to detect and
   %  remember the datatype and dimensions of the input signal - so it saves
   %  the value in the same manner as the TOP block (i.e. to base workspace).
   %  Unlike a TOP though the value itself is not needed - just need type and
   %  size.
   %
   %    Documentation for TSP_Mask/TSP_Mask
   %       doc TSP_Mask
   %
end
end
end

classdef LineTrace
% LINETRACE Traces Simulink lines to find nearby blocks.
%    The undocumented TraceDestinationInputPorts and TraceDestinationOutputPorts
%  are unpleasant both because they are undocumented and also because
%  their use causes model display refreshes and line colorizations. This
%  class is a simplistic alternative. It offers less sophistication in
%  its tracing logic, but we own it.
%  LineTrace will navigate up/down through virtual subsystems and attempt to
%  follow Goto/From blocks. Bus Element ports are also supported, but only
%  for the original content of the line. Connecting only partial elements of
%  an originally bussed line is ignored just like an unconnected line. Note
%  that adding to a larger bus and then terminating on a block will still
%  count since the entirety of the original line content makes contact.
%
%    Documentation for LineTrace
%       doc LineTrace
%
%
methods
function GetBlockHandlesFromPortBlock
   %  Returns block handles of blocks immediately upstream or downstream
   %  from the port block.
end
function GetSrcBlockHandles
   %  Returns block handles of blocks immediately upstream. Optionally 
   %  returns any traversed port block and goto/from handles. If specified
   %  and only traces the the inports specified by inPortIndex.
   %  Both outputs are vertical arrays (i.e. one column)
end
function GetDstBlockHandles
   %  Returns block handles of blocks immediately downstream. Optionally 
   %  returns any traversed port block and goto/from handles. If specified
   %  and only traces the the outports specified by outPortIndex.
   %  Both outputs are vertical arrays (i.e. one column)
end
function LineTrace
   % LINETRACE Traces Simulink lines to find nearby blocks.
   %    The undocumented TraceDestinationInputPorts and TraceDestinationOutputPorts
   %  are unpleasant both because they are undocumented and also because
   %  their use causes model display refreshes and line colorizations. This
   %  class is a simplistic alternative. It offers less sophistication in
   %  its tracing logic, but we own it.
   %  LineTrace will navigate up/down through virtual subsystems and attempt to
   %  follow Goto/From blocks. Bus Element ports are also supported, but only
   %  for the original content of the line. Connecting only partial elements of
   %  an originally bussed line is ignored just like an unconnected line. Note
   %  that adding to a larger bus and then terminating on a block will still
   %  count since the entirety of the original line content makes contact.
   %
   %    Documentation for LineTrace/LineTrace
   %       doc LineTrace
   %
end
end
end

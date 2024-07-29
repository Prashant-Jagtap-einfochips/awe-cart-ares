classdef TSP_CaptureControlMask
%  TSP_CaptureControlMask: This block offers convenient control over the
%  size of the TSP capture buffer and the behavior of all TSP blocks in
%  terms of advanced trigger-based captures. Both of these features are
%  ultimately just Simulink.Parameter objects in the model's global data
%  interface (e.g. base workspace or data dictionary)
%
%    Documentation for TSP_CaptureControlMask
%       doc TSP_CaptureControlMask
%
%
methods
function CB_TriggerInit
   %  If the location has changed, force the block to run its init by
   %  changing a parameter. This in turn causes the icon to refresh.
end
function DefineBufferSize
   %  DefineBufferSize ensures that the capture buffer size will be
   %  present, optionally a value can be specified to set the size
end
function DefineEnableAdvanced
   %  DefineAdvancedCapture ensures that the advanced capture switch is
   %  present, optionally a value can be specified to enable or not
end
function TSP_CaptureControlMask
   %  TSP_CaptureControlMask: This block offers convenient control over the
   %  size of the TSP capture buffer and the behavior of all TSP blocks in
   %  terms of advanced trigger-based captures. Both of these features are
   %  ultimately just Simulink.Parameter objects in the model's global data
   %  interface (e.g. base workspace or data dictionary)
   %
   %    Documentation for TSP_CaptureControlMask/TSP_CaptureControlMask
   %       doc TSP_CaptureControlMask
   %
end
end
end

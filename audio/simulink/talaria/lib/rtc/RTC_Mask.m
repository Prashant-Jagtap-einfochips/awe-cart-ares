classdef RTC_Mask
% RTC_Mask Runtime Control block mask
%
%    Documentation for RTC_Mask
%       doc RTC_Mask
%
%
methods
function Transform194
   %  Gracefully drop parameter from existence
end
function Transform190
   %  Gracefully drop parameter from existence
end
function MexMaskEval
   %  Call Mask.GetEvaluatedParameter - through here for breakpoints
end
function CreatePorts
   %  get input to the DestinationBlock
end
function GetReqFuncName
   %  Return C source function name for request handler
end
function CalculatePadding
   %  TODO: padding has too many options right now and we do not know
   %  what the compiler's rules will be.
end
function CalculateTypeCharCounts
   %  Find the size in chars on specified hardware for each data type.
end
function EnableStatusIn
   %  Based on block name (not full name), swaps in a ground or an
   %  inport. Optionally a port number can be specified.
end
function EnableTriggerOut
   %  Based on block name (not full name), swaps in a terminator or an
   %  outport. Optionally a port number can be specified.
end
function InitSimData
   %  Attempt to acquire official SimData object and gracefully fail
end
function ChangeElemPanelVisibility
   %  This function hides any payload element controls with no name
   %  defined - unless requested to add one to the end, in which case
   %  it does *not* hide blank elements until it first reveals a new
   %  blank element.
end
function HackDialogResize
   %  Hack fix to make dialog resize correctly. Otherwise, sometimes
   %  it's not high enough and scroll bars appear
end
function UpdateDialogText
   %  Update text
end
function CB_ElemName
   %  Nothing to do here. We only validate names once they are
   %  actually applied now.
end
function CB_TriggerInit
   %  Generally, this callback only happens when the dialog is first
   %  opened because the parameter is hidden and so cannot be changed.
end
function CB_EnableRSP_StatusIn
   %  We'll take action in the Init process to add/remove the inport
end
function CB_EnableNTF_PrepTrig
   %  We'll take action in the Init process to add/remove the outport
end
function CB_EnableRequest
   %  We'll take action in the Init process to add/remove the outport
   %  that sends the REQ RSP function trigger
end
function RTC_Mask
   % RTC_Mask Runtime Control block mask
   %
   %    Documentation for RTC_Mask/RTC_Mask
   %       doc RTC_Mask
   %
end
end
end

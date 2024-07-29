classdef RTC_SimData
%  RTC_SimData Contains and coordinates RTC events during simulation
%
%    Documentation for RTC_SimData
%       doc RTC_SimData
%
%
methods
function SummarizeService
   %  Summarizes the captured message traffic for the specified
   %  service. Dumps output to console window UNLESS return array
   %  of struct payloads is received.
   %  Usage:
   %    % To dump text to command console:
   %    RTC_SIM_OBJ.SummarizeService('MyExampleRtcService');
   % 
   %    % To return raw payloads
   %    thePayloads = RTC_SIM_OBJ.SummarizeService('MyExampleRtcService');
   %  
   %  Returned payload format is a MATLAB struct with these fields
   %  .name : the FeaturePath name of the RTC service
   %  .time : the simulation time in seconds of the event (double)
   %  .data : a MATLAB struct containing named payload elements.
   %          NOTE that these payload elements have the datatypes
   %          specified in the RTC payload definition e.g. usually 
   %          int32, uint32, or single
   %  .type : one of 'Request', 'Response', or 'Notification'
   % 
   %  See also: TalariaTest.reviewRTC
end
function RecordREQ
   %RTC_SimData/RecordREQ is a function.
   %    RecordREQ(this, name, time, data)
end
function RecordNTF
   %RTC_SimData/RecordNTF is a function.
   %    RecordNTF(this, name, time, data)
end
function RecordRSP
   %RTC_SimData/RecordRSP is a function.
   %    RecordRSP(this, name, time, data)
end
function RetrieveMyReq
   %RTC_SimData/RetrieveMyReq is a function.
   %    [data] = RetrieveMyReq(this, name)
end
function CheckForREQ
   %RTC_SimData/CheckForREQ is a function.
   %    [isHit, iteratorOut] = CheckForREQ(this, name, min_time, max_time, iterator)
end
function ValidateREQs
   %RTC_SimData/ValidateREQs is a function.
   %    [ok] = ValidateREQs(this, clearInvalidWithWarning)
end
function AddNTF
   %RTC_SimData/AddNTF is a function.
   %    AddNTF(this, name, time, data)
end
function AddRSP
   %RTC_SimData/AddRSP is a function.
   %    AddRSP(this, name, time, data)
end
function AddREQ
   %RTC_SimData/AddREQ is a function.
   %    AddREQ(this, name, time, data)
end
function RTC_SimData
   %  RTC_SimData Contains and coordinates RTC events during simulation
   %
   %    Documentation for RTC_SimData/RTC_SimData
   %       doc RTC_SimData
   %
end
end
end

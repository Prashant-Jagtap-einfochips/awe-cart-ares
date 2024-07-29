classdef RtcManager
%  FeaturePathManager Manages unique interface names Run-Time Control blocks
%
%    Documentation for RtcManager
%       doc RtcManager
%
%
methods
function Get
   %  Get retrieves the singleton instance of the manager for the model
   %  associated with the provided block handle. Uses "bdroot" if no
   %  block handle is provided.
   %  Usage:
   %     manager = RtcManager.Get(blockHandle);
end
function GetDispatchTableName
   %  GetDispatchTableName TLC access to dispatch table names
   %  Usage:
   %    theName = GetDispatchTableName(groupNum,partitionNum)
end
function AcquireFeaturePathTokenIndexes
   %  AcquireFeaturePathTokenIndexes TLC access to ordered RTC indexes.
   %  Returns struct of all feature paths mapped to RTC token indexes
   %  and dispatch table names mapped to their sizes.
   %  Assumes all RTC blocks have registered and expected to only be
   %  called from RTC_REQ_RSP.BlockTypeSetup during code generation.
end
function RtcManager
   %  FeaturePathManager Manages unique interface names Run-Time Control blocks
   %
   %    Documentation for RtcManager/RtcManager
   %       doc RtcManager
   %
end
end
end

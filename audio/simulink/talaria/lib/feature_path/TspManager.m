classdef TspManager
%  TspManager Manages unique interface names for Target State Probe blocks
%
%    Documentation for TspManager
%       doc TspManager
%
%
methods
function Get
   %  Get retrieves the singleton instance of the manager for the model
   %  associated with the provided block handle. Uses "bdroot" if no
   %  block handle is provided.
   %  Usage:
   %     manager = TspManager.Get(blockHandle);
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
function TspManager
   %  TspManager Manages unique interface names for Target State Probe blocks
   %
   %    Documentation for TspManager/TspManager
   %       doc TspManager
   %
end
end
end

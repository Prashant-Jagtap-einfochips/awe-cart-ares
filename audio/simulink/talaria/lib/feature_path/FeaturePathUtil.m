classdef FeaturePathUtil
%  FeaturePathUtil Manages unique feature names for Talaria configuration block instances
%
%    Documentation for FeaturePathUtil
%       doc FeaturePathUtil
%
%
methods
function GetPathInfo
   %  GetPathInfo Given a block from which to search, returns structure
   %  with feature path information as specified in options:
   %  Options:
   %   (featurePath is always returned)
   %   G = groupPath and subPath
   %   N = groupNumber (i.e. for indirect addressing mode)
   %   M = modules
   %  Example:
   %   info = FeaturePathUtil.GetPathInfo(gcb,'Gain','GN');
   %   returns info with fields for groupPath, subPath, and groupNumber
   %    in addition to the featurePath.
end
function CatalogPartitions
   %  Optional argument - block handle/path
end
function CatalogGroup
   %  CatalogPath Lists all the overlay group blocks in the
   %  current model which have registered during initialization.
   %  Optional argument - block handle/path
end
function ExistsGroup
   %  ExistsGroup Checks if the provided overlay group has registered.
   %  Optional argument - block handle/path
end
function RegisterGroup
   %  RegisterGroup Used by FeaturePath block mask intialization to register and
   %  verify that the overlay group is unique.
end
function CatalogStateVar
   %  CATALOGSTATE Lists all the StateVar blocks (State Variable) in the
   %  current model which have registered during initialization.
   %  Optional argument - block handle/path
end
function ExistsStateVar
   %  ExistsState Checks if the provided StateVar feature name has registered.
   %  Optional argument - block handle/path
end
function RegisterStateVar
   %  RegisterState Used by StateVar block mask intialization to register and
   %  verify that the StateVar feature name is unique.
end
function CatalogTSP
   %  CatalogTSP Lists all the TSP blocks (Target State Probe) in the
   %  current model which have registered during initialization.
   %  Optional argument - block handle/path
end
function ExistsTSP
   %  ExistsTSP Checks if the specified TSP feature name has registered.
   %  Optional argument - block handle/path
end
function RegisterTSP
   %  RegisterTune Used by TuneVar block mask intialization to register and
   %  verify that the TuneVar feature name is unique.
end
function CatalogTune
   %  CATALOGTUNE Lists all the TuneVar blocks (Tune Variable) in the
   %  current model which have registered during initialization.
   %  Optional argument - block handle/path
end
function ExistsTune
   %  ExistsTune Checks if the provided TuneVar feature name has registered.
   %  Optional argument - block handle/path
end
function RegisterTune
   %  RegisterTune Used by TuneVar block mask intialization to register and
   %  verify that the TuneVar feature name is unique.
end
function CatalogTOP
   %  CATALOGTOP Lists all the TOP blocks (Target Overlay Parameter) in
   %  the current model which have registered during initialization.
   %  Optional argument - block handle/path
end
function ExistsTOP
   %  ExistsTOP Checks if the provided TOP feature name has registered.
   %  Optional argument - block handle/path
end
function RegisterTOP
   %  RegisterTOP Used by TOP block mask intialization to register and
   %  verify that the TOP feature name is unique.
end
function AggregateReferencedModelRTCs
   %  Copy all the registered RTCs from referenced models into the
   %  parent model's registrations.
end
function CatalogRTC
   %  CATALOGRTC Lists all the RTC blocks (Runtime Control) in the
   %  current model which have registered during initialization.
   %  Optional argument - block handle/path
end
function ExistsRTC
   %  ExistsRTC Checks if the provided RTC feature name has registered.
end
function RegisterRTC
   %  REGISTERRTC Used by RTC block mask intialization to register and
   %  verify that the RTC feature name is unique.
end
function FeaturePathUtil
   %  FeaturePathUtil Manages unique feature names for Talaria configuration block instances
   %
   %    Documentation for FeaturePathUtil/FeaturePathUtil
   %       doc FeaturePathUtil
   %
end
end
end

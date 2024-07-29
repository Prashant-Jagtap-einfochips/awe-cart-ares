classdef FeaturePathManager
%  FeaturePathManager Manages unique interface names for Talaria blocks
%  providing external integration paths such as control and calibration.
%  This class replaces the registration features of FeaturePathUtil,
%  enabling better encapsulation of shared methods and separation of
%  non-shared methods.
%  Derived classes must accept modelName as a constructor argument, but do
%  not need to forward it as this class will set it up. Expected access to
%  model-level singletons provided by this class via (see RtcManager.Get):
%  this = FeaturePathManager.GetModelSingleton(blockHandle,mfilename('class'))
%
%    Documentation for FeaturePathManager
%       doc FeaturePathManager
%
%
methods
function AggregateReferencedModels
   %  AggregateReferencedModels will copy all FeaturePath registrations
   %  into the requesting manager (although called recursively, the
   %  requesting "this" object never changes). The purpose of this
   %  aggregation includes:
   %  1 - ensure no FeaturePath uniqueness conflicts among all models
   %  2 - enable existence verification for all simulated events
   %  Usage:
   %    AggregateReferencedModels(this,[searchModel])
   %  (Do not specify the searchModel - this is used for recursion)
end
function DebugAccessPathToBlockMap
   %  Debug-only debug access to the feature path block map: deprecated
   %  This access is not expected to stay - just a temporary feature
   %  since this class is pretty new
end
function CatalogList
   %  CatalogList returns current feature path registrations
   %  Usage:
   %    [theCatalog,structuredResults] = CatalogList(this)
end
function Exists
   %  Exists checks if the provided feature name registration exists
   %  Usage:
   %    [ok,theBlock] = Exists(this,featurePath)
end
function Register
   %  Register associates a feature path with a block
   %  Usage:
   %    [ok, errMsg] = Register(this,blockHandle,featurePath)
end
function FeaturePathManager
   %  FeaturePathManager Manages unique interface names for Talaria blocks
   %  providing external integration paths such as control and calibration.
   %  This class replaces the registration features of FeaturePathUtil,
   %  enabling better encapsulation of shared methods and separation of
   %  non-shared methods.
   %  Derived classes must accept modelName as a constructor argument, but do
   %  not need to forward it as this class will set it up. Expected access to
   %  model-level singletons provided by this class via (see RtcManager.Get):
   %  this = FeaturePathManager.GetModelSingleton(blockHandle,mfilename('class'))
   %
   %    Documentation for FeaturePathManager/FeaturePathManager
   %       doc FeaturePathManager
   %
end
end
end

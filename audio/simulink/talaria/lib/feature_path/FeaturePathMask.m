classdef FeaturePathMask
%  FeaturePathMask block mask
%
%    Documentation for FeaturePathMask
%       doc FeaturePathMask
%
%
methods
function Transform1122
   %  Transform function automatically created by RenameLibraryMaskParameters
end
function Transform108
   %  Set value of the new DeclarePackages parameter
end
function FixItFailedGroup
   %  FixItFailedGroup turns off the request to declare an overlay
   %  group - referenced by the automatic "FIX IT" link in error
   %  messages produced by this mask. This routine attempts to save
   %  time by modifying the underlying Simulink Library involved when
   %  the FeaturePath block in question is contained as part of a
   %  resolved library link. This convenience also avoids accidentally
   %  updating a libary with the specific link-instance settings that
   %  model link might overwise pass back into its library via the
   %  "resolve link" mechanism.
end
function CB_TriggerInit
   %  If the location has changed, force the block to run its init by
   %  changing a parameter. This in turn causes the icon to refresh.
end
function CB_PromoteMe
   %  Promote the feature-path name to the parent mask
end
function CB_FindFreeNumber
   %  Find a free number by validating "-1" which will perform a search
end
function GetActiveIsModule
   %  returns whether block actively defines a module
end
function GetActiveIsGroup
   %  returns whether block actively defines an Overlay Group
end
function FeaturePathMask
   %  FeaturePathMask block mask
   %
   %    Documentation for FeaturePathMask/FeaturePathMask
   %       doc FeaturePathMask
   %
end
end
end

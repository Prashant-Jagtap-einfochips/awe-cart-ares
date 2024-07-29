classdef FeatureVersionMask
%  FeatureVersionMask contains logic for the operation of the FeatureVersion
%  library block.
%
%    Documentation for FeatureVersionMask
%       doc FeatureVersionMask
%
%
methods
function Transform52
   %  Transform1109 library transform to upgrade old links to this
   %  block by turning on legacy interfaces which by default are now
   %  normally off.
end
function CB_TriggerInit
   %  If the location has changed, force the block to run its init by
   %  changing a parameter. This in turn causes the icon to refresh.
end
function ValidateNumber
   %  Validate that the provided value is a non-empty numeric value
end
function FeatureVersionMask
   %  FeatureVersionMask contains logic for the operation of the FeatureVersion
   %  library block.
   %
   %    Documentation for FeatureVersionMask/FeatureVersionMask
   %       doc FeatureVersionMask
   %
end
end
end

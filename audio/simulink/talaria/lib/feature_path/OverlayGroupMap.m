classdef OverlayGroupMap
% OverlayGroupMap Tracks claimed number usage by overlay group or globally
%    Usage:
%        OverlayGroupMap(paramName,description,minValue,maxValue,isGlobal)
%    paramName : name of block parameter holding the claimed number
%    description: text description of the number (for error messages)
%    minValue: minimum value for the number
%    maxValue: maximum value for the number
%    isGlobal: true if the number is assigned globally for the whole
%        model, otherwise false keeps track of numbers based on the
%        block handle (H) provided to methods.
%
%    Documentation for OverlayGroupMap
%       doc OverlayGroupMap
%
%
methods
function ClaimNumber
   %  ClaimNumber attempts to claim the specified 
   %  number. Returns true if the claim has been made.
end
function ValidateNumber
   %  ValidateNumber ensures the provided number is valid and claimed.
   %  groupUrlsCell is a cell of block URLs that are all considered
   %  equivalent to block H. If any of them "owns" a number, they
   %  all do.
end
function GetKey
   %OverlayGroupMap/GetKey is a function.
   %    blockKey = GetKey(this, H)
end
function OverlayGroupMap
   %  Setup base class with our map name and *NO* sharing among
   %  partitioned models via ModelRootName
   %
   %    Documentation for OverlayGroupMap/OverlayGroupMap
   %       doc OverlayGroupMap
   %
end
end
end

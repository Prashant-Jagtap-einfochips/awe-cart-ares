function RenameLibraryMaskParameters(blockPath)
% RenameLibraryMaskParameters accelerates the effort involved in changing
% the name of a mask parameter for a library block that has already been
% released
% Usage:
%    RenameLibraryMaskParameters(pathOfBlock);
% TODO RIGHT NOW ALL SUB-BLOCK PARAMETERS GET SEARCH/REPLACE - BUT SHOULD
% LIMIT TO ONLY THOSE THAT ARE EVALUATED, EXCEPT BUILT-IN BLOCKS HAVE
% UNKNOWN MECHANISM TO DETERMINE WHETHER DIALOG PARAM IS EVALUATED

% Copyright 2020 Bose Corporation

% redirect to new static classdef version
AutomateMask.RenameParameters(blockPath);

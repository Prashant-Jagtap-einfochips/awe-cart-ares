function ReplaceGeneratedCodeFile
%  ReplaceGeneratedCodeFile copies provided replacement file over any found
%  matching files in generated code for models
%  Usage:
%    ReplaceGeneratedCodeFile(replacementFilePath,newFileName,modelCell,isModelReference)
%  Where:
%   replacementFilePath : full path to file used as copy source
%   newFileName : pre-generated filename to search for and replace
%   modelCell : cell-array of already-generated model names with code
%   isModelReference : boolean flag indicating whether the cell array is for
%    referenced-models instead of "normal" top-level models (impacts where
%    code will be found).

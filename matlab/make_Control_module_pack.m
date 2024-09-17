function [MM, NAME] = make_Control_module_pack(GENDOC)
%
% make_Control_module_pack(GENDOC)
% Generates the Control_module library.
% this includes sources, header files, schema, and overall module
% list. Arguments:
% GENDOC - Boolean that specifies whether documentation (in Word)
% format should be generated. By default, GENDOC=0.
%
% [MM, NAME]=make_Control_module_pack(GENDOC)
% Optional output arguments return the cell array of modules that was
% used to build the library and the name of the library. When an
% output argument is requested, then the function DOES NOT GENERATE
% code.
%
% Copyright (c) 2019 DSP Concepts, Inc. All Rights Reserved.
if (nargin < 1)
 GENDOC = 0;
end

% ----------------------------------------------------------------------
% Instantiate each module using default arguments
% ----------------------------------------------------------------------
NAME = 'Control';
MM = cell(0,0);
MM{end+1} = Control_module('temp');
if (nargout > 0)
 return;
end

% ----------------------------------------------------------------------
% Set the code generation directory relative to the directory
% in which this m-file is stored.
% ----------------------------------------------------------------------
MFILE = mfilename('fullpath');
[pathstr, ~] = fileparts(MFILE);

% Remove the last directory level
ind = find(pathstr == filesep);
ind = max(ind);
SRC_DIR = pathstr(1:ind-1);
INC_DIR = fullfile(SRC_DIR, 'Include');
DOC_DIR = fullfile(SRC_DIR, 'Doc');
INNER_DIR = fullfile(SRC_DIR, 'Inner');
DIR = {SRC_DIR; INC_DIR; DOC_DIR; INNER_DIR};

% ----------------------------------------------------------------------
% Generate the library.
% ----------------------------------------------------------------------
global AWE_INFO;
AWE_INFO.buildControl.combineSourceFiles = 0;
AWE_INFO.buildControl.ignoreClassIDs = 0;
awe_generate_library(MM, DIR, NAME, '', GENDOC);
awe_generate_doc(MM, DOC_DIR, fullfile(DOC_DIR, '_Control_module.html'), 'Control_module');

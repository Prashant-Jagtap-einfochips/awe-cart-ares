function build_bdsp_system(sys, name, bdsp_work_folder)
%Runs code generation of BDSP system sys using xfile xmodel

%   Copyright 2018-2019 Bose Corporation
if ~isa(sys, 'bdspsystem')
    error('First argument must be BDSP system object');
end

% Create xfile xmodel for mex'ing
xfile = bdspxfile(copyWithShare(sys));

%  Run BDSP code generation
bdsp_code_gen_folder = [bdsp_work_folder '\' name];
xmodelName = 'xmodelMaster';
cFilename = [bdsp_code_gen_folder '/xmodelgendef.c'];
genall(xfile, [], xmodelName, cFilename, [], bdsp_code_gen_folder, [], ...
    bdsp_code_gen_folder);

% Create xmodelVA xmodel for code generation
xm = bdspva(copyWithShare(sys));
bdsp_code_gen_folder = [bdsp_work_folder '\' name '_visual_DSP'];
cFilename = [bdsp_code_gen_folder '/xmodelgendef.c'];
genall(xm, [], xmodelName, cFilename, [], bdsp_code_gen_folder, [], ...
    bdsp_code_gen_folder);
end


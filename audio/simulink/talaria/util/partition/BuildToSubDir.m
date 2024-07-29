function BuildToSubDir
% BUILDTOSUBDIR Builds DLL model in subdirectory of normal codegen folder
%    BuildToSubDir temporarily changes the code generation folder to a
%    subdirectory of the current code generation folder during a build of
%    the indicated model. Copies the resulting *.dll file up to the normal
%    build folder after build completes.
% 
%    Usage:
%       BuildToSubDir(modelName,subDir);

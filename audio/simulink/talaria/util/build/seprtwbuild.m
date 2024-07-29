% 
% This function will invoke a Real-Time Workshop build process for a model
% in a separate Matlab session.  This is advantageous for optimizing
% license utilization of concurrent Mathworks tools.  By using this build
% method, the required licenses to complete a build are returned immediately
% to the license server following the completion of the code-generation and
% compilation. 
% 
% SEPRTWBUILD with no input arguments will build the currently open
%   model.
% 
% SEPRTWBUILD('MODELNAME') will build the model specified by the input
%   string 'MODELNAME'
% 
% SEPRTWBUILD('-wait') or SEPRTWBUILD('MODELNAME','-wait') will cause the
%   calling Matlab session to wait for the completion of the build in the
%   second session prior to returning to a Ready state.
% 
% 
% 20100920: Tony Sangermano: Initial release
% 20100924: Tony Sangermano: Enhanced to only find "shown" models to build
% 20101001: Tony Sangermano: Added handling of input model extensions
% 20100105: Tony Sangermano: Added the ability to pass additional arguments
%                            to the invocation of matlab.exe, i.e. -wait
% 20120920: Tony Sangermano: Bug fix for when spaces are in the paths or
%                            arguments
% 

%   Copyright 2018-2019 Bose Corporation

function seprtwbuild(varargin)

% handel input arguments
modelname = '';
optexeargs = '';
if nargin==0
    %nop
elseif nargin==1
    tmpvar = varargin{1};
    if isequal(tmpvar(1),'-')
        optexeargs = tmpvar;
    else
        modelname = tmpvar;
    end
else
    modelname = varargin{1};
    optexeargs = varargin{2};
end

if isequal(modelname,'')
    openmodels = find_system('type','block_diagram','BlockDiagramType','model','Shown','on');
    if numel(openmodels)==1
        modelname = openmodels{1};
    elseif numel(openmodels)==0
        error(sprintf(['No Open Models Detected!\n\n',...
               'The target model must be open,\n',...
               'or the model name must be passed as a parameter.\n']));
    else
        error(sprintf(['Multiple Open Models Detected!\n\n',...
               'Only the target model can be open,\n',...
               'or the model name must be passed as a parameter.\n']));
    end
else
    % strip the file extension if one exists
    [PATHSTR,modelname,EXT] = fileparts(modelname);
    clear PATHSTR EXT
end

% path to the matlab application
fullappname = fullfile(matlabroot,'bin','matlab.exe');
if ~exist(fullappname,'file')
    error(sprintf(['Matlab executable not found!\n',...
                   'Expected path: %s'],fullappname));
end

% setup calling arguments to matlab.exe
args = sprintf('-nosplash -nodesktop -nojvm -logfile "%s" %s',fullfile(pwd,'rumblebuildlog.txt'),optexeargs);

% setup commands to be executed
commands = sprintf(['cd(''%s'');',...
                    'pause(0.1);',...
                    'rtwbuild(''%s'');',...
                    'pause(0.1);',...
                    'exit;'],pwd,modelname);

% assemble full call to matlab
dosstr = sprintf('"%s" %s -r "%s"',fullappname,args,commands);

%disp(dosstr) % For Debug, print the dos cmd

% save the path of the current matlab session to be propagated to the new
% session
savepath;

% invoke the build
fprintf('### Invoking RTW build in separate Matlab window for model: %s\n',modelname)
[status, result] = dos(dosstr);

return

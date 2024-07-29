function profile_model(model_name, config_params, display_option, profile_run_count)
%PROFILE_MODEL Computes MIPS/memory of a model
%   profile_model(model_name, config_params, display_option)
%   runs code generation on a model, runs it through a cycle accurate
%   simulator to compute MIPS, and computes memory usage.
%
%   Parameters:
%   model_name : single string or cell array of strings of model names
%   config_params  (optional): single string or cell array of strings of 
%                              model configuration parameters 
%   display_option (optional): 0    - no table display (default)
%                              else - with table display
%   profile_run_count (optional): 1  - runs profiling once for base rate(default)
%                              else - runs profiling the number of times entered for the base rate
%   
%   Sample usage:
%       profile_model('test_model');
%       profile_model('test_model', 'sharcPlusConfiguration');
%       profile_model('test_model', 'sharcPlusConfiguration', 0 , 1);
%       profile_model('test_model', [], 1);
%       profile_model('test_model', [], 1, 4);
%
%   Copyright 2018-2019 Bose Corporation
% Below are the list of toolchains that support benchmarking, and the
% profiling scripts for each. Note the the order of the toolchains and
% profilers must match, meaning the first profiler should profile the first
% listed toolchain

% Discover which profiling hookups are present by searching in the arch
% folder for any files named "register_profile_*.m". By acquiring this list
% everytime this function is called, new registrations instantly work
% without confusion about Simulink project startups. The search is fast.
supported_toolchains = {};
supported_profilers = {};
% Use relative pathnames from the location of THIS script to find the arch
% folder. Start by grabbing the full path and scraping off the last 3 parts
thisFilePath = mfilename("fullpath");
thisPathParts = split(thisFilePath,filesep);
moduleFolder = thisPathParts(1:end-3);
% Add in the directory components that come after the module folder
moduleFolder{end+1} = 'arch'; % arch folder of module
moduleFolder{end+1} = '**'; % search to any folder depth
moduleFolder{end+1} = 'register_profile_*.m'; % file-matching pattern
dirExpression = fullfile(moduleFolder{:});
registerFunctions = dir(dirExpression);
% Call each found file (as a function) to aggregate profiler registrations
for i=1:numel(registerFunctions)
    % If the function is not on the path, warn but don't call it. We could
    % call it by "cd" to the path and running it there, but likely it's
    % calling other functions that would also not be on the path.
    anEntry = registerFunctions(i);
    funcPath = fullfile(anEntry.folder,anEntry.name);
    fileOnPath = which(funcPath);
    if (isempty(fileOnPath))
        warning('Ignoring profiling registration not on path: [%s]',funcPath);
    else
        [~,funcName] = fileparts(anEntry.name);
        theFunc = str2func(funcName);
        % Rely on the MATLAB path to find the function:
        [chainName,funcHandle] = theFunc();
        supported_toolchains{end+1} = chainName; %#ok<AGROW> 
        supported_profilers{end+1} = funcHandle; %#ok<AGROW> 
    end
end

% Input can be individual model or cell array of strings. If it is a 
% single model, turn it in to a cell array
if isa(model_name, 'char')
    model_name = {model_name};
end

for i = 1:length(model_name)
    if ~exist(model_name{i}, 'file')
        error(['Unknown model ' model_name{i}]);
    end
end

% Check if config_params is set, and if so, make sure it is set for 
% each model
if exist('config_params', 'var') && ~isempty(config_params)
    % Make sure toolchain is cell array
    if isa(config_params, 'char')
        config_params = {config_params};
    end

    % Make sure there are equal numbers of config_params and models
    if (length(config_params) ~= length(model_name))
        error('Number of config_params does not equal number of models');
    end
else
    config_params = repmat({''},length(model_name),1); 
end

% Check if display_option is set, and if not, set to default value
if ~exist('display_option', 'var')
    display_option = 0;
end

% Check if profile_run_count is set, and if not, set to default value
if (~exist('profile_run_count', 'var') || (profile_run_count == 0))
    profile_run_count = 1;
end

% Run code generation for each model. Find out which toolchain is being
% used to be used later in the profiling step
profilers = cell(1,length(model_name));
partitioned_model = cell(1,length(model_name));
for i = 1:length(model_name)
    config = partitionModel(model_name{i});
    newModelNames = config.newModelPaths;
    [~,partitioned_model{i},~] = fileparts(newModelNames{1});
    aModelHandle = load_system(newModelNames{1});
    if ~isempty(config_params{i})
        Simulink.BlockDiagram.loadActiveConfigSet(aModelHandle, config_params{i});
    end
    
    % Keep track of which toolchain is used so the correct profiling script
    % can be found later on
    tc = get_param(aModelHandle, 'Toolchain');
    for j = 1:length(supported_toolchains)
        if strcmp(tc, supported_toolchains{j})
            profilers{i} = supported_profilers{j};
            break
        end
    end
    if isempty(profilers{i})
        error(['Could not determine which profiling script to use for toolchain' tc]);
    end
    
    % Execute the build and close without saving
    disp([' Building ' model_name{i}]);
    aModelHandle = GenPartsModelCode(aModelHandle);
    disp(' ');
    close_system(aModelHandle, 0);
end

% Run as usual without extracting profiler info
if (display_option == 0)
    % Call the profiler
    for i = 1:length(model_name)
        profiler = profilers{i};
        disp([' Profiling ' model_name{i}]);
        profiler(partitioned_model{i}, profile_run_count);
        disp(' ');
    end
    
% Run and display profiling stats in table (cces only)
else
    results = {};
    % Call the profiler
    for i = 1:length(model_name)
        profiler = profilers{i};
        disp([' Profiling ' model_name{i}]);
        [mips,num_kB] = profiler(partitioned_model{i}, profile_run_count);
        % Convert memory from kB to Mbit
        num_Mbit = (num_kB * 8) / (2^10);
        currentStats = {model_name{i}, mips, num_Mbit};
        results = [results, {currentStats}];
        disp(' '); %disp(' ');    
    end
    display_in_table(results);
end
end

function display_in_table(results)
    fprintf('   MIPS      Mem(Mbits)     Block     \n');
    fprintf('----------  -----------  -----------  \n');
    for i = 1:length(results)
        current = results{i};
        fprintf('%9.5f    %9.6f   %s\n', ...
                current{2}, current{3}, current{1});
    end
end
    


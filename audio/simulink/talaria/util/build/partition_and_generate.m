function partition_and_generate(modelName, buildOptions)
% partition_and_generate Creates partitioned models and performs code
% generation (slbuild) on them.
% partition_and_generate(MODEL, OPTS) creates partitioned models based on
% MODEL. The partitioning process can be configured by passing in the
% string OPTS with the following tags: (Not Case-Sensitive)
%   TAG   :  BEHAVIOR
%  -t32   : Skip generating the 32-bit translation engine
%  -t64   : Skip generating the 64-bit translation engine
%  -i32   : Skip generating the 32-bit interpretation engine
%  -i64   : Skip generating the 64-bit interpretation engine
%  64     : Generate 64-bit translation and interpretation engine
%  32     : Generate 32-bit translation and interpretation engine
%  -engines or -e: Skip generating all engine models and code
%  -ball  : Skip generating code for partitioned model
%  +parallel or +p: run tasks in parallel on all available cores (also +p works)
%  +cleanBuild or +c: ignores checksums and builds all parts of the script (just 
%           like the very first build) also +c works
%  CrossChipFrameDelay or ChipDelay: Frame delay for a cross chip transfer
%  CrossCoreFrameDelay or CoreDelay: Frame delay for a cross core transfer
%
% Instead of a string, the OPTS argument may contain an instance of the
% PartitionGenerateOptions class which allows for additional options.
% e.x. partition_and_generate('myModel', '-t64 -i64')
% e.x. partition_and_generate('myModel', '-t32 -i32 +parallel')
% e.x. partition_and_generate('myModel',PartitionGenerateOptions('64 +c +p CrossChipFrameDelay 2 CoreDelay 1'))

%   Copyright 2018-2019 Bose Corporation
if nargin < 2
    buildOptions = '';
end
if (isa(buildOptions,'PartitionGenerateOptions'))
    % Detected that the full options object is passed in
else
    % Assume options string - use to create an options object
    buildOptions = PartitionGenerateOptions(buildOptions);
end

% Validate MATLAB version due to MathWorks bug #2123286
% https://www.mathworks.com/support/bugreports/2123286
% (Raised by MathWorks support case 04212175)
if verLessThan('MATLAB','9.7.0.1410870 (R2019b) Update 6')
    error('Please upgrade your MATLAB installation to 2019b Update 6 or later');
end

fprintf('----- Build Options -----\n');
buildOptions.Print;
fprintf('-------------------------\n');
fprintf('Talaria Version: %s\n',talariaVersion);
fprintf('-------------------------\n');

% Determine part and ensure existence of parts folder
partsFolderPath = peerCodeFolder('parts');
if (false == exist(partsFolderPath,'dir'))
    mkdir(partsFolderPath);
end

% Save the base workspace of the primary MATLAB client session so
% that the worker sessions can work using the same variables. This is only
% needed for parallel processing, but for debugging in all cases it might
% be useful so just save it always even for non-parallel runs.
% Particularly important for BTC variables, see TALARIA-773
BaseWorkspaceSave(modelName,partsFolderPath,'Generate');

if buildOptions.Parallel
    availableToolboxes  = ver;
    existParallelToolBox =  contains('Parallel Computing Toolbox',...
        {availableToolboxes.Name});

    if existParallelToolBox
        fprintf('Using parallel processing with all available processors\n');
        if ~isempty(gcp('nocreate'))
            fprintf('Parallel pool already running\n');
            fprintf('Shutting down exisiting Parallel pool\n');
            delete(gcp);
        end
        fprintf('Starting parallel pool\n');
        myCluster = parcluster;
        poolobj = parpool(myCluster);
    else
        fprintf('Parallel Computing Toolbox is NOT INSTALLED\n');
        fprintf('Not using parallel processing\n');
        buildOptions.Parallel = false; % override selected option
    end
else
    fprintf('Not using parallel processing\n');
end

fprintf('Performing partition separation on [%s]\n',modelName);

% Reset the code build directory to project-specified value since the
% engine builds below could temporary change it. Set it back just in case
% something interrupted processing while still in the temporary folders.
currProj = currentProject;
% Reset codegen folder to normal state
Simulink.fileGenControl('set','CodeGenFolder',currProj.SimulinkCodeGenFolder);

% create the partitioned core models
config = partitionModel(modelName,buildOptions);

newModelPaths = config.newModelPaths;

if buildOptions.GetCoreTagModelAction('default') < ModelAction.BothModelAndCode
    fprintf('Skipping embedded core code generation\n');
else
    % run the build (i.e. generate code) on each created model
    numModels = numel(newModelPaths);
    codeFolder = Simulink.fileGenControl('get','CodeGenFolder');
    fprintf('Generating code with root folder %s\n',codeFolder);

    if buildOptions.Parallel
        parfor i = 1:numModels
            BaseWorkspaceLoad(modelName,partsFolderPath,'Generate');
            buildModelInPrivateFolder(codeFolder,newModelPaths{i});
        end
    else
        for i = 1:numModels
            buildModelInPrivateFolder(codeFolder,newModelPaths{i});
        end
    end

    % Reconstitute the FeaturePath module listing xml file path for config
    config.modulesFileName = fullfile(codeFolder,[modelName '_ModuleFeaturePath.xml']);

    % Save config to assist with debug sessions on the aggregations below
    save(fullfile(partsFolderPath,[modelName '_config.mat']),'config');

    % TOP token xml files are ready from the target core builds. Pull
    % these together and calculate the TOP COMPAT hashes.
    strTopCompatDefine = aggregateTopTokens(config);

    % Do the same thing for TSPs
    strTspCompatDefine = aggregateTspTokens(config);

    % And now for the RTC payload XML
    aggregateRtcTokens(config);

    % Combine the SCH tokens
    aggregateSchTokens(config)

    % Combine RTC ID headers
    aggregateRtcHeaders(config);

    % Combine TOP ID headers
    aggregateTopHeaders(config);

    % Combine TSP ID headers
    aggregateTspHeaders(config);

    % Combine SCH ID headers
    aggregateSchHeaders(config);

    % Combine AudioStats header files
    aggregateAudioStatsHeaders(config);

    % Combine SyncApi XML
    aggregateSyncApi(config);
    
    % Surface the PingPongStruct XML
    aggregatePingPongBuffers(config);

    % And finally, copy in some header files to the Control folder
    SetupControlFolder(config.tagModelPath,config.newModelPaths, config.systemTargetFile);

    fprintf('Creating engine DLLs...\n');

    engineTaskStruct = struct( ...
        'func',{@createTranslationEngine,@createInterpretationEngine,@createTranslationEngine,@createInterpretationEngine}, ...
        'bit', {32, 32, 64, 64}, ...
        'subDir',{translation_engine.SubDir32,interpretation_engine.SubDir32,translation_engine.SubDir64,interpretation_engine.SubDir64}, ...
        'strCompatDefine',{strTopCompatDefine,strTspCompatDefine,strTopCompatDefine,strTspCompatDefine}, ...
        'modelAction', {buildOptions.TranslateEngine32Bit,buildOptions.InterpretEngine32Bit,buildOptions.TranslateEngine64Bit,buildOptions.InterpretEngine64Bit}, ...
        'type',{'translation' 'interpretation' 'translation' 'interpretation'});

    codeGenFolder = currProj.SimulinkCodeGenFolder;
    cacheFolder = currProj.SimulinkCacheFolder;
    engineModels = cell(1,numel(engineTaskStruct));
    if buildOptions.Parallel
        parfor i = 1:numel(engineTaskStruct)
            BaseWorkspaceLoad(modelName,partsFolderPath,'Generate');
            engineModels{i} = createAndBuildEngine(codeGenFolder,cacheFolder,engineTaskStruct(i),config);
        end
    else
        for i = 1:numel(engineTaskStruct)
            engineModels{i} = createAndBuildEngine(codeGenFolder,cacheFolder,engineTaskStruct(i),config);
        end
    end

    fprintf(' Done generating code!\n');

    fprintf(' Writing checksums to file\n');
    modelChecksumMapFileName = fullfile(codeGenFolder,[modelName '_modelChecksumMap.json']);
    writeMapToJSON(config.modelChecksumMap, modelChecksumMapFileName);

    fileChecksumMapFileName = fullfile(codeGenFolder,[modelName '_fileChecksumMap.json']);
    newFileChecksumMap = createFileChecksums(config,engineModels);
    writeMapToJSON(newFileChecksumMap, fileChecksumMapFileName);
    fprintf(' Done writing checksums!\n');
end

% Stop parallel processing once completed all tasks
if buildOptions.Parallel
    delete(poolobj);
end

fprintf('** Partitioning Complete **\n');

end

%--------------------------------------------------------------------------
function buildModelInPrivateFolder(codeFolder,partitionedModelName)
    % need to set the code and cache folders again since parallel pool
    % seems to change it back to the project folder
    % created cache folder for each model to avoid parallel processes to wait for each other
    [~,name,~] = fileparts(partitionedModelName);
    cacheFolderName = sprintf('%s_slprj', name);
    cacheFolder = fullfile(codeFolder,cacheFolderName);
    fprintf(' Building %s ...\n',partitionedModelName);

    % added to explicitly generate code and cache in the project code gen and cache folder
    Simulink.fileGenControl('set','CodeGenFolder', codeFolder,...
    'CacheFolder', cacheFolder,'createDir', true)

    % Load the model (but don't show it). Make sure to load model
    % references also because if the parts folder is not on the MATLAB
    % path they will not be found. Ironically, we have the full path to the
    % top-level model, but we need modelname only to load from parts.
    % Load and generate with code-mapping disappearing DSM workarounds:
    aModelHandle = GenPartsModelCode(name);

    % ... and close it without saving
    close_system(aModelHandle,0);
end

%--------------------------------------------------------------------------
function model = createAndBuildEngine(codeGenFolder,cacheFolder,engineTaskStruct,config)
    if (engineTaskStruct.modelAction == ModelAction.Nothing)
        fprintf(' Skip %d bit %s engine creation - disabled in options\n',engineTaskStruct.bit,engineTaskStruct.type);
        model = [];
        return;
    end
    % explicitly reset the code and cache folder to the project code and cache folder
    Simulink.fileGenControl('set','CodeGenFolder',codeGenFolder,'CacheFolder',cacheFolder);
    model = engineTaskStruct.func(config.tagModelPath,engineTaskStruct.strCompatDefine,...
        config,engineTaskStruct.bit);
    
    if (engineTaskStruct.modelAction == ModelAction.BothModelAndCode)
        fprintf(' Building %s ...\n',model);
        BuildToSubDir(model,engineTaskStruct.subDir)
    else
        fprintf('Skipping code generation for %d bit %s engine\n',engineTaskStruct.bit,engineTaskStruct.type);
    end
end

%--------------------------------------------------------------------------
function writeMapToJSON(mapObject, fileName)
    checksumMap = jsonencode(mapObject);
    fid=fopen(fileName,'w');
    fprintf(fid, checksumMap);
    fclose(fid);
end

%MIPS2EXCEL Create an excel file with MIPS and Memory for a given model
%   [array_MIPS] = MIPS2EXCEL(model_name, file_name, config_param, MIPSStruct)
%   returns an array of MIPS and memory values as well as prints as an xlsx
%   file for a single talaria block model.
%   Runs code generation on a model, runs it through a cycle accurate
%   simulator to compute MIPS and computes memory usage to be exported as
%   an array and as an xlsx file.
%
%   Parameters:
%   model_name : single string of model name
%   file_name  : single string of a preferred file name and directory for the
%   exported excel file
%   config_param  : single string of model configuration parameters
%   MIPSstruct  :  a 1x1 struct with 4 fields
%           1. MIPSstruct.sample_rate_in_hertz : the array of possible
%           sample rates to test on the feature block
%           2. MIPSstruct.frame_size  : a single integer of the amount of
%           samples in the fram
%           3. MIPSstruct.channelList  : an array of the possible number of
%           input channels to test on the feature block
%           4. MIPSstruct.channelArr  : an array of the ramp map for the
%           feature block mask if it is necessary to map each input
%
%   Output:
%   array_MIPS : A Nx2 double array of the MIPS in one column and the Memory
%   in Mbits given in the other
%   XLSX File : An Excel compatible file of array_MIPS with added Variable
%   Names saved at the given directory requested in the file_name input.
%
%   N = (# of sample rates) x (# of channel options that are input
%   into the function)
%
%   Examples:
%   MIPS2Excel('MuteMIPS', 'C:\Users\sl1047176\OneDrive - Bose ...
%   Corporation\Documents\Assignments\table.xlsx', 'sharcPlusConfiguration')
%   MIPS2Excel('VolumeMIPS', 'C:\', 'sharcPlusConfiguration')
%
%   When creating a 'Talaria Feature Block' MIPS file, a simulink file with
%   the feature block you want to profile, the necessary number of in port
%   blocks, out port blocks and a feature path with 'declare overlay' checked.
%
%   The in port block signal properties need to be changed accordingly:
%   Input Type: single
%   Port Dimensions: [frame_size numChannel]
%   Sample Time: 1/sampleRate*frame_size
%
%   The feature block mask needs to be capable of handling the changing
%   number of channel inputs and sample rates.
%
%   If copying from a simulink test file, delete any callbacks but the init
%   callback under properties manager. The init function may require the
%   sample rate value to be set to 'sampleRate'.
%
%   An accompanying script file needs to be run in order for both the
%   base workspace and function workspace to assign variables correctly.
%   This script should initialize the variables used in the inputs of the
%   feature blocks and input port block.
%
%
%   See also PROFILE_MODEL, RTWBUILD.
%   Copyright 2021 Bose Corporation

function [arrayMIPS] = MIPS2Excel(model_name, file_name, config_param, MIPSstruct)


sample_rate_in_hertz = MIPSstruct.sample_rate_in_hertz;
frame_size = MIPSstruct.frame_size;
channelList = MIPSstruct.channelList;
channelArr = MIPSstruct.channelArr;

nFs = length(sample_rate_in_hertz);
nChannel = length(channelList);

% Initialize Output Array
arrayMIPS = zeros(nChannel*nFs,2);
% Initialize Test # tracking
testCounter = 1;

%Show that the tests are about to begin
fprintf('\nRunning MIPS2Excel: %d MIPS and Memory Profiling Tests for %s...\n', nChannel*nFs, model_name)

% For each given sample rate and channel vector
for idx = 1:length(sample_rate_in_hertz) % Change
    for fr = 1:length(frame_size)
        for k = 1:length(channelList) %Change
            
            %    Variables used in Simulink model
            % Used in Sample Size under In port block
            
            sampleRate = sample_rate_in_hertz(idx);
            assignin('base', 'sampleRate',  sample_rate_in_hertz(idx))
            % Used in Port Dimensions under In port block
            numChannel = channelList(k);
            assignin('base', 'numChannel',  channelList(k))
            
            % Used in Mask of testing block to correspond to input if needed
            channelMap = channelArr{k};
            assignin('base', 'channelMap',  channelArr{k})
            
            this_frame_size = frame_size(fr);
            assignin('base', 'frame_size', frame_size(fr))
            
            % Analyze MIPS and Memory of the testing block
            
            [~,mips, num_Mbit] = evalc('partitionConfigProfile(model_name, config_param)');
            
            
            % Tell the user testing information during each iteration
            fprintf('\nTest %d of %d: \n# of Channels: %d \nFs: %d \nFrame Size: %d \n', ... 
                testCounter, nFs*nChannel, numChannel, sampleRate, this_frame_size)
            fprintf('\nMIPS: %d \nMem (Mbits): %d\n', mips, num_Mbit)
            
            % Confirm if test was successful and needs to be redone
            %         troubleShoot = input('\nRedo Current Test? \nY/N? ', 's');
            troubleShoot = 'n';
            if strncmpi(troubleShoot, 'y', 1)
                % Rerun the given test iteration if N
                %             profile_model(model, 'sharcPlusConfiguration', 'b')
                [mips1, num_Mbit1] = partitionConfigProfile(model_name, config_param);
                %             x1 = input('\nUse Previous? ', 's');
                
                if strncmpi(x1, 'y', 1)
                    arrayMIPS(testCounter,:) = [mips, num_Mbit];
                    testCounter = testCounter + 1;
                else
                    arrayMIPS(testCounter,:) = [mips1, num_Mbit1];
                    testCounter = testCounter + 1;
                end
            else
                arrayMIPS(testCounter,:) = [mips, num_Mbit];
                testCounter = testCounter + 1;
            end
        end
    end
end


% Create .xlsx file from simulation values
varNames = {'MIPS', 'Mem (Mbits)'};
tableMIPS = array2table(arrayMIPS,'VariableNames', varNames);

% Check if file already exists
file_error = (exist(file_name,'file'));
if (file_error ~= 0)
    overwrite = input('Would you like to overwrite this file? (y or n) ', 's');
    if (any(strcmpi(overwrite, {'n', 'no'})))
        [path, file, ext] = fileparts(file_name);
        file_rename = [path '/' file '_' date ext];
        writetable(tableMIPS, file_rename);
        fprintf('\nExcel File Successfully Created as: %s\n', file_rename)
    else
        writetable(tableMIPS, file_name);
        fprintf('\nExcel File Successfully Created as: %s\n', file_name)
        
    end
else
    % Output the file and text
    writetable(tableMIPS, file_name);
    fprintf('\nExcel File Successfully Created as: %s\n', file_name)
end




    function [mips, num_Mbit] = partitionConfigProfile(model_name, config_param)
        
        % List toolchains and corresponding profile methods
        supported_toolchains = {
            'ADSP-SC58x v2.3 | gmake makefile (64-bit Windows)', ...
            'VisualDSP 5.0 | gmake makefile (64-bit Windows)'
            };
        
        supported_profilers = {
            @profile_model_in_cces, ...
            @profile_model_in_vdsp
            };
        
        
        % Partition Model and Find Filenname
        [newModelName,~] = partitionModel(model_name);
        [~,partitioned_model,~] = fileparts(newModelName{1});
        
        h = load_system(newModelName{1});
        % config_param = 'sharcPlusConfiguration';
        Simulink.BlockDiagram.loadActiveConfigSet(h, config_param);
        
        % Map the appropriate toolchain/profiler to the model
        tc = get_param(h, 'Toolchain');
        for j = 1:length(supported_toolchains)
            if strcmp(tc, supported_toolchains{j})
                profiler = supported_profilers{j};
                break
            end
        end
        rtwbuild(h);
        
        [mips, num_kB] = profiler(partitioned_model, 1);
        num_Mbit = (num_kB * 8) / (2^10);
    end
end
function [allOk] = VerifyModelConfig(modelName, changeIt)
%VERIFYMODELCONFIG Checks critical model configuration set parameters
%   The Talaria blockset requires certain model settings in order to
%   generate buildable C source code. This function accepts a model name
%   and verifies these settings are in place.
% Usage:
%   VerifyModelConfig(modelname,[boolean fix it]);
% Examples:
%   VerifyModelConfig(bdroot) % just check
%   VerifyModelConfig(bdroot, true) % fix any discrepancies

%   Copyright 2018-2019 Bose Corporation

if (nargin < 2)
    changeIt = false;
end

% Ensure the model is loaded
load_system(modelName);

% Setup what the correct settings should be... and why
Settings = containers.Map();
Reasons = containers.Map();

Settings('Solver') = 'FixedStep';
Reasons('Solver') = 'There are no continuous states in our normal audio processing.';

Settings('FixedStep') = 'auto';
Reasons('FixedStep') = 'We are creating sample-based signal processing. This effort is a fixed-step endeavor automatically determined by sampling frequency and frame size.';

Settings('ERTFilePackagingFormat') = 'Modular';
Reasons('ERTFilePackagingFormat') = 'TranslateTrigger needs to call the translation functions which become hidden (static scope) when this setting is not correct.';

Settings('PassReuseOutputArgsAs') = 'Structure reference';
Reasons('PassReuseOutputArgsAs') = 'RTC handlers sometimes need to access signal values that get implemented a local stack variables inside the step() function when this setting is not correct.';

Settings('MultiTaskDSMMsg') = 'none';
Reasons('MultiTaskDSMMsg') = 'Datastore Memories are often used to share data between low-priority RTC handlers and high-priority signal processing threads. We know we need to be careful- no need to remind us!';

Settings('UniqueDataStoreMsg') = 'none';
Reasons('UniqueDataStoreMsg') = 'Library blocks may use a datastore memory. The name would happen for every instance! DSMs are scoped anyway by the subsystem hierarchy. Make this quiet.';

Settings('ModelReferenceNumInstancesAllowed') = 'Single';
Reasons('ModelReferenceNumInstancesAllowed') = 'Talaria blocks like TOPs and RTCs allow external access to each block via a token identifier. Currently this token generation and lookup scheme does not support mulitple instances.';

aReason = 'Include a Bose Corporation copyright notice and eliminate genereation timestamps which trigger modification detection even with nothing has changed.';
Settings('ERTDataHdrFileTemplate') = 'bose_ert_code_template.cgt';
Reasons('ERTDataHdrFileTemplate') = aReason;
Settings('ERTDataSrcFileTemplate') = 'bose_ert_code_template.cgt';
Reasons('ERTDataSrcFileTemplate') = aReason;
Settings('ERTHdrFileBannerTemplate') = 'bose_ert_code_template.cgt';
Reasons('ERTHdrFileBannerTemplate') = aReason;
Settings('ERTSrcFileBannerTemplate') = 'bose_ert_code_template.cgt';
Reasons('ERTSrcFileBannerTemplate') = aReason;

Settings('EnableMultiTasking') = 'on';
Reasons('EnableMultiTasking') = 'To allow multiple threads/priorities simultaneously on processor. Enables faster rates to finish on time yet still allow slower rate groups to leverage their less-frequent frame setup and schedule.';

Settings('GenCodeOnly') = 'on';
Reasons('GenCodeOnly') = 'To support the built-in encoding of COMPAT compatibility numbers for token sets, the generated code must wait until all cores have finished generating code (and tokens) before being compiled. This enables Talaria to insert COMPAT numbers into the code before the binaries are built. You can still run the Simulink-generated toolchain files if desired (e.g. makefile) but just do it afterward instead of automatically as part of Simulink''s individual model build.';

% Verify each setting
theKeys = Settings.keys;

% This one setting is so important, it must be handled first, so after
% collecting all the keys, add this one and prepend it to the list:
Settings('SystemTargetFile') = 'ert_shrlib.tlc';
Reasons('SystemTargetFile') = 'Our convention is to create libraries which are joined with platform-level code to create executables.';
theKeys = ['SystemTargetFile', theKeys];

for i=1:numel(theKeys)
    aKey = theKeys{i};
    try
        modelVal = get_param(modelName,aKey);
    catch ME
        fprintf(2,[ME.message newline]);
        continue;
    end
    correctVal = Settings(aKey);
    if (isempty(regexp(modelVal, correctVal,'once')))
        fprintf('****: [%s] is [%s] instead of [%s]\n', ...
            aKey, modelVal,correctVal);
        strWrapped=regexprep(Reasons(aKey),'.{1,50}([\s\.!]|$)','$0\n');
        fprintf('   Why %s should be ''%s'': \n%s\n', aKey, correctVal, strWrapped);
        if (changeIt)
            set_param(modelName, aKey, correctVal);
            fprintf('--> changed!\n');
        end
    end
end

end


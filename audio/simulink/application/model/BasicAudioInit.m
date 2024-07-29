ijt = exist('BasicAudioCfg','class');
if (ijt == 8)
    % If it was started as a JTest
    ModelCfg = BasicAudioCfg;
else
    % Setup root input and output ports
    sampleRateHz = 48000;
    frameSamples = 32;
    sysinChannels = 2;

    ModelCfg = struct();
    ModelCfg.In = ModelRootInportConfig;
    ModelCfg.Out = ModelRootOutportConfig;
    ModelCfg.In.BufferName='UnitTest_Target_AudioIn';
    ModelCfg.In.AccessType='ImportedPointer';
    ModelCfg.In.ChannelCount=sysinChannels;
    ModelCfg.In.FrameSamples=frameSamples;
    ModelCfg.In.SampleRateInHertz=sampleRateHz;
    ModelCfg.Out.BufferName='UnitTest_Target_AudioOut';
    ModelCfg.Out.AccessType='ImportedPointer';
end

% Set boundary conditions
rampRateBounds = [5,50000];
rampTimeBounds = [1,100000];

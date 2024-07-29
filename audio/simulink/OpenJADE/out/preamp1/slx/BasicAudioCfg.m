classdef BasicAudioCfg
    properties ( Constant = false )
      In = ModelRootInportConfig;
      Out = ModelRootOutportConfig;
   end
   methods 
      function obj=BasicAudioCfg
         obj.In.BufferName='UnitTest_Target_AudioIn';
         obj.In.AccessType='ImportedPointer';
         obj.In.ChannelCount=2;
         obj.In.FrameSamples=32;
         obj.In.SampleRateInHertz=48000;
         obj.Out.BufferName='UnitTest_Target_AudioOut';
         obj.Out.AccessType='ImportedPointer';
      end
   end
end

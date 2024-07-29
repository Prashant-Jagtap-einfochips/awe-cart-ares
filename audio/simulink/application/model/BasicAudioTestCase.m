classdef BasicAudioTestCase < TalariaTestOJT
%VOLUMETESTCASE Test cases for volume block

    methods(TestMethodSetup)
        function init(self)
           % Set the model name so the test runner knows what model to run
           self.setModelName('BasicAudio');

           % Set the testcase projectPath based on this script's location
           [current_path, ~] = fileparts(mfilename('fullpath'));
           self.projectPath = current_path;
        end
    end % methods(TestMethodSetup)
    methods(Access = protected)
        % ---- internal method--------------
        function testTail(self)
           % Log the output
           self.registerSignalToLog([self.modelName '/Input/Environment Controller'],1,'audioIn');
           self.registerSignalToLog([self.modelName '/BasicAudio'],1,'audioOut');
           self.setStopTime(.1);
           self.simulateModel;
        end
    end
    methods (Test)

        function test_default(self)
           self.registerRTC( 'BasicAudioVolumeSet', 0.04, [3000, 240, 0,  30, 0]);
           self.registerRTC( 'BasicAudioVolumeSet', 0.08, [4000, 255, 0,  9, 0]);
           self.testTail();
        end
    end % methods(Test)

end

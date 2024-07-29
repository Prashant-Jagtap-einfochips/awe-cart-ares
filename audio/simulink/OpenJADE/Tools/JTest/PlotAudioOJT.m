%---------------------------------------------------------------------
%
%  COPYRIGHT BOSE CORPORATION ALL RIGHTS RESERVED
%  This program may not be reproduced, in whole or in part in any
%  form or any means whatsoever without the written permission of:
%
%     BOSE CORPORATION
%     The Mountain
%     Framingham, MA 01701-9168
%
%---------------------------------------------------------------------
%

classdef PlotAudioOJT 
    properties
        % Audio Folder
        audio_folder
        
        % Frame size in Sample count
        frame_size
        % Number of channels
        out_channel_count
        in_channel_count
        
        % Input Audio Object
        inputObj
        % Expected Output Audio Object
        expectedObj
        % Actual Output Audio Object
        actualObj
        
        % Number of Frames (Common)
        frame_cnt
    end
    
    methods
    
        function a = PlotAudioOJT(audio_folder, frame_size, in_channel_count, out_channel_count)
            if nargin == 4
                a.audio_folder = audio_folder;
                a.frame_size = frame_size;
                a.in_channel_count = in_channel_count;
                a.out_channel_count = out_channel_count;
            else
                error ('Error: Not enough input arguments.\n Syntax PlotAudioOJT(audio_folder, frame_size, channel_count)');
            end
            
            % Input Audio File
            input_file = fullfile(audio_folder, 'audioIn.bin'); 
            % Expected Output Audio File
            expected_file = fullfile(audio_folder, 'audioRef.bin');  
            % Actual Output Audio File
            actual_file = fullfile(audio_folder, 'audioOut.bin');   
        
            a.inputObj = LoadAudioOJT(input_file, frame_size, in_channel_count); 
            a.expectedObj = LoadAudioOJT(expected_file, frame_size, out_channel_count); 
            a.actualObj = LoadAudioOJT(actual_file, frame_size, out_channel_count); 
            
            a.frame_cnt = min([a.inputObj.getFrameCnt(), a.expectedObj.getFrameCnt(), a.actualObj.getFrameCnt()]);
        end
        
        function frame_cnt = getFrameCnt(self)
            frame_cnt = self.frame_cnt
        end
        
        function plotAudioForChannel(self, in_channel_num, out_channel_num)
            if (in_channel_num >= 1) && (in_channel_num <= self.in_channel_count) && (out_channel_num >= 1) && (out_channel_num <= self.out_channel_count)
                input    = self.inputObj.getAudioSamplesForChannelByFrame(in_channel_num, 1, self.frame_cnt);
                expected = self.expectedObj.getAudioSamplesForChannelByFrame(out_channel_num, 1, self.frame_cnt);
                actual   = self.actualObj.getAudioSamplesForChannelByFrame(out_channel_num, 1, self.frame_cnt);
                self.plotAudio(input, expected, actual); 
            else
                error('Error: Invalid channel number')
            end
        end
        
        function plotAudioForChannelByIndex(self, in_channel_num, out_channel_num, start_idx, end_idx)
            max_idx = self.frame_cnt * self.frame_size
            if (start_idx < 1) || (start_idx > max_idx)
                error('Error: start index out of range')
            end
            if (end_idx < 1) || (end_idx > max_idx)
                error('Error: end index out of range')
            end
            if (start_idx > end_idx)
                error('Error: start index larger than end index')
            end
            input    = self.inputObj.getAudioSamplesForChannelByIndex(in_channel_num, start_idx, end_idx);
            expected = self.expectedObj.getAudioSamplesForChannelByIndex(out_channel_num, start_idx, end_idx);
            actual   = self.actualObj.getAudioSamplesForChannelByIndex(out_channel_num, start_idx, end_idx);
            self.plotAudio(input, expected, actual); 
        end
        
        function plotAudioForChannelByFrame(self, in_channel_num, out_channel_num, start_frame, end_frame)
            if (start_frame < 1) || (start_frame > self.frame_cnt)
                error('Error: start frame out of range')
            end
            if (end_frame < 1) || (end_frame > self.frame_cnt)
                error('Error: end frame out of range')
            end
            if (start_frame > end_frame)
                error('Error: start frame larger than end frame')
            end
            
            input    = self.inputObj.getAudioSamplesForChannelByFrame(in_channel_num, start_frame, end_frame);
            expected = self.expectedObj.getAudioSamplesForChannelByFrame(out_channel_num, start_frame, end_frame);
            actual   = self.actualObj.getAudioSamplesForChannelByFrame(out_channel_num, start_frame, end_frame);
            self.plotAudio(input, expected, actual); 
        end
    end % end of public methods
        
    methods(Access = protected)
    % ---- internal method--------------
        function plotAudio(self, input, expected_out, actual_out)
            figure
            
            subplot(4,1,1);
            plot(input)
            title('audioIn')
            
            subplot(4,1,2);
            plot(expected_out)
            title('audioRef')
            
            subplot(4,1,3);
            plot(actual_out)
            title('audioOut')
            
            subplot(4,1,4);
            plot(expected_out-actual_out)
            title('Difference')
        end
        
    end % end of internal methods
end



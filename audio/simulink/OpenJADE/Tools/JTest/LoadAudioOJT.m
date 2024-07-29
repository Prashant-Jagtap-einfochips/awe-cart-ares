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

classdef LoadAudioOJT 
    properties
        % Audio File
        audio_file
        % Frame size in Sample count
        frame_size
        % Number of channels
        channel_count
        % Audio Samples
        samples
        % Number of Frames
        frame_cnt
    end
    
    methods
    
        function a = LoadAudioOJT(audio_file, frame_size, channel_count)
            if nargin == 3
                a.audio_file = audio_file;
                a.frame_size = frame_size;
                a.channel_count = channel_count;
            else
                error ('Error: Not enough input arguments.\n Syntax LoadAudioOJT(audio_file, frame_size, channel_count)');
            end
            fid = fopen(audio_file);
            samples_x = fread(fid,'single');
            fclose(fid);
            samples_size = size(samples_x);
            frame_cnt = floor(samples_size(1)/(channel_count * frame_size));
            samples_a = samples_x(1:(channel_count * frame_size*frame_cnt));
            samples_b = reshape(samples_a, frame_size, channel_count, frame_cnt);
            samples = permute(samples_b, [1, 3, 2]);
            a.frame_cnt = frame_cnt;
            a.samples = samples;
        end
        
        function frame_cnt = getFrameCnt(self)
            frame_cnt = self.frame_cnt;
        end
        
        function samples = getAudioSamples(self)
            samples = self.samples;
        end
        
        function samples = getAudioSamplesForChannel(self, channel_num)
            if (channel_num >= 1) && (channel_num <= self.channel_count)
                samples = reshape(self.samples(:,:,channel_num),[],1);
            else
                error('Error: Invalid channel number');
            end
        end
        
        function samples = getAudioSamplesForChannelByIndex(self, channel_num, start_idx, end_idx)
            max_idx = self.frame_cnt * self.frame_size;
            if (start_idx < 1) || (start_idx > max_idx)
                error('Error: start index out of range');
            end
            if (end_idx < 1) || (end_idx > max_idx)
                error('Error: end index out of range');
            end
            if (start_idx > end_idx)
                error('Error: start index larger than end index');
            end
        
            data = self.getAudioSamplesForChannel(channel_num);
            
            samples = data(start_idx:end_idx);
        end
        
        function samples = getAudioSamplesForChannelByFrame(self, channel_num, start_frame, end_frame)
            if (start_frame < 1) || (start_frame > self.frame_cnt)
                error('Error: start frame out of range');
            end
            if (end_frame < 1) || (end_frame > self.frame_cnt)
                error('Error: end frame out of range');
            end
            if (start_frame > end_frame)
                error('Error: start frame larger than end frame');
            end
            
            data = self.getAudioSamplesForChannel(channel_num);
            
            start_idx = ((start_frame - 1) * self.frame_size) + 1;
            end_idx = end_frame * self.frame_size;
            
            samples = data(start_idx:end_idx);
        end
    end % end of public methods
        
    methods(Static)        
        function plotAudio(input, expected_out, actual_out)
            figure;
            subplot(4,1,1);
            plot(input);
            
            subplot(4,1,2);
            plot(expected_out);
            
            subplot(4,1,3);
            plot(actual_out);
            
            subplot(4,1,4);
            plot(expected_out-actual_out);
        end
        
    end % end of static methods
end


% Sample Usage
% addpath('C:\work\ASD_OpenJADE\Application\UnitTest\trunk\Runtime\JTest')
% inOJT = LoadAudioOJT('audioIn.bin', 32, 10);
% outOJT = LoadAudioOJT('audioOut.bin', 32, 10);
% refOJT = LoadAudioOJT('audioRef.bin', 32, 10);
% inch01 = inOJT.getAudioSamplesForChannel(1);
% outch01 = outOJT.getAudioSamplesForChannel(1);
% refch01 = refOJT.getAudioSamplesForChannel(1);
% LoadAudioOJT.plotAudio(inch01, refch01, outch01);
        

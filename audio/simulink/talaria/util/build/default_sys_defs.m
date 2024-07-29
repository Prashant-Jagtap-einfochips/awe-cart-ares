function x = default_sys_defs()
%

%   Copyright 2018-2019 Bose Corporation
x.Fs = 44100;
x.frame_size_in_samples = 32;
x.number_of_input_entertainment_channels = 2;
x.number_of_input_vnc_microphones = 1;
x.maxMusicDelay = 512;
x.include_boost_on_demand = 0;
x.number_of_output_announcement_channels=6;
x.number_of_output_audio_channels=8;

x.frameSize = x.frame_size_in_samples;
x.Fs_inverse = 1 / x.Fs;

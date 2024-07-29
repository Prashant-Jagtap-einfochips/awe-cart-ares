% Setup model parameters
% structBalance.sample_rate_in_hertz = [44100,48000];
% structBalance.frame_size = [16,32,64];
% structBalance.channelList = [1,2,4,6,8,10,12,14,16];
% structBalance.channelArr = {1, [1,2], [1,2,1,2], [1,2,1,2,1,2] ...
%     [1,2,1,2,1,2,1,2], [1,2,1,2,1,2,1,2,1,2], [1,2,1,2,1,2,1,2,1,2,1,2] ...
%     [1,2,1,2,1,2,1,2,1,2,1,2,1,2], [1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2]};

structBalance.sample_rate_in_hertz = 44100;
structBalance.frame_size = 32;
structBalance.channelList = 1;
structBalance.channelArr = {1};

file_name = 'C:\Users\sl1047176\OneDrive - Bose Corporation\Documents\Assignments\Balancev2.xlsx';
[arrayMIPS] = MIPS2Excel('BalanceMIPS', file_name, 'sharcPlusConfiguration', structBalance);
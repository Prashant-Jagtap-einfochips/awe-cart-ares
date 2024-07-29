% Setup model parameters for each "Init" callback (e.g. update diagram)
sample_rate_in_hertz = 44100;  
frame_size = 32;
sysin_channel_count = 2;

% Channel Ramper Map
%    'FL'  => 1 , FRONT LEFT
%    'FR'  => 2 , FRONT RIGHT
%    'FC'  => 3 , FRONT CENTER
%    'GL'  => 4 , BACK LEFT
%    'GR'  => 5 , BACK RIGHT
%    'BC'  => 6 , BACK CENTER
%    'FM'  => 7 , FRONT MONO
%    'BM'  => 8 , BACK MONO
%    'BL'  => 9 , BACK LEFT SURROUND
%    'BR'  => 10, BACK RIGHT SURROUND
%    'FLH' => 11, FRONT LEFT SURROUND
%    'FRH' => 12, FRONT RIGHT SURROUND

%  group lefts and rights for testing, CHANGE LATER
%                    [FL BL FLH BLH FR BR FRH BRH FC BC FM BM]
% balanceChanRampMap = [1  4 11  9   2  5  12  10  3  6  7  8];

% setup simulation RTC data
% run('BalanceMIPSSimEvents.m');
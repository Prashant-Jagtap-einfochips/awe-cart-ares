% Number of microphone
numMicIn = 2;
%set default phase detect start count to 1000, 
% 1000 frames is used for  calculation
start_count = 1000;  
% set the delay count to zero
delay_count	= 50;
% Total number of frames
frame_count  = start_count + delay_count;	

% Create a Phase Detector configuration struct
PhaseDetectorConfig = struct('numMicIn',numMicIn,...
                             'start_count',start_count,...
                             'frame_count',frame_count);

% assigns sys_defs model parameters
sys_defs.sineTableLength            =   1024;
sys_defs.Fs_Hz                      =   sys_defs.Fs;
sys_defs.frameSize_samples          =   32;

% calculates and assigns variables based on sys_defs variables
start_time                          =   0;
end_time                            =   ( sys_defs.sineTableLength - 1 )/sys_defs.sineTableLength;
inc_time                            =   1/sys_defs.sineTableLength;
time_values                         =   [start_time : inc_time : end_time];
sineTable                           =   0.99 * sin( 2 * pi * time_values );
sys_defs.sineTable                  =   sineTable;

% number of channel
Numch                                     = 2;

linear_sweep_ON_phaseDetector             = 2;
start_freq_Hz_phaseDetector               = [1200 1200];
end_freq_Hz_phaseDetector                 = [1200 1200];
sweep_time_sec_phaseDetector              = [0.25 0.25];
start_phase_rad_LinearSweep_phaseDetector = [0 pi/2];
frameSize                                 = sys_defs.frameSize_samples ;				 

% passes the defualt RTC values to the model
% conditional check to avoid multiple RTC object initialization
if (~exist('RTC_SIM_OBJ', 'var') || (~RTC_SIM_OBJ.REQ_list.Count))
    RTC_SIM_OBJ= RTC_SimData();
%    RTC_SIM_OBJ.AddREQ('SignalGeneratorLinearSweepTestSet',0,...
%        [0001,linear_sweep_ON_phaseDetector,start_freq_Hz_phaseDetector,end_freq_Hz_phaseDetector,sweep_time_sec_phaseDetector,start_phase_rad_LinearSweep_phaseDetector]);   
   
end
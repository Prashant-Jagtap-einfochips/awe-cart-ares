function Loudness_init( sys_defs )
%loudnessInit initializes variables used by Loudness library block and
%assigns them to the struct LD in the base workspace

%   Copyright 2018-2019 Bose Corporation
%% Loud gain calc parameters
LGC_LFNoiseLow = 83;
LGC_LFNoiseHigh = 88;

LGC_HFNoiseLow = 20;
LGC_HFNoiseHigh = 30;

LGC_bassThresh0 = -3;
LGC_bassSlope0 = 0.65;
LGC_bassThresh1 = -4;
LGC_bassSlope1 = 0.75;

LGC_midThresh0 = -4.5;
LGC_midSlope0 = 0.52;
LGC_midThresh1 = -5.5;
LGC_midSlope1 = 0.80;

LGC_trebleThresh0 = -7;
LGC_trebleSlope0 = 0.35;
LGC_trebleThresh1 = -7.5;
LGC_trebleSlope1 = 0.50;

LGC_minVNCSignal = 40; %vnc tuning file

% Bass
LGC_bassLoudThresh = 95.0;
LGC_bassLoudSlope = 0.35;
LGC_bassDownThresh = 40; %dBSPL
LGC_bassDownSlope = -1.25;
LGC_bassMaxLoudBoost = 20.0;
LGC_bassMaxVNCBoost = 20.0; %20
LGC_bassMaxBoost = 40.0; %40
LGC_downward_expansion_on = 1;

% Mid Band
LGC_midDownThresh = 40; % dBSPL
LGC_midDownSlope = -1.0;
LGC_midMaxBoost = 20.0; % dB %20by default

% Treble band (wide band)
LGC_trebDownThresh = 43; % dBSPL
LGC_trebDownSlope = -1.0;
LGC_trebMaxBoost = 14.0; % dB %20by default

% AudioPilot2 parameters
LGC_bassVNCThresh = -4.0;
LGC_bassVNCSlope = 0.58;
LGC_trebVNCThresh = -6.0; % dB
LGC_trebVNCSlope = 0.48;

% Enable Switch Ramper
enableRampTau = 0.1;            % s
enableRampCoef = (1 - sin(1/(enableRampTau * sys_defs.Fs))) ...
    / cos(1/(enableRampTau * sys_defs.Fs));


LGC = struct('LFNoiseLow', LGC_LFNoiseLow, 'LFNoiseHigh', LGC_LFNoiseHigh, ...
    'HFNoiseLow', LGC_HFNoiseLow, 'HFNoiseHigh', LGC_HFNoiseHigh, ...
	'bassThresh0', LGC_bassThresh0, 'bassSlope0', LGC_bassSlope0, ...
	'bassThresh1', LGC_bassThresh1, 'bassSlope1', LGC_bassSlope1, ...
	'midThresh0', LGC_midThresh0, 'midSlope0', LGC_midSlope0, ...
	'midThresh1', LGC_midThresh1, 'midSlope1', LGC_midSlope1, ...
	'trebThresh0', LGC_trebleThresh0, 'trebSlope0', LGC_trebleSlope0, ...
	'trebThresh1', LGC_trebleThresh1, 'trebSlope1', LGC_trebleSlope1, ...
	'minVNCSignal', LGC_minVNCSignal, ...
	'bassLoudThresh', LGC_bassLoudThresh, 'bassLoudSlope', LGC_bassLoudSlope, ...
	'bassDownThresh', LGC_bassDownThresh, 'bassDownSlope', LGC_bassDownSlope, ...
	'bassMaxLoudBoost', LGC_bassMaxLoudBoost, 'bassMaxVNCBoost', LGC_bassMaxVNCBoost, ...
	'bassMaxBoost', LGC_bassMaxBoost, 'enableRampCoef', enableRampCoef, ...
	'midDownThresh', LGC_midDownThresh, 'midDownSlope', LGC_midDownSlope, ...
	'midMaxBoost', LGC_midMaxBoost, ...
	'trebDownThresh', LGC_trebDownThresh, 'trebDownSlope', LGC_trebDownSlope, ...
	'trebMaxBoost', LGC_trebMaxBoost, ...
	'bassVNCThresh', LGC_bassVNCThresh, 'bassVNCSlope', LGC_bassVNCSlope, ...
	'trebVNCThresh', LGC_trebVNCThresh, 'trebVNCSlope', LGC_trebVNCSlope,...
    'downward_expansion_on',LGC_downward_expansion_on);

assignin('base', 'LGC', LGC);

%% Compensation parameters

% Zipper noise reduction filter
COMPZIP_Fc = 20;
COMPZIP_wc = 2 * pi * COMPZIP_Fc / sys_defs.Fs;
COMP_zipCoef = (1 - sin(COMPZIP_wc)) / cos(COMPZIP_wc);

% Bass band BPF
COMPBASS_Fc = 40;
COMPBASS_Q = 1.1;
COMPBASS_wc = 2 * pi * COMPBASS_Fc / sys_defs.Fs;
COMPBASS_sinCoef = sin(COMPBASS_wc);
COMPBASS_cosCoef = cos(COMPBASS_wc);

COMPBASS_sos = [0 0 0];
COMPBASS_sos(1) = (2*COMPBASS_Q - COMPBASS_sinCoef)/(2*COMPBASS_Q + COMPBASS_sinCoef);
COMPBASS_sos(2) = -4*COMPBASS_Q*COMPBASS_cosCoef/(2*COMPBASS_Q + COMPBASS_sinCoef);
COMPBASS_sos(3) = COMPBASS_sinCoef/(2*COMPBASS_Q + COMPBASS_sinCoef);

COMPBASS_Num = COMPBASS_sos(3)*[1 0 -1];
COMPBASS_Denom = [1 COMPBASS_sos(2) COMPBASS_sos(1)];

% Mid band LPF
COMPMID_Fc = 4000;
COMPMID_Q = sqrt(2)/2;
COMPMID_wc = 2*pi*COMPMID_Fc/sys_defs.Fs;
COMPMID_sinCoef = sin(COMPMID_wc);
COMPMID_cosCoef = cos(COMPMID_wc);

COMPMID_sos = [0 0 0];
COMPMID_sos(1) = (2*COMPMID_Q - COMPMID_sinCoef)/(2*COMPMID_Q + COMPMID_sinCoef);
COMPMID_sos(2) = -4*COMPMID_Q*COMPMID_cosCoef/(2*COMPMID_Q  + COMPMID_sinCoef);
COMPMID_sos(3) = COMPMID_Q*(1 - COMPMID_cosCoef)/(2*COMPMID_Q + COMPMID_sinCoef);

COMPMID_Num = COMPMID_sos(3)*[1 2 1];
COMPMID_Denom = [1 COMPMID_sos(2) COMPMID_sos(1)];

COMP = struct('zipCoef', COMP_zipCoef, ...
    'LoudBPFNum', COMPBASS_Num, 'LoudBPFDenom', COMPBASS_Denom, ...
    'LoudLPFNum', COMPMID_Num, 'LoudLPFDenom', COMPMID_Denom, ...
    'LoudBPFFreq',COMPBASS_Fc,'LoudBPFQ',COMPBASS_Q, ...
    'LoudLPFFreq',COMPMID_Fc,'LoudLPFQ',COMPMID_Q);

assignin('base', 'COMP', COMP);
end


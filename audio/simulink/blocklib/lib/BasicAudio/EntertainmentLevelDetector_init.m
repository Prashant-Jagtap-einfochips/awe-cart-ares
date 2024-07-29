function EntertainmentLevelDetector_init( sys_defs )
%entertainmentLevelDetectorInit Initialize variables used by the
% EntertainmentLevelDetecotr and assigns them to a struct "LD" in the base
% workspace

%   Copyright 2018-2019 Bose Corporation
dec_rate = sys_defs.Fs / sys_defs.frameSize;

fastDecay	 = 5.0; % db/sec 10
fastDecay    = -abs(fastDecay / dec_rate);
slowDecay	 = 0.1; % db/sec
slowDecay    = -abs(slowDecay / dec_rate);
holdTime   = 3.0; % sec 4.0
holdTime   = ceil(holdTime * dec_rate);
holdMargin = 6.0; % db (same as ld_hold_margin)
min		 = -80.0; % dBSPL
max		 = 0.0; % dBSPL
maxSPL	 = 123; % dBSPL

LD = struct('fastDecay', fastDecay, 'slowDecay', slowDecay, ...
	'holdTime', holdTime, 'holdMargin', holdMargin, ...
    'min', min, 'max', max, 'maxSPL', maxSPL);



%% Band splitter
order = 5;
fc = 100;

wc = fc / (sys_defs.Fs / 2);
numSOSStages = ceil(order / 2);

[z, p, k] = butter(order, wc);
sosLp = zp2sos(z, p, k);
pooliirLpCoeffs = sos2pooliir(sosLp);

[z, p, k] = butter(order, wc, 'high');
sosHp = zp2sos(z, p, k);
pooliirHpCoeffs = sos2pooliir(sosHp);

LD.bsLpCoeffs = [pooliirLpCoeffs; pooliirLpCoeffs];
LD.bsHpCoeffs = [pooliirHpCoeffs; pooliirHpCoeffs];
LD.bsNumStages = [numSOSStages; numSOSStages];
LD.bsMaxNumStages = sum(LD.bsNumStages);

assignin('base', 'LD', LD);

end

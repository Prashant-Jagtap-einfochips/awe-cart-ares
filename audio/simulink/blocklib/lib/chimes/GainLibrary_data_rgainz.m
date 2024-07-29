function GainLibrary_data_rgainz() 
% GAINLIBRARY_DATA_CHIMES initializes a set of bus objects in the MATLAB base workspace 

%   Copyright 2018-2019 Bose Corporation
% Bus object: bus_rgainz 
clear elems_rgainz;
elems_rgainz(1) = Simulink.BusElement;
elems_rgainz(1).Name = 'riseFrameCount';
elems_rgainz(1).Dimensions = 1;
elems_rgainz(1).DimensionsMode = 'Fixed';
elems_rgainz(1).DataType = 'int32';
elems_rgainz(1).SampleTime = -1;
elems_rgainz(1).Complexity = 'real';
elems_rgainz(1).Min = [];
elems_rgainz(1).Max = [];
elems_rgainz(1).DocUnits = '';
elems_rgainz(1).Description = sprintf(['This count indicates how many frames ',...
'remain until the targetGain is reached.\nWhen frameCount reaches "0", ',...
'the current gain should be switched with the target gain.']);

elems_rgainz(2) = Simulink.BusElement;
elems_rgainz(2).Name = 'fallFrameCount';
elems_rgainz(2).Dimensions = 1;
elems_rgainz(2).DimensionsMode = 'Fixed';
elems_rgainz(2).DataType = 'int32';
elems_rgainz(2).SampleTime = -1;
elems_rgainz(2).Complexity = 'real';
elems_rgainz(2).Min = [];
elems_rgainz(2).Max = [];
elems_rgainz(2).DocUnits = '';
elems_rgainz(2).Description = sprintf(['This count indicates how many frames ',...
'remain until the targetGain is reached.\nWhen frameCount reaches "0", ',...
'the current gain should be switched with the target gain.']);

elems_rgainz(3) = Simulink.BusElement;
elems_rgainz(3).Name = 'onFrameCount';
elems_rgainz(3).Dimensions = 1;
elems_rgainz(3).DimensionsMode = 'Fixed';
elems_rgainz(3).DataType = 'int32';
elems_rgainz(3).SampleTime = -1;
elems_rgainz(3).Complexity = 'real';
elems_rgainz(3).Min = [];
elems_rgainz(3).Max = [];
elems_rgainz(3).DocUnits = '';
elems_rgainz(3).Description = sprintf(['This count indicates how many frames ',...
'the output is to be ON.\nWhen frameCount reaches "0", there should be',...
'no output.']);

elems_rgainz(4) = Simulink.BusElement;
elems_rgainz(4).Name = 'offFrameCount';
elems_rgainz(4).Dimensions = 1;
elems_rgainz(4).DimensionsMode = 'Fixed';
elems_rgainz(4).DataType = 'int32';
elems_rgainz(4).SampleTime = -1;
elems_rgainz(4).Complexity = 'real';
elems_rgainz(4).Min = [];
elems_rgainz(4).Max = [];
elems_rgainz(4).DocUnits = '';
elems_rgainz(4).Description = sprintf(['This count indicates how many frames ',...
'the output is to be OFF.\nWhen frameCount reaches "0", output should be',...
'produced.']);

elems_rgainz(5) = Simulink.BusElement;
elems_rgainz(5).Name = 'riseRampCoeff';
elems_rgainz(5).Dimensions = 1;
elems_rgainz(5).DimensionsMode = 'Fixed';
elems_rgainz(5).DataType = 'single';
elems_rgainz(5).SampleTime = -1;
elems_rgainz(5).Complexity = 'real';
elems_rgainz(5).Min = [];
elems_rgainz(5).Max = [];
elems_rgainz(5).DocUnits = '';
elems_rgainz(5).Description = sprintf(['Multiplicative coefficient of gain. Each ',...
    'sample during the rise phase, current gain will be multiplied by ',...
    'this value.\nIn steady state (i.e. no ramping) the coefficient value will be "1.0".']);

elems_rgainz(6) = Simulink.BusElement;
elems_rgainz(6).Name = 'fallRampCoeff';
elems_rgainz(6).Dimensions = 1;
elems_rgainz(6).DimensionsMode = 'Fixed';
elems_rgainz(6).DataType = 'single';
elems_rgainz(6).SampleTime = -1;
elems_rgainz(6).Complexity = 'real';
elems_rgainz(6).Min = [];
elems_rgainz(6).Max = [];
elems_rgainz(6).DocUnits = '';
elems_rgainz(6).Description = sprintf(['Multiplicative coefficient of gain. Each ',...
    'sample during the fall phase, current gain will be multiplied by ',...
    'this value.\nIn steady state (i.e. no ramping) the coefficient value will be "1.0".']);

elems_rgainz(7) = Simulink.BusElement;
elems_rgainz(7).Name = 'targetGain';
elems_rgainz(7).Dimensions = 1;
elems_rgainz(7).DimensionsMode = 'Fixed';
elems_rgainz(7).DataType = 'single';
elems_rgainz(7).SampleTime = -1;
elems_rgainz(7).Complexity = 'real';
elems_rgainz(7).Min = [];
elems_rgainz(7).Max = [];
elems_rgainz(7).DocUnits = '';
elems_rgainz(7).Description = sprintf('This is the target gain value the ramper is trying to achieve.');

elems_rgainz(8) = Simulink.BusElement;
elems_rgainz(8).Name = 'currentGain';
elems_rgainz(8).Dimensions = 1;
elems_rgainz(8).DimensionsMode = 'Fixed';
elems_rgainz(8).DataType = 'single';
elems_rgainz(8).SampleTime = -1;
elems_rgainz(8).Complexity = 'real';
elems_rgainz(8).Min = [];
elems_rgainz(8).Max = [];
elems_rgainz(8).DocUnits = '';
elems_rgainz(8).Description = 'This is the current applied gain value.';

elems_rgainz(9) = Simulink.BusElement;
elems_rgainz(9).Name = 'isRisePhase';
elems_rgainz(9).Dimensions = 1;
elems_rgainz(9).DimensionsMode = 'Fixed';
elems_rgainz(9).DataType = 'int32';
elems_rgainz(9).SampleTime = -1;
elems_rgainz(9).Complexity = 'real';
elems_rgainz(9).Min = [];
elems_rgainz(9).Max = [];
elems_rgainz(9).DocUnits = '';
elems_rgainz(9).Description = sprintf(['This flag indicates whether we are ',...
'increasing or decreasing the volume.  Increase means isRisePhase = 1\n']);

elems_rgainz(10) = Simulink.BusElement;
elems_rgainz(10).Name = 'isFallPhase';
elems_rgainz(10).Dimensions = 1;
elems_rgainz(10).DimensionsMode = 'Fixed';
elems_rgainz(10).DataType = 'int32';
elems_rgainz(10).SampleTime = -1;
elems_rgainz(10).Complexity = 'real';
elems_rgainz(10).Min = [];
elems_rgainz(10).Max = [];
elems_rgainz(10).DocUnits = '';
elems_rgainz(10).Description = sprintf(['This flag indicates whether we are ',...
'increasing or decreasing the volume.  Decrease means isFallPhase = 1\n']);

rgainz_bus = Simulink.Bus;
rgainz_bus.HeaderFile = '';
rgainz_bus.Description = sprintf(['This structure contains a the complete ',...
    'set of control parameters for the rgain block''s "SetGain" service.']);
rgainz_bus.DataScope = 'Auto';
rgainz_bus.Alignment = -1;
rgainz_bus.Elements = elems_rgainz;
clear elems_rgainz;
assignin('base','rgainz_bus', rgainz_bus);

% RGAIN needs to nudge gain values a bit if they are sitting at zero since
% ramping is accomplished via multiplation. Otherwise, gain values of zero
% would never move! What we really want is a value which is on the edge of
% perception - not too small or too large. If the value is too small, we'll
% waste some of our ramp time in perceived silence. If the value is too
% large, the audio will lurch into perceptability at the start of the ramp.
SILENT_GAIN = single(10 ^ (-100 / 20)); % e.g. uncalibrated -100dB
assignin('base','rgain_SILENT_GAIN', SILENT_GAIN);

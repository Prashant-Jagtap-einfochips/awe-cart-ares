function GainLibrary_data() 
% GAINLIBRARY_DATA initializes a set of bus objects in the MATLAB base workspace 

%   Copyright 2018-2019 Bose Corporation
% Bus object: bus_rgain 
clear elems;
elems(1) = Simulink.BusElement;
elems(1).Name = 'frameCount';
elems(1).Dimensions = 1;
elems(1).DimensionsMode = 'Fixed';
elems(1).DataType = 'int32';
elems(1).SampleTime = -1;
elems(1).Complexity = 'real';
elems(1).Min = [];
elems(1).Max = [];
elems(1).DocUnits = '';
elems(1).Description = sprintf('This count indicates how many frames remain until the targetGain is reached.\nWhen frameCount reaches "0", the current gain should be explicitly set to target gain.');

elems(2) = Simulink.BusElement;
elems(2).Name = 'rampCoeff';
elems(2).Dimensions = 1;
elems(2).DimensionsMode = 'Fixed';
elems(2).DataType = 'single';
elems(2).SampleTime = -1;
elems(2).Complexity = 'real';
elems(2).Min = [];
elems(2).Max = [];
elems(2).DocUnits = '';
elems(2).Description = sprintf('Multiplicative coefficient of gain. Each sample, current gain will be multiplied by this value.\nIn steady state (i.e. no ramping) the coefficient value will be "1.0".');

elems(3) = Simulink.BusElement;
elems(3).Name = 'targetGain';
elems(3).Dimensions = 1;
elems(3).DimensionsMode = 'Fixed';
elems(3).DataType = 'single';
elems(3).SampleTime = -1;
elems(3).Complexity = 'real';
elems(3).Min = [];
elems(3).Max = [];
elems(3).DocUnits = '';
elems(3).Description = sprintf('This is the target gain value the ramper is trying to achieve.');

elems(4) = Simulink.BusElement;
elems(4).Name = 'currentGain';
elems(4).Dimensions = 1;
elems(4).DimensionsMode = 'Fixed';
elems(4).DataType = 'single';
elems(4).SampleTime = -1;
elems(4).Complexity = 'real';
elems(4).Min = [];
elems(4).Max = [];
elems(4).DocUnits = '';
elems(4).Description = 'This is the current applied gain value.';

rgain_bus = Simulink.Bus;
rgain_bus.HeaderFile = '';
rgain_bus.Description = sprintf('This structure contains a the complete set of control parameters for the rgain block''s "SetGain" service.');
rgain_bus.DataScope = 'Auto';
rgain_bus.Alignment = -1;
rgain_bus.Elements = elems;
clear elems;
assignin('base','rgain_bus', rgain_bus);

% RGAIN needs to nudge gain values a bit if they are sitting at zero since
% ramping is accomplished via multiplation. Otherwise, gain values of zero
% would never move! What we really want is a value which is on the edge of
% perception - not too small or too large. If the value is too small, we'll
% waste some of our ramp time in perceived silence. If the value is too
% large, the audio will lurch into perceptability at the start of the ramp.
SILENT_GAIN = single(10 ^ (-100 / 20)); % e.g. uncalibrated -100dB
assignin('base','rgain_SILENT_GAIN', SILENT_GAIN);

classdef RgainBusUtil
% Utility class of static methods to create a ramper bus and initialization
% values
    
%   Copyright 2021 Bose Corporation

methods (Static)
    
    function initalValue = GetInitStruct(initialGains)
        % GetInitStruct creates a MATLAB struct used to initialize an array
        % of rgain_bus objects for rampers. Pass in a vector containing an
        % initial gain for each ramper. For example if there are two
        % rampers, the initialGains would be [1 1].
        % Usage:
        %    initalValue = GetInitStruct(initialGains);

        ramperCount = numel(initialGains);
      
        % Prepare the initialization value for the ramper control bus
        all0 = zeros(1,ramperCount);
        all1 = ones(1,ramperCount);
        initalValue = struct( ...
            'frameCount', num2cell(all0), ...
            'rampCoeff', num2cell(all1), ...
            'targetGain', num2cell(initialGains), ...
            'currentGain', num2cell(initialGains) ...
            );
    end
        
    function Create(H) 
        % Ensure the rgain_bus object exists in global data associated with
        % block/model reference H (e.g. base workspace). If "rgain_bus" is
        % already found, no action is taken.
        if (nargin < 1)
            theModel = bdroot;
        else
            theModel = bdroot(H);
        end
        
        if false == Simulink.data.existsInGlobal(theModel,'rgain_bus')
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
            Simulink.data.assigninGlobal(theModel,'rgain_bus', rgain_bus);
        end
    end

    function silentGain = GetSilentGainLinear()
        % RGAIN needs to nudge gain values a bit if they are sitting at zero since
        % ramping is accomplished via multiplication. Otherwise, gain values of zero
        % would never move! What we really want is a value which is on the edge of
        % perception - not too small or too large. If the value is too small, we'll
        % waste some of our ramp time in perceived silence. If the value is too
        % large, the audio will lurch into perceptability at the start of the ramp.
        silentGain = single(10 ^ (-126 / 20)); % e.g. uncalibrated -126dB
    end
end
end

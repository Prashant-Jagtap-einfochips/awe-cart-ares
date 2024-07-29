classdef TalariaTest < matlab.unittest.TestCase
%TALARIATEST Base class for Talaria enhancements to matlab.unittest.TestCase

%   Copyright 2018-2019 Bose Corporation
    properties
        % Name of the model, minus .slx extension
        modelName
        
        % Test folder name
        projectPath
        
        % List of TuneVar new default values to change at start of simulation
        tuneVarChangeList
        
        % List of RTC events to run through during simulation
        rtcEventList
        
        % Map from RTC message name to RTC block path
        rtcMessageNameToBlockPath
        
        % Name of .mat file containing input vector data
        input
        
        % Array containing output data after simulation
        output
        
        % Array containing reference data
        reference
        
        % Name of reference file
        referenceFilename
        
        % Flag used to overwrite the reference file
        overwriteReferenceFlag
        
        % Stop time for simulation
        stopTime
        
        % Simulation mode (e.g. normal vs. SIL)
        simulationMode
        
        % Cell array containing other test parameters
        testParameterList
        
        % Struct array containing other logged signals
        signalsToLog
        
        % Callback function invoked before simulation
        preSimCallback
        
        % Callback function invoked after simulation
        postSimCallback
        
        % Boolean value indicating whether pragmas can be mapped in the 
        % generated code using pragma.h 
        pragmaMappingEnable
    end
    properties (Constant,Hidden)
        % Folder name off current Simulink project's RootFolder to receive
        % tunings. If present, calculated tunings from each simulateModel
        % are saved.
        TuneSaveFolder = 'tunings_save'

        % Folder name off current Simulink project's RootFolder containing
        % previously saved tunings. If present, tunings are loaded before
        % each simulateModel.
        TuneLoadFolder = 'tunings_load'
    end
    properties (Access=protected)
        % Track the sequential count of calls to simulateModel from each
        % method. The goal is to indentify all the characteristics involved
        % in calculating TOPs (test method parameters, BTC values, etc).
        % However, we cannot easily acquire the test's input arguments
        % since those are trapped on a stack-frame workspace with greater
        % depth than an immediate caller. We cannot hash BTC values to get
        % an indentifying setup because some tests insert RANDOM values
        % into their BTC properties (hash is different every time). The
        % next best thing might just be counting how many times the each
        % method calls simulateModel. The weakness of this scheme is that
        % if the test method is run for only some input arguments anything
        % about the ordering of input arguments is changed, it fails.
        MethodCount
        MethodCountIsFrozen
        MethodName
    end
    methods (TestClassSetup)
        function ResetMethodCounts(self)
            % Initialize a new blank method count map every time the test
            % class initializes
            self.MethodCount = containers.Map;
        end
    end
    
    methods(TestMethodSetup)
        % These methods are automatically called before each test method
        % The TuneVar change list uses an EventList object to store info but
        % the 'time' aspect is not supported and always set to 0.
        function initTuneVarChangeList(self)
            self.tuneVarChangeList = EventList();
        end
        
        function initRtcEvents(self)
            self.rtcEventList = RTC_SimData();
        end
        
        function initTestParamList(self)
            self.testParameterList = {};
        end
        
        function initSignalsToLog(self)
            self.signalsToLog = struct('Path',{},'Port',{},'Name',{});
        end
        
        function initSimMode(self)
            self.simulationMode = 'Normal';
        end
        
        function initPreAndPostSimCallback(self)
            self.preSimCallback = [];
            self.postSimCallback = [];
        end

        function initMethodCountState(self)
            % Release the method count to start counting up. This test
            % method is not called yet for this time, but will be soon.
            self.ReleaseMethodCount();
            self.MethodName = [];
        end
    end
    
    methods(TestMethodTeardown)
        function cleanupWorkspace(self)
            % Note: leaving any RTC simulation events behind in workspace
        end
%         function cleanupMethodCountState(self)
%             % Check if the count is not left frozen. This would be BAD
%             % and indicate that something has incremented the count without
%             % calling SimulateModel.
%             if (self.MethodCountIsFrozen)
%                 error(['GetMethodNameAndCount was called after the last ' ...
%                     'simulateModel of the test. This is not allowed']);
%             end
%         end            
    end
    
    methods
        function setModelName(self, name)
            % setModelName - Store the name of the model under test
            % Usage:
            %    self.setModelName(theModelName);
            self.modelName = name;
        end

        function setReference(self, filename)
            % setReference - Store the name of the reference .mat file and
            % load it into corresponding property.
            % Note: reference data variable name must be 'reference'
            % Usage:
            %    self.setReference(filename)
            self.referenceFilename = [filename '.mat'];
            load(filename, 'reference');
            self.reference = reference;            
        end
        
        function setStopTimeBasedOnReference(self)
            % setStopTimeBasedOnReference - Extract the number of samples
            % based on reference data length *AND* assuming a sample rate
            % of 44100.
            % Usage:
            %    self.setStopTimeBasedOnReference;
            %
            % See also: setReference, setStopTime
            if ~isempty(self.reference)
                samplesReference = length(self.reference);
                % Simulation time should be derived from the reference data
                simulationTime = (samplesReference - 1) / 44100;
                self.setStopTime(simulationTime);
            else
                error('Reference data is empty');
            end
        end
               
        function setInput(self, input_data)
            % setInput - Stores name of input data file which will be
            % loaded into the base workspace before simulation.
            % NOTE: This mechanism only loads a file. It does not connect
            % anything to the model's input ports or perform other actions.
            % Usage:
            %    self.setInput(fileName);
            self.input = input_data;
        end
        
        function setSimModeSIL(self)
            % setSimModeSIL Specifies that the model will be simulated in
            % SIL (Software-in-the-loop) mode.
            % Usage:
            %    self.setSimModeSIL;
            %
            % See also: setSimModeNormal
             self.simulationMode = 'Software-in-the-Loop (SIL)';
        end
        
        function setSimModePIL(self)
            % setSimModePIL Specifies that the model will be simulated in
            % PIL (Processor-in-the-loop) mode.
            % Usage:
            %    self.setSimModeSIL;
            %
            % See also: setSimModeNormal
             self.simulationMode = 'Processor-in-the-Loop (PIL)';
        end
        
        function enablePragmaMapping(self, enable)
            % enablePragmaMapping Enable the ability to map pragmas by 
            % specifying them in a pragma.h file.
            % Usage:
            %    self.enablePragmaMapping;
            % Alternatively specify whether to enable or not:
            %    self.enablePragmaMapping(false);
            if nargin < 2
                enable = true;
            end
            self.pragmaMappingEnable = enable;
        end
        
        function setSimModeNormal(self)
            % setSimModeNormal Specifies that the model will be simulated
            % in Normal mode which runs via the Simulink engine.
            % Usage:
            %    self.setSimModeNormal;
            %
            % See also: setSimModePIL
            self.simulationMode = 'Normal';
        end
        
        function setPreSimCallback(self,theCallback)
            % setPreSimCallback - Provide a function handle to be called
            % before simulation (via self.simulateModel) before the test
            % model is closed. The function should take one argument which
            % will be the test case "self" object. Make sure the function
            % is not itself in the test methods block!
            % Usage:
            %    self.setPreSimCallback(funcHandle)
            % Example:
            %    self.setPreSimCallback(@self.MyCallback)
            if isa(theCallback,'function_handle')
                self.preSimCallback = theCallback;
            else
                error('Argument must be a function handle, not a [%s].', ...
                    class(theCallBack));
            end
        end
        
        function setPostSimCallback(self,theCallback)
            % setPostSimCallback - Provide a function handle to be called
            % after simulation (via self.simulateModel) before the test
            % model is closed. The function should take one argument which
            % will be the test case "self" object. Make sure the function
            % is not itself in the test methods block!
            % Usage:
            %    self.setPostSimCallback(funcHandle)
            % Example:
            %    self.setPostSimCallback(@self.MyCallback)
            if isa(theCallback,'function_handle')
                self.postSimCallback = theCallback;
            else
                error('Argument must be a function handle, not a [%s].', ...
                    class(theCallBack));
            end
        end
        
        function simulateModel(self)
            % simulateModel - Runs a simulation for the current test case
            % Usage:
            %    self.simulateModel
            
            % Load the model
            load_system(self.modelName);
            % Ensure that the model is unmodified because this routine
            % will forcibly close the model without saving- all unsaved
            % changes will be lost
            self.fatalAssertTrue(strcmp(get_param(self.modelName,'Dirty'),'off'), ... 
                sprintf('Model %s is open with unsaved changes. Please save/cancel before running test.', ...
                    self.modelName))

            try
                % If pragma mapping is required, add MAP_PRAGMA defines and
                % add the project path to the include dir list so the
                % pragma definition file (pragma.h) can be found
                if self.pragmaMappingEnable
                    cmd = sprintf("TalariaTest.setLinkFlags(buildInfo,['-flags-link -MDMAP_PRAGMAS -I%s'])",self.projectPath);
                    set_param(gcs,'PostCodeGenCommand',cmd);
                end
                
                % Load the input data into base workspace
                self.configureInputs();

                % Set the simulation time
                if ~isempty(self.stopTime)
                    set_param(self.modelName, 'StopTime', num2str(self.stopTime));
                end

                % Go through the TuneVar change list and set new values
                self.changeParameters();
                
                % Setup the RTC event list
                self.setupRTCEvents();

                % Go through test parameter list and set new values
                self.changeTestParameters();

                % Setup logging for extra signals registered for capture
                self.logRegisteredSignals();
                
                % Setup the simulation mode
                self.setupSimulationMode();

                % Invoke the callback (if there is one)
                if isa(self.preSimCallback,'function_handle')
                    self.preSimCallback();
                end
                
                % Load pre-calculated TOPs
                self.LoadTuningForSimulation();

                % Run the simulation
                self.output = sim(self.modelName, 'SaveOutput', 'on');
                
                % Invoke the callback (if there is one)
                if isa(self.postSimCallback,'function_handle')
                    self.postSimCallback();
                end

                % Save the TOP values if the tuning save folder is present.
                % Do this now before closing the model since variant
                % subsystems are all resolved - no need to update diagram
                % yet again:
                self.SaveTuningFromSimulation();
                
                % Close the system and save the output
                close_system(self.modelName, 0);
                
            catch ME
                % The model under test has likely been modified with broken
                % links and other test instrumentation. "Try" to make sure
                % this modified version of the model is forcibly closed!
                close_system(self.modelName, 0);
                rethrow(ME);
            end

            % Simulation has run. Release the method count to increment
            % again in case the test method simulates more than once.
            self.ReleaseMethodCount();

            % Validate that the RTC sim object was not hijaaked
            self.checkRTCList();

            if self.overwriteReferenceFlag
                % Adding explicit manual step to check the output and bless
                % it before overwriting the reference vector.  If
                % everything looks good, type 'dbcont', else 'dbstop'.
                % Note that .mat file will be created at top level of
                % project and must be manually overwritten for the specific
                % test case
                
                % Pull out Time series data
                simOut = self.output;
                outputDataset = simOut.get('yout');
                outputSignal = outputDataset.get(1);
                outputTimeSeries = outputSignal.Values;
                self.output = outputTimeSeries;
                
                keyboard
                reference = self.output;
                save('tmp', 'reference');
                refFile = fullfile(self.projectPath, self.referenceFilename);
                movefile('tmp.mat', refFile);
                self.reference = reference;
            end
        end
        
        function setStopTime(self, time)
            % setStopTime - Directly sets the stop time for the simulation
            % Usage:
            %    self.setStopTime(timeInSeconds);
            %
            % See also: setStopTimeBasedOnReference
            self.stopTime = time;
        end
            
        function setOverwriteReferenceFlag(self, flag)
            % setOverwriteReferenceFlag - Sets flag that determines whether
            % to overwrite the stored reference data. When set, the test
            % method pauses in debug mode after simulation finishes:
            % This explicit manual step is to check the output and bless
            % it before overwriting the reference vector.  If
            % everything looks good, type 'dbcont', else 'dbstop'.
            % Note that .mat file will be created at top level of
            % project and must be manually overwritten for the specific
            % test case
            % Note: This flag is not automatically reset to false (default)
            self.overwriteReferenceFlag = flag;
        end
        
        % DEPRICATED:
        function registerParameterChange(self, tuneVarBlockURL, value)
            % The name of this method is changing to registerTuneVarByUrl
            %
            % See also: registerTuneVarByUrl
            self.tuneVarChangeList.AddEvent(tuneVarBlockURL, 0, value)
        end
        % DEPRICATED:
        function registerRTCEvent(self, rtcBlockURL, time, value)
            % This method is being replaced by registerRTC
            %
            % See also: registerRTC
            registerRTC(self, rtcBlockURL, time, value);
        end
        % DEPRICATED:
        function registerRtcRequest(self, rtcMessageName, time, value)
            % This method is being replaced by registerRTC
            %
            % See also: registerRTC
            registerRTC(self, rtcMessageName, time, value)
        end

        function registerTuneVar(self, tuneVarBlockURL, value)
            % registerTuneVar - Add a TuneVar default value change
            % to the pre-simulation list. Provide the Simulink block
            % URL and the new value to be used when simulation starts.
            % Note that TuneVar feature paths are not yet supported here.
            % Usage:
            %    self.registerTuneVar(tuneVarBlockURL, value);
            %
            % Example
            %    self.registerTuneVar('myModel/subsystem/myBlock',[0,1,2]);

            % Determine if the identifier is a feature path or Simulink URL
            if (contains(tuneVarBlockURL,'/'))
                % Yep - good to go with the URL:
                self.tuneVarChangeList.AddEvent(tuneVarBlockURL, 0, value);
            else
                % Doesn't look like a URL. This is not yet supported
                error('Unable to registerTuneVar [%s]. Only Simulink block paths are currently supported.', ...
                    tuneVarBlockURL);
            end
        end

        function registerRTC(self, rtcIdentifier, time, values)
            % registerRTC - Adds an RTC request to the test simulation.
            % Either the RTC block's Simulink URL path or its Talaria
            % FeaturePath can be used to identify the block (URLs have '/'
            % characters in them so we can tell the difference).
            % Usage:
            %    self.registerRtc(rtcIdentifier, time, values)
            %
            % Examples:
            %    self.registerRTC('MyModel/RtcBlock1', 0.1, [001,128,0,500]);
            %    self.registerRTC('PreampVolumeSetIt', 0.1, [001,128,0,500]);
            %    self.registerRTC('TestTrigger',1,[]);
            
            % Determine if the identifier is a feature path or Simulink URL
            if (contains(rtcIdentifier,'/'))
                % It's a URL. Discover the RTC block's actual message name.
                load_system(self.modelName);
                get_param(rtcIdentifier,'Name'); % fully load the block
                featurePath = MaskUtil.GetEvaluatedParameter(rtcIdentifier,'MessagePath');
            else
                % It must already be a feature path. Just use it.
                featurePath = rtcIdentifier;
            end
            % Add event to RTC event list
            self.rtcEventList.AddREQ(featurePath, time, values);        
        end

        function [reqStruct,rspStruct,ntfStruct,timeDiff] = reviewRTC(self,rtcIdentifier, time)
            % reviewRTC can be called after a simulation to review the
            % resulting simulated RTC traffic. Given the same rtcIdentifier
            % and time as the original registerRTC from before the call to
            % simulateModel, this routine attempts to find the closest
            % matching request, response, and notification payloads based
            % on time. 
            % 
            % Usage:
            %    [reqStruct,rspStruct,ntfStruct,timeDiff] = reviewRTC(self,rtcIdentifier,time);
            %
            % self : the instance of a TalariaTest-derived classdef
            % rtcIdentifier : FeaturePath of RTC message (block path also supported)
            % time : simulation time in seconds previously registered for the request
            % 
            % reqStruct is the closest-found simulated request
            % rspStruct is the matching response, or empty if the RTC does
            %    not return responses
            % ntfStruct is the matching notification, or empty if the RTC
            %    does not return notifications OR if another request 
            %    happened before a notification was generated for the
            %    closest matching request.
            %
            % See also: RTC_SimData.SummarizeService
            reqStruct = [];
            rspStruct = [];
            ntfStruct = [];

            % First retrieve the rtc sim object
            simObj = Simulink.data.evalinGlobal(self.modelName,RTC_Mask.SimObjName);

            % Endure old-school block-URL-style specifications by opening
            % the model and asking the block what its feature path is:
            if (contains(rtcIdentifier,'/'))
                % It's a URL. Discover the RTC block's actual message name.
                load_system(self.modelName);
                get_param(rtcIdentifier,'Name'); % fully load the block
                featurePath = MaskUtil.GetEvaluatedParameter(rtcIdentifier,'MessagePath');
            else
                % It must already be a feature path. Just use it.
                featurePath = rtcIdentifier;
            end

            % Retrieve the full summarize service output
            thePayloads = simObj.SummarizeService(featurePath);

            % Iterate through list and fill the returned payloads. We do
            % not have the RTC block's SampleTime to best determine the
            % matching event hit, so just grab the closest one
            reqSecondsDiff = 100; % assume we have to be closer than this
            lookingForNtf = false;
            for i=1:numel(thePayloads)
                aPayload = thePayloads(i);
                aTimeDiff = aPayload.time - time;
                % Always scan for matching request : closest time resets
                % all the payloads
                if ("Request" == aPayload.type)
                    % check time difference
                    if abs(aTimeDiff) <= abs(reqSecondsDiff)
                        % New best time match! (... or another RTC in same
                        % simulation time which is weird, but nothing to
                        % stop it)
                        reqSecondsDiff = aTimeDiff;
                        reqStruct = aPayload;
                        rspStruct = [];
                        ntfStruct = [];
                        % we're looking for an NTF after this request
                        lookingForNtf = true;
                    else
                        % This request is not the new best time... but if
                        % it's here before an NTF, lock out NTF until a new
                        % REQ match is match
                        if (isempty(ntfStruct))
                            % uh oh... new request found BEFORE the NTF was
                            % found. This means we are no longer looking
                            % for the NTF of that winning REQ (because this
                            % new REQ came in)
                            lookingForNtf = false;
                        end
                    end
                elseif ("Response" == aPayload.type ) && ...
                    (eps >= (aTimeDiff - reqSecondsDiff))
                    % This response happened at the same time as the
                    % selected request, so capture it!
                    rspStruct = aPayload;
                elseif ("Notification" == aPayload.type && lookingForNtf)
                    % Found the first NTF after request. Grab it!
                    ntfStruct = aPayload;
                    lookingForNtf = false;
                end
            end
            timeDiff = reqSecondsDiff;
        end
        
        function registerTestParameterChange(self, paramPathToBlock, paramName, value)
            % registerTestParameterChange - Add a test parameter
            % path/name/value to the list of ones to change pre-simulation.
            % Provide the Simulink URL of the block, the block's parameter
            % name, and the value to use for simulation.
            % Usage:
            %    self.registerTestParameterChange(pathToBlock, paramName, paramValue);
            newTestParameter = {paramPathToBlock, paramName, value};
            self.testParameterList = [self.testParameterList, {newTestParameter}];
        end
 
        function registerSignalToLog(self,blockPath,outportNumber,logName)
            % registerSignalToLog  - Sets up log capture for an output
            % signal of  the specified block. After simulation has run, you
            % can save the logged signals as references, verify logged
            % signals match previously saved references, or access each
            % logged signal  individually for other purposes.
            % Usage:
            %    self.registerSignalToLog(blockPath, outportNumber, logName)          
            %
            % See also: saveLoggedSignals, verifyLoggedSignals,
            %      getSignalLog, GetUnframedLoggedSignal
            self.signalsToLog(end+1) = struct( ...
                'Path',char(blockPath), ...
                'Port',double(outportNumber), ...
                'Name',char(logName));
        end
        
        function signalData = getSignalLog(self,logName)
            % getSignalLog - Gets an individual logged signal's data for a 
            % previously registered and logged signal. Call after normal
            % simulation.
            % Usage:
            %    self.getSignalLog(logName);
            %
            % See also: registerSignalToLog, saveLoggedSignals,
            % verifyLoggedSignals, GetUnframedLoggedSignal
            signalData = self.output.logsout.getElement(logName);
        end

        function saveLoggedSignals(self, testMethodName)
            % saveLoggedSignals - Saves the currently logged signals to the
            % filesystem in the same directory as the test model. This
            % routine can be used to record reference signals. To
            % differentiate logs from multiple test methods, provide an
            % optional string argument to be used for saving/loading files.
            % Otherwise (with no arguments) logs are saved to default
            % names.
            % Usage:
            %    saveLoggedSignals(self)
            %    saveLoggedSignals(self, testMethodName)
            %
            % See also: registerSignalToLog, verifyLoggedSignals, getSignalLog
            folder = self.projectPath;
            
            if (nargin < 2)
                testMethodName = '';
            end
            testMethodName = char(testMethodName);

            % Loop through registered signals to save logs
            numSignals = numel(self.signalsToLog);
            for i=1:numSignals
                sig = self.signalsToLog(i);
                filename = fullfile(folder,[testMethodName sig.Name '.mat']);
                reference = self.getSignalLog(sig.Name);
                save(filename,'reference');
            end
        end

        function verifyLoggedSignals(self, testMethodName, optionalTolerance, optionalToleranceType)
            % verifyLoggedSignals Verifies that logged signals match
            % previously saved reference signals. Optionally provide a
            % string argument to differentiate among saved files, and
            % another to specify a tolerance for the comparisons.
            % Usage:
            %    self.verifyLoggedSignals();
            %    self.verifyLoggedSignals(testMethodName);
            %    self.verifyLoggedSignals(testMethodName,optionalTolerance,optionalToleranceType);
            %      The default tolerance is single(1e-6) but can be
            %      overridden.
            %      The default tolerance type is 'RelTol' but also can be
            %      overridden - choices are 'AbsTol' or 'RelTol'
            %
            % See also saveLoggedSignals, registerSignalToLog
            folder = self.projectPath;
            
            % set default tolerance and method name
            if (nargin < 2)
                testMethodName = '';
            end
            if nargin < 3
                optionalTolerance = single(1e-6);
            end
            if (nargin < 4)
                optionalToleranceType = 'RelTol';
            end
            % Check for switcheroos between our function arguments
            if (isnumeric(testMethodName))
                temp = optionalTolerance;
                optionalTolerance = testMethodName;
                if (ischar(temp))
                    testMethodName = temp;
                else
                    testMethodName = '';
                end
            end
            testMethodName = char(testMethodName);

            % Loop through registered signals to compare logs
            numSignals = numel(self.signalsToLog);
            for i=1:numSignals
                sig = self.signalsToLog(i);
                filename = fullfile(folder,[testMethodName sig.Name '.mat']);
                % retrieve signal logged during recent simulation
                simulated = self.getSignalLog(sig.Name);
                % retrieve signal saved previously as a reference
                load(filename,'reference');
                % Compare the signal to the reference
                theSignal = simulated.Values.Data;
                theReference = reference.Values.Data;
                failMsg = sprintf(['Signal ''%s'' ([%s] outport #%d) ' ...
                    'did not match reference'], ...
                    sig.Name, MaskUtil.GetBlockLinkString(sig.Path), sig.Port);
                
                % Add the pre-load of the model to the block link string:
                loadSysStr = sprintf('matlab:load_system(''%s'');',self.modelName);
                failMsg = strrep(failMsg,'matlab:',loadSysStr);
                
                self.verifyEqual(theSignal, theReference, optionalToleranceType, ...
                    optionalTolerance, failMsg);
            end
        end
        
        function signal = GetUnframedOutputSignal(self,signalName)
            % GetUnframedOutputSignal Retrieves model output as 2D array.
            % Simulation data is commonly returned in a 3D array in the
            % format: num_frames x frame_size x num_channels. This method
            % converts an output into a 2D array in the format:
            % num_samples x num_channels
            % Usage:
            %   self.GetUnframedOutputSignal(signalName)
            %
            % See also: GetUnframedLoggedSignal

            outSeries = self.output.get(signalName);
            signal = self.RemoveFramesDimension(outSeries);
        end

        function signal = GetUnframedLoggedSignal(self,logName)
            % GetUnframedLoggedSignal Retrieves model output as 2D array.
            % Simulation data is commonly returned in a 3D array in the
            % format: num_frames x frame_size x num_channels. This method
            % converts an output into a 2D array in the format:
            % num_samples x num_channels
            % Usage:
            %   self.GetUnframedLoggedSignal(logName)
            % where logName is the name used during registerSignalToLog
            %
            % See also: registerSignalToLog, GetUnframedOutputSignal

            outSeries = self.output.logsout.get(logName);
            signal = self.RemoveFramesDimension(outSeries);
        end        
        function signal = RemoveFramesDimension(self, numericSignalObject)
            % RemoveFramesDimension will recognize signal type, extract its
            % data, and remove the flatten out the frames dimension. The
            % supported cases are generally simplistic assuming defaults
            % and only one data series.
            % Usage:
            %   self.RemoveFramesDimension(numericSignalObject);
            %
            % See also: GetUnframedLoggedSignal, GetUnframedOutputSignal

            % Although the model has a configurable 'SaveFormat' parameter
            % that indicates the desired format for the saved output, logs
            % can use a different format. Best to just look at the data and
            % figure it out

            switch (class(numericSignalObject))
                case 'Simulink.SimulationData.Dataset'
                    elem = numericSignalObject.getElement(1);
                    vals = elem.Values;
                    outData = vals.Data;
                case 'Simulink.SimulationData.Signal'
                    vals = numericSignalObject.Values;
                    outData = vals.Data;
                case 'struct'
                    outData = numericSignalObject.signals.values;
                otherwise % e.g. 'Array' for SaveFormat
                    outData = numericSignalObject;
            end
            
            permutation = permute(outData, [1 3 2]);
            signal = reshape(permutation, [], size(outData,2), 1);
        end

        %------------------------------------------------------------------
        function [methodName,theCount] = GetMethodNameAndCount(self)
            % Returns sequential index of each method's invocation of
            % simulateModel for this initialization of the test class. When
            % a test method is about to be called by the test runner, the
            % count opens up to increment, tracking the next simulateModel.
            % Once simulateMode completes, the count opens back up to
            % increment if the test method has more than one simulation
            % call in it.
            % ALERT: Just make sure NOT to call this after simulateModel
            % unless the test plans to still execute another simulateModel!
            % Usage:
            %    [methodName,theCount] = self.GetMethodNameAndCount();
            if (false == self.MethodCountIsFrozen)
                % First call for this method invocation. Discover this
                % method's name and count. Then - lock it down again.
                self.MethodName = self.GetTestMethod();
                methodName = self.MethodName;
                if (self.MethodCount.isKey(methodName))
                    % Method has already been called since class-wide init
                    theCount = self.MethodCount(methodName) + 1;
                else
                    % First call from this method for this class-wide init
                    theCount = 1;
                end
                self.MethodCount(methodName) = theCount;
                % Remember count is now determined. Further calls will only
                % return what has already been figured out... until either
                % the test method is called by the runner again, or a
                % simulateModel releases the count to increment.
                self.MethodCountIsFrozen = true;
            else
                theCount = self.MethodCount(self.MethodName);
                methodName = self.MethodName;
            end
        end
        %------------------------------------------------------------------
        function FakeSimulation(self)
            % This method is exposed only to assist with 3rd party
            % integrations that do not want to call simulateModel, but
            % still do want to iterate through the test tunings.
            % Usage:
            %   self.FakeSimulation(); % replacing simulateModel

            % Load tuning
            self.LoadTuningForSimulation();

            % Release the count to increment again
            self.ReleaseMethodCount();
        end

    end % end of public methods
    methods(Access = protected)
        % ---- internal method--------------
        function configureInputs(self)
            % Called by simulateModel
            if ~isempty(self.input)
                evalin('base', ['load ' self.input]);
            end
        end
        % ---- internal method--------------
        function setupSimulationMode(self)
            % Called by simulateModel
            set_param(self.modelName,'SimulationMode',self.simulationMode);
        end
        % ---- internal method--------------
        function changeParameters(self)
            % Go through TuneVar change list and set values in the model
            for i = 1:self.tuneVarChangeList.Count
                paramData = self.tuneVarChangeList.Array(i);
                set_param(paramData.name, 'LinkStatus', 'inactive');
                
                maskType = get_param(paramData.name, 'MaskType');
                if (strcmp(maskType, TuneVarMask.MaskType) || ...
                    strcmp(maskType, TuneTOP_Mask.MaskType))
                    MaskUtil.SetPromotedParameter(paramData.name, 'MaskValue', paramData.data);
                else
                    error('Unsupported block type [%s] for registered tune changes', maskType);
                end
            end
        end
        % ---- internal method--------------
        function setupRTCEvents(self)
            % Sets up the simulated event list before simulation starts
            % based on requests previously registered
            
            % First, verify all the simulated RTC requests refer to
            % services which actually exist. The best time to do this is
            % *after* the model initializes but before simulation actually
            % begins running: aka the model's "Start" callback.
            startFcn = get_param(self.modelName,'StartFcn');
            
            % Ensure all the RTC events actually exist
            validationCommand = sprintf('%s.ValidateREQs();',RTC_Mask.SimObjName);
            searchExpression = ['^\s*' RTC_Mask.SimObjName '\.ValidateREQs\(\)'];
            cmdFound = regexp(startFcn,searchExpression,'lineanchors','once');
            if isempty(cmdFound)
                startFcn = [startFcn newline '%Added by TalariaTest' newline validationCommand];
                set_param(self.modelName,'startFcn',startFcn);
            end
            
            % Place the rtc sim object in global view
            Simulink.data.assigninGlobal(self.modelName,RTC_Mask.SimObjName,self.rtcEventList);
        end
        % ---- internal method--------------
        function checkRTCList(self)
            % Internal method: Checks that the global RTC event list still
            % matches what we set up before simulation
            
            % Grab the list tht is currently in global view
            fromGlobal = Simulink.data.evalinGlobal(self.modelName,RTC_Mask.SimObjName);
            fromTest = self.rtcEventList;
            % Make sure the lists matches up - if not, trigger an error
            errMsg = sprintf(['The RTC simulated request list was altered by the model during ' newline ...
                'simulation. RTC requests registered by the test case were either ' newline ...
                'overwritten or new unexpected requests were added. Check the model''s ' newline ...
                '<a href="matlab:Simulink.internal.OpenCallbackParamsDialog([''' ...
                self.modelName '''],''InitFcn'');">InitFcn</a> ' ...
                'callback (and others) for interations with the %s object.'], ...
                RTC_Mask.SimObjName);
            self.verifyEqual(fromGlobal.REQ_list, fromTest.REQ_list, errMsg);
        end
        % ---- internal method--------------
        function changeTestParameters(self)
            % Internal method: Go through test parameter list and set values in the model
            for i = 1:length(self.testParameterList)
                currentParam = self.testParameterList{i};
                MaskUtil.DisableLibraryLink(currentParam{1});
                set_param(currentParam{1}, currentParam{2}, currentParam{3});
            end
        end
        % ---- internal method--------------
        function logRegisteredSignals(self)
            % Internal method to set up logging for previously registered
            % signals. This method is automatically invoked during
            % simulateModel.
            %
            % See also registerSignalToLog
            
            % Loop through previously registered signals to log
            numSignals = numel(self.signalsToLog);
            for i=1:numSignals
                % Get a handle to the line
                sig = self.signalsToLog(i);
                lineHandles = get_param(sig.Path,'LineHandles');
                aLine = lineHandles.Outport(sig.Port);
                % Turn logging on, use custom name, and set the name
                set(aLine,'DataLogging',1);
                set(aLine,'DataLoggingNameMode','Custom');
                set(aLine,'DataLoggingName',sig.Name);
            end
            % Now simulation must be run to log the data
        end
        %------------------------------------------------------------------
        function SaveTuningFromSimulation(self)
            % Called by simulateModel after simulation has completed and
            % calculated all TOP values. If the folder specified by
            % self.TuneSaveFolder exists, then this method saves the
            % calculated TOPs into the self.TuneSaveFolder as a *.mat file.
            % Additionally saves build-time configuration data (BTC) in
            % both *.mat and *.xml formats as well as the TOP external
            % integration xml file.

            % First determine if tunings are being saved
            cp = currentProject;
            saveTuningsFolder = fullfile(cp.RootFolder,self.TuneSaveFolder);

            if (false == isfolder(saveTuningsFolder))
                % Folder not present. Tunings are not being saved.
                return;
            end

            [methodName,methodCount] = self.GetMethodNameAndCount();
            fileNameBase = sprintf('%s.%s.%d',class(self),methodName,methodCount);

            % Save #1: Save the BTC settings to xml.
            % Use bootstrap mode to acquire BTC - the model just simulated
            % so the variables will be up-to-date. Saving this may assist
            % with making sense of what each test is doing.
            btcXmlName = strcat(fileNameBase,'.BTC.xml');
            btcXmlFilePath = fullfile(saveTuningsFolder,btcXmlName);
            ExtractModelBTC(self.modelName,btcXmlFilePath,true,true);

            % Save #2 : The TOP Values. This one matters. All the other
            % saved files are mostly just for documentation and review.
            % If map was previously saved. Ensure it matches. This check
            % ensures that file names capture enough to differentiate
            % between tunings. So DELETE THE OLD FILES when things change.
            topFileName = strcat(fileNameBase,'.TOP');
            previousMap = TOP_Mask.DataMap.LoadDataMap(self.modelName, ...
                saveTuningsFolder,topFileName);
            if (false == isempty(previousMap))
                isSame = TOP_Mask.DataMap.CompareToMap(self.modelName,previousMap);
                if (false == isSame)
                    error('Previously saved map does not match [%s]', ...
                        topFileName);
                end
            end
            TOP_Mask.DataMap.SaveDataMap(self.modelName,saveTuningsFolder,topFileName);

            % Save #3: Also save the TOP external integration XML for more
            % convenient review of the TOPs present in this scenario
            topXmlName = strcat(fileNameBase,'.TOP.xml');
            topXmlFilePath = fullfile(saveTuningsFolder,topXmlName);
            DumpTopExternalIntegrationXml(self.modelName,topXmlFilePath,false,true);

            % Save #4: Save model-level BTC object so it be loaded along
            % with the TOP values as a "manual" test run
            hasConfig = BTConfig.HasModelLevelConfig(self.modelName);
            if (hasConfig)
                theModelSettings = BTConfig.GetModelSettings(self.modelName);
                modelBtcSaveFileName = strcat(fileNameBase,'.BTC.mat');
                modelBtcSaveFilePath = fullfile(saveTuningsFolder,modelBtcSaveFileName);
                save(modelBtcSaveFilePath,"theModelSettings");
            end
        end
        %------------------------------------------------------------------
        function LoadTuningForSimulation(self)
            % Called by simulateModel before simulation starts.
            % If the folder specified by self.TuneLoadFolder exists, then
            % this method loads the saved TOPs from the TuneLoadFolder
            % for the matching classname, methodname, and count number.

            % First determine if tunings are being loaded
            cp = currentProject;
            loadTuningsFolder = fullfile(cp.RootFolder,self.TuneLoadFolder);

            if (false == isfolder(loadTuningsFolder))
                % Folder not present. Tunings are not being loaded.
                return;
            end

            [methodName,methodCount] = self.GetMethodNameAndCount();
            fileNameBase = sprintf('%s.%s.%d',class(self),methodName,methodCount);
            topFileName = strcat(fileNameBase,'.TOP');
            TOP_Mask.DataMap.LoadDataMap(self.modelName,loadTuningsFolder,topFileName);
        end
        %------------------------------------------------------------------
        function [methodName, methodClass, line] = GetTestMethod(self)
            % Grabs call stack and classdef type information to find which
            % test method is currently running (if any)
            methodName = [];
            methodClass = [];
            line = 0;
            thisClassName = class(self);
            theStack = dbstack(1);
            mco = meta.class.fromName(thisClassName);
            for i=1:numel(theStack)
                aFrame = theStack(i);
                methodNameParts = string(aFrame.name).split('.');
                if (numel(methodNameParts) < 2)
                    % not a class member
                    continue;
                end
                aMethodClass = methodNameParts(1);
                aMethodName = methodNameParts(2);
                if (false == strcmp(aMethodClass,thisClassName))
                    % class name does not match
                    continue;
                end
                methodObject = findobj(mco.MethodList,'Name',aMethodName);
                if (isempty(methodObject) || 0 == methodObject.Test)
                    % method not found or not a test case method
                    continue;
                end
                % Found the method! Too bad we cannot also acquire the 
                % current values for methodObject.InputNames :-(
                methodName = aMethodName;
                methodClass = aMethodClass;
                line = aFrame.line;
                break;
            end
        end
        %------------------------------------------------------------------
        function ReleaseMethodCount(self)
            % Releases the method count to increment again. Called whe one
            % of the following happens (internal use only)
            % 1- a test method is initialized, getting ready to run
            % 2- a simulateModel finishes, getting ready for another
            % 3- a fake simulation call replaces simulateModel (3rd party)
            % Usage:
            %    self.ReleaseMethodCount();
            self.MethodCountIsFrozen = false;
        end
        
    end % end of protected methods
    
    methods(Static)
        function setLinkFlags(buildInfo, linkFlags)
            % Post code gen command where build info can be updated.
            % Adding the test folder to the linker path
            addLinkFlags(buildInfo, linkFlags);
        end
    end
end


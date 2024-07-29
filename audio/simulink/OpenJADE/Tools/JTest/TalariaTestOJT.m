classdef TalariaTestOJT < TalariaTest
%TALARIATEST Base class for Talaria enhancements to matlab.unittest.TestCase

%   Copyright 2018-2019 Bose Corporation
    properties
        % Name of the test
        testName
        % Name of the test
        testOutPath
        % combine partition generate
        partitionGenerate
        % partition generate options
        partitionGenerateOptions
		% TSPs
		tspChangeList
    end
    methods
        function a = TalariaTestOJT(test_name, test_path, partition_generate_options) % Constructor to pick test name
            if nargin ~= 0
                a.testName = test_name;
            else
                a.testName = '';
            end
            if nargin > 1
                a.testOutPath = test_path;
            else
                a.testOutPath = './';
            end
            if nargin > 2
                a.partitionGenerateOptions = partition_generate_options;
                a.partitionGenerate = true;
            else
                a.partitionGenerate = false;
            end
			
			a.tspChangeList = EventList(); % Same type as tuneVarChangeList
        end
		
        function registerOjtTSP(self, tspBlockURL, time, charOffset, charCount)
            % registerOjtTSP - Add a TSP Read Request
            % Provide the Simulink block URL, the new value
            % to be used when simulation starts.
            % Note that TSP feature paths are not yet supported here.
            % Usage:
            %    self.registerOjtTSP(tspBlockURL, value);
            %
            % Example
            %    self.registerOjtTSP('myModel/subsystem/myBlock',[0,1,2]);
			
			if (nargin < 4)
				% Default values set in ConvertRequest.py?
				charOffset = [];
			end
			
			if (nargin < 5)
				% Default values set in ConvertRequest.py?
				charCount = [];
			end
			
            % Determine if the identifier is a feature path or Simulink URL
            if (contains(tspBlockURL,'/'))
                % Yep - good to go with the URL:
				charData = [charOffset charCount];
                self.tspChangeList.AddEvent(tspBlockURL, time, charData);
            else
                % Doesn't look like a URL. This is not yet supported
                error('Unable to registerOjtTSP [%s]. Only Simulink block paths are currently supported.', ...
                    tspBlockURL);
            end
        end

        function postSimCallbackOJT(self)
            % Create OJT file
            reqFd = fopen([self.testOutPath '/request' self.testName '.ojt'], 'w');

            %global sample_rate_in_hertz;
            stop_time_as_str = get_param(self.modelName, 'StopTime');
            stop_time = str2double(stop_time_as_str);
            fprintf( '  Stop Time = %f\n', stop_time);
            fprintf(reqFd, '[SYS_NUM_FRAMES %s], registerBAF, 0\n', stop_time_as_str);
            if isempty(self.output.logsout)
                fprintf( 'Warning: Missing audio capture\n');
            end
            cmd_handle_index = 100;
            fprintf('  Tune Count = %d\n', self.tuneVarChangeList.Count);
            for i=1:self.tuneVarChangeList.Count
                fprintf(reqFd, 'PASSTHRU_SINGLE, registerTuneVar, %s, %f, %d, %s\n', ...
                    MaskUtil.GetEvaluatedParameter(self.tuneVarChangeList.Array(i).name,'activeTunePath'), ...
                    self.tuneVarChangeList.Array(i).time, ...
                    cmd_handle_index, ...
                    self.tuneVarChangeList.Array(i).data  ...
                );
                data = self.tuneVarChangeList.Array(i).data;
                if ~(find(data) == ';') % is 2d
                    data(1)=' ';
                    len = size(data,2);
                    data(len)=' ';
                end
            end
			
            cmd_handle_index = cmd_handle_index+100;
			fprintf('  TSP Count = %d\n', self.tspChangeList.Count);
            for i=1:self.tspChangeList.Count
                fprintf(reqFd, 'PASSTHRU_SINGLE, registerOjtTSP, %s, %f, %d', ...
                    MaskUtil.GetEvaluatedParameter(self.tspChangeList.Array(i).name,'tspPath'), ...
                    self.tspChangeList.Array(i).time, ...
                    cmd_handle_index ...
                );
                data = self.tspChangeList.Array(i).data;
				if ~isempty(data)
					for j = 1:numel(data)
						fprintf(reqFd, ', %d', data(j));
					end
				end
				fprintf(reqFd, '\n');
                if ~(find(data) == ';') % is 2d
                    data(1)=' ';
                    len = size(data,2);
                    data(len)=' ';
                end
            end
			
            fprintf('  RTC Count = %d\n', self.rtcEventList.REQ_list.Count);
            for i=1:self.rtcEventList.REQ_list.Count
                fprintf(reqFd, 'PASSTHRU_SINGLE, registerRTC, %s, %f, ', ...
                    self.rtcEventList.REQ_list.Array(i).name, ...
                    self.rtcEventList.REQ_list.Array(i).time  ...
                );
                fprintf(reqFd, '[');
                data = self.rtcEventList.REQ_list.Array(i).data;
                len = size(data, 2);
                for j=1:len
                    if data(j)==floor(data(j))
                        fprintf(reqFd, ' %d', data(j));
                    else
                        fprintf(reqFd, ' %g', data(j));
                    end
                end
                fprintf(reqFd, ' ]\n');
                cmd_handle_index = cmd_handle_index+1;
            end
            if ~isempty(self.output.logsout)
                for i=1:self.output.logsout.numElements
                    if strncmp (self.output.logsout{i}.Name,'audioIn',7) || strncmp(self.output.logsout{i}.Name, 'audioOut',8) || ...
                            strncmp(self.output.logsout{i}.Name, 'audioIO', 7)
                        binFd = fopen([self.testOutPath '/' self.output.logsout{i}.Name self.testName '.bin'], 'w');
                        fwrite( binFd, self.output.logsout{i}.Values.Data, 'single' );
                        fclose( binFd );
                    end
                    if strcmp(self.output.logsout{i}.Name, 'audioOutUnroll')
                        binFd = fopen([self.testOutPath '/' self.output.logsout{i}.Name self.testName '.bin'], 'w');
                        fwrite( binFd, permute(self.output.logsout{i}.Values.Data, [1 3 2]), 'single');
                        fclose( binFd );
                    end
                end
                refName = [self.testOutPath '/audioOutUnroll' self.testName 'Ref.bin'];
                if isfile( refName )
                    refFd = fopen(refName , 'r');
                    refOut=fread(refFd, inf, 'single');
                    fclose(refFd);
                    binName = [self.testOutPath '/audioOutUnroll' self.testName '.bin'];
                    if  isfile( binName )
                        binFd = fopen(binName, 'r');
                        currOut=fread( binFd, inf, 'single');
                        fclose( binFd );
                        difOut=refOut - currOut;
                        fprintf('Regression Error max:%f\n', max(abs(difOut)));
                        histogram(difOut(difOut~=0));
                    end
                end
            end
            fclose( reqFd );
            end

        function simulateModel(self)
            if self.partitionGenerate
                if (string(talariaVersion) < "V2200BL")
                    partition_and_generate(self.modelName, self.partitionGenerateOptions);
                else
                    artifactInfo = partition_and_generate(self.modelName, self.partitionGenerateOptions);
                    saveGeneratedCodePathsOJT(artifactInfo);
                end
            end
            self.setPostSimCallback(@self.postSimCallbackOJT);
            simulateModel@TalariaTest(self);
        end
    end % end of public methods

end


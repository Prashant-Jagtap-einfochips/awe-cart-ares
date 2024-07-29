classdef ConnectivityConfig < rtw.connectivity.Config
%CONNECTIVITYCONFIG target connectivity configuration class for SHARC+ PIL
    
%   Copyright 2018-2019 Bose Corporation
    
    methods
        function this = ConnectivityConfig(componentArgs)
            
            % A target application framework specifies additional source files and libraries
            % required for building the executable
            targetApplicationFramework = ...
                SharcPlusPILConfig.TargetApplicationFramework(componentArgs);
            
            % Filename extension for executable on the target system (e.g.
            % '.exe' for Windows or '' for Unix
            exeExtension = '.dxe';
            
            % Create an instance of MakefileBuilder; this works in conjunction with your
            % template makefile to build the PIL executable
            builder = rtw.connectivity.MakefileBuilder(componentArgs, ...
                targetApplicationFramework, ...
                exeExtension);
            
            % Launcher
            launcher = SharcPlusPILConfig.Launcher(componentArgs, builder);
            
            % File extension for shared libraries (e.g. .dll on Windows)
            [~, ~, sharedLibExt] = coder.BuildConfig.getStdLibInfo;

            % Evaluate name of the rtIOStream shared library
            connectivityConfigPath = fileparts(mfilename('fullpath'));
            rtiostreamLib = fullfile(connectivityConfigPath, ...
                ['librtiostream_file' sharedLibExt]);
            
            hostCommunicator = rtw.connectivity.RtIOStreamHostCommunicator(...
                componentArgs, ...
                launcher, ...
                rtiostreamLib);
            
            % For some targets it may be necessary to set a timeout value
            % for initial setup of the communications channel. For example,
            % the target processor may take a few seconds before it is
            % ready to open its side of the communications channel. If a
            % non-zero timeout value is set then the communicator will
            % repeatedly try to open the communications channel until the
            % timeout value is reached.
            hostCommunicator.setInitCommsTimeout(20); 
            
            % Configure a timeout period for reading of data by the host 
            % from the target. If no data is received with the specified 
            % period an error will be thrown.
            timeoutReadDataSecs = 60;
            hostCommunicator.setTimeoutRecvSecs(timeoutReadDataSecs);

            % Construct the file paths used for communication
            appCodePath = componentArgs.getApplicationCodePath();
            appCodePath = [appCodePath filesep];

            % HOST-SIDE SETUP
            % ---------------------------------------------------
            % Custom arguments that will be passed to the              
            % rtIOStreamOpen function in the rtIOStream shared        
            % library (this configures the host-side of the           
            % communications channel)                                  
            rtIOStreamOpenArgs = {...                                  
                '-path', appCodePath, ...
                '-verbose', '2', ...
                };                                                     
            hostCommunicator.setOpenRtIOStreamArgList(...          
                rtIOStreamOpenArgs);
            fprintf('Host-Side PIL arguments: [%s]\n',strjoin(rtIOStreamOpenArgs));
            
            % Reset IO files (Create empty files)
            % ---------------------------------------------------
            toHost = fullfile(appCodePath,'toHostPIL');
            toTarget = fullfile(appCodePath,'toTargetPIL');
            fid = fopen(toHost,'w');
            if (fid == -1)
                error('Unable to reset file %s',toHost);
            end
            fclose(fid);
            fid = fopen(toTarget,'w');
            if (fid == -1)
                error('Unable to reset file %s',toTarget);
            end
            fclose(fid);

            
            % Put it together
            % ---------------------------------------------------
            % call super class constructor to register components
            this@rtw.connectivity.Config(componentArgs,...
                                         builder,...
                                         launcher,...
                                         hostCommunicator);
            
            % Optionally, you can register a hardware-specific timer. Registering a timer
            % enables the code execution profiling feature. In this example
            % implementation, we use a timer for the host platform.
            timer = coder.profile.crlHostTimer();
            this.setTimer(timer);
            
        end
    end
end


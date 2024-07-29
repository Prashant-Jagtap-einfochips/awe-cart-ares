classdef Launcher < rtw.connectivity.Launcher
%LAUNCHER is an example target connectivity configuration class

%   Copyright 2007-2012 The MathWorks, Inc.

    properties
        % For the host-based example, it is necessary to
        % keep track of the process ID of the executable
        % so that this process can be killed when no longer
        % required
        ExePid = '';

        % For the host-based example, it is necessary to keep track of a temporary file
        % created by the process launcher so that it can be deleted when the
        % process is terminated
        TempFile = '';
        
        imageName = '';
    end

    methods
        % constructor
        function this = Launcher(componentArgs, builder)
            narginchk(2, 2);
            % call super class constructor
            this@rtw.connectivity.Launcher(componentArgs, builder);
        end
        
        % destructor
        function delete(this) %#ok
            
            % This method is called when an instance of this class is cleared from memory,
            % e.g. when the associated Simulink model is closed. You can use
            % this destructor method to close down any processes, e.g. an IDE or
            % debugger that was originally started by this class. If the
            % stopApplication method already performs this housekeeping at the
            % end of each on-target simulation run then it is not necessary to
            % insert any code in this destructor method. However, if the IDE or
            % debugger may be left open between successive on-target simulation
            % runs then it is recommended to insert code here to terminate that
            % application.

        end

        % Start the application
        function startApplication(this)
            % get name of the target executable file
            exe = this.getBuilder.getApplicationExecutable;

            % calculate path to host-based processor simulator
            cces_dir = getenv('CCES_2_10');
            this.imageName = 'CCES_Runner.exe';
            adisim = fullfile(cces_dir, this.imageName);
            
            % setup argument string for ADI simulator
            processor = 'ADSP-SC594';
            target = 'ChipFactory Simulator';
            platform = 'ADSP-SC594 Cycle-Accurate-Sim';
            core = 0;
            simArgs = sprintf('--processor %s --target "%s" --platform "%s" --core "%d,%s"', ...
                processor, target, platform, core, exe);
            % "C:\Analog Devices\CrossCore Embedded Studio 2.10.0\CCES_Runner.exe" --processor ADSP-SC594 --target "ChipFactory Simulator" --platform "ADSP-SC594 Functional-Sim" --core "0,rtiostream_project_Core1.dxe"

            % launch                                                 
            disp('Starting PIL simulation')    

            [this.ExePid, this.TempFile] = ...                       
                rtw.connectivity.Utils.launchProcess(...             
                    adisim, ...                                         
                    simArgs);
                
            % Pause to ensure that server-side of TCP/IP connection  
            % is established and ready to accept a client connection 
            pause(0.4)                                               
            if ~rtw.connectivity.Utils.isAlive(this.ExePid)          
                disp('')                                             
                disp(['Process is not alive, displaying contents '...
                     'of log file:'])                                
                disp('')                                             
                type(this.TempFile)                                  
                disp('')                                             
                error(['Failed to start process with PID = '...      
                    num2str(this.ExePid) ' using arguments '...      
                    simArgs '. '...                           
                    'The process may have failed to start '...       
                    'correctly from image [' adisim '] in pwd:' pwd])                   
            end                                                      
            disp(['Started new process, pid = ' ...                  
                  int2str(this.ExePid) ' stdout:' this.TempFile ' using arguments ' simArgs])                            

        end
        
        % Stop the application
        function stopApplication(this)
            
            disp('Stopping PIL simulation')                                  
%             if ~isempty(this.ExePid)                                       
%                 rtw.connectivity.Utils.killProcess(this.ExePid, ...        
%                                                    this.TempFile);    
%                 if ~rtw.connectivity.Utils.isAlive(this.ExePid)
%                     disp(['Unable to terminate process normally, pid = ' int2str(this.ExePid)]);
                    superKillCmd = sprintf('taskkill /F /IM %s', this.imageName);
                    [killStat, killOutput] = system(superKillCmd);
                    if (killStat)
                        fprintf('Unable to kill process with %s, %s\n', ...
                            superKillCmd, killOutput);
                    else
                        disp(['Terminated process, image name = ' this.imageName]);
                    end

%             end                                                            
            this.ExePid = '';                                              

        end
    end
end

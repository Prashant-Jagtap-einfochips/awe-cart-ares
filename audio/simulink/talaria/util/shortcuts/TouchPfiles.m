function TouchPfiles(inThisFolder)
%TOUCHPFILES updates the date/time stamp on all *.p files
%   This action is a workaround to undesired interactions between SVN
%   checkouts timestamping files based on download time, while MATLAB believes
%   the timestamps indicate edit/creation times. MATLAB will emit a warning
%   if it detects that a *.p file has a more recent timestamp than its
%   matching *.m file. The solution: update the timestamps of all the *.p
%   files in a SVN-downloaded sandbox.
% Usage:
%   TouchPfiles();
%   TouchPfiles(inThisFolder)
% inThisFolder specifies a folder name under which to search for *.p files.
% If not provided (the expected case) then the root folder of the current
% Simulink Project is used.

if (exist('inThisFolder','var'))
    % A particular folder has been specified. Use this one.
else
    % No folder was specified, so use the current project's root folder
    project = currentProject;
    inThisFolder = project.RootFolder;
end

if ispc
    theCmd = sprintf('powershell "Get-ChildItem %s -Filter *.p -Recurse | foreach {$_.LastWriteTime = [System.DateTime]::Now}"',inThisFolder);
elseif isunix
    theCmd = sprintf('find %s -type f -name "*.p" -exec touch {} \\;', inThisFolder); 
end

fprintf('Updating *.p file timestamps in folder %s...\n',inThisFolder);
[status,cmdout] = system(theCmd);
if (status)
    error('%s',cmdout);
elseif (false == isempty(cmdout))
    fprintf('%s\n',cmdout);
end

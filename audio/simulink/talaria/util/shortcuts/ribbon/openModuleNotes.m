function openModuleNotes()
%   This function opens module notes in the editor - using location of this
%   script to determine where module_notes.txt will be found

%   Copyright 2022 Bose Corporation

    p = mfilename('fullpath');
    fileName = fullfile(fileparts(fileparts(fileparts(fileparts(p)))),'module_notes.txt');
    if exist(fileName, 'file')
        edit(fileName);
    else
        warning('File [%s] not found', fileName);
    end
end

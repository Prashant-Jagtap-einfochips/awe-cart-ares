% Script run at MATLAB startup while the mingw toolbox is installed

%   Copyright 2018-2019 Bose Corporation
fprintf('MinGW-gw64 C/C++ compiler 6.3.0 64-bit & 32-bit installed here: %s\n',mfilename('fullpath'));

ensure_mingw_path();

function ensure_mingw_path()
    installPath = fileparts(mfilename('fullpath'));
    correct_location = fullfile(installPath,'mingw64-6.3.0');
    
    % Set the MW_MINGW64_LOC environment variable so that MATLAB can "find"
    % this installation when searching for compilers (e.g. mex -setup -v)
    MW_MINGW64_LOC = getenv('MW_MINGW64_LOC');
    if (false == strcmp(MW_MINGW64_LOC,correct_location))
        if (false == isempty(MW_MINGW64_LOC))
            fprintf('Correcting MW_MINGW64_LOC\nPreviously: %s\nNew value: %s\n', ...
                MW_MINGW64_LOC,correct_location);
        end
        setenv('MW_MINGW64_LOC',correct_location);
    end
    
    % Add to the system path so compilation can work. Something about this
    % toolbox installation neglects to create the *.bat file normally found
    % from code generation with the standard MathWorks version of MinGW
    addedPath = fullfile(correct_location,'bin');
    thePath = getenv('path');
    if (contains(thePath,addedPath))
        %fprintf('MinGW path verified\n');
    else
        %fprintf('MinGW added to path\n');
        newPath = [addedPath ';' thePath];
        setenv('path',newPath);
    end
    
end

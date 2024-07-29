function isOk = CheckTalariaRibbonVersion(updateIt)
% CheckTalariaRibbonVersion updates Talaria version string in ribbon
% Usage:
%   isOk = CheckTalariaRibbonVersion(updateIt)

%   Copyright 2022 Bose Corporation

    if (nargin < 1)
        updateIt = false;
    end
    isOk = false;

    p = mfilename('fullpath');
    ribbonFolder = fileparts(p);
    actFilePath = fullfile(ribbonFolder,'ribbon','resources','json','customTab_actions.json');
    fileContent = fileread(actFilePath);

    % Find this pattern and tokenize the preamble-text and "------" parts
    %    "id": "talariaVersionAction",
    %    "text": "------",
    theExp = '(?<PRE>"id": "talariaVersionAction",\s*"text": ")(?<VER>[^"]*)';
    % Get the current Talaria version and replace the match with the
    % before-text and current version
    theVer = talariaVersion();
    replaceExp = ['$1' theVer];

    % Do replace:
    replacedContent = regexprep(fileContent,theExp,replaceExp);

    % Also find the old version:
    tokens = regexp(fileContent,theExp,'names');

    isMatch = strcmp(replacedContent,fileContent);

    % If empowered to update, write out the up-to-date file
    if (updateIt)
        if (isMatch)
            fprintf('Verified ribbon version is [%s]\n',theVer);
        else
            fprintf('Automatically updating ribbon version from [%s] to [%s]\n', ...
                tokens.VER,theVer);
            % Write it back out:
            fid = fopen(actFilePath,'w+');
            fprintf(fid,'%s',replacedContent);
            fclose(fid);
            isOk = true;
        end
    else
        % Only checking for a match
        isOk = isMatch;
        if (isMatch)
            fprintf('Verified ribbon version is [%s]\n',theVer);
        else
            fprintf('Ribbon version mismatch [%s] vs. [%s]. Pass optional argument true to update.\n', ...
                tokens.VER,theVer);
        end
    end
end
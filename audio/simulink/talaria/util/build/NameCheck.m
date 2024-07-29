classdef NameCheck
    
    properties (Constant)
        IgnoreSlx = {'BoseTalaria'}
        IgnoreBlock = {'RTC/RTC'}
    end
    
methods (Static)
    
function theIssues = Folder(folderName)
    % CheckNameGuidelines Opens and analyzes *.slx files checking guideline
    % compliance.

    theIssues = NameCheck.ValidateDir(folderName);

    % Dump out all the issues to the command window
    issueText = strjoin(theIssues,newline);
%     fprintf('%s\n',issueText);
end

function [issues] = ValidateDir(dirName)

    issues = {};
    dirList = dir(dirName);
    if (isempty(dirList))
        issues{end+1} = sprintf('Unable to find folder [%s]',dirName);
    end
    entityCount = numel(dirList);
    for i=1:entityCount
        entityName = dirList(i).name;
        % If the name is nothing but dots, skip it
        if (entityName == '.')
            continue;
        end
        
        [~,~,fileExtension] = fileparts(entityName);
        entityPath = fullfile(dirName,entityName);
        
        % Run name checks as appropriate for the type of entity
        newIssues = {};
        if (dirList(i).isdir)
            newIssues = NameCheck.ValidateDir(entityPath);
        elseif (fileExtension == ".slx")
            newIssues = NameCheck.ValidateSlx(entityPath);
        elseif (fileExtension == ".m")
%             newIssues = NameCheck.ValidateM(entityPath);
        end
        % TODO ".m" filenames, function names, etc.
        
        issues = horzcat(issues, newIssues);        
    end
end

function [issues] = ValidateSlx(entityPath)
% ValidateSlx discovers top-level masked blocks and checks names
    [~,fileName] = fileparts(entityPath);
    if (ismember(fileName, NameCheck.IgnoreSlx))
        issues = {sprintf('Ignoring slx [%s]',entityPath)};
        return;
    end
    issues = {};
    [~,fileName,~] = fileparts(entityPath);
    load_system(entityPath);
    blocks = Simulink.findBlocks(fileName);
    numBlocks = numel(blocks);
    for i=1:numBlocks
        aBlock = getfullname(blocks(i));
        % First check the block/diagram name:
        blockName = get_param(aBlock,'Name');
        if (i==1)
            % It's a diagram (library or model). Do checks here:
            entityDescription = sprintf('FileName[%s]',blockName);
            newIssues = NameCheck.ValidatePascalCase(entityDescription,blockName);
            issues = horzcat(issues, newIssues);
        else
            [newIssues] = NameCheck.ValidateBlock(aBlock);
            issues = horzcat(issues, newIssues);
        end        
    end
end

function [issues] = ValidateBlock(aBlock)
    if (ismember(aBlock,NameCheck.IgnoreBlock))
        issues = {sprintf('Ignoring block [%s]',aBlock)};
        return;
    end
    issues = {};
    
    % Block name is PascalCase
    blockName = get_param(aBlock,'Name');
    entityDescription = sprintf('BlockName[%s]',aBlock);
    newIssues = NameCheck.ValidatePascalCase(entityDescription,blockName);
    issues = horzcat(issues, newIssues);

    % Check the mask, if any:
    newIssues = NameCheck.ValidateMask(aBlock);
    issues = horzcat(issues, newIssues);
    
    % Ports must have useful names if more than one per type
    portH = get_param(aBlock,'PortHandles');
    portCheckArray = struct(...
        'Ports',{portH.Inport,portH.Outport}, ...
        'BaseName',{'In','Out'});
    for structIdx = 1:numel(portCheckArray)
        portCheck = portCheckArray(structIdx);
        % More than one port - look for trivial names
        if (numel(portCheck.Ports) > 1)
            portNames = get(portCheck.Ports,'Name');
            % match trivial-looking names
            trivialNames = regexpi(portNames, ...
                sprintf('^%s\\d*',portCheck.BaseName),'once','match');
            % eliminate empties
            trivialNames = trivialNames(false==cellfun('isempty',trivialNames));
            if (false == isempty(trivialNames))
                issues{end+1} = sprintf('%s Trivial port names found when more than one of type [%s]', ...
                    entityDescription,strjoin(trivialNames));
            end
        end
    end

    % Checks specific to libraries
    if (bdIsLibrary(bdroot(aBlock)))
        % library block names should not be hidden
        if ("off" == get_param(aBlock,'ShowName'))
            issues{end+1} = sprintf('%s Should not hide block name in library', ...
                entityDescription);
        elseif ("on" ~= get_param(aBlock,'HideAutomaticName'))
            issues{end+1} = sprintf('%s Probably wants to automatic hide name for library block', ...
                entityDescription);
        end
    end
end

function [issues] = ValidateMask(aBlock)
    issues = {};
    theMask = Simulink.Mask.get(aBlock);
    if (isempty(theMask))
        % Not masked. We're done!
        return;
    end
    
    % If the block is a link to a library, do not validate the mask
    referenceBlock = get_param(aBlock,'ReferenceBlock');
    if (false == isempty(referenceBlock))
        % Stop. Catch any mask problems when checking the library itself
        return;
    end
    
    blockName = get_param(aBlock,'Name');
    
    % TODO Mask name should start with component project (e.g. Talaria,
    % Blocklib). Start with currentProject and recurse ProjectReferences,
    % grab each ProjectPath.File and determine which path "finds" the
    % mask's block's slx file. This project name is the one we want. ;-)
%     if (false == strcmp(theMask.Type,blockName))
    projectName = 'Talaria';
    if (isempty(regexp(theMask.Type,'^Talaria')))
        issues{end+1} = sprintf('MaskType[%s] Mask name does not start with [%s]: [%s]', ...
            aBlock,projectName,theMask.Type);
    end

    % TODO Descriptions should be brief - rest in help instead
    
    % Determine if parameters names are PascalCase (or camelCase)
    caseType = 'PascalCase';
    numParams = numel(theMask.Parameters);
    if (numParams > 1)
        allNames = char({theMask.Parameters.Name});
        firstLetters = allNames(:,1);
        numCapitals = sum(isstrprop(firstLetters,'upper'));
        if (numCapitals < numParams/2)
            caseType = 'camelCase';
        end
    end
    
    % Check parameters
    for i=1:numParams
        aParam = theMask.Parameters(i);
        entityDescription = sprintf('MaskParam[%s.%s]',aBlock,aParam.Name);
        if ("on" == aParam.Tunable)
            issues{end+1} = sprintf('%s is tunable', ...
                entityDescription);
        end
        
        % Check mask parameter names are consistent in case
        newIssues = NameCheck.ValidateCase(entityDescription,aParam.Name,caseType);
        issues = horzcat(issues, newIssues);        
        
        % Check edit box prompts end in colon
        % First find the originating parameter just to discover the type
        origParam = MaskUtil.FindPromotedParameter(theMask,aParam.Name);
        if ("off" == aParam.Hidden && "edit" == origParam.Type)
            % Make exception for BTConfig.Assert params
            checkIt = true;
            if ("on" == origParam.Evaluate)
                paramValue = MaskUtil.GetEvaluatedParameter(aBlock,aParam.Name);
                if (isobject(paramValue))
                    checkIt = false;
                end
            end
            if (checkIt)
                trimmedPrompt = strtrim(aParam.Prompt);
                if (false == isempty(trimmedPrompt) && trimmedPrompt(end) ~= ':')
                    issues{end+1} = sprintf('%s edit prompt does not end in colon [%s]', ...
                        entityDescription,aParam.Prompt);
                end
            end
        end
        
        % TODO: FeaturePath names should have library default of block name
        
        % TODO: prompts are sentence case
        
    end
end

function [issues] = ValidatePascalCase(entityDescription,name)
    issues = NameCheck.ValidateCase(entityDescription,name,'PascalCase');
end

function [issues] = ValidateCase(entityDescription,name,caseType)
    issues = {};
    isUpper = isstrprop(name,'upper');
    isAlpha = isstrprop(name,'alpha');
    isPunct = isstrprop(name,'punct');
    if ("PascalCase" == caseType && false == isUpper(1))
        issues{end+1} = sprintf('%s %s violation lowercase first character [%c]', ...
            entityDescription, caseType, name(1));
    end
    if ("camelCase" == caseType && true == isUpper(1))
        issues{end+1} = sprintf('%s %s violation uppercase first character [%c]', ...
            entityDescription, caseType, name(1));
    end
    badChars = ~ismember(name,['a':'z','A':'Z','0':'9']);
    if (false == isAlpha(1))
        issues{end+1} = sprintf('%s %s violation first character must be alpha', ...
            entityDescription, caseType);
    end
    badCount = sum(badChars);
    if (badCount > 0)
        issues{end+1} = sprintf('%s %s violation %d invalid character(s): [%s]', ...
            entityDescription, caseType, badCount, name(badChars));
    end
    
    % Attempt to detect acronyms that are not treated as words. The check
    % is ultimately detection of multiple consecutive capitals
    idxStart = 0;
    idxEnd = 0;
    length = numel(name);
    for i=1:length+1
        if (idxStart)
            % String of caps in progress
            if (i > length || (false == isUpper(i)) && (isAlpha(i) || isPunct(i)))
                idxEnd = i-1;
            end
        elseif (i <= length)
            % No caps string in progress yet
            if (isUpper(i))
                % Start one here
                idxStart = i;
            end
        end
        % Check if a string of caps is completed
        if (idxStart && idxEnd)
            % Found some caps - check if more than one
            %isNotEntireWordOrEndOfSymbolAcronym = (idxEnd<length);
            if ((idxEnd - idxStart) >= 1)
                acronym = extractBetween(name,idxStart,idxEnd);
                issues{end+1} = sprintf('%s %s violation suspected all-caps acronym [%s]', ...
                    entityDescription, caseType, acronym{1});
            end
            idxStart = 0;
            idxEnd = 0;
        end
    end  
end

end % methods Static
end % classdef
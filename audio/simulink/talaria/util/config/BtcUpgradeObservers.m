classdef BtcUpgradeObservers
%BTCUPGRADEOBSERVERS Adds new observer features to *Config.m classes
%   Profile a root folder in which to search for files ending in Config.m
% This function will attempt to upgrade the files it finds with support for
% the observer-based lock mechanism instead of relying on the old Validate
% based lock mechanism. The Validate function is still important (to
% validate the data!) but is no longer required to enforce locks once the
% observer attributes are in place.
%
% See also: BtcUpgradeTuneVars

%   Copyright 2022 Bose Corporation

methods
    %--------------------------------------------------------------------------
    function this = BtcUpgradeObservers(rootFolder,writeChangesToFile)
        % "Constructor" runs UpgradeFolder
        % Usage:
        %   BtcUpgradeObservers(rootFolder,writeChangesToFile)
        %
        % rootFolder is string path to search within for Config classes
        % writeChangesToFile default false, set true to make changes.
        %    Otherwise just looks for possible improvements and reports
        %
        % See also: UpgradeFolder
        if (nargin ==0)
            return;
        end
        if false == exist('writeChangesToFile','var')
            writeChangesToFile = false;
        end
        BtcUpgradeObservers.UpgradeFolder(rootFolder,writeChangesToFile);
    
    end
end % member methods
methods (Static)
    %--------------------------------------------------------------------------
    function UpgradeFolder(rootFolder,writeChangesToFile)
        % Performs Observer upgrade on Config classes found in folder
        if false == exist('writeChangesToFile','var')
            writeChangesToFile = false;
        end
        
        % Search for potential BTC files
        filesystemSearchExpr = sprintf('%s/**/*Config.m',rootFolder);
        foundFiles = dir(filesystemSearchExpr);
        
        % Loop through files and discover what they contain, attempting to upgrade
        % as appears appropriate
        notClassOnPathList = string.empty;
        notBtcClassList = string.empty;
        
        for i=1:numel(foundFiles)
            aDirEntry = foundFiles(i);
            aFilename = fullfile(aDirEntry.folder,aDirEntry.name);
            className = string(extractBefore(aDirEntry.name,'.'));
        
            % Check if the file even represents a class on the path
            isClass = exist(className,'class');
            if (isClass)
                baseClasses = superclasses(className) ;
                if (ismember('BtcBaseClass',baseClasses))
        %             fprintf('Class %s is BTC\n',className);
                    BtcUpgradeObservers.UpgradeBtcClass(className,writeChangesToFile);
                else
                    notBtcClassList(end+1) = aFilename; %#ok<AGROW> 
                end
            else
                notClassOnPathList(end+1) = aFilename; %#ok<AGROW> 
            end
        end
    end
    %--------------------------------------------------------------------------
    function UpgradeBtcClass(className,writeChangesToFile)
        % Given a className and matching filename, attempt to upgrade
        className = string(className);
        if (false == exist('writeChangesToFile','var'))
            writeChangesToFile = false;
        end
        aFileName = which(className);
    
        % Get the metaclass
        theClass = meta.class.fromName(className);
    
        % Read in the text also
        theCode = fileread(aFileName);
        newCode = theCode;
    
        numberOfChanges = 0;
        fprintf('Upgrading %s ...',aFileName);
    
        % Iterate through properties.
        for i=1:numel(theClass.PropertyList)
            aProp = theClass.PropertyList(i);
            propName = aProp.Name;
            [canGet,canSet,isUserBtcClass] = BtcBaseClass.GetPropertyStats(aProp);
    
            % only adjust properties defined directly in this class file
            if (aProp.DefiningClass.Name ~= className)
                %fprintf('Skip property %s, inherited\n',propName);
                continue;
            end
            
            if (isUserBtcClass)
                if (aProp.SetAccess ~= "immutable")
                    %fprintf('%s.%s is component %s should be immutable\n',className,propName,requiredClassObj.Name);
                end
            elseif (canGet && canSet)
                if (false == aProp.GetObservable)
                    % Found a property that should be upgraded to GetObservable
                    updatedCode = BtcUpgradeObservers.UpdatePropertyBlockAttributes(newCode,propName);
                    if (false == isempty(updatedCode))
                        % Code change made! Start with this code for next prop
                        newCode = updatedCode;
                        numberOfChanges = numberOfChanges + 1;
                    else
                        % No changes (typical)
                    end
                end
            end
        end
    
        if (numberOfChanges)
            if (writeChangesToFile)
                % Update the file
                fid = CreateTextFile(aFileName);
                fwrite(fid,newCode);
                fclose(fid);
                fprintf('%d changes made\n',numberOfChanges);
            else
                fprintf('changes could be made (%d)\n',numberOfChanges);
            end
        else
            fprintf(' .\n');
        end
    end

    %--------------------------------------------------------------------------
    function newCode = UpdatePropertyBlockAttributes(theCode,propertyName)
    % given existing code and the property whose block should update, adds the
    % indicated property attributes. Returns empty if attributes are already
    % present
    
        newCode = [];
        xPropBlock = '(?<=^\s*)properties';
        xAttribs = '(?<ATTR>\s*\([^\)]*\))?';
        xPreText = '(?<PRET>.*?)';
        xProperty= ['(?<PROP>^\s*' propertyName '\>.*?)?'];
        xTheEnd  = '(?<END>\<end\>)';
        superExp = [xPropBlock xAttribs xPreText xProperty xTheEnd];
        matches = regexp(theCode,superExp,'lineanchors','names');
    
        aMatch = [];
        for i=1:numel(matches)
            if (false == isempty(matches(i).PROP))
                aMatch = matches(i);
                break;
            end
        end
        % Ensure one and only one match
        if (isempty(aMatch))
            error('Could not find %s',propertyName);
        end
        if (numel(aMatch) > 1)
            error('Somehow found multiple matches to %s',propertyName);
        end
    
        attributes = aMatch.ATTR;
    
        % Bail out if attribute is already present
        if (contains(attributes,'GetObservable'))
            % already present! Bail out now, returning empty code.
            return;
        end
    
        if (false == isempty(attributes))
            % The property block already has attributes. Just insert a
            % few more immediately before the closing parenthesis
            newAttributes = insertBefore(attributes,numel(attributes),',GetObservable,SetObservable');
        else
            newAttributes = ' (GetObservable,SetObservable)';
        end
    
        replaceExp = sprintf('(%s)(%s%s%s)', ...
            BtcUpgradeObservers.EscapeRegex(aMatch.ATTR), ...
            BtcUpgradeObservers.EscapeRegex(aMatch.PRET), ...
            BtcUpgradeObservers.EscapeRegex(aMatch.PROP), ...
            BtcUpgradeObservers.EscapeRegex(aMatch.END)); 
    
        replacement = [newAttributes '$2'];
        newCode = regexprep(theCode,replaceExp,replacement,'lineanchors');
        if (isequal(theCode,newCode))
            fprintf('[replace failed]')
        end
    end
    
    function escapedStr = EscapeRegex(inStr)
    % Escape regex-relevant symbols to be interpreted literally
        escapedStr = regexprep(inStr,'([\[\]\(\)\{\}\*\+\?\|\^\$\.\\])','\\$0');
    
    end

end % static methods
end % classdef

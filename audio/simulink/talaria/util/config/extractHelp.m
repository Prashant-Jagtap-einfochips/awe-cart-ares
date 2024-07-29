function [theHelp] = extractHelp(inFileName)
%extractHelp Returns "help file" text file as a string array
%   Works for both functions and classdefs. Intended for use with pcode-
%   just write the returned string to replace the original *.m file
%   afterwards.

%   Copyright 2018-2019 Bose Corporation

theHelp = "";

[~, name, ~] = fileparts(inFileName);

try
    % Try to extract a classdef "meta.method"
    mco = eval(['?' name]);
catch ME
    % Unknown cause on a "fresh" instance of MATLAB, the above line
    % RELIABLY fails with 'MATLAB:subscripting:classHasNoPropertyOrMethod'
    % while processing FeaturePathUtil, but because FeaturePathMask is
    % somehow messed up in a cache somewhere.
    %   'Invalid default value for property 'MaskName' in class 'FeaturePathUtil':
    %   The class FeaturePathMask has no Constant property or Static method named 'MaskType'.'
    % Trying to instantiate a FeaturePathMask, we see more symptoms of fail
    %   FeaturePathMask
    %   Error using FeaturePathMask
    %   The class 'FeaturePathMask' contains a parse error,
    %   cannot be found on MATLAB's search path, or is shadowed
    %   by another file with the same name.
    % Our Workaround: find the class that "has no" and clear it
    expr = 'The class (\w+) has no';
    hasNoClassTokens = regexp(ME.message,expr,'tokens');
    hasNoClass = hasNoClassTokens{1}{1};
    warning('Clearing [%s] as workaround', hasNoClass);
    eval(['clear ' hasNoClass]);
    % (try again)
    mco = eval(['?' name]);
end

if (isempty(mco))
    % Not a classs: just do function help
    emptyHelp = [name ' is a function.'];
    aHelp = help(name);
    lines = string(split(aHelp,newline));
    if (lines(1) ~= emptyHelp && ...
        lines(1) ~= "" && ...
        lines{1}(1) ~= '-')
        theHelp = sprintf("function %s",name);
        lines(end) = [];
        commentHelp = "%" + lines;
        theHelp = vertcat(theHelp,commentHelp);
    end
else
    % We've got a class
    emptyHelp = [name ' is a class.'];
    lines = split(help(name),newline);
    theHelp = sprintf("classdef %s",name);
    gotHelp = false;
    % Add class-level help
    if (false == isempty(lines{1}) && ...
        false == strcmp(emptyHelp,lines{1}))
        gotHelp = true;
        commentHelp = "%" + string(lines);
        theHelp = vertcat(theHelp,commentHelp);
    end
    % Handle enumeration classes and normal classes differently
    if (mco.Enumeration)
        enumerationList = strjoin({mco.EnumerationMemberList.Name},',');
        commentHelp = "% EnumerationMemberList: " + enumerationList;
        theHelp = vertcat(theHelp,commentHelp);
    else
        % Loop through methods for help
        theHelp = vertcat(theHelp,"methods");
        numMethods = numel(mco.MethodList);
        for i = 1:numMethods
            aMethod = mco.MethodList(i);
            methodAccess = aMethod.Access;
            if (false == ischar(methodAccess) || ...
                false == contains(methodAccess,'public'))
                % Skip methods that are not simple public access
                continue;
            end
            if (false == strcmp(aMethod.DefiningClass.Name,name))
                % Skip inherited methods also
                continue;
            end
            funcName = [name '.' aMethod.Name];
            emptyHelp = sprintf("%s is a function.",funcName);
            aHelp = string(split(help(funcName),newline));
            if (emptyHelp ~= aHelp(1) && "empty" ~= aMethod.Name)
                gotHelp = true;
                lines = string(split(aHelp,newline));
                % remove the last line which is always empty
                lines(end) = [];
                commentHelp = "   %" + lines;
                theHelp = vertcat(theHelp,"function "+aMethod.Name, ...
                    commentHelp,"end");
            end
        end
        theHelp = vertcat(theHelp,"end");
    end
    if (gotHelp)
        theHelp = vertcat(theHelp,"end");
    else
        theHelp = "";
    end
end


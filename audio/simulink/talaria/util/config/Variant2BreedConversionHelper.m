function Variant2BreedConversionHelper(rootFileName)
% Attempts to refactor from Variant BTC patterns to Breed BTC pattern

fprintf('Searching for files that start with [%s]...\n',rootFileName);
dirExp = sprintf('**/%s*.m', rootFileName);
fileSpecs = dir(dirExp);
numFiles = numel(fileSpecs);
fprintf('Found %d files:\n   %s\n', ...
    numFiles, ...
    strjoin({fileSpecs.name},[newline '   ']));

for i=1:numFiles
    aFile = fileSpecs(i);
    fullPath = fullfile(aFile.folder,aFile.name);
    fprintf('\n* %s: ...\n',fullPath);
    fileText = string(fileread(fullPath));
    fileLines = fileText.splitlines;
    fprintf('%s\n',fileLines(1));
    if (contains(fileLines(1),'BtcBaseClassWithVariants'))
        fprintf(' ... main BTC file: switch to BtcBaseClassWithBreeds\n');
        DoMainFile(fullPath,fileText);
    elseif (contains(fileLines(1),'BtcVariantEnum'))
        fprintf('... enum file: to be deleted but also port content to main file\n');
        DoEnumFile(fullPath,fileText);
    elseif (contains(fileLines(1),'BtcVariant'))
        fprintf('... breed file: switch to BtcBreedBaseClass, etc.\n');
        DoBreedFile(fullPath,fileText);
    else
        fprintf("... \n")
    end
end

end

function WriteNewFile(fullPath,fileText)
    modifiedFile = fullPath;
    hFile = CreateTextFile(modifiedFile);
    fprintf(hFile,'%s',fileText);
    fclose(hFile);
    fprintf('Updated %s\n',modifiedFile);
end

function DoMainFile(fullPath,fileText)
    origText = fileText;
    % Remove base class constructor call with argument(s)
    regx = '^\s*(this)@BtcBaseClassWithVariants\([^\n]*';
    fileText = regexprep(fileText,regx,'','lineanchors');

    % Replace superclass
    fileText = strrep(fileText,'BtcBaseClassWithVariants','BtcBaseClassWithBreeds');

    WriteNewFile(fullPath,fileText);
end

function DoBreedFile(fullPath,fileText)
    origText = fileText;
    % Remove base class constructor call with argument(s)
    regx = '^\s*(this)@BtcBaseClassWithVariants\([^\n]*';
    fileText = regexprep(fileText,regx,'','lineanchors');

    % Replace superclass
    fileText = strrep(fileText,'VariantValidate','BreedValidate');
    fileText = strrep(fileText,'VariantDoMaskInit','BreedDoMaskInit');
    fileText = strrep(fileText,'"Variant"','"Breed"');
    fileText = strrep(fileText,'BtcVariant','BtcBreedBaseClass');

    WriteNewFile(fullPath,fileText);
end

function DoEnumFile(fullPath,fileText)

    regx = "\s*(?<NAME>\w*)\s*\((?<NUMBER>\d*),\s*'(?<CLASS>\w*)'\s*\)\s*(?<COMMENTS>%[^\n\r]*)?";
    matches = regexp(fileText,regx,"names");
    % properties (Constant,Hidden)
    %     BreedClassMap = dictionary( ...
    %         AsdHwEnum.Generic, "PoolIirWithResetGenericVariantConfig", ...
    %         AsdHwEnum.SharcPlusCore, "PoolIirWithResetSharcPlusCoreVariantConfig", ...
    %         AsdHwEnum.SharcPlusAccelGul, "PoolIirWithResetSharcPlusAccelGulVariantConfig", ...
    %         ... "SharcPlusAccelGxp", "PoolIirWithResetSharcPlusAccelGxpVariantConfig", ...
    %         AsdHwEnum.Hexagon,"PoolIirWithResetHexagonVariantConfig", ...
    %         AsdHwEnum.ArmNeonV7V8, "PoolIirWithResetArmNeonV7V8VariantConfig")
    % end

    code = "";
    code(end+1) = "properties (Constant,Hidden)";
    code(end+1) = "    BreedClassMap = dictionary( ...";
    for i=1:numel(matches)
        if (i < numel(matches))
            lineEnding = sprintf(", ... %s",matches(i).COMMENTS);
        else
            lineEnding = sprintf("); %s",matches(i).COMMENTS);
        end
        code(end+1) = sprintf('        AsdHwEnum.%s, "%s"%s', ...
            matches(i).NAME,matches(i).CLASS,lineEnding);
    end
    code(end+1) = "end";

    fprintf('-------------------------------------------------\n');
    fprintf('%s',code.join(newline));
    fprintf('\n-------------------------------------------------\n');

    disp(matches);

end
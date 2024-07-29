classdef GenerateBtcMfileFromXml < handle
% GenerateBtcMfileFromXml Accepts a BTC XML file created by ExtractModelBtc
% and translates its contents into an M file with the settings described.
% Usage:
%   GenerateBtcMfileFromXml(modelNameOrFilename,[verbosity]);
%
% modelNameOrFilename : If modelNameOrFilename has an .xml extension, then
%     it's a filename, otherwise the value is assumed to be a model name
%     and the conventional "_BtcDefs.xml" suffix will be added and used as
%     a filename....
% verbosity : The verbosity indicates whether to spell out ALL settable
%     parameters, or only mention those parameters that currently do not
%     match the default.
%
% Outputs a *.m file in the same folder as the loaded XML file. The name of
% the M file matches the XML file just with the extension ".m"

%   Copyright 2022 Bose Corporation

properties (Constant,Access=protected)

end
properties (Access=protected)
    FileNameSuffix = 'BtcDefs'
    FileDescription = 'Build-time configuration definition script'
    XmlDoc
    Mfile
    Verbosity
    AwaitingHeaderText
end

methods
%--------------------------------------------------------------------------
function this = GenerateBtcMfileFromXml(modelNameOrFilename,verbosity)
    if (nargin < 2)
        this.Verbosity = 0;
    else
        this.Verbosity = verbosity;
    end
    % Acquire the XML filename
    if nargin < 1
        modelNameOrFilename = bdroot;
    end
    if (endsWith(modelNameOrFilename,'.xml'))
        fileName = modelNameOrFilename;
    else
        fileName = sprintf('%s_%s.xml', ...
            modelNameOrFilename, this.FileNameSuffix);
    end
    % Acquire the XML file path
    % ... First search to see if the filename by itself gets a hit
    inputXmlFilePath = which(fileName);
    if (isempty(inputXmlFilePath))
        % Not found on path - try in the codeGenDir
        codeGenDir = Simulink.fileGenControl('get','CodeGenFolder');        
        inputXmlFilePath = fullfile(codeGenDir,fileName);
        if (false == isfile(inputXmlFilePath))
            error('Could not find file [%s]', inputXmlFilePath);
        end
    end

    % Load it up!
    fprintf(' Reading file [%s]\n',ModelDataXmlWriter.GetEditMeLink(inputXmlFilePath));
    this.XmlDoc = xmlread(inputXmlFilePath);

    % Open the M file also
    MfilePath = strrep(inputXmlFilePath,'.xml','.m');
    fprintf(' Writing file [%s]\n',ModelDataXmlWriter.GetEditMeLink(MfilePath));
    this.Mfile = CreateTextFile(MfilePath);
    [~,MfileName] = fileparts(MfilePath);
    thisMfileName = mfilename; % built-in function to return this file
    inputXmlFileInformation = dir(inputXmlFilePath);
    this.WriteToM('%% File: %s.m \n',MfileName);
    this.WriteToM('%% Auto-generated: %s\n',char(datetime));
    this.WriteToM('%% Verbosity level: %d\n',this.Verbosity);
    this.WriteToM('%% Generator script: %s\n',thisMfileName);
    this.WriteToM('%% Input file path: %s\n',inputXmlFilePath);
    this.WriteToM('%% Input file timestamp: %s\n',inputXmlFileInformation.date);

    % Dump whatever information is contained in the source XML root node
    elemRoot = this.XmlDoc.getDocumentElement;
    attribs = elemRoot.getAttributes();
    this.AwaitingHeaderText = sprintf('%% |---- from input file:\n');
    for i=1:attribs.getLength
        this.WriteToM('%% | %s\n',char(attribs.item(i-1)));
    end
    this.AwaitingHeaderText = [];

    % Start dumping
    this.DumpModel();
    this.DumpBlocks();

    % Close the M file
    fclose(this.Mfile);
    this.Mfile = [];
end
function delete(this)
    % This is the destructor
    if (false == isempty(this.Mfile))
        fclose(this.Mfile);
    end
end
end % methods

methods (Access=protected)
%--------------------------------------------------------------------------
function DumpModel(this)
    elemsModel = this.XmlDoc.getElementsByTagName('Model');
    for i = 0:elemsModel.getLength-1
        % Grab the model node
        anItem = elemsModel.item(i);
        variableName = char(anItem.getAttribute('id'));
        classdefName = char(anItem.getAttribute('classdef'));

        % Setup some AwaitingHeaderText that will only print if something
        % worth printing comes up.
        this.AwaitingHeaderText = sprintf('\n%% ----- Model[%s] type[%s]\n',variableName,classdefName);

        this.DumpComponent(anItem);
    end
end
%--------------------------------------------------------------------------
function DumpBlocks(this)
% DumpBlocks dumps variables to M of all Block Elements of provided node

    elemsBlock = this.XmlDoc.getElementsByTagName('Block');
    for i = 0:elemsBlock.getLength-1
        % Grab the block node
        anItem = elemsBlock.item(i);
        blockPath = char(anItem.getAttribute('id'));
        libref = char(anItem.getAttribute('libref'));

        % Setup some AwaitingHeaderText that will only print if something
        % worth printing comes up.
        this.AwaitingHeaderText = sprintf('\n%% ----- Block[%s] type[%s]\n',blockPath,libref);

        % Dump each configuration element for this block
        aChild = anItem.getFirstChild();
        while (false == isempty(aChild))
            nodeName = char(aChild.getNodeName());
            switch (nodeName)
                case 'Parameter'
                    this.DumpParameter(aChild);
                case 'Component'
                    this.DumpComponent(aChild);
            end
            % Next please
            aChild = aChild.getNextSibling();
        end
    end
end
%--------------------------------------------------------------------------
function DumpParameter(this,theNode)
% DumpParameter dumps block-mask parameter to M based on verbosity

%<Parameter current_value="8" default="1" id="numMusicIn" internal_name="ColStartIndex" internal_prompt="Starting column index:" />

    btcExpression = char(theNode.getAttribute('id'));
    currentValue = char(theNode.getAttribute('current_value'));
    defaultValue = char(theNode.getAttribute('default'));
    paramName = char(theNode.getAttribute('internal_name'));

    % 3rd argument indicates that the default is not yet evaluated:
    [isDefault, defaultComment] = this.GetIsDefault(currentValue,defaultValue,true);

    if (this.ExpressionIsaVariable(btcExpression))
        % Starts with a letter and contains only letters and numbers. Good.
        % This is a variable!
        if ( (false == isDefault) || ...
             (isDefault && this.ShouldDumpCurrentlyDefault()) )
            this.WriteToM('%s = %s; %s\n', ...
                btcExpression, currentValue, defaultComment);
        end
    else
        % This looks like an expression (so not a BTC variable)
        if (this.ShouldDumpNonBtc)
            this.WriteToM('%%Parameter [%s] is an expression [%s]\n', ...
                paramName, btcExpression);
        end
    end
end

%--------------------------------------------------------------------------
function DumpComponent(this,theNode,objectPath)
% DumpComponent dumps a classdef object to M based on verbosity

%<Component classdef="ExampleVolumeConfig" id="V1">

    btcExpression = char(theNode.getAttribute('id'));
    theClass = char(theNode.getAttribute('classdef'));

    if (nargin<3)
        % There is no previous object... so allocate this one now
        if (false == this.ExpressionIsaVariable(btcExpression))
            this.WriteToM('%% Alert: %s is not a regular variable\n',btcExpression);
        end
        objectPath = btcExpression;
        this.WriteToM('%s = %s();\n', objectPath, theClass);
    else
        % An object already exists, just navigate into it
        objectPath = [objectPath '.' btcExpression];
    end

    % Loop through all the properties
    aChild = theNode.getFirstChild();
    while (false == isempty(aChild))
        nodeName = char(aChild.getNodeName());
        switch (nodeName)
            case 'Property'
                this.DumpProperty(aChild,objectPath);
            case 'Component'
                this.DumpComponent(aChild,objectPath);
        end
        % Next please
        aChild = aChild.getNextSibling();
    end
end
%--------------------------------------------------------------------------
function DumpProperty(this,theNode,objectPath)
% DumpProperty dumps a classdef property to M based on verbosity

%<Property current_value="100" default="100" id="DefaultRampMilliseconds" />
    propName = char(theNode.getAttribute('id'));
    defaultValue = char(theNode.getAttribute('default'));
    currentValue = char(theNode.getAttribute('current_value'));

    % Does it look like the current value matches the default?
    % For classdef properties all we have access to is the evaluated
    % property because the string evaluation happened back when the
    % assignment was made. In this case, those evaluated properties have
    % been converted to string anyway - so let's just compare strings
    [isDefault, defaultComment] = this.GetIsDefault(currentValue,defaultValue);
    if ( (false == isDefault) || ...
         (isDefault && this.ShouldDumpCurrentlyDefault()) )
        this.WriteToM('%s.%s = %s; %s\n', ...
            objectPath,propName,currentValue,defaultComment);
    end
end

%--------------------------------------------------------------------------
function doit = ShouldDumpCurrentlyDefault(this)
    doit = this.Verbosity >= 1;
end
%--------------------------------------------------------------------------
function doit = ShouldDumpNonBtc(this)
    doit = this.Verbosity >= 1;
end
%--------------------------------------------------------------------------
function itIs = ExpressionIsaVariable(~,btcExpression)
% ExpressionIsaVariable Determines if provided expression is a variable we
% can set vs. some design-time expression that requires evaluation

    % Trim off whitespace
    analysisExp = strtrim(btcExpression);
    % Regex for start-with-letter, then only word-characters or period
    matchingExpression = '^[a-zA-z][\w.]*$';
    itIs = (false == isempty(regexp(analysisExp,matchingExpression,'once')));
end
%--------------------------------------------------------------------------
function [isDefault, defaultComment] = GetIsDefault(~,currentValue,defaultValue,notEvalYet)
% GetIsDefault compares the string values for current and default and
% returns the boolean result as well as a comment string to add to the M
% file with the default value in it

    if (nargin < 4)
        % Assume the default value has already been evaluated
        notEvalYet = false;
    end

    % Does it look like the current value is the default value? A challenge
    % here is that current values have always been evaluated, but the
    % defaultValue is evalated for classdef properties (happened with the
    % property assignment was made) but NOT evaluated for mask parameters
    % (stored as strings a not evaluated until block "update/init").
    % The third argument tells us whether the defaultValue is evaluated or
    % not - and if not, try it now and convert to string to compare the
    % best we can.
    if (defaultValue == "N/A: 1x1 function_handle")
        % Calculated defaults expect the current value to be empty in order
        % for the default to kick in:
        defaultForCompare = char.empty;
    else
        % Otherwise, use the default value to compare
        defaultForCompare = defaultValue;
    end
    if (notEvalYet)
        try
            evaluatedDefault = eval(paramDefaultExpr);
            evaluatedDefaultStr = ModelDataXmlWriter.ValueToStr(evaluatedDefault);
            defaultForCompare = evaluatedDefaultStr;
        catch
            % whatever happened, it means we cannot see the evaluated
            % default, so just use what we've got
        end
    end
    isDefault = strcmp(currentValue,defaultForCompare);
    if (isDefault)
        defaultComment = ''; % it's already the default, so add nothing
    else
        defaultComment = sprintf('%%Default:%s',defaultValue);
    end
end
%--------------------------------------------------------------------------
function WriteToM(this,varargin)
    % WriteToM writes to the M file, but also tracks whether if this is the
    % first item worth printing for a block and dumps a comment about the
    % block too.
    if (false == isempty(this.AwaitingHeaderText))
        fprintf(this.Mfile,'%s',this.AwaitingHeaderText);
        this.AwaitingHeaderText = [];
    end
    fprintf(this.Mfile,varargin{:});
end


end %methods
end %classdef

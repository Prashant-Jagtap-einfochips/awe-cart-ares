classdef ModelDataXmlWriter < handle
% ModelDataXmlWriter Abstract base class to exports XML file from model

properties (Access=protected)
    ElemRoot
    NodeRoot
    ModelName char
end
properties (GetAccess=public)
    XmlFileName char
end
properties (Abstract, Access=protected)
    FileNameSuffix char
    FileDescription char
end

methods (Static)
    function TidyInPlace(filename)
        cfgPath = fullfile(fileparts(mfilename('fullpath')), 'xml-tidy.cfg');
        if (isfile(filename))
            mlreportgen.utils.tidy(filename,'OutputFile',filename,'ConfigFile',cfgPath);
        else
            error([filename ' not found']);
        end
    end
    %--------------------------------------------------------------------------
    function theCharStr = ValueToStr(theValue)
        % ValueToString Formats the provided value as a string appropriate for
        % use as an XML attribute
        if isenum(theValue)
            % Simplify enums to remove the enumerated class name
            if (isempty(theValue))
                theValue = [];
            else
                theValue = char(theValue);
            end
        end
        if (isnumeric(theValue) || ischar(theValue))
            % Previous implementations flattened the vector - but this
            % action will not take a round-trip as the actual dimensions
            % are an important part of the value. Switch to mostly using
            % the MaskUtil routine instead
            theCharStr = MaskUtil.ValueToStr(theValue);
        elseif (isstring(theValue))
            theCharStr = MaskUtil.ValueToStr(theValue);
        elseif (isa(theValue,'function_handle'))
            theCharStr = sprintf('CALC:%s',func2str(theValue));
        elseif islogical(theValue)
            theCharStr = char(string(theValue));
        else
            theCharStr = sprintf("N/A: %s %s", ...
                strjoin(string(size(theValue)),'x'), class(theValue));
        end
    end
end

methods
%--------------------------------------------------------------------------
function this = ModelDataXmlWriter(modelName,ensureModelUpdated)
    % ModelDataXmlWriter Creates XML doc, loads model & gets it ready
    % Create the XML document
    this.NodeRoot = com.mathworks.xml.XMLUtils.createDocument('root');
    this.ElemRoot = this.NodeRoot.getDocumentElement;
    this.ElemRoot.setAttribute('version','1.0');

    % Load the model and make sure it's ready to look at
    load_system(modelName);
    modelName = getfullname(bdroot(modelName));
    
    if (nargin < 2)
        ensureModelUpdated = true;
    end
    if (ensureModelUpdated)
        % Ensure that the model is updated e.g. all evaluated mask parameters
        modelStatus = get_param(modelName,'SimulationStatus');
        if (strcmp('stopped',modelStatus))
            disp('Updating diagram to ensure accuracy');
            set_param(modelName,'SimulationCommand','update');
        end
    end
    this.ModelName = modelName;
end

%--------------------------------------------------------------------------
function anElement = AppendElement(this,parentNode,elementName,idString,attributeNameValuePairs)
    % Creates an element and appends to parent node
    if (nargin < 5)
        attributeNameValuePairs = {};
    end
    anElement = this.CreateNewElement(elementName,idString,attributeNameValuePairs);
    parentNode.appendChild(anElement);
end
%--------------------------------------------------------------------------
function anElement = ReplaceElement(this,currentNode,elementName,idString,attributeNameValuePairs)
    % Creates an element and replaces an existing one
    if (nargin < 5)
        attributeNameValuePairs = {};
    end
    anElement = this.CreateNewElement(elementName,idString,attributeNameValuePairs);
    nodeParent = currentNode.getParentNode;
    nodeParent.replaceChild(anElement,currentNode);
end
%--------------------------------------------------------------------------
function anElement = CreateNewElement(this,elementName,idString, ...
        attributeNameValuePairs)
    % CreateElement Creates new XML element
    % Usage:
    %  newElement = this.CreateElement(parentNode, elementName, idString,
    %      attributeNameValuePairs);
    anElement = this.NodeRoot.createElement(elementName);
    anElement.setAttribute('id',idString);
    anElement.setIdAttribute('id',true);
    if (nargin >= 4)
        numAttribs = numel(attributeNameValuePairs) / 2;
        for i=1:numAttribs
            idx = ((i-1) * 2) + 1;
            anElement.setAttribute(attributeNameValuePairs{idx}, ...
                attributeNameValuePairs{idx+1});
        end
    end
end

%--------------------------------------------------------------------------
function newElem = AppendTextNode(this,parentNode,elementName,nodeText)
    % AppendTextNode
    newElem = this.NodeRoot.createElement(elementName);
    if (isnumeric(nodeText))
        % ooops - the text is still a number. fix that:
        nodeText = this.ValueToStr(nodeText);
    end
    newElem.appendChild(this.NodeRoot.createTextNode(nodeText));
    parentNode.appendChild(newElem);
end

%--------------------------------------------------------------------------
function WriteXml(this,filename,quiet)
    % WriteXml Writes XML document to file. Normally filename is automatic
    if (nargin >=2 && false == isempty(filename))
        this.XmlFileName = filename;
    else
        % Automatically dump into code gen folder
        codeGenDir = Simulink.fileGenControl('get','CodeGenFolder');
        this.XmlFileName = fullfile(codeGenDir,sprintf('%s_%s.xml', ...
            this.ModelName, this.FileNameSuffix));
    end
    if (exist("quiet","var") && quiet)
        % Do not print notification of the save
    else
        fprintf(' Writing to file "%s"\n', this.GetEditMeLink(this.XmlFileName));
    end
    xmlwrite(this.XmlFileName,this.NodeRoot);
    ModelDataXmlWriter.TidyInPlace(this.XmlFileName);
end

end %methods
methods (Static)
%--------------------------------------------------------------------------
function linkText = GetEditMeLink(filePath)
    % GetEditMeLink returns hypertext anchor link to edit file
    % linkText = this.GetEditMeLink(filePath);
    linkText = sprintf('<a href="matlab:edit %s">%s</a>',filePath,filePath);
end

end %classdef

end

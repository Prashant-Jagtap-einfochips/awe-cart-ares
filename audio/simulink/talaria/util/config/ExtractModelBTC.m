classdef ExtractModelBTC < ModelDataXmlWriter
% ExtractModelBTC Attempts to extract model build-time configuration (BTC)
%    This method finds all blocks (without peering underneath masks) to
%    discover their block parameters. It would very much like to know
%    whether each parameter is evaluated or not (since only evaluated
%    parameters can become BTC) but unfortunately for built-in blocks there
%    does not appear to be a way to determine the "evaluate" property -- so
%    built-in blocks from MathWorks are generally ignored.
% Usage:
%   ExtractModelBTC(modelName,[filename],[bootStrap]);
%
% In (bootStrap==true) mode, the model is not updated and current values
% are either gathered from library references or left empty.

%   Copyright 2021 Bose Corporation

properties (Access=protected)
    FileNameSuffix = 'BtcDefs'
    FileDescription = 'Build-time configuration definitions'
    IsBootStrap
end

methods
%--------------------------------------------------------------------------
function this = ExtractModelBTC(modelName,filename,bootStrap,quiet)
    arguments
        modelName char = []
        filename char = []
        bootStrap logical = false
        quiet logical = false
    end
    % Construct the baseclass writer
    this@ModelDataXmlWriter(modelName,(false == bootStrap));
    this.IsBootStrap = bootStrap;

    % Capture design information from the model
    this.ElemRoot.setAttribute('model',this.ModelName);
    this.ElemRoot.setAttribute('model_version',get_param(this.ModelName,'ModelVersion'));
    this.ElemRoot.setAttribute('model_created_date',get_param(this.ModelName,'Created'));
    this.ElemRoot.setAttribute('last_modified_by',get_param(this.ModelName,'LastModifiedBy'));
    this.ElemRoot.setAttribute('last_modified_date',get_param(this.ModelName,'LastModifiedDate'));
    this.ElemRoot.setAttribute('rtw_modified_timestamp', ...
        num2str(get_param(this.ModelName,'RTWModifiedTimeStamp')));

    % Do not look under masks that have parameters - graphical masks only
    F = Simulink.FindOptions("LookUnderMasks","Graphical");
    F.Variants =  'ActiveVariants'; % TODO [TALARIA-523]: Workaround
    blocks = Simulink.findBlocks(this.ModelName,F);

    numBlocks = numel(blocks);

    % Handle model-level BTC
    modelBtcVariableName = [];
    if (BTConfig.HasModelLevelConfig(this.ModelName))
        % Add top-level model element
        [modelConfig,modelBtcVariableName] = BTConfig.GetModelSettings(this.ModelName);
        this.DumpClassdef(modelBtcVariableName,this.ElemRoot,modelConfig,'Model');
    end

    % Get all blocks, but don't look under masks. A mask presents ALL
    % build-time configuration for the contents within.
    % For built-in blocks (no user mask) we have two massive problems:
    %  1 - Cannot determine if a dialog parameter is evaluated!
    %  2 - Cannot acquire the current evaluated value of the parameter!

    for blockIdx=1:numBlocks
        aBlock = blocks(blockIdx);
        theMask = Simulink.Mask.get(aBlock);
        if (false == isempty(theMask))
            % Masked. We can do this
            this.DumpMaskedBlock(theMask,modelBtcVariableName);
        else
            % Not masked. Just a subsystem or a built-in block that we have
            % little chance of extracting BTC from (don't know which parameters
            % are evaluted)
            % Just skip it!
            continue;
        end
    end

    if (nargin < 2)
        filename = [];
    end
    this.WriteXml(filename,quiet);
end

%--------------------------------------------------------------------------
function DumpMaskedBlock(this,theMask,modelBtcVariableName)
    % Masked blocks dump to XML right off the ElemRoot node.

    % To figure out if a BTC class is present in the mask, search the mask
    % initialization code for classdef assertions from calls to BTConfig.
    % Use regex token groups to parse the init code as follows:
    %   Group 1:Start: start-of-line or semicolon. Doesn't matter
    %   Group 2:Output: Mask workspace variable. Doesn't matter
    %   Group 3:Method: Method - BTConfig or MaskUtil. Doesn't matter
    %   Group 4:Source: Classname OR block-path with "Config" param prompt
    %  *Group 5:Name: Mask parameter used to accept classdef
    % Although the "Source" token can be used to discover the classdef
    % associated with the classdef-asserted parameter, the assert has
    % already done its work and the current value already knows the class.
    initCode = theMask.Initialization;
    searchExp = [ ...
        '(?<Start>^|;)\s*' ...
        '(?<Output>\w*)\s*=\s*' ...
        '(?<Method>BTConfig\.Assert|MaskUtil.ConfigAssert)\s*\(\s*' ...
        '(?<Source>\w*|''\w*''|"\w*")\s*,\s*' ...
        '(?<Name>\w*)\s*\)'];
    % Note that the 'names' argument of regexp causes a struct output:
    btcConfigVars = regexp(initCode,searchExp,'lineanchors','names');
    
    % Get mask parameters and only dump them if they are BTC. Parameters
    % must be evaluated, visible, and contain a variable name as the
    % current value.
    % Also, if the mask parameter name is an asserted classdef (Group 5)
    %   ... dump as a component instead of parameter
    % Otherwise, do parameter Dump
    %   If block is active link, get library version for default value
    %   Otherwise, no default value available: only current value as name
    %   Parameter's mask name is a Description of what the variable does
    
    theBlock = theMask.getOwner.getFullName;
    libRef = get_param(theBlock,'ReferenceBlock');
    if (isempty(libRef))
        % Not a library link. No default values available for parameters
        libMask = [];
    else
        % Ensure library is loaded
        libraryName = extractBefore(libRef,'/');
        load_system(libraryName);
        libMask = Simulink.Mask.get(libRef);
    end
    
    elemBlock = [];
    numParameters = numel(theMask.Parameters);
    for maskParamIdx=1:numParameters
        aParam = theMask.Parameters(maskParamIdx);
        if "on" ~= aParam.Evaluate || "edit" ~= aParam.Type
            % Not an evaluated edit. Cannot be build-time configuration
            continue;
        end
        if ("off" == aParam.Visible)
            % Not visible. Cannot be build-time configuration.
            continue;
        end
        if (this.IsNotWritableVariableName(aParam.Value))
            % Not a variable name.
            continue;
        end
        % If the value is pulling something from model-config, skip it
        if (false == isempty(modelBtcVariableName) && ...
                startsWith(aParam.Value,modelBtcVariableName))
            % This parameter is fed from model-level configuration which
            % was already dumped. Skip it
            continue;
        end
        % Found some build-time configuration. Get current value.
        if (this.IsBootStrap)
            % First attempt the regular BTC evaluation
            [currentValue,wasFound] = MaskUtil.GetEvaluatedParameter(theBlock,aParam.Name);
            if (false == wasFound)
                % Library-link blocks should be more helpful in boot-strap
                % situations because they have a reference block that hopefully
                % does not reference external data, or variable names in its
                % dialog. In any case do the best we can:
                refBlock = get_param(theBlock,'ReferenceBlock');
                if (isempty(refBlock))
                    % uh oh... if this block references variables, we may have
                    % a difficult time. Proceed with caution.
                    refBlock = theBlock;
                end
                [currentValue,wasFound] = MaskUtil.GetEvaluatedParameter(refBlock,aParam.Name);
                if (false == wasFound || isempty(currentValue))
                    fprintf(2,'Block [%s] parameter [%s] value [%s] is empty or could not be resolved.\n', ...
                        refBlock,aParam.Name,get_param(refBlock,aParam.Name));
                    currentValue = [];
                end
            end
        else
            % Normal current-value extraction from an updated model:
            currentValue = MaskUtil.GetEvaluatedParameter(theBlock,aParam.Name);
        end
        % Ensure the block has an XML element:
        if isempty(elemBlock)
            blockName = GetSourceModelUrls(theBlock);
            elemBlock = this.AppendElement(this.ElemRoot,'Block',blockName,{'libref',libRef});
        end
        % Check if the parameter is asserted to be a BTC classdef instance
        matchingIdxs = {btcConfigVars.Name} == string(aParam.Name);
        if (isempty(matchingIdxs))
            % Just a regular parameter
            default= '';
            if (false == isempty(libMask))
                % Acquire default from the library mask
                default = libMask.Parameters(maskParamIdx).Value;
            end
            if (isempty(currentValue))
                currentValueStr = '';
            else
                currentValueStr = this.ValueToStr(currentValue);
            end
            this.AppendElement(elemBlock,'Parameter',aParam.Value, ...
                {'internal_name',aParam.Name, ...
                'internal_prompt',aParam.Prompt', ...
                'current_value',currentValueStr, ...
                 'default', default});
        else
            % This is a classdef parameter because it matches a BTC Assert
            % Since asserted classdef parameters already are the correct
            % class even in bootstrap modes, we can dump as classdef and
            % the current value already "knows" the correct class.
            this.DumpClassdef(aParam.Value,elemBlock,currentValue);
        end
    end
    
end

%--------------------------------------------------------------------------
function DumpClassdef(this,componentName,parentElem,obj,elementType)
    % Classdefs dump inside the provided parent XML element
    % Dump the classdef component as child of given node, then everything
    % else goes inside the classdef component.
    % Gather all public-write-access parameters
    % If parameter has validation class, then recurse into sub-component
    % If parameter has default, dump it also
    if (nargin < 5)
        elementType = 'Component';
    end
    className = class(obj);
    elemClass = this.AppendElement(parentElem,elementType,componentName, ...
        {'classdef',className});

    if (isempty(obj))
        % BTC component properties are never expected to be empty. The
        % constructor of the containing class should construct all the
        % component
        parentClassdefName = string(parentElem.getAttribute('classdef'));
        error("EXTRACTMODELBTC:EMPTYCOMPONENT","Encountered empty component property [%s] in class [%s]. Allocate " + ...
            "this property in the %s constructor e.g. this.%s = %s();.", ...
            componentName,parentClassdefName, ...
            MaskUtil.GetFileEditLink(parentClassdefName),componentName,className);
    end

    mco = meta.class.fromName(className);
    numProps = numel(mco.PropertyList);
    for propIdx = 1:numProps
        aProp = mco.PropertyList(propIdx);

        [canGet,canSet,isBtcClass,propClass] = BtcBaseClass.GetPropertyStats(aProp);

        if (false == canGet)
            % If cannot even get it, certainly not BTC
            continue;
        end
        if ((false == canSet) && (isBtcClass == false))
            % If you cannot set it and it's not a user BTC class, skip it!
            continue;
        end

        % Attempt to support non-BTC classes for export.
        isOtherComponent = false;
        if (false == isempty(propClass) && (false == isBtcClass))
            % Metadata suggests this property is restricted to be a
            % specific class. If it's not a built-in one, consider it as an
            % "other component" class we should attempt to descend into to
            % extract properties
            builtinValueClasses = {'double' 'single' 'int8' 'uint8' 'int16' ...
                'uint16' 'int32' 'uint32' 'boolean' 'char' 'string' 'logical'};
            isBuiltIn = ismember(propClass.Name,builtinValueClasses);
            if ((false == isBuiltIn) && (false == propClass.Enumeration))
                % This property appears to be class that could have other
                % properties inside it that we wish to explore. Note that
                % support for this type of class in the BTC hierarchy is
                % disavowed and only supported here to transition into
                % using strictly approved BTC container classes.
                % (i.e. derive from BtcLockAndDefaultClass)
                isOtherComponent = true;
            end
        end

        if (isBtcClass || isOtherComponent)
            % It's a component - recurse into dumping of that class
            % ... and add it to our array of component properties
            this.DumpClassdef(aProp.Name,elemClass,obj.(aProp.Name));
        else
            % It's a normal property
            default = '';
            if (aProp.HasDefault)
                default = this.ValueToStr(aProp.DefaultValue);
            end
            currentValue = obj.(aProp.Name);
            currentValueStr = this.ValueToStr(currentValue);

            % Handle BTC Lock and Default behaviors
            elemProp = [];
            defaultingClass = [];
            if isa(obj,'BtcLockAndDefaultClass')
                theEntry = obj.GetPropertyEntry(aProp.Name);
                if (false == isempty(theEntry))
                    if (theEntry.HasLock)
                        % This property is locked - insert a Locked element
                        lockValue = theEntry.GetLockValue();
                        lockValueStr = this.ValueToStr(lockValue);
                        lockingClass = class(theEntry.LockControllingObject);
                        elemProp = this.AppendElement( ...
                            elemClass,'LockedProperty',aProp.Name, ...
                            {'lock_value',lockValueStr, ...
                            'locked_by',lockingClass});
                    elseif (theEntry.HasDefault)
                        % Update the default, and use IsSet
                        defaultValue = theEntry.GetDefaultValue();
                        default = this.ValueToStr(defaultValue);
                        defaultingClass = class(theEntry.DefaultControllingObject);
                    end
                end
            end
            
            % If the locked property was not created, create a non-locked
            % one now:
            if (isempty(elemProp))
                elemProp = this.AppendElement(elemClass,'Property',aProp.Name, ...
                    {'current_value', currentValueStr, ...
                    'default', default});
                if (false == isempty(defaultingClass))
                    elemProp.setAttribute('default_by',defaultingClass);
                end
            end

            % Add something extra for enums
            if (false == isempty(propClass) && propClass.Enumeration)
                elemProp.setAttribute('enum_list', ...
                    strjoin({propClass.EnumerationMemberList.Name},','));
            end            
        end
    end
end

%--------------------------------------------------------------------------
function isNot = IsNotWritableVariableName(~, strExpression)
% IsNotVariableName returns boolean true if the provided strExpression does
% not look like a variable name. This means either that the expression does
% not contain word characters, or it does but the word is actually a
% classdef or function. This search intends to return true if the provided
% expression is *not* a variable that can be changed.

    % For easier debugging, assume it's a variable and weed out from there.
    % It's not as efficient, but makes it easier to figure out why
    % something is not showing up as a variable
    isNot = false; % assume it is a variable

    % Remove any leading or trailing spaces to keep analysis simpler
    strExpression = strtrim(strExpression);

    % If the string doesn't start with a character... then not variable
    if (regexp(strExpression,'^[^a-zA-Z]','once'))
        % Oops - the expression starts with something other than letter
        isNot = true;
        return;
    end

    % If the string contains any non-word characters (except '.')
    if (regexp(strExpression,'[^\w\.]+','once'))
        % Non-word character found. This is not a variable.
        isNot = true;
        return;
    end

    % If the string is actually the name of a class or function
    % The "exist" method returns a numeric code for various types of
    % existence, see reference. All are bad except 1=variable and 7=folder
    if (ismember(exist(strExpression), [2 3 4 5 6 8])) %#ok<EXIST> 
        isNot = true;
        return;
    end
    
    % If before the dot is actually the name of a class or function
    beforeDot = extractBefore(strExpression,'.');
    if (ismember(exist(beforeDot), [2 3 4 5 6 8])) %#ok<EXIST> 
        isNot = true;
        return;
    end
end


end %methods
end %classdef


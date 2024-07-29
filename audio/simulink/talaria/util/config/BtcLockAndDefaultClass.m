classdef BtcLockAndDefaultClass < handle & matlab.mixin.CustomDisplay
% BtcLockAndDefaultClass Base class for build-time configuration classses.
% Provides mechanisms to lock and set defaults for properties. Locks and
% default values can be either immediate values, or calculated values using
% anonymous functions with no input arguments (other than the object
% itself). To use locking, properties must enable the GetObservable
% property. Legacy support of locking via calls to Validate is no longer
% supported. Use BtcUpgradeObservers to update your Config classes.

%   Copyright 2022 Bose Corporation

%--------------------------------------------------------------------------
% Public Methods
%--------------------------------------------------------------------------
methods (Access = public)
    %----------------------------------------------------------------------
    function Lock(this, propertyPath, theValue)
        % Lock Removes the specified propertyPath from being configurable
        % at build-time. Instead, the propertyPath is locked into receiving
        % its value from this lock and can no longer be directly changed. 
        % Locks must happen from inside a constructor to ensure hierarachy
        % locking rules. Once a property is locked, higher-level classes
        % can no longer adjust it.
        % Usage:
        %   Lock(this, propertyPath, theValue)
        % Argument detail:
        %   * this: the configuration object
        %   * propertyPath: char/string of path to property. Simple
        %       function handles can also be used here as they are
        %       recognized and replaced with the property-path string
        %       equivalent e.g. @this.Property vs. 'property'
        %   * theValue: a single value to locked in for the property.
        %       This value can be an anonymous function handle @() which
        %       will update the locked value any time it is viewed, or it
        %       can be a simple immediate value which never changes.
        % Examples:
        %  Lock to an immediate value:
        %   this.Lock('Tone.FilterType',ToneFilterType.Highpass);
        %  Use anonymous-function-hack to specify property path
        %   this.Lock(@this.Tone.FilterType,"Highpass");
        %  Use anonymous-function VALUE to specify an alias or calculation:
        %   this.Lock(@this.Filter.FrameSize,@()this.FrameSize);
        %
        % See also: Default

        % Locks must be performed only from within a constructor
        IsCalledFromConstructor(this,'locking');

        % Get information about the property
        [propertyPath,propOwner,propName] = GetPropertyInfo(this,propertyPath);        

        % Ensure that this property can be locked
        theMetaProp = this.CheckClassOkToLockOrDefault(propOwner,propName,'Lock',propertyPath);

        % Ensure the property is not already locked
        theEntry = propOwner.GetPropertyEntry(propName);
        if ((false == isempty(theEntry)) && theEntry.HasLock)
            error('BTCLOCKANDDEFAULT:CONTAINERRELOCK','[%s] attempts to re-lock property [%s] to [%s] but already locked to [%s] by [%s]', ...
                class(this),propertyPath,this.GetValueAsString(theValue), ...
                this.GetValueAsString(theEntry.LockValue),class(theEntry.LockControllingObject));
        end

        % Hooray - no previous locks found.
        % Lock at the deepest BTC object level. This is as close as we
        % can get to the property itself in the hierachy... and where
        % we will look if somebody else tries to lock.
        if (isempty(theEntry))
            % No entry yet... create it
            theEntry = CreatePropertyEntry(propOwner,propName,theMetaProp);
        end
        theEntry.LockValue = theValue;
        theEntry.LockControllingObject = this;
        theEntry.HasLock = true;

        % That's it. Any attempt to see the property will now trigger it to
        % be updated and Validated as per the property's classdef specs.
    end
    %----------------------------------------------------------------------
    function Default(this, propertyPath, theValue)
        % Default Sets a new default value for a  property.
        % Defaults must happen inside a constructor to ensure hierarachy
        % locking rules. Properties can receive a new default at each level
        % of the BTC hierarchy. The "highest" object wins.
        % Usage:
        %   Default(this, propertyPath, newDefault)
        % Argument detail:
        %   * this: the configuration object
        %   * propertyPath: char/string of path to property. Simple
        %       function handles can also be used here as they are
        %       recognized and replaced with the property-path string
        %       equivalent e.g. @this.Property vs. 'property'
        %   * theValue: a single value to locked in for the property.
        %       This value can be an anonymous function handle @() which
        %       will update the locked value any time it is viewed, or it
        %       can be a simple immediate value which never changes.
        % Examples:
        %   this.Default('Tone.CutoffHertz',1000);
        %   this.Default(@Tone.BoostHertz,200);
        %   this.Default(@this.Tone.TunableDial.DefaultCenter,@()this.AliasToThisProperty);
        %
        % See also: Lock

        % Defaults must be performed only from within a constructor
        IsCalledFromConstructor(this,'defaulting');

        % Get information about the property
        [propertyPath,propOwner,propName] = GetPropertyInfo(this,propertyPath);        

        % Ensure this property can receive a default.
        theMetaProp = this.CheckClassOkToLockOrDefault(propOwner,propName, ...
            'Default',propertyPath);

        % Hooray, this property can receive a default.
        % Create or update the PropertyEntry
        theEntry = propOwner.GetPropertyEntry(propName);
        if (isempty(theEntry))
            % No entry yet... create it
            theEntry = CreatePropertyEntry(propOwner,propName,theMetaProp);
        end
        theEntry.DefaultValue = theValue;
        theEntry.DefaultControllingObject = this;
        theEntry.HasDefault = true;

        % That's it. Any attempt to see the property will now trigger it to
        % be filled in with this default value and Validated as per the
        % property's classdef specs unless something else sets the value.
    end
    %----------------------------------------------------------------------
    function [hasLock,usingDefault,lastSetBy] = IsLockedOrDefaulted(this,propertyName)
        % Returns status of property in terms of locks and defaults
        % Usage:
        %   [hasLock,hasDefault] = IsLockedOrDefaulted(this,propertyName)
        % hasLock returns true if property is locked
        % usingDefault returns true if property has default and is
        % currently unspecified (e.g. using its default)
        hasLock = false;
        usingDefault = false;
        lastSetBy = string.empty;
        theEntry = GetPropertyEntry(this,propertyName);
        if (false == isempty(theEntry))
            if (theEntry.HasLock)
                hasLock = true;
                % Check if the locked value surrenders to the default
                if (theEntry.HasDefault)
                    lockedValue = theEntry.GetLockValue();
                    if (isempty(lockedValue))
                        usingDefault = true;
                    end
                end
            elseif (theEntry.HasDefault && (false == theEntry.IsSet))
                usingDefault = true;
            end
            lastSetBy = theEntry.LastSetBy;
        end
    end
    %----------------------------------------------------------------------
    function ExportToTools(this, propertyPath)
        % ExportToTools Flags the specified property for tools export.
        % The ExtractModelBtcToolsExport function dumps select BTC values
        % to a JSON file for consumption by tuning tools. In property
        % declaration, the validator {ExportToTools} will flag a property
        % for export... however if the property is inherited or defined in
        % a component class, the ability to set property validators is not
        % available. Instead, this method enables *anybody* to flag the
        % property as export via this inherited method. Additionally, this
        % export flag can be set anytime - not only in constructors.
        % Usage:
        %   ExportToTools(this, propertyPath)
        % Argument detail:
        %   * this: the configuration object
        %   * propertyPath: char/string of path to property. Simple
        %       function handles can also be used here as they are
        %       recognized and replaced with the property-path string
        %       equivalent e.g. @this.Property vs. 'property'
        % Examples:
        %  Flag a local property for export:
        %   this.ExportToTools(@this.Breed);
        %   -or-
        %   this.ExportToTools('Breed');
        %
        % See also: ExtractModelBtcToolsExport, ExportToTools        

        % Get information about the property
        [propertyPath,propOwner,propName] = GetPropertyInfo(this,propertyPath);        

        % Ensure that this property can be exported. Unlike lock/default,
        % the only thing that matters is the class of property owner
        % i.e. instead of CheckClassOkToLockOrDefault, just do this:
        theProp = findprop(propOwner,propName);
        description = 'ExportToTools';
        if (false == isa(propOwner,"BtcLockAndDefaultClass"))
            whereLink = MaskUtil.NotThisFileCallerInfo(mfilename,false);
            error('BTCLOCKANDDEFAULT:NOTSUPERCLASS',"%s cannot %s property [%s] " + ...
                "because property owner class [%s] does not inherit from BtcLockAndDefaultClass", ...
                whereLink,description,propertyPath,class(propOwner));
        end

        % Acquire existing PropertyEntry, if any
        theEntry = propOwner.GetPropertyEntry(propName);
        if (isempty(theEntry))
            % No entry yet... create it
            theEntry = CreatePropertyEntry(propOwner,propName,theProp);
        end
        % Register the desire to export
        theEntry.WillExportToTools = true;
        theEntry.ExportControllingObject = this;
    end    
    %----------------------------------------------------------------------
    function [exportIt,isUserBtcClass,exportControllingObject] = IsExportedToTools(this,propertyPath)
        % Returns status of property in terms of ExportToTools
        % Usage:
        %   [exportIt,isUserBtcClass,exportControllingObject] = IsExportedToTools(this,propertyPath)
        % exportIt returns true if property is flagged for export to tools
        % isUserBtcClass returns true if search should continue in property
        exportIt = false;
        exportControllingObject = [];

        % Get information about the property
        [~,propOwner,propName] = GetPropertyInfo(this,propertyPath);
        theProp = findprop(this,propName);

        % If we cannot even get the value, going to be hard to export. Also
        % component-class properties cannot be exported - only individual
        % value properties.
        [canGet,~,isUserBtcClass] = BtcBaseClass.GetPropertyStats(theProp);
        if (false == canGet || isUserBtcClass)
            % Done: either cannot get it, or it's time to descend into it
            return;
        end

        % Check if this property is annointed for export.
        % A special validator function that will trigger export
        funcToFind = @ExportToTools;
        theVal = theProp.Validation;
        if (false == isempty(theVal))
            valFuncs = theVal.ValidatorFunctions;
            for v=1:numel(valFuncs)
                if (isequal(valFuncs{v},funcToFind))
                    exportIt = true;
                    return;
                end
            end
        end

        % So far, it's still not exported. Check PropertyEntry 
        theEntry = propOwner.GetPropertyEntry(propName);
        if (false == isempty(theEntry))
            % Found an entry - this will be the final word on whether the
            % export was requested or not.
            exportIt = theEntry.WillExportToTools;
            exportControllingObject = theEntry.ExportControllingObject;
        end
    end
    %----------------------------------------------------------------------
    function DefaultAllChildren(this,childBasedPropPath,theValue)
        arguments
            this
            childBasedPropPath string
            theValue
        end
        % Loop through child component properties and find those that
        % understand the provided property path
        mco = metaclass(this);
        for i=1:numel(mco.PropertyList)
            aProp = mco.PropertyList(i);
            try
                aChildPath = sprintf("%s.%s",aProp.Name,childBasedPropPath);
                this.ExpandPropertyPath(aChildPath);
            catch
                % Not this one. Move on
                continue;
            end
            this.Default(aChildPath,theValue);
        end
    end
    %----------------------------------------------------------------------
    function LockAllChildren(this,childBasedPropPath,theValue)
        arguments
            this
            childBasedPropPath string
            theValue
        end
        % Loop through child component properties and find those that
        % understand the provided property path
        mco = metaclass(this);
        for i=1:numel(mco.PropertyList)
            aProp = mco.PropertyList(i);
            try
                aChildPath = sprintf("%s.%s",aProp.Name,childBasedPropPath);
                this.ExpandPropertyPath(aChildPath);
            catch
                % Not this one. Move on
                continue;
            end
            this.Lock(aChildPath,theValue);
        end
    end
end

%--------------------------------------------------------------------------
% Hidden Public Methods : still supported but out-of-the-way
%--------------------------------------------------------------------------
methods (Access=public, Hidden)
    %----------------------------------------------------------------------
    function this = BtcLockAndDefaultClass()
        % Constructor
        
        % If this assertion fails, code in HandlePreSetForLock must change.
        assert(mfilename == "BtcLockAndDefaultClass");
    end
    %----------------------------------------------------------------------
    function theValue = Get(this,propertyPath)
        % Get was mis-guided. Support it for awhile to ease transition off
        % of V2200AK V2200AL V2200AM and V2200AN
        % Use of "Get" is DEPRECATED - but doesn't really do anything.
        propertyPath = this.SimpleFuncToPropPath(propertyPath);
        [objects,components] = ExpandPropertyPath(this,propertyPath);
        propOwner = objects{end};
        propName = components(end);
        theValue = propOwner.(propName);
    end    
    %----------------------------------------------------------------------
    function LockPropertyAsAlias(this, lockedPropertyPath, controlPropertyName)
        % LockPropoertyAsAlias Locks a property path to match exactly the
        % value of the specified control property of the locking class.
        % The locked propery essentially becomes as alias to the control
        % property.
        % Locks must happen from inside a constructor to ensure hierarachy
        % locking rules. Once a property is locked, higher-level classes
        % can no longer adjust it.
        % Note that instead of using this method, locking property a value
        % to an anonymous function value @() offers additional flexibility.
        % This method is therefore a bit outdated.
        % Usage:
        %   LockPropertyAsAlias(this, lockedPropertyPath, controlPropertyPath)
        % Examples:
        %  this.LockPropertyAsAlias('ZigaFilter.FrameSize','FrameSize');
        % Note that a simple function handle also works for the property
        % name like this:
        %  this.LockPropertyAsAlias(@this.ZigaFilter.FrameSize','FrameSize')
        %
        % See also: Lock
        
        % Convert any anonymous function hacks to property paths
        % Convert any anonymous function hacks to property paths
        lockedPropertyPath = this.SimpleFuncToPropPath(lockedPropertyPath);
        controlPropertyName = this.SimpleFuncToPropPath(controlPropertyName);

        % Convert control property name into meta.property object. This
        % type of object signals that the lock is an "AsAlias" type lock.
        controlProp = findprop(this,controlPropertyName);
        if (isempty(controlProp))
            if (contains(controlPropertyName,'.'))
                error('BTCLOCKANDDEFAULT:BADCTRLPROP','Control property [%s] for class [%s] must be a property, not a property path', ...
                    controlPropertyName, class(this));
            else
                error('BTCLOCKANDDEFAULT:BADCTRLPROP','Control property [%s] could not be located in class [%s]', ...
                    controlPropertyName, class(this));
            end
        end
        
        % Forward the locking to the general Lock method
        this.Lock(lockedPropertyPath, controlProp);
    end
    %----------------------------------------------------------------------
    function LockPropertyValue(this, propertyPath, theValue)
        % LockPropertyValue is a more descriptive legacy name for Lock
        % See also: Lock
        Lock(this, propertyPath, theValue);
    end
    %----------------------------------------------------------------------
    function UpdatePropertyDefault(this, propertyPath, newDefault)
        % UpdatePropertyDefault is a more descriptive legacy name for Default
        % See also: Default
        Default(this, propertyPath, newDefault);
    end
end    
%--------------------------------------------------------------------------
% Content below has restricted access. It is here to make everything work
%--------------------------------------------------------------------------
properties (Access=private)
    PropertyEntryList PropertyEntry
end
methods (Access={?ExtractModelBTC,?BtcTreeView})
    %----------------------------------------------------------------------
    function theList = GetPropertyEntryList(this)
        % Used by ExtractModelBtc and BtcBaseClass, but should be reworked
        % as ExtractModelBtc could figure things out the first time and
        % BtcBaseClass is only supporting legacy Validate locks
        theList = this.PropertyEntryList;
    end
    %----------------------------------------------------------------------
    %----------------------------------------------------------------------
    function theEntry = GetPropertyEntry(this,propertyPath)
        % GetPropertyEntry returns property entry, if one found
        % Usage:
        %    PropertyEntry = GetPropertyEntry(object,propertyPath);
        theEntry = findobj(this.PropertyEntryList,'PropertyPath',propertyPath);
    end
    %----------------------------------------------------------------------
    function [objects,components] = ExpandPropertyPath(this,propertyPath)
        % ExpandPropertyPath validates the propertyPath is valid,
        % returning an array of path objects and string components.
        % Usage:
        %   [objects,components] = ExpandPropertyPath(this,propertyPath);
        % Although the returned arrays have the same number of elements,
        % they are off-by-one in terms of how they match up. The first obj
        % is "this" which is not explicitly stated in the path. Similarly,
        % the last component is the property name which is not an object.
        components = split(string(propertyPath),'.');
        obj = this;
        depth = numel(components);
        objects = cell(1,depth);
        for i=1:depth
            % Ensure handle classes are being used
            if (false == isa(obj,'handle'))
                error('BTCLOCKANDDEFAULT:NONHANDLE','Build-time-configuration classes must be handle objects. [%s] in [%s] does not inherit from handle', ...
                    class(obj),propertyPath);
            end
            % Record the object at this depth
            objects{i} = obj;
            % If there are more objects, descend to the next level
            if (i < depth)
                obj = obj.(components(i));
                % During editing and design, sometimes a new component is
                % added to a class so existing instances have the property
                % but it has not been constructed yet.
                if (isempty(obj))
                    error('BTCLOCKANDDEFAULT:EMPTYCOMPONENT','Empty object found [%s]. Perhaps BTC must be constructed again?', ...
                        strjoin(components(1:i),'.'));
                end                
            end
        end
        % Finally, verify that the property exists in the final object
        if (false == isprop(obj, components(i)))
            whereLink = MaskUtil.NotThisFileCallerInfo(mfilename,false);
            error('BTCLOCKANDDEFAULT:UNKNOWNPROP','Unrecognized property [%s] for class [%s] called from %s', ...
                components(i),class(obj),whereLink);
        end
    end
    %----------------------------------------------------------------------
    function stringValue = GetValueAsString(~,value)
        % GetValueAsString attempts to dump almost any value as a string.
        % Used for error messages
        if (isnumeric(value) || ischar(value))
            stringValue = string(MaskUtil.ValueToStr(value));
        elseif (iscell(value))
            stringValue = sprintf("%s-cell", ...
                strjoin(string(size(value)),'x'));
        elseif (isobject(value))
            stringValue = sprintf("%s object", class(value));
        elseif (isstring(value))
            stringValue = value;
        else
            stringValue = sprintf("%s %s", ...
                strjoin(string(size(value)),'x'), class(value));
        end
    end
end
methods (Access = private)
    %----------------------------------------------------------------------
    function propertyPath = SimpleFuncToPropPath(~,aPropName)
        % Converts values like @()this.aProperty into 'aProperty'
        propertyPath = aPropName;
        if (isa(aPropName,'function_handle'))
            funcContent = func2str(aPropName);
            % Sometimes (not sure when or why) simple function handles of
            % object properties get extra varargin decorations we hate:
            funcContent = strrep(funcContent,'@(varargin)','');
            funcContent = strrep(funcContent,'(varargin{:})','');
            searchExp = '^\w*\.(.*)';
            tokens = regexp(funcContent,searchExp,"tokens");
            if (false == isempty(tokens))
                propertyPath = tokens{1}{1};
            else
                error('BTCLOCKANDDEFAULT:EXPECTEDSIMPLEFUNC','Expected simple function handle @this.PropertyName, but instead found [%s]', ...
                    funcContent);
            end
        elseif (false == ischar(aPropName) && false==isstring(aPropName))
            error('BTCLOCKANDDEFAULT:BADPROPPATHTYPE', ...
                ['Provided property path must be a character string or a ' ...
                'function handle. Cannot be a [%s]. Did you forget a "@"?'], ...
                class(aPropName));
        end
    end
    %----------------------------------------------------------------------
    function [propertyPath,propOwner,propName] = GetPropertyInfo(this,propertyPath)
        % GetPropertyInfo Interprets path and returns owner, propName

        % Convert any anonymous function hacks to property paths
        propertyPath = this.SimpleFuncToPropPath(propertyPath);

        % Validate and expand the path into its constituent parts
        [objects,components] = ExpandPropertyPath(this,propertyPath);
        propOwner = objects{end};
        propName = components(end);
    end
    %----------------------------------------------------------------------
    function [itIs,where] = IsCalledFromConstructor(this,description)
        % IsCalledFromConstructor returns true if the current execution is
        % launched from the current object's constructor.
        objType = class(this);
        stackTrace = dbstack(2);
        % Ensure the class constructor is part of the trace
        goodStuff = sprintf('%s.%s', objType, objType);
        itIs = ismember(goodStuff,{stackTrace.name});
        where = stackTrace(1).name;
        if (false == itIs)
            error('BTCLOCKANDDEFAULT:INVOKE','Property %s must happen inside constructor, not [%s]', ...
                description,where);
        end
    end
    %----------------------------------------------------------------------
    function theMetaProp = CheckClassOkToLockOrDefault(~,propOwner,propName,description,propertyPath)
        % CheckClassOkToLockOrDefault checks property and owner class are
        % able to support the Lock and Default features.
        theMetaProp = findprop(propOwner,propName);
        if (false == isa(propOwner,"BtcLockAndDefaultClass"))
            whereLink = MaskUtil.NotThisFileCallerInfo(mfilename,false);
            error('BTCLOCKANDDEFAULT:NOTSUPERCLASS',"%s cannot %s property [%s] " + ...
                "because property owner class [%s] does not inherit from BtcLockAndDefaultClass", ...
                whereLink,description,propertyPath,class(propOwner));
        end
        if (false == theMetaProp.GetObservable || false == theMetaProp.SetObservable)
            ownerClass = class(propOwner);
            fixItLink = sprintf(['<a href="matlab:BtcUpgradeObservers.' ...
                'UpgradeBtcClass(''%s'',true)">BtcUpgradeObservers</a>'], ...
                ownerClass);
            whereLink = MaskUtil.NotThisFileCallerInfo(mfilename,false);
            error('BTCLOCKANDDEFAULT:NOTOBSERVABLE',"%s cannot %s property [%s] " + ...
                "until it is Get/SetObservable in class %s. Fix now with %s."', ...
                whereLink,description,propertyPath,ownerClass,fixItLink);
        end
    end    
    %----------------------------------------------------------------------
    function theEntry = CreatePropertyEntry(this,propName,theMetaProp)
        theEntry = PropertyEntry(propName,theMetaProp.Validation);
        this.PropertyEntryList(end+1) = theEntry;
        addlistener(this,propName,'PreGet', ...
            @(src,event)this.HandlePreGetForLock(src,event,theEntry));
        addlistener(this,propName,'PreSet', ...
            @(src,event)this.HandlePreSetForLock(src,event,theEntry));
        addlistener(this,propName,'PostSet', ...
            @(src,event)this.HandlePostSetForLock(src,event,theEntry));
    end

    %----------------------------------------------------------------------
    function HandlePreGetForLock(~,source,event,theEntry)
        % HandlePreGetForLock enforces/updates Lock and/or Default

        % Retrieve the current property value
        propertyOwner = event.AffectedObject;
        currentValue = propertyOwner.(source.Name);

        % Assume there is no value until proven otherwise
        valueToAssert = [];
        assertTheValue = false;

        % First handle the lock
        if (theEntry.HasLock)
            % Acquire the lock value to assert into place
            valueToAssert = theEntry.GetLockValue();
            assertTheValue = true;
        end

        % Now handle the default
        if (isempty(valueToAssert) && theEntry.HasDefault && (false == theEntry.IsSet))
            % There's no valueToAssert yet, we have a default, and the property's unset!
            % Update current value to match the default value when not the same
            valueToAssert = theEntry.GetDefaultValue();
            assertTheValue = true;
        end

        % Snap the property's value into line with what we figured out
        if (assertTheValue && (false == isequal(valueToAssert,currentValue)))
            % Updated value to match lock or default discovered above
            propertyOwner.(source.Name) = valueToAssert;
        end
    end
    %----------------------------------------------------------------------
    function HandlePreSetForLock(~,source,event,theEntry)
        % HandlePreSetForLock enforces lock or updates IsSet state if no
        % lock and the value is being set from something other than us.
        % Note that the value is set whether we want to or not... but the
        % get listener will fix things if anything tries to look at the
        % value.

        % Check the call stack.
        stackTrace = dbstack(2);
        if (isempty(stackTrace))
            % Empty call-stack means command-prompt or other non-code set
            callerSource = 'interactive caller';
            isReallySet = true;
        elseif ((stackTrace(1).file == "BtcLockAndDefaultClass.m") || ...
            (stackTrace(1).file == "BtcLockAndDefaultClass.p"))
            % this set operation is ok - just putting things back on track
            isReallySet = false;
        else
            % otherwise, some code somewhere is trying to set the property:
            callerSource = MaskUtil.NotThisFileCallerInfo(mfilename,false);
            isReallySet = true;
        end
        if (isReallySet)
            if (theEntry.HasLock)
                % No can do: really set a locked property!
                propertyOwner = event.AffectedObject;
                MaskUtil.ReportError('[%s.%s] locked by [%s]: cannot set by [%s]', ...
                    class(propertyOwner),source.Name, ...
                    class(theEntry.LockControllingObject),callerSource);
                if (strcmp(callerSource,'interactive caller'))
                    fprintf('\n***** Error: [%s.%s]\nlocked by [%s] so cannot be set\n\n', ...
                    class(propertyOwner),source.Name, ...
                    class(theEntry.LockControllingObject));
                end
            else
                % This property is not locked and is being really set!
                % Turn on IsSet so the get listener doesn't clobber
                % whatever value is coming in.
                theEntry.IsSet = true;
                theEntry.LastSetBy = callerSource;
            end
        end
    end
    %----------------------------------------------------------------------
    function HandlePostSetForLock(~,source,event,theEntry)
        % HandlePreSetForLock enforces lock or updates IsSet state if no
        % lock and the value is being set from something other than us.
        % Note that the value is set whether we want to or not... but the
        % get listener will fix things if anything tries to look at the
        % value.

        if ((false == theEntry.HasLock) && theEntry.IsSet)
            % There's a real value in there now - if it's empty, turn the
            % IsSet back off
            propertyOwner = event.AffectedObject;
            currentValue = propertyOwner.(source.Name);
            if (false == isempty(currentValue))
                theEntry.IsSet = true;
            else
                % This property has been set to empty! This would return
                % the property back to using the default (if there is one)
                theEntry.IsSet = false;
                theEntry.LastSetBy = [];
            end
        end
    end
end % methods



%--------------------------------------------------------------------------
% Custom text display handling
%--------------------------------------------------------------------------
methods (Access=protected)
    function propGroups = getPropertyGroups(this)
        % Return customized property grouping for display

        % First use a base workspace variable as a hack to turn on and off
        % this customized display
        doRegularPrint = evalin('base','exist(''BtcLockAndDefaultClassNormalDisplay'')');
        if (doRegularPrint)
            propGroups = getPropertyGroups@matlab.mixin.CustomDisplay(this);
            return;
        end

        groupReadOnly = {};
        groupLocked = {};
        groupUnspecified = {};
        groupNormal = {};
        mco = meta.class.fromName(class(this));
        for i=1:numel(mco.PropertyList)
            aProp = mco.PropertyList(i);
            [getGet,canSet] = BtcBaseClass.GetPropertyStats(aProp);
            
            % Cannot get. Skip it
            if (false == getGet)
                continue;
            end

            % Cannot set. Add to read-only and move on.
            if (false == canSet)
                groupReadOnly{end+1} = aProp.Name; %#ok<AGROW> 
                continue;
            end

            % Retrieve any Lock/Default
            [hasLock,usingDefault] = IsLockedOrDefaulted(this,aProp.Name);
            
            % Locked - 
            if (hasLock)
                groupLocked{end+1} = aProp.Name; %#ok<AGROW> 
                continue;
            end

            % Unspecified            
            if (usingDefault)
                % Got one! This property not specified but has default
                groupUnspecified{end+1} = aProp.Name; %#ok<AGROW> 
                continue;
            end

            % Must just be a normal property
            groupNormal{end+1} = aProp.Name; %#ok<AGROW> 
        end

        propGroups(1) = matlab.mixin.util.PropertyGroup(groupNormal,'');
        if (false == isempty(groupUnspecified))
            propGroups(end+1) = matlab.mixin.util.PropertyGroup(groupUnspecified,'----- Unspecified, Using Default -----');
        end
        if (false == isempty(groupLocked))
            propGroups(end+1) = matlab.mixin.util.PropertyGroup(groupLocked,'----- Locked -----');
        end
        if (false == isempty(groupReadOnly))
            propGroups(end+1) = matlab.mixin.util.PropertyGroup(groupReadOnly,'----- Read Only -----');
        end
    end
    %----------------------------------------------------------------------
    function text = getFooter(~)
        % getFooter display hyperlink to turn on/off the custom display

        % cannot find avowed method to determine if we are supposed to
        % supress markup, so just hack it for now
        suppressMarkup = false;
        try
            % Check for experimentally determined caller variable that
            % tells us whether markup is being suppressed. If this hazard
            % code is unsuccessful, the markup will just always show.
            nvPairs = evalin('caller','nvPairs');
            if (isfield(nvPairs,'SuppressMarkup') && ...
                    nvPairs.SuppressMarkup)
                suppressMarkup = true;
            end
        catch
        end

        % If markup is OK to display, show toggle to display the other way
        if (false == suppressMarkup)
            doRegularPrint = evalin('base','exist(''BtcLockAndDefaultClassNormalDisplay'')');
            if (doRegularPrint)
                text = sprintf('   <a href="matlab:BtcLockAndDefaultClass.DoCustomDisplay">Show custom display</a>\n');
            else
                text = sprintf('   <a href="matlab:BtcLockAndDefaultClass.DoNormalDisplay">Show normal display</a>\n');
            end
        else
            text = '';
        end
    end
end
methods (Static,Hidden)
        %----------------------------------------------------------------------
    function DoNormalDisplay()
        % Hack to display classes normally instead of with custom display
        assignin('base','BtcLockAndDefaultClassNormalDisplay',[]);
        obj = evalin('base','ans');
        if (isa(obj,mfilename))
            % Print it
            obj %#ok<NOPRT> 
        else
            fprintf('Reverted to normal display\n')
        end
    end
    function DoCustomDisplay()
        % Hack to display classes custom instead of with normal display
        evalin('base','clear ''BtcLockAndDefaultClassNormalDisplay''');
        obj = evalin('base','ans');
        if (isa(obj,mfilename))
            % Print it
            obj %#ok<NOPRT> 
        else
            fprintf('Enabled custom display\n')
        end
    end
end

end

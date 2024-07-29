classdef BtcBaseClass < BtcLockAndDefaultClass
% ConfigBase Base class for Simulink-block build-time configuration (BTC) classses.
% Provides the critical method API definitions for Simulink block masks:
% * Validate (no additional arguments, emits errors if properties are bad)
% * DoMaskInit (Simulink block handle provided, set code-gen and other
% behaviors that interact with the Simulink canvas).
% Note that property Lock and Default capability also provided via
% inheritance from BtcLockAndDefaultClass.

%   Copyright 2020 Bose Corporation

%--------------------------------------------------------------------------
% Public Methods
%--------------------------------------------------------------------------
methods (Access = public)
    %----------------------------------------------------------------------
    function [ok, errCell] = Validate(this)
        % Validate Called at end of mask initialization to check properties
        % and cross-property interactions look good. Override this method
        % in subclasses and then call this routine as Validate@BtcBaseClass(this)
        % Usage:
        %    [ok, errCell] = Validate(this);
        % If the errCell output is not collected, then any error
        % encountered will trigger an actual error instead of just a
        % message in the errCell.
        errCell = {};
        ok = true; 
    end
    %----------------------------------------------------------------------
    function DoMaskInit(this,blockHandle)  %#ok<INUSD> 
        % DoMaskInit is where BTC classes can perform automation of code
        % generation settings (e.g. CscUtil) or otherwise interact with the
        % Simulink canvas. Avoid making design-time changes if possible
        % (e.g. avoid calling: set_param, add_block, replace_block). 
        % The expected callstack arriving at this function starts with a
        % block's mask initialization code automated by BTConfig.

        % Currently, the base class has no business whatsoever with the
        % Simulink Canvas. This method is here so that BTConfig automation
        % can invoke DoMaskInit for all BTC classes. This implementation is
        % the final endpoint of the DoMaskInit invocation after visiting
        % any overridden implementations from sub classes... but there is
        % nothing to do here.
    end

end
%--------------------------------------------------------------------------
% Content below is hidden - backward-compatible, but no longer "avowed"
%--------------------------------------------------------------------------
methods (Hidden)
    %----------------------------------------------------------------------
    function anInstance = ExtractBaseClassInstance(this, baseClassName)
        % ExtractBaseClassInstance Creates shallow copy of base class obj.
        % Returning a new object based the provided base-class name (aka
        % super class). This is particularly useful if a containing block's
        % configuration class derives from the the class of a contained
        % block. To configure the contained block, this method can extract
        % the contained block's configuration from this block's derived
        % configuration. Note that private properties do not get copied.
        % Usage:
        %   obj = this.DoExtractBaseClassInstallce('MySuperClass');
        
        if (false == isa(this,baseClassName))
            error('BTCBASECLASS:NOTBASE','Current object class [%s] does not derive from [%s]', ...
                class(this),baseClassName);
        end
        % Get metadata for the base class
        mco = meta.class.fromName(baseClassName);
        % Create an instance of the base class
        constructor = str2func(baseClassName);
        anInstance = constructor();
        % Iterate through properties and set them all
        for i=1:numel(mco.PropertyList)
            aProperty = mco.PropertyList(i);
            if (false == aProperty.Dependent && "private" ~= aProperty.SetAccess)
                anInstance.(aProperty.Name) = this.(aProperty.Name);
            end
        end
    end
    %----------------------------------------------------------------------
    function anInstance = InjectBaseClassInstance(this, baseClassInstance)
        % InjectBaseClassInstance Copies properties from a base-class
        % object instance into the current object.
        % Usage:
        %   this.InjectBaseClassInstance('MySuperClass',anObject);
        baseClassName = class(baseClassInstance);
        if (false == isa(this,baseClassName))
            error('BTCBASECLASS:NOTBASE','Current object class [%s] does not derive from [%s]', ...
                class(this),baseClassName);
        end
        % Get metadata for the base class
        mco = meta.class.fromName(baseClassName);
        anInstance = baseClassInstance;
        % Iterate through properties and copy them all
        for i=1:numel(mco.PropertyList)
            aProperty = mco.PropertyList(i);
            if (false == aProperty.Dependent && "private" ~= aProperty.SetAccess)
                this.(aProperty.Name) = anInstance.(aProperty.Name);
            end
        end
    end
end
methods(Static, Hidden)
    function value = FillEmpty(possiblyEmptyValue,fillValue)
        % FillEmpty returns either the provided first argument, or else the
        % second argument if the first argument is empty.
        % Usage:
        %   value = BtcBaseClass.FillEmpty(possiblyEmptyValue,fillValue)
        % Example:
        %   this.Property = this.FillEmpty(this.Property, defaultValue);
        if (isempty(possiblyEmptyValue))
            value = fillValue;
        else
            value = possiblyEmptyValue;
        end
    end
    %----------------------------------------------------------------------
    function [canGet,canSet,isBtcClass,requiredClassObj] = GetPropertyStats(aMetaPropertyObject)
        % GetPropertyStats returns digested information about property
        % Returns multiple booleans indicating whether the proprerty could
        % be set or set, the required class (if any) and whether that class
        % is just a built-in type or not.
        % Note: isBtcClass only returns true if all the these are true:
        %  * property's class derives from BtcLockAndDefaultClass
        %  * property has public read access and is not dependent or hidden
        % Usage:
        %    [canGet,canSet,isBtcClass,requiredClassObj] = BtcBaseClass.GetPropertyStats(aMetaPropertyObject)
        aProp = aMetaPropertyObject;

        % Determine if property meets our publically "Getable" specs
        getAccess = aProp.GetAccess;
        canGet = ((false == aProp.Hidden) && ...
                  (false == iscell(getAccess)) && ...
                  ("public" == getAccess)) && ...
                 ((false == aProp.Dependent) || ...
                  (false == isempty(aProp.GetMethod)));
        
        % Determine if property meets our publically "Setable" specs
        setAccess = aProp.SetAccess;
        canSet = ((false == aProp.Hidden) && ...
                  (false == iscell(setAccess)) && ...
                  ("public" == setAccess)) && ...
                 ((false == aProp.Dependent) || ...
                  (false == isempty(aProp.SetMethod)));
        
        % Acquire the defined class required (if any) and whether that
        % class is a built-in type or not
        if ( nargout > 2)
            % Assume the class is not a user-defined BTC class
            isBtcClass = false;
            if (false==isempty(aProp.Validation) && false==isempty(aProp.Validation.Class))
                requiredClassObj = aProp.Validation.Class;
                reqClassName = requiredClassObj.Name;
                % Simple rules: must be both a BtcLockAndDefaultClass and
                % also be for public viewing and also have its value stored
                % in memory  (i.e. not a dependent property)
                isBtc = contains('BtcLockAndDefaultClass',superclasses(reqClassName));
                isHiddenOrDependent = (aProp.Hidden || aProp.Dependent);
                isBtcClass = isBtc && (false == isHiddenOrDependent);
            else
                requiredClassObj = [];
            end
        end
    end
end

end

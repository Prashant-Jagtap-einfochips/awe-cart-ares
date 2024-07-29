classdef BtcVariant < BtcLockAndDefaultClass
% BtcVariant Base class for build-time variant classes. This class
% works with BtcBassClassWithVariants and BtcVariantEnum classes to create
% variant build time properties and methods. There are no special
% requirements for this class because BtcBaseClassWithVariants does the
% heavy lifting. Derived classes are intended to handle those configuration
% duties and data associated with block variants (e.g. whatever is needed
% for a variant of choice in a Simulink Variant Subsystem)
% Usage:
%    classdef MyBlockVariant < BtcVariant
%      properties
%        Prop1
%        Prop2
%          ...etc
%      end
%    end

%   Copyright 2023 Bose Corporation

properties (SetAccess=immutable,GetAccess=protected)
    % Parent must be available in constructor for Lock and Default calls to
    % see what properties are available even if they are still empty.
    % Cannot specify the BtcBaseClassWithVariants type here because its
    % abstract. Instead, check that class is correct with a validation
    % function.
    Parent {MustBeBtcBaseClassWithVariants(Parent)}
end

methods
    function this = BtcVariant(parentObj)
        % Constructor for BtcVariant: remember the Parent object
        this.Parent = parentObj;
    end
    %----------------------------------------------------------------------
    function EnsureCodeReplacementLibrary(this,blockHandle,libraryNameFragment)
        % EnsureCodeReplacementLibrary checks the CRL's of containing model
        % to ensure the specified library name fragment is found via a case
        % sensitive match. If the name is not found, this method generates
        % an error. Normally call this method from VariantDoMaskInit.
        % Usage:
        %    EnsureCodeReplacementLibrary(this,blockHandle,libraryNameFragment)
        % Example:
        %    this.EnsureCodeReplacementLibrary(blockHandle,'Hexagon');
        %
        % See also VariantDoMaskInit
        theModel = bdroot(blockHandle);
        theCrls = get_param(theModel,'CodeReplacementLibrary');
        found = contains(theCrls,libraryNameFragment);
        if (false == any(found))
            error("Missing Code Replacement Library. " + ...
                "Block [%s] requires CRL [%s] but found only [%s]. " + ...
                "Add the required CRL in Model Settings under Code " + ...
                "Generation / Interface.", ...
                getfullname(blockHandle),libraryNameFragment,theCrls);
        end
    end
    %----------------------------------------------------------------------
    function CopyOldProperties(this, oldObj)
        % CopyOldProperties attempts to copy whatever properties it can
        % from the provided oldObj into this one.

        % Get metadata for the classes
        srcClass = class(oldObj);
        dstClass = class(this);
        srcMco = meta.class.fromName(srcClass);
        dstMco = meta.class.fromName(dstClass);

        % Iterate through properties of this (the destination) and copy
        % over anything with the same name from the oldObj (the source)
        for i=1:numel(dstMco.PropertyList)
            dstProperty = dstMco.PropertyList(i);
            propName = dstProperty.Name;
            if ("EnumValue" == propName || "Parent" == propName)
                % Do not copy this one - it's about to change anyway
                continue;
            end
            % If this property is stored (i.e. no dependent) then see if
            % one with the same name is found in the old Obj we can read
            srcProperty = findobj(srcMco.PropertyList,'Name',propName);
            if (false == isempty(srcProperty))
                % Found one. Let's give it a try
                [canGet,~] = BtcBaseClass.GetPropertyStats(srcProperty);
                [~,canSet] = BtcBaseClass.GetPropertyStats(dstProperty);
                hasLock = IsLockedOrDefaulted(this,propName);
                if (canGet && canSet)
                    if (hasLock)
                        fprintf('%s locked (not copied)\n',propName);
                        continue;
                    end
                    try
                        this.(propName) = oldObj.(propName);
                        fprintf('Copied %s\n',propName);
                    catch ME
                        warning('Unable to copy %s from %s to %s: %s', ...
                            propName,srcClass,dstClass,ME.message);
                    end
                end
            end
        end
    end
    %----------------------------------------------------------------------
    function VariantValidate(this) %#ok<MANU>
        % VariantValidate is where variants can check cross-property
        % interactions and ensure the build-time-configuration looks
        % healthy. The BtcBaseClassWithVariants object that contains us has
        % set up the "Parent" property to point back to itself in case
        % validating variant properties requires knowledge of parent
        % properties. Note that there is no inherited Validate to call as
        % we inherit from just the handle class.

        % Nothing to do here in the base class. The name of this function
        % is ValidateVariant instead of just Validate to help clarify that
        % this method does not inherit from BtcBaseClass
    end
    %----------------------------------------------------------------------
    function VariantDoMaskInit(this,blockHandle)  %#ok<INUSD> 
        % VariantDoMaskInit is where variants perform automation of code
        % generation settings (e.g. CscUtil) or otherwise interact with the
        % Simulink canvas. Avoid making design-time changes if possible
        % (e.g. avoid calling: set_param, add_block, replace_block). 
        % The expected callstack arriving at this function starts with a
        % block's mask initialization code automated by BTConfig.
        %
        % See also EnsureCodeReplacementLibrary

        % Derived classes will have access to the variant object's
        % properties and methods via "this", to the simulink block and its
        % mask workspace via "blockHandle", and to the containing
        % BtcBaseClassWithVariants object via the "Parent" property.

        % Nothing to do here in the base class. The name of this function
        % is DoMaskInitVariant instead of just DoMaskInit to help clarify
        % that this method does not inherit from BtcBaseClass
    end
end

end
% local property validation functions
function MustBeBtcBaseClassWithVariants(obj)
    % Perform property datatype validation here because this class is
    % abstract and can cause trouble when initial class loading :-(
    if (isempty(obj))
        % When classes first load, sometimes an empty double comes in
        return;
    end
    if (false == isa(obj,'BtcBaseClassWithVariants'))
        error("Class [%s] does not derive from BtcBaseClassWithVariants.", ...
            class(obj));
    end
end

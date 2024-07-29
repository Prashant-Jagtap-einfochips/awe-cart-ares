classdef BtcBaseClassWithBreeds < BtcBaseClass
% BtcBaseClassWithBreeds Base class for build-time classes that include
% custom breed component objects. Derived classes must define the
% BreedClassMap abstract property to be a dictionary mapping from an enum
% to a string. The enum must derive from Simulink.IntEnumType and string
% must be a classname derived from BtcBreedBaseClass assocated with the
% selected breed. With everything properly configured,
% BtcBaseClassWithBreeds introduces two new properties:
%   Breed : enum property that controls which type of breed is present
%   BreedObj : A component object property of the appropriate class for the
%      selected breed, discovered via the BreedClassMap dictionary.
% Usage:
%    classdef MyBlockConfig < BtcBaseClassWithBreeds
%    properties (Constant)
%        BreedClassMap = dictionary( ...
%            AsdHwEnum.Generic,       "MyBlockGenericBreed", ...
%            AsdHwEnum.SharcPlusCore, "MyBlockGenericSharcPlusCoreBreed")
%    end

%   Copyright 2022 Bose Corporation

properties (Constant,Abstract,Hidden)
    % Must be dictionary object mapping from Simulink.IntEnumType-derived
    % enumeration to string. For example (note multi-line constructs can
    % comment out one of the lines by starting it with elipsis) :
    % BreedClassMap = dictionary( ...
    %     AsdHwEnum.Generic, "MyBlockGenericBreed", ...
    %     ... AsdHwEnum.SharcPlusCore, "MyBlockGenericSharcPlusCoreBreed", ...
    %     AsdHwEnum.SharcPlusAccel, "MyBlockGenericSharcPlusAccelBreed")
    BreedClassMap
end

%--------------------------------------------------------------------------
properties (GetObservable,SetObservable)
    % Fuzzy match Breed with array of string fragments in priority order.
    % Note these goals are set up as a default to select the Breed, but
    % this default is overridden when Breed is set directly or when the
    % BTC hierarchy registers other Lock or Default values.
    BreedSelectionGoals string = ""
end
properties (Dependent,GetObservable,SetObservable)
    % Choice of breed: the enum class comes from the BreedClassMap dictionary
    Breed
end
properties (Hidden,Access=protected)
    VerifiedBreed % enum of current breed - known to be supported
    AllocatedBreed % enum of breed for currently allocated object
    AllocatedBreedObj % Copy of breed object to avoid get handlers
end
properties (SetAccess=private)
    BreedObj BtcBreedBaseClass % this component object allocated based on the value of the Breed property above
end
%--------------------------------------------------------------------------

properties (Hidden,SetAccess=immutable)
    BreedEnumClassName string % set by constructor, this names the class of the Breed property above
end
methods
    function this = BtcBaseClassWithBreeds(initialBreed)
        % Constructor requires name of the enumerated class used to specify
        % the type of BreedProps component. Optionally, also specify the
        % default breed with which to start. If none specified, the first
        % member of the enumeration will serve as the default.

        % Give helpful error message if a derived class is not properly
        % specifying it's enumerated class in its constructor
        if (false == isa(this.BreedClassMap,'dictionary'))
            error("The BreedClassMap property must defined as a " + ...
                "dictionary that maps from an enum to a class name. " + ...
                "Currently BreedClassMap is a [%s]",class(this.BreedClassMap));
        end
        [variantEnumClassName,valueClassName] = this.BreedClassMap.types;

        if ("string" ~= valueClassName || ...
                false == contains('Simulink.IntEnumType',superclasses(variantEnumClassName)))
            error("The BreedClassMap property is defined as a dictionary, " + ...
                "but it must map from a Simulink.IntEnumType-derived enumeration " + ...
                "to a string... not from [%s] to [%s]", ...
                variantEnumClassName, valueClassName);
        end

        % Remember the associated enumerated class
        this.BreedEnumClassName = variantEnumClassName;

        % Default the breed to search based on provided goals
        this.Default(@this.Breed,@()this.FindBreedMatch(this.BreedSelectionGoals));
        if (exist("initialBreed","var"))
            this.Breed = initialBreed;
        end
    end
    function theMatch = FindBreedMatch(this,varargin)
        % FindBreedMatch returns best match breed from prioritized name
        % fragments. Specify one or more string or char-vector arguments
        % that each represent a partial match of the desired breed name.
        % The specified order of these name fragments is critical as each
        % one narrows down the remaining choices for the next fragment to
        % match against. If at any point a name fragment results in zero
        % remaining matches, the fragment is ignored. The returned match
        % has the lowest integer equivalent among those choices remaining.
        % In other words, if no matches are ever made, the lowest value
        % option gets selected, which is usually the "Generic" breed. All
        % string comparisons are case insensitive.
        % Usage:
        %    theMatch = TheObject.FindBreedMatch('sharc','accel');
        %    i.e. Find a SHARC breed with accelerator if possible
        %
        %    qualities = {'sharc','core','gul'};
        %    theMatch = TheObject.FindBreedMatch(qualities{:});
        %    i.e. passes in the name fragments via a cell array
        %
        %    badMatch = TheObject.FindBreedMatch(["accel","hexagon"])
        %    i.e the order of these fragments is likely *BAD* and might
        %    return a SHARC breed that would not work with a SnapDragon!

        % Retrieve a list of all enumeration element names sorted by their
        % integer equivalents.
        allOptions = enumeration(this.BreedEnumClassName);
        sortedOptions = sort(allOptions);
        allStrings = string(sortedOptions);

        % Filter out only those variants that are supported:
        isSupported = ismember(allStrings,this.BreedClassMap.keys);
        supportedOptions = sortedOptions(isSupported);
        supportedStrings = allStrings(isSupported);

        % Be super flexible and allow people to use Strings, chars, and
        % vectors/arrays of strings with as many arguments as desired.
        % Ignore anything that's not a char or string.
        inputFragments = string.empty;
        for i=1:numel(varargin)
            theArg = varargin{i};
            if ischar(theArg)
                inputFragments(end+1) = (theArg); %#ok<AGROW> 
            elseif isstring(theArg)
                inputFragments = horzcat(inputFragments, theArg(:)'); %#ok<AGROW> 
            end
        end

        % Case insensitive: To perform case-insensitive matches, make
        % everything lower case
        fragments = lower(inputFragments);
        supportedStrings = lower(supportedStrings);

        % Loop through each fragment specified, narrowing down the choices.
        % To start off, all options have a chance to match
        active = true(numel(supportedStrings),1);
        numFragments = numel(fragments);
        for i=1:numFragments
            aFragment = fragments{i};
            % Among the active strings, search for partial matches
            thisMatch = contains(supportedStrings,aFragment);
            stillActive = active & thisMatch;
            if any(stillActive)
                % Something still matches, so narrow down the active
                % choices to those that are still matching
                active = stillActive;
            else
                % Nothing matched after this fragment, so ignore it and
                % leave the currently active choices in place
            end
        end
        % Choose the first remaining option, which will be the lowest #
        theMatch = supportedOptions(find(active,1));
    end
    %----------------------------------------------------------------------
    function set.Breed(this,newValue)
        % To support dynamic defaults (which we know are in place), allow
        % empty [] to pass through, signalling default to take over
        if (isempty(newValue))
            % By setting VerifiedBreed to empty, the Default handlers will
            % return the property to unspecified-using-default status

            % % BUT don't let the default value itself be empty!
            % stackTrace = dbstack();
            % if (numel(stackTrace))
            %     stackfiles = strjoin({stackTrace.file},',');
            %     if (contains(stackfiles,'BtcLockAndDefaultClass'))
            %         warning("Breed cannot default to empty");
            %         return;
            %     end
            % end
            this.VerifiedBreed = [];
                return;
        end
        % Ensure/convert value to the supported enumeration type
        newValue = feval(this.BreedEnumClassName,newValue);
        
        % Ensure that the specified breed is supported
        if (false == this.BreedClassMap.isKey(newValue))
            error("Class [%s] does not support the [%s.%s] Breed. Supported " + ...
                "variants: [%s]", ...
                class(this),this.BreedEnumClassName,string(newValue), ...
                strjoin(string(this.BreedClassMap.keys),','));
        end
        % This breed is supported. Record it!
        this.VerifiedBreed = newValue;

        % Now check if the currently allocated breed object matches. No
        % need to "view" the Breed as we are already setting it right now!
        this.EnsureBreedObject(this.VerifiedBreed);
    end
    %----------------------------------------------------------------------
    function value = get.Breed(this)
        value = this.VerifiedBreed;
    end
    %----------------------------------------------------------------------
    function value = get.BreedObj(this)
        % When getting the obj, one must "view" the breed to update any
        % defaults
        this.Breed;
        value = this.BreedObj;
    end
    %----------------------------------------------------------------------
    function EnsureBreedObject(this,currentBreed)
        % Ensure the current breed object matches the current verified
        % breed choice
        % currentBreed = this.Breed; % This may pull updated lock/default
        localBreedObjCopy = this.AllocatedBreedObj;
        allocatedBreed = this.AllocatedBreed;
        if (isempty(allocatedBreed) || currentBreed ~= allocatedBreed)
            fprintf("<<Breed Enum update: [%s] -> [%s]>>\n",allocatedBreed,currentBreed);
            requiredClassName = this.BreedClassMap(currentBreed);
            
            % Ensure the BreedProps property is of the correct type
            % localBreedObjCopy = this.BreedObj;
            currentClass = class(localBreedObjCopy);
            if (false == isequal(currentClass,requiredClassName) || ...
                    isempty(localBreedObjCopy))
                % The correct class is not in place! Clobber whatever is there
                % and replace with the correct class
                if (false == isempty(localBreedObjCopy))
                    fprintf('<<Breed Object update: [%s] -> [%s]>>\n',currentClass,requiredClassName);
                end
                % Construct the BtcBreed object, passing in this as parent so
                % the breed could perform lock/default operations.
                this.AllocatedBreedObj = feval(requiredClassName,this);
                this.BreedObj = this.AllocatedBreedObj;
                
                % Attempt to transfer properties from old breed to this one
                if (false == isempty(localBreedObjCopy))
                    this.BreedObj.CopyOldProperties(localBreedObjCopy);
                end
            end
            this.AllocatedBreed = currentBreed;
        end
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate also invokes active breed's validation before finally
        % calling the BtcBaseClass
        this.BreedObj.BreedValidate();
        Validate@BtcBaseClass(this);
    end
    %----------------------------------------------------------------------
    function DoMaskInit(this,blockHandle)
        % DoMaskInit is where BTC classes can perform automation of code
        % generation settings (e.g. CscUtil) or otherwise interact with the
        % Simulink canvas. Avoid making design-time changes if possible
        % (e.g. avoid calling: set_param, add_block, replace_block). 
        % The expected callstack arriving at this function starts with a
        % block's mask initialization code automated by BTConfig.

        % Invoke mask initialization for the currently selected breed
        this.BreedObj.BreedDoMaskInit(blockHandle);

        % ... and for our own base class
        DoMaskInit@BtcBaseClass(this,blockHandle);
    end

end
end

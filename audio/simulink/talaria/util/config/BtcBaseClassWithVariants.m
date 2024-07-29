classdef BtcBaseClassWithVariants < BtcBaseClass
% BtcBaseClassWithVariants Base class for build-time classes that include
% variants. This class requires the constructor to specify the name of the
% enumeration classdef that lists all supported variants. The enumeration
% classdef must itself derive from BtcVariantEnum. With everything properly
% configured, BtcBaseClassWithVariants introduces two new properties:
%   Variant : A property to control which type of variant is present
%   VariantObj : A component property of the appropriate class for the
%      selected variant, discovered via the enumeration.
% Usage:
%    classdef MyBlockConfig < BtcBaseClassWithVariants
%      methods
%        function this = MyBlockConfig()
%           this@BtcBassClassWithVariants(NameOfVariantEnumClass)
%        end
%      end

% Note that a new BtcBaseClassWithBreeds class has several improvements:
% 1- Enables use of shared enumeration class(es) among many blocks
% 2- The shared enumeration is supported natively by MATLAB Function Blocks
% 3- The enumeration-to-BTC-class mapping is more reliable (not as flimsy)
% 4- The control property "Variant" is instead named "Breed" to eliminate
%    the name conflict with Simulink's built-in property associated with
%    all Subsystem blocks including MATLAB Function Blocks.
% The decision to introduce a new set of Breed-based classes instead of
% enhancing this class enables the adoption of these improvements to happen
% slowly over time block-by-block while this old class remains functional.

%--------------------------------------------------------------------------
properties (GetObservable,SetObservable)
    % Fuzzy match variant with array of string fragments in priority order.
    % Note these goals are set up as a default to select the Variant, but
    % this default is overridden when Variant is set directly or when the
    % BTC hierarchy registers other Lock or Default values.
    VariantSelectionGoals string = ""
end
properties (Dependent,GetObservable,SetObservable)
    % Choice of Variant: the enum class comes from the VariantClassMap dictionary
    Variant
end
properties (Hidden,Access=protected)
    VerifiedVariant % enum of current Variant - known to be supported
    AllocatedVariant % enum of Variant for currently allocated object
    AllocatedVariantObj % Copy of Variant object to avoid get handlers
end
properties (SetAccess=private)
    VariantObj BtcVariant % this component object allocated based on the value of the Variant property above
end
%--------------------------------------------------------------------------

properties (Hidden,SetAccess=immutable)
    VariantEnumClassName string % set by constructor, this names the class of the Variant property above
end
methods
    function this = BtcBaseClassWithVariants(variantEnumClassName,initialVariant)
        % Constructor requires name of the enumerated class used to specify
        % the type of VariantProps component. Optionally, also specify the
        % default Variant with which to start. If none specified, the first
        % member of the enumeration will serve as the default.

        % Give helpful error message if a derived class is not properly
        % specifying it's enumerated class in its constructor
        if (nargin < 1)
            exampleName = regexprep(class(this),'Config$','');
            exampleName = [exampleName 'VariantEnum'];
            error(['The %s constructor must pass the name of the enumerated ' ...
                'class e.g. this@BtcBaseClassWithVariants(''%s'');'], ...
                class(this),exampleName);
        end

        % Remember the associated enumerated class
        this.VariantEnumClassName = variantEnumClassName;

        % Default the Variant to search based on provided goals
        this.Default(@this.Variant,@()this.FindVariantMatch(this.VariantSelectionGoals));
        if (exist("initialVariant","var"))
            this.Variant = initialVariant;
        end
    end
    function SelectVariant(this,varargin)
        % For backward compatibility support - this just sets the
        % VariantSelectionGoals.

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
        
        this.VariantSelectionGoals = inputFragments;

        fprintf(['SelectVariant deprecated: Set the VariantSelectionGoals ' ...
            'instead [%s]\n'], MaskUtil.NotThisFileCallerInfo(mfilename,false));
    end
    function theMatch = FindVariantMatch(this,varargin)
        % FindVariantMatch returns best match Variant from prioritized name
        % fragments. Specify one or more string or char-vector arguments
        % that each represent a partial match of the desired Variant name.
        % The specified order of these name fragments is critical as each
        % one narrows down the remaining choices for the next fragment to
        % match against. If at any point a name fragment results in zero
        % remaining matches, the fragment is ignored. The returned match
        % has the lowest integer equivalent among those choices remaining.
        % In other words, if no matches are ever made, the lowest value
        % option gets selected, which is usually the "Generic" Variant. All
        % string comparisons are case insensitive.
        % Usage:
        %    theMatch = TheObject.FindVariantMatch('sharc','accel');
        %    i.e. Find a SHARC Variant with accelerator if possible
        %
        %    qualities = {'sharc','core','gul'};
        %    theMatch = TheObject.FindVariantMatch(qualities{:});
        %    i.e. passes in the name fragments via a cell array
        %
        %    badMatch = TheObject.FindVariantMatch(["accel","hexagon"])
        %    i.e the order of these fragments is likely *BAD* and might
        %    return a SHARC Variant that would not work with a SnapDragon!

        % Retrieve a list of all enumeration element names sorted by their
        % integer equivalents.
        allOptions = enumeration(this.VariantEnumClassName);
        sortedOptions = sort(allOptions);
        allStrings = string(sortedOptions);

        % Filter out only those variants that are supported:
        isSupported = ismember(allStrings,allStrings);
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
    function set.Variant(this,newValue)
        % To support dynamic defaults (which we know are in place), allow
        % empty [] to pass through, signalling default to take over
        if (isempty(newValue))
            % By setting VerifiedVariant to empty, the Default handlers will
            % return the property to unspecified-using-default status
            this.VerifiedVariant = [];
                return;
        end
        % Ensure/convert value to the supported enumeration type
        newValue = feval(this.VariantEnumClassName,newValue);
        
        % This Variant is supported. Record it!
        this.VerifiedVariant = newValue;

        % Now check if the currently allocated Variant object matches. No
        % need to "view" the Variant as we are already setting it right now!
        this.EnsureVariantObject(this.VerifiedVariant);
    end
    %----------------------------------------------------------------------
    function value = get.Variant(this)
        value = this.VerifiedVariant;
    end
    %----------------------------------------------------------------------
    function value = get.VariantObj(this)
        % When getting the obj, one must "view" the Variant to update any
        % defaults
        this.Variant;
        value = this.VariantObj;
    end
    %----------------------------------------------------------------------
    function EnsureVariantObject(this,currentVariant)
        % Ensure the current Variant object matches the current verified
        % Variant choice
        % currentVariant = this.Variant; % This may pull updated lock/default
        localVariantObjCopy = this.AllocatedVariantObj;
        allocatedVariant = this.AllocatedVariant;
        if (isempty(allocatedVariant) || currentVariant ~= allocatedVariant)
            if (false == isempty(allocatedVariant))
                fprintf("<<Variant Enum update: [%s] -> [%s]>>\n",allocatedVariant,currentVariant);
            end

            % Retrieve the associated classname from this variant enumeration
            intValue = int32(currentVariant);
            requiredClassName = BtcVariantEnum.ClassNameCache( ...
                this.VariantEnumClassName, intValue);            
            
            % Ensure the VariantProps property is of the correct type
            % localVariantObjCopy = this.VariantObj;
            currentClass = class(localVariantObjCopy);
            if (false == isequal(currentClass,requiredClassName) || ...
                    isempty(localVariantObjCopy))
                % The correct class is not in place! Clobber whatever is there
                % and replace with the correct class
                if (false == isempty(localVariantObjCopy))
                    fprintf('<<Variant Object update: [%s] -> [%s]>>\n',currentClass,requiredClassName);
                end
                % Construct the BtcVariant object, passing in this as parent so
                % the Variant could perform lock/default operations.
                this.AllocatedVariantObj = feval(requiredClassName,this);
                this.VariantObj = this.AllocatedVariantObj;
                
                % Attempt to transfer properties from old Variant to this one
                if (false == isempty(localVariantObjCopy))
                    this.VariantObj.CopyOldProperties(localVariantObjCopy);
                end
            end
            this.AllocatedVariant = currentVariant;
        end
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate also invokes active Variant's validation before finally
        % calling the BtcBaseClass
        this.VariantObj.VariantValidate();
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

        % Invoke mask initialization for the currently selected Variant
        this.VariantObj.VariantDoMaskInit(blockHandle);

        % ... and for our own base class
        DoMaskInit@BtcBaseClass(this,blockHandle);
    end

end
end

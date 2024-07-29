classdef BtcVariantEnum < Simulink.IntEnumType
% BtcVariantEnum Base class for build-time variant enumerations. This class
% works with the BtcBassClassWithVariants and BtcVariant classes. Primarily
% the service of this class is to associate a BtcVariant classname with
% each enumeration element.
% Usage:
%    classdef MyBlockVariantEnum < BtcVariantEnum
%      enumeration
%        Element1 (1,'Element1ClassName')
%        Element2 (2,'Element2ClassName')
%      end
%    end

%   Copyright 2022 Bose Corporation

methods
    function this = BtcVariantEnum(intValue,configClassName)
        this@Simulink.IntEnumType(intValue);
        BtcVariantEnum.ClassNameCache(class(this),intValue,configClassName);
    end

    function itDoes = StartsWith(this,prefixStr)
        itDoes = false;
        prefixStr = char(prefixStr);
        thisChar = char(this);
        prefixLen = numel(prefixStr);
        if (prefixLen <= numel(thisChar))
            itDoes = isequal(prefixStr,thisChar(1:prefixLen));
        end
    end

    function itDoes = Contains(this,stringFragment)
        % Contains performs a case-insensitive comparison returning true if
        % the provided stringFragment is found in the name of this variant.
        itDoes = false;
        fragmentChar = char(stringFragment);
        thisChar = char(this);
        if (contains(thisChar,fragmentChar,"IgnoreCase",true))
            itDoes = true;
        end
    end

    function uint8Str = Uint8Str(this)
        % Uint8Str returns the string-name of the enumeration element
        % as a vector of uint8 values i.e. uint8(char(enumvalue))
        % This format is appropriate for passing the string name to MATLAB
        % Function Blocks if desired instead of just the integer enum
        % equivalent value e.g. uint32(enumValue).
        % The string-name can be re-constituted inside the MATLAB Function
        % Block by passing the vector to char().
        % Usage:
        %    variantUint8Str = Config.Variant.Uint8Str; % mask init
        %    variantStr = char(variantUint8Str); % inside MFB
        uint8Str = uint8(char(this));
    end

end
methods (Static,Access=?BtcBaseClassWithVariants)
    function aOut = ClassNameCache(enumClassName,intValue,configClassName)
        % Each enumerated subclass (i.e. a class derived from this one)
        % requires its own storage of which BtcVariant class to use for
        % each enumeration choice. Use a map where key is the enumerated
        % classname and the value is string array of classnames for each
        % enumeration member (by associated integer).

        % Declare the map secret storage
        persistent theCache;
        if isempty(theCache)
            % Map from enum class name to array of config class names
            theCache = containers.Map;
        end

        % If all parameters provided, this is a registration
        if (nargin == 3)
            % Grab the existing array of config classname if any
            if (theCache.isKey(enumClassName))
                strArray= theCache(enumClassName);
            else
                strArray = strings;
            end
            % Update the array of BtcConfig class names for this enum class
            strArray(intValue) = configClassName;
            theCache(enumClassName) = strArray;

            % Return the whole array if somebody wants it
            if (nargout >= 1)
                aOut = strArray;
            else
                aOut = [];
            end
        end

        % If only the first two arguments are there, this is a lookup
        if (nargin == 2)
            % Find the enumerated class in the map to grab the array
            if (false == theCache.isKey(enumClassName))
                % Hmm... cache is gone. Attempt to re-build:
                warning('BtcVariantEnum [%s] registration not found. attempting rebuild', ...
                    enumClassName);
                BtcVariantEnum.Workaround631(enumClassName);                
            end
            if (theCache.isKey(enumClassName))
                strArray= theCache(enumClassName);
                aOut = strArray(intValue);
            else
                validKeys = theCache.keys;
                error(['Cannot find configuration classes for ' ...
                    'enumeration [%s]. Registered enumeration classes:[%s]. ' ...
                    'Try clear classes? (close all models first)'], ...
                    enumClassName,strjoin(validKeys,','));
            end
        end

        % Otherwise, we're debugging. Return the entire cache.
        if (nargin < 2)
            aOut = theCache;
        end
    end
    %----------------------------------------------------------------------
    function Workaround631(enumClassName)
        % See [TALARIA-631] sometimes the ClassNameCache gets cleared but
        % the enumerated class(es) do not. There appears to be no way to
        % force a classdef to re-load, so just forget about that and
        % discover the data we need ourselves. Unstoppable.

        % Acquire the source code for the enumerated class which might be
        % in an M or P file. Just trust the which command. However note
        % that P files cannot be parsed, so the attempt will fail.
        codePath = which(enumClassName);
        theCode = fileread(codePath);

        % First grab the enumerated member definitions
        memberSearch = '(?<=^\s*enumeration).*?(?=\<end\>)';
        memberBlock = regexp(theCode,memberSearch,'lineanchors','match');
        if (isempty(memberBlock))
            % Failed.
            return;
        end
        memberBlock = memberBlock{1};

        % Next parse our the pieces we require
        %^\s*(?<Name>\w*)\s*\(\s*(?<Num>\d*)\s*,\s*'(?<ClassName>\w*)
        dataSearch = "\s*(?<Name>\w*)\s*\(\s*(?<Num>\d*)\s*,\s*'(?<ClassName>\w*)";
        theData = regexp(memberBlock,dataSearch,'lineanchors','names');

        for i=1:numel(theData)
            memberEntry = theData(i);
            BtcVariantEnum.ClassNameCache( ...
                enumClassName, ...
                str2double(memberEntry.Num), ...
                memberEntry.ClassName);
        end
    end
end
end

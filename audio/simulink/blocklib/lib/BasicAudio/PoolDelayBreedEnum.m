classdef PoolDelayBreedEnum < Simulink.IntEnumType
% PoolDelayEnum enumerates all types of PoolDelay variant select.
% Note: Please do not use string comparisons in code that goes through code 
% generation, for example in Matlab Function Blocks. String Comparisons ARE
% acceptable during build-time. In general, using EnumTypes is better
% practice.
%    Bad Practice:    Config.Breed == "Generic"
%    Good Practice:   Config.Breed == PoolDelayEnum.Generic

  enumeration
    Generic              ( 1)
    L1Memory             ( 2)
    L2Memory             ( 3)
  end

  methods
    function fragmentCell = GoalFragmentsFromModelHardware(blockHandle)
        % TODO: Discover the model's configured hardware device vendor
        % and device type. Map these to fragment(s) that would be
        % appropriate for optimizations on that hardware
        fragmentCell = {};
    end
    function fragmentCell = GoalFragmentsFromModelCrl(blockHandle)
        % TODO: Discover the model's code-replacement-library entries.
        % Presumably just the first one is used in mapping to fragments?
        % Not sure how to know which CRLs to map and which ones not to
        % map. Perhaps the first one recognized by the map is the one
        % that gets used.
        fragmentCell = {};
    end

    function itDoes = StartsWith(this,prefixStr)
        itDoes = false;
        prefixStr = char(prefixStr);
        thisChar = char(this);
        if (startsWith(thisChar,prefixStr,"IgnoreCase",true))
            itDoes = true;
        end
    end

    function itDoes = Contains(this,stringFragment)
        % Contains performs a case-insensitive comparison returning true if
        % the provided stringFragment is found in the name of this breed.
        itDoes = false;
        fragmentChar = char(stringFragment);
        thisChar = char(this);
        if (contains(thisChar,fragmentChar,"IgnoreCase",true))
            itDoes = true;
        end
    end      
  end
end
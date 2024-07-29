classdef AsdHwEnum < Simulink.IntEnumType
% AsdHwEnum enumerates all types of hardware-based optimization supported
% by the Asd blocklib block set.
% Note: Please do not use string comparisons in code that goes through code 
% generation, for example in Matlab Function Blocks. String Comparisons ARE
% acceptable during build-time. In general, using EnumTypes is better
% practice.
%    Bad Practice:    Config.Breed == "Hexagon"
%    Good Practice:   Config.Breed == AsdHwEnum.Hexagon

  enumeration
    Generic              ( 1)
    Hexagon              ( 2)
    ArmNeonV7V8          ( 3)
    HiFi5                ( 4)
    SharcPlus            ( 5)
    SharcPlusCore        ( 6)
    SharcPlusAccel       ( 7)
    SharcPlusCoreGriffin ( 8)
    SharcPlusCoreGul     ( 9)
    SharcPlusCoreGxp     (10)
    SharcPlusCoreGlxp    (11)
    SharcPlusAccelGul    (12)
    SharcPlusAccelGxp    (13)
    SharcPlusAccelGlxp   (14)
    Simulink             (15)
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
        % This function is supported in build time such as in Variant subsystems and BTC 
        % but should not be used in code-gen / product-code scenarios. 
        % String comparisions should be avoided in those scenarios.
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
        % This function is supported in build time such as in Variant subsystems and BTC 
        % but should not be used in code-gen / product-code scenarios. 
        % String comparisions should be avoided in those scenarios.
        itDoes = false;
        fragmentChar = char(stringFragment);
        thisChar = char(this);
        if (contains(thisChar,fragmentChar,"IgnoreCase",true))
            itDoes = true;
        end
    end      
  end
end
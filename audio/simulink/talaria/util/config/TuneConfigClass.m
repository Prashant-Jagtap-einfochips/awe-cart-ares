classdef TuneConfigClass < BtcLockAndDefaultClass
    %TUNECONFIGCLASS Base class for TuneConfig build-time-config classes
    %   This class is created only as part of a larger block-level BTC
    %   class. When allocating an instance of this class in a BTC class
    %   contructor, pass in the constructing object's handle e.g. "this".
    %   Offers Lock and Default capability and access to Parent. Remember
    %   that TuneConfig classes do not use Validate - instead use
    %   translation logic on canvas to ensure cross-property interactions.
    
    properties (SetAccess=immutable,GetAccess=protected)
        % Parent class expected to be a block-level BTC class
        Parent BtcBaseClass
    end
    
    methods
        function this = TuneConfigClass(parentObj)
            %TUNECONFIGCLASS Remember the parent object
            %   This parent object may be accessed via property "Parent"
            this.Parent = parentObj;
            this.EasyConstructor();
        end
        function EasyConstructor(this)
            % Called by constructor, override this to perform lock and
            % default without worrying about passing the parent object
        end
    end
end


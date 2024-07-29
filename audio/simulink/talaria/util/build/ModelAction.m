classdef ModelAction < int32
    %MODELACTION Indicates what action to take for partition and generate
    %   For partitioned cores and engines, can take no action, only create
    %   the model, or both model creation and code generation
    
enumeration
    Nothing             (0)
    OnlyCreateModel     (1)
    BothModelAndCode    (2)    
end

end


classdef TalariaApiBufferAccess < uint32
% TalariaApiBufferAccess Enumeration to specify access type for API buffers

% Copyright 2020 Bose Corporatino
enumeration
    Exported (1)
    ImportedDirect (2)
    ImportedPointer (3)
    ExportedPointer (4)
end

end
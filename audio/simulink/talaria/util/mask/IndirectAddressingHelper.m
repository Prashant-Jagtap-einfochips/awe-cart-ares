classdef IndirectAddressingHelper
%INDIRECTADDRESSINGHELPER Constructs indirect addressing structure
%   Determines if indirect addressing is enabled and if so searches to find
%   a block's group and partition numbers.
    
%   Copyright 2018-2019 Bose Corporation
    
    methods (Static)
        
        function [indirectAddressing,errCell] = Get(H,groupNumber,errCell)
            % Handle indirect addressing modes
            indirectAddressing.ok = true;
            indirectAddressing.isIndirect = CoreIndexMask.IsIndirectAddressing(H);
            if (indirectAddressing.isIndirect)
                if (MaskUtil.IsCommented(H))
                    % Blocks that are commented out do not get locators
                    indirectAddressing.ok = false;
                    indirectAddressing.groupNumber = -1;
                    indirectAddressing.partitionNumber = -1;
                else
                    % We already retrieved group number- make sure it's good
                    if (isempty(groupNumber) || groupNumber < 0)
                        errMsg = sprintf('Valid group number not found for %s', ...
                            MaskUtil.GetBlockErrorLink(H));
                        errCell{end+1} = errMsg;
                        MaskUtil.ReportError(errMsg);
                        indirectAddressing.ok = false;
                        indirectAddressing.groupNumber = -1;
                    else
                        indirectAddressing.groupNumber = double(groupNumber);
                    end
                    % Lookup the partition number also
                    partitionNumber = PartitionMask.GetPartitionNumber(H);
                    if (isempty(partitionNumber) || partitionNumber < 0)
                        errMsg = sprintf('Valid partition number not found for %s', ...
                            MaskUtil.GetBlockErrorLink(H));
                        errCell{end+1} = errMsg;
                        MaskUtil.ReportError(errMsg);
                        indirectAddressing.ok = false;
                        indirectAddressing.partitionNumber = -1;
                    else
                        indirectAddressing.partitionNumber = double(partitionNumber);
                    end
                end
            end
        end
        
    end
end


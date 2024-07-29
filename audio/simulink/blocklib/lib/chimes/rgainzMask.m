classdef rgainzMask
%rgainz_MASK This class performs mask operations for the rgainz block
%   To aid with debugging and change management, the code associated
%   with the rgainz mask has been placed here instead of in the mask
%   dialog itself.

%   Copyright 2018-2019 Bose Corporation
  methods (Static)
    function [MASK_RAMPER_INIT, MASK_CURRENT_VAL_INIT] = Init(H,maskNumRampers,maskInitialLinearGains)

        errCell = {};
        
        % Validate the initial gains vector
        [d1,d2] = size(maskInitialLinearGains);
        if (d1 ~= 1 || d2 ~= maskNumRampers)
            errCell{end+1} = sprintf(['Initial gains vector must be of ' ...
                'of size [%d,%d]. Current size is [%d,%d].'], ...
                1, maskNumRampers, d1, d2);
            % For now, fill in a valid value for use with the ramper init
            maskInitialLinearGains = ones(1,maskNumRampers);
        end
        
        % Prepare the initialization value for the ramper control bus
        fc = zeros(1,maskNumRampers);
        rc = ones(1,maskNumRampers);
        MASK_RAMPER_INIT = struct( ...
            'riseFrameCount', num2cell(fc), ...
            'fallFrameCount', num2cell(fc), ...
            'riseRampCoeff', num2cell(rc), ...
            'fallRampCoeff', num2cell(rc), ...
            'onFrameCount',num2cell(fc), ...
            'offFrameCount',num2cell(fc), ...            
            'targetGain', num2cell(maskInitialLinearGains), ...
            'currentGain', num2cell(maskInitialLinearGains), ...
            'isRisePhase', num2cell(fc), ...
            'isFallPhase', num2cell(fc) ...
            ); %#ok<*NOPRT>
        % ... Show or hide error box
          MaskUtil.ErrorBoxDisplay(H, errCell);
          
          MASK_CURRENT_VAL_INIT = MASK_RAMPER_INIT;
    end
    
    
  end % methods (Static)
    
end



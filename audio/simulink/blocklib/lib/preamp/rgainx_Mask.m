classdef rgainx_Mask
%RGAINX_MASK This class performs mask operations for the rgainx block
%   To aid with debugging and change management, the code associated
%   with the rgainx mask has been placed here instead of in the mask
%   dialog itself.

%   Copyright 2018-2019 Bose Corporation
  properties (Constant)
      UseSignalMap = 1;
      UseMaskMap = 2;
  end
    
  methods (Static)
    function [MASK_RAMPER_INIT] = Init(H,maskNumChannels,maskNumRampers,maskInitialiLinearGains,maskRamperMap,maskMapSource)

        errCell = {};
        
        % Validate rampers vs. channels
        if (maskNumChannels < maskNumRampers)
            errCell{end+1} = 'Having more rampers than channels is not very useful.';
        end
        
        % Validate the initial gains vector
        [d1,d2] = size(maskInitialiLinearGains);
        if (d1 ~= 1 || d2 ~= maskNumRampers)
            errCell{end+1} = sprintf(['Initial gains vector must be of ' ...
                'of size [%d,%d]. Current size is [%d,%d].'], ...
                1, maskNumRampers, d1, d2);
        end
        
        switch(maskMapSource)
            case rgainx_Mask.UseSignalMap
                replace_block(H,'FollowLinks','on','Name','RamperMap','BlockType','Constant','inport','noprompt');
            case rgainx_Mask.UseMaskMap
                replace_block(H,'FollowLinks','on','Name','RamperMap','BlockType','Inport','Constant','noprompt');
                blockPath = sprintf('%s/RamperMap',getfullname(H));
                MaskUtil.safe_setparam(blockPath,'Value','MaskRamperMap');
        end
        
        if (maskMapSource == rgainx_Mask.UseMaskMap)
            % Validate the dimensions maskRamperMap
            expectedLength = maskNumRampers + maskNumChannels;
            [d1,d2] = size(maskRamperMap);
            if (d1 ~= 1 || d2 ~= expectedLength)
                errCell{end+1} = sprintf(['Ramper-to-channel map must be a vector ' ...
                    'of size [%d,%d]. Current size is [%d,%d].'], ...
                    1, expectedLength, d1, d2);
            else
                % Validate the content of maskRamperMap
                % (ensure the counts are positive and add to the number of channels)
                maskRamperMap(1:maskNumRampers) = max(maskRamperMap(1:maskNumRampers), 0);
                ramperSum = sum( maskRamperMap(1:maskNumRampers) );
                if (ramperSum > maskNumChannels)
                    errCell{end+1} = sprintf(['The first %d values of the ramper-to-channel ' ...
                        'map must add to %d or less (i.e. the number of channels). Currently the sum is %d.'], ...
                        maskNumRampers, maskNumChannels, ramperSum);
                end
                % Ensure every channel is listed exactly once
                validRangeArray = max(maskRamperMap(maskNumRampers + 1:end), 1);
                noRepeatsArray = unique(validRangeArray);
                if (sum(1:maskNumChannels) ~= sum(noRepeatsArray))
                    errCell{end+1} = sprintf(['Ramper-to-channel map must list the index ' ...
                        'of every channel 1 through %d exactly once.'], maskNumChannels);
                end
            end
        end
        
        % Prepare the initialization value for the ramper control bus
        if (isempty(errCell))
            fc = zeros(1,maskNumRampers);
            rc = ones(1,maskNumRampers);
            MASK_RAMPER_INIT = struct( ...
                'frameCount', num2cell(fc), ...
                'rampCoeff', num2cell(rc), ...
                'targetGain', num2cell(maskInitialiLinearGains), ...
                'currentGain', num2cell(maskInitialiLinearGains) ...
                );
        else
            MASK_RAMPER_INIT = [];
        end

        % ... Show or hide error box
        MaskUtil.ErrorBoxDisplay(H, errCell);
    end
    %----------------------------------------------------------------------
    function CB_MaskMapSource(H)
        theMask = Simulink.Mask.get(H);
        maskRamperMap = theMask.getParameter('MaskRamperMap');
        maskMapSourceStr = get_param(H,'MaskMapSource');
        if (maskMapSourceStr(1) == 'A')
            maskRamperMap.Visible = 'off';
        else
            maskRamperMap.Visible = 'on';
        end        
    end
    %----------------------------------------------------------------------
    
  end % methods (Static)
    
end


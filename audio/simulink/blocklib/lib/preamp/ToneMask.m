classdef ToneMask
%ToneMask This class performs mask operations for the tone block
%   To aid with debugging and change management, the code associated
%   with the tone mask has been placed here instead of in the mask
%   dialog itself.
% Tone control works with two filters - one for boost and one for cut.
% Depending on the type of filtering, the coefficients are for either a
% single stage (low & high) or a 2 stage (midrange) filter. The gain
% adjustment controls the impact of the tone control - linear gain of 0
% implies passthrough (no impact). A negative linear gain activates the
% cut filter coefficients while a positive linear gain uses the boost
% coefficients.

%   Copyright 2018-2019 Bose Corporation
  properties (Constant = true)
      MaskType = 'Blocklib Tone';
      TypeLowpass = 1;
      TypeHighpass = 3;
      RampStages = 2;
  end
    
  methods (Static)
    function Init(H)
        % This block appears to replicate much of the Rgainy block - so it
        % must also create the bus and setup rampers like Rgainy:
        
        % Create rgain_bus datatype for our ramper DSM
        RgainBusUtil.Create(H);
        
        % Setup DataStoreMemories
        theDSMs = {'freeze control','phase control','ramp control'};
        CscUtil.SetupDsmsForStaticInit(H,theDSMs);
    end
    %--------------------------------------------------
    
    function CB_TriggerInit(H)

        % If the location has changed, force the block to run its init by
        % changing a parameter. This in turn causes the icon to refresh.
        pn = 'TriggerInit';
        loc = get_param(H,'Position');
        locStr = sprintf('[%d %d %d %d]',loc);
        MaskUtil.safe_setparam(H,pn,locStr);
    end
    
    %----------------------------------------------------------------------
    function outData = Transform1_3(inData)
    % Transform function automatically created by AutomateMask
       outData = MaskUtil.TransformFunctionRename(inData, ...
          {'MaskFeatureName'}, ...
          {'FeatureName'});
    end
    
  end % methods (Static)  
end


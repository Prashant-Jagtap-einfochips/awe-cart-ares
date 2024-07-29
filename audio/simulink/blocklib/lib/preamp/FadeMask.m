classdef FadeMask
%fade_MASK <ADD EXPLANATION>

%   Copyright 2018-2019 Bose Corporation
  properties (Constant = true)
      MaskType = 'Blocklib Fade'; % This name must match the library block name
  end
    
  methods (Static)
    
    %--------------------------------------------------
    function CB_TriggerInit(H)

        % If the location has changed, force the block to run its init by
        % changing a parameter. This in turn causes the icon to refresh.
        pn = 'TriggerInit';
        loc = get_param(H,'Position');
        locStr = sprintf('[%d %d %d %d]',loc);
        MaskUtil.safe_setparam(H,pn,locStr);
    end
    
    %--------------------------------------------------    
    function outData = Transform1_Fade(inData)
        % While rename the block from fade to Fade, also standardize the
        % feature path name.
        outData = MaskUtil.TransformFunctionRename(inData, ...
          {'MaskFeatureName'}, ...
          {'FeatureName'});
    end
    
  end % methods (Static)    
end
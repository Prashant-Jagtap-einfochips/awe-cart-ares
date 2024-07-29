classdef VolumeMask
%VolumeMask Audio Volume Mask
%   This block sets audio volume via a tunable gain map

%   Copyright 2018-2019 Bose Corporation
  properties (Constant = true)
    MaskType = 'Blocklib Volume';
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
    function outData = Transform1_68(inData)
    % Library transform function to rename FeaturePath and remove unused
    % packages and last-name change checking.
      afterRename = MaskUtil.TransformFunctionRename(inData, ...
          {'MaskFeatureName'}, ...
          {'FeatureName'});
      afterRename.InstanceData = afterRename.NewInstanceData;
      outData = MaskUtil.TransformFunctionDelete(afterRename, ...
          {'LastName','PackageName'});
    end
  
  end % methods (Static)
   
end


classdef BalanceMask
%balance_MASK This class performs mask operations for the balance block
%   To aid with debugging and change management, the code associated
%   with the tone mask has been placed here instead of in the mask
%   dialog itself.

%   Copyright 2018-2019 Bose Corporation
  properties (Constant = true)
      MaskType = 'Blocklib Balance'; % This name must match the library block name
  end
    
  methods (Static)
    function Init(H)

        % This block appears to replicate much of the Rgainy block - so it
        % must also create the bus and setup rampers like Rgainy:
        
        % Create rgain_bus datatype for our ramper DSM
        RgainBusUtil.Create(H);
        
        % Setup DataStoreMemories
        theDSMs = {'freeze control','ramp control'};
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

    %------------------------------------------%
    function outData = Transform1_128(inData)
        afterRename = MaskUtil.TransformFunctionRename(inData, ...
          {'MaskFeatureName'}, ...
          {'FeatureName'});
        afterRename.InstanceData = afterRename.NewInstanceData;
        outData = MaskUtil.TransformFunctionDelete(inData, ...
          {'MaskPackageNumber'});
    end
    
    %----------------------------------------------------------------------
    function outData = Transform1_8(inData)
    % Transform function automatically created by AutomateMask
       outData = MaskUtil.TransformFunctionRename(inData, ...
          {'MaskFeatureName'}, ...
          {'FeatureName'});
    end


  end % methods (Static)    
end
classdef FadeRamperMask
%fadeRamper_MASK <ADD EXPLANATION>

%   Copyright 2018-2019 Bose Corporation
  properties (Constant = true)
      MaskType = 'Blocklib FadeRamper';
  end
    
  methods (Static)
    %--------------------------------------------------
    function Init(H,enableGainOut)
        
        % Control whether output gain port is present
        % NOTE: when present, the output port is 2 (out of 3 total)
        outputPortNumber = '2';
        EnableBlockPort(H,'fadeGains',enableGainOut,outputPortNumber);

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
    %--------------------------------------------------
  end % methods (Static)    
end
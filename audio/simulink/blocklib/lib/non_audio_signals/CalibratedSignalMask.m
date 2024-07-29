classdef CalibratedSignalMask
%CalibratedSignalMask Calibrated Signal Mask
%   This block illustrates how one might create a re-useable mechanism to
%   define streaming scalar signals such as vehicle engine speed.
%   Internally the block creates a high priority RTC signal receiver as
%   well as two tuning variables and TOPS to coordinate calibration of
%   the signal.

%   Copyright 2018-2019 Bose Corporation
  properties (Constant = true)
    MaskType = 'Bose Calibrated Streaming Scalar Signal';
  end
  properties (Constant = true, Access = protected)
  end
    
  methods (Static)
    %----------------------------------------------------------------------
    function DoInit(H,Config)
        % Set the RTC request payload element 1 datatype... which is a
        % drop-list control and so cannot directly receive a variable name
        subBlockName = 'RTC Set';
        subBlockPath = MaskUtil.SubBlockName(H,subBlockName);
        subBlockHandle = getSimulinkBlockHandle(subBlockPath);
        if (ishandle(subBlockHandle))
            % The block is there! Set the datatype
            set_param(subBlockHandle,'REQElemType1',Config.SignalDataTypeName);
        end
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



  
  end % methods (Static)
   
end


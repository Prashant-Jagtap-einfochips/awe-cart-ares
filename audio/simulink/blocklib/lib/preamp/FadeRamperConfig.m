classdef FadeRamperConfig < BtcBaseClass
% FadeRamperConfig Build-time configuration class for the FadeRamper block
% Usage:
%    aConfig = FadeRamperConfig
%

%   Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)
    DefaultChanRampMap  = [1,1,1,1,1,1,2,2,2,2,2,2]
  end
  properties (Constant)
    NumChannelGroups = 2 % Channels are assigned to 'Front' or 'Rear'
    % There are a total of 2 rampers per channel group, hence 4 
    % rampers in this case. Two rampers each for the front and rear 
    % channel groups, and in each group, one ramper each for low and 
    % midrange+high frequency content since they are both faded 
    % differently.
    NumRampers = 4
  end
  properties (Dependent)
      DlgRamperInit
  end
  
  methods
    %----------------------------------------------------------------------
    function value = get.DlgRamperInit(this)
        % Use the InitalLinearGains and number of rampers to create an
        % initial structure value for the rampers
        initialGains = ones(1,this.NumRampers);
        value = RgainBusUtil.GetInitStruct(initialGains);
    end
  end
end

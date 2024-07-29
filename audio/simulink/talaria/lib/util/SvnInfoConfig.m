classdef SvnInfoConfig < BtcBaseClass
% SvnInfoConfig Build-time configuration class for the SvnInfo block
% Usage:
%    aConfig = SvnInfoConfig
%

%   Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)
      % DiagramChars for name of diagram (i.e. model or library name)
      DiagramChars = 32
      % RepoChars for name of SVN Repository
      RepoChars = 32
      % TagChars for name of SVN branch (limited to 2 folders deep)
      TagChars = 32
      % RevChars for string number of revision (i.e. "1234")
      RevChars = 32
  end
  methods
    %----------------------------------------------------------------------
    function this = SvnInfoConfig()
        % SvnInfoConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClass(this);

        % If there are no cross-property interactions, then this entire
        % method can be deleted!
    end

  end
end

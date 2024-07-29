function blocklib_project_setup()
%blocklib_project_setup  Simulink Project init script for blocklib
%
%   Copyright 2019 Bose Corporation

% Refresh simulink customizations to register CRL interfaces... 
% Note that since this explicit call is HERE, do *not* enable the
% refresh-customizations checkbox in the Simulink project startup options.
sl_refresh_customizations

% Check that Talaria version has features this module expects.
requiredTalariaVersion = "V2200BI"; % need TALARIA-808 ProcessUnitMap
% Based on the order of project references, possibly the Talaria module is
% not loaded yet! To account for this, just do the check when we can :-(
if (exist('talariaVersion','file'))
    currentTalariaVersion = talariaVersion;
    if (currentTalariaVersion < requiredTalariaVersion)
        error("This release of blocklib requires Talaria [%s] or later.\n" + ...
            "Talaria [%s] is too old", ...
            requiredTalariaVersion,currentTalariaVersion);
    end
else
    fprintf("-----\n" + ...
        "Simulink project references list Blocklib before Talaria,\n" + ...
        "so version check of [%s] or later could not be automated\n" + ...
        "-----\n", ...
        requiredTalariaVersion);
end

end

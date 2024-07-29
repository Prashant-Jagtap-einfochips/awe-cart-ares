function out = find_disabled_links(modelName,attemptToRestore)
%FIND_DISABLED_LINKS Perform DEEP search of ALL blocks to find disabled
%links
%   The model advisor checks only check if the local link is disabled...
%   not whether or not the referenced library itself has disabled links
%   inside it. This function *does* check inside libraries as well. This is
%   inefficient and redudant and all that - but it is intended to save time
%   compared to opening each library model and running the model advisor on
%   it to find disabled links. As an additional feature, this function can
%   optionally attempt to restore disabled links.
% Usage:
%    To discover disabled links:
%       find_disabled_links('modelName')
%
%    To discover and restore disabled links:
%       find_disabled_links('modelName', true)  

%   Copyright 2019 Bose Corporation
    if (nargin < 2)
        attemptToRestore = false;
    end
    load_system(modelName);
    F = Simulink.FindOptions("FollowLinks",true);
    allBlockHs = Simulink.findBlocks(modelName,F);
    % Switch to using string block paths since handles can shift on us as
    % blocks update.
    allBlocks = MaskUtil.MakeCell(getfullname(allBlockHs));
    numBlocks = numel(allBlocks);
    out = {};
    for i=1:numBlocks
        aBlock = allBlocks{i};
        
        % Chase down libraries within libraries
        linkStatus = get_param(aBlock,'LinkStatus');
        while (strcmp(linkStatus,'implicit'))
            aBlock = get_param(aBlock,'ReferenceBlock');
            if (ishandle(getSimulinkBlockHandle(aBlock)))
                linkStatus = get_param(aBlock,'LinkStatus');
            else
                linkStatus = '';
            end
        end
        if (strcmp(linkStatus,'inactive'))
            if (nargout == 0)
                disp(aBlock);
            else
                out{end + 1} = aBlock;
            end
            if (attemptToRestore)
                set_param(aBlock,'LinkStatus','restore');
            end
        end
    end


end


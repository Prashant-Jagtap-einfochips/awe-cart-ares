function  find_parameterized_links(modelName,attemptToRemove)
%find_parameterized_links Perform DEEP search of ALL blocks to find any
%parameterized links
%   To use this method to full effect, open a model first and update its
%   diagram to ensure all blocks are fully loaded and up to date. Then,
%   this method will find any parameterized library links present in the
%   model. As an additional feature, this function can optionally attempt
%   to remove parameterized links by destroying the extra parameterized
%   data associated with the link. However, without fixing the undelying
%   issue that is causing the parameterized link, the problem will recurr
%   anyway.
% Usage:
%    To discover parameterized links:
%       find_parameterized_links('modelName')
%
%    To find and remove parameterized links:
%       find_parameterized_links('modelName', true)  

%   Copyright 2019 Bose Corporation
    if (nargin < 2)
        attemptToRemove = false;
    end
    load_system(modelName);
    
    % Get all resolved library blocks
    F = Simulink.FindOptions("FollowLinks",true);
    linkblocks = Simulink.findBlocks(modelName,'LinkStatus','resolved',F);

    % Find any library blocks with non-blank LinkData
    linkData = get_param(linkblocks,'LinkData');
    hasData = ~cellfun('isempty',linkData);
    parameterizedLinkBlocks = linkblocks(hasData);
    numBlocks = numel(parameterizedLinkBlocks);
    for i=1:numBlocks
        aBlock = getfullname(parameterizedLinkBlocks(i));
        if (strcmp(aBlock,modelName))
            continue;
        end
        disp(MaskUtil.GetBlockLinkString(aBlock));
        if (attemptToRemove)
            set_param(aBlock,'LinkData',[]);
        end
    end


end


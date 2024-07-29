function EnableBlockPort(H, portBlockName, enable, portNumStr)
    % EnableBlockPort Based on block name (not full name), swaps in a
    % terminator/ground or an outport/inport. Optionally a port number
    % can be specified.
    % Usage:
    %   EnableBlockPort(H,portBlockName,enable,portNumStr);
    % H: handle/URL of parent subsystem containing the port
    % portBlockName : name of block that is the in/out port
    % enable : true if port is to be enabled, false to ground/terminate
    % portNumStr : optional port number as a char/string

    % First determine whether port is in/out
    blockPath = GetSubBlockPathOrEmpty(H,portBlockName);
    if (isempty(blockPath))
        % Bail out
        return;
    end
    blockType = get_param(blockPath,'BlockType');
    if ismember(lower(blockType),{'outport','terminator'})
        offBlock = 'Terminator';
        onBlock = 'Outport';
    else
        offBlock = 'Ground';
        onBlock = 'Inport';
    end

    if (enable)
        replace_block(H,'FollowLinks','on','SearchDepth','1','Name',portBlockName,'BlockType',offBlock,onBlock,'noprompt');
        if (nargin >= 4)
            set_param(blockPath,'Port',portNumStr);
        end
    else
        replace_block(H,'FollowLinks','on','SearchDepth','1','Name',portBlockName,'BlockType',onBlock,offBlock,'noprompt');
    end
end

function blockPath = GetSubBlockPathOrEmpty(H,portBlockName)
% GetSubBlockPathOrEmpty will either return a valid block path, emit an
% error if the block should be found but is not, or return empty [] if the
% block is not found but the current diagram is a Talaria engine model
    blockPath = MaskUtil.SubBlockName(H,portBlockName);
    blockHandle = getSimulinkBlockHandle(blockPath);
    if (false == ishandle(blockHandle))
        coreIndexRole = CoreIndexMask.GetCoreIndex(H);
        inEngine = (coreIndexRole > CoreIndexMask.SourceModel);
        if (false == inEngine)
            % Not in an engine and the block is not found. Not good.
            error("Block '%s' is not found",getfullname(blockPath));
        else
            % Block not found, but we're in an engine- return empty
            blockPath = [];
        end
    end
end
function TunableDelayMask(fcn_h, bufferAlias, headerFilename)

% Get block handle
bufferBlockPath = MaskUtil.SubBlockName(fcn_h, 'Data Store Memory');
bufferBlockHandle = getSimulinkBlockHandle(bufferBlockPath);

% If the block is in *OUR* home library just return
if MaskUtil.IsInBlockDiagram(fcn_h,'TunableDelay')
    return;
end

if ishandle(bufferBlockHandle)
    % The buffer block is present - check if the code-generation variable
    % name has been specified:
        
    if (false == isempty(bufferAlias))
        % If headerfile is specified, prefix it with model name
        if (false == isempty(headerFilename))
            theHeader = sprintf('%s_%s', ...
                getfullname(bdroot(bufferBlockHandle)), ...
                headerFilename);
        else
            theHeader = '';
        end
        
        CscUtil.SetDatastoreCsc(bufferBlockHandle, ...
            'Bose','ExportToFile', ...
            'Identifier', bufferAlias, ...
            'HeaderFile',char(theHeader));
    else
        % Ooops - CscUtil does not have a method to reset to Auto. Fix this is
        % the 2021b upgrade.
        MaskUtil.safe_setparam(bufferBlockHandle, 'StateStorageClass', 'Auto');
    end
end
            

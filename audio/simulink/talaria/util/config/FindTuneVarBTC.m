classdef FindTuneVarBTC < ModelDataXmlWriter
% FindTuneVarBTC Attempts to extract BTC for TuneVar default values
%   This method finds all TuneVar blocks and chases down the source of
%   their default value. A known limitation however is that all mask values
%   involved must be simple variable names which are defined as mask
%   parameters. Any additional computational expressions found such as
%   datatype casting, arithmetic functions, or referencing mask workspace
%   variables that are not mask parameters will cause this logic to give
%   up and consider the default value expression to be non-configurable.
%   Also note that current build-time variables must be found in the
%   model's external data source (e.g. base workspace or data dictionary)
%   as otherwise the script will again consider the default value
%   expression to be non-configurable.
% Usage:
%   FindTuneVarBTC(modelName,[filename]);

%   Copyright 2021 Bose Corporation

properties (Access=protected)
    FileNameSuffix = 'TuneVarDefaults'
    FileDescription = 'Variables defining TuneVar defaults'
end

methods
%--------------------------------------------------------------------------
function this = FindTuneVarBTC(modelName,filename,quiet)
    arguments
        modelName char
        filename char = []
        quiet logical = false
    end

    this@ModelDataXmlWriter(modelName,false);

    F = Simulink.FindOptions("FollowLinks",true);
    F.Variants =  'ActiveVariants';  % TODO [TALARIA-523]: Workaround
    blocks = Simulink.findBlocks(modelName,'MaskType',TuneVarMask.MaskType,F);

    numBlocks = numel(blocks);

    for blockIdx=1:numBlocks
        aTuneVarBlock = blocks(blockIdx);
        this.DumpTuneVar(aTuneVarBlock);
    end

    if (nargin < 2)
        filename = [];
    end
    this.WriteXml(filename,quiet);
end

%--------------------------------------------------------------------------
function DumpTuneVar(this,aTuneVarBlock)
    % DumpTuneVar researches the specified TuneVar block to find the source
    % of its default value. This miracle is (almost) achieved by peeling
    % back the layers of Simulink masks and accumulating the mask variable
    % sources on the way. A simplifying assumption is that all expressions
    % are only mask parameter names except the top-level variable found in
    % a model's external data source.
    % Usage:
    %   DumpTuneVar(this, aTuneVarBlock);
    
    % Skip this TuneVar if it is commented out
    if (MaskUtil.IsCommented(aTuneVarBlock))
        % Yes this one is commented- ignore it!
        return;
    end

    % Retrieve the Feature Path
    [~,tunePath] = TuneVarMask.IsTuneVar(aTuneVarBlock);

    % Now retrieve the source variable that feeds MaskValue. We attempt
    % this feat by keeping it simple and only supporting MaskValues
    % that meet the following limitation:
    %     variable name only - no math expressions
    %     Also - if under a mask, *must* use the mask workspace
    %
    % With this assumption, we can trace back variables through mask
    % workspaces until we arrive at an external data source (i.e. base
    % workspace or data dictionary)
    [variableName,foundInGlobal] = BTConfig.DiscoverSourceVariable(aTuneVarBlock,'MaskValue');
    blockUrl = GetSourceModelUrls(aTuneVarBlock);
    hasLockStr = '0';
    if (foundInGlobal)
        hasLockStr = IsPropertyLocked(this,variableName);
    end
    if (foundInGlobal && (hasLockStr == "0"))
        isConfigurable = '1';
    else
        isConfigurable = '0';
    end
    
    this.AppendElement(this.ElemRoot,'TuneVar',tunePath, ...
            {'configurable',isConfigurable, ...
            'default',variableName, ...
            'simulink_block_path',blockUrl, ...
            'has_lock',hasLockStr});
end

function hasLockStr = IsPropertyLocked(~,variableName)
    % Parses provided variablePathString and returns true if it represents
    % a property that is locked via BTC features
    hasLockStr = '0';
    try
        pathParts = strsplit(string(variableName),'.');
        if (numel(pathParts) > 1)
            ownerObjectName = pathParts(1:end-1).join('.');
            propName = pathParts(end);
            ownerObj = evalin('base',ownerObjectName);
            if (false == isempty(ownerObj) && isa(ownerObj,'BtcLockAndDefaultClass'))
                if (ownerObj.IsLockedOrDefaulted(propName))
                    hasLockStr = '1';
                end
            end
        end
    catch
        % Trouble - probably the variableName is some complex expression
        % Consider that "cannot even determine if it's locked", or "2"
        hasLockStr = '2';
    end
end

end %methods
end %classdef


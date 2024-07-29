function [foundLibraries,foundSourceBlocks,foundLibLinkBlocks] = FindLibLinksByFolder(libFolder,searchFolder)
%FINDLIBLINKSBYFOLDER Searches SLX files in searchFolder for library links
% to libraries residing in the libFolder
% Usage:
%    [fl,fsb,sllb] = FindLibLinksByFolder(libFolder,searchFolder)
% Input arguments
%    libFolder : relative or absolute path containing libraries
%    searchFolder : relative or absolute path containing models
% Outputs:
%   *If nargout (outputs are used/captued into variables) then results will
%    compile into the following string lists. Otherwise (outputs not used)
%    the process will print status as it goes along.
%
%    foundLibraries : string array of libraries in libFolder which are used
%    foundSourceBlocks : string array of specific blocks in those libraries 
%    foundLibLinkBlocks : string array of blocks in the searchFolder that
%       link to the foundSourceBlocks
arguments
    libFolder string
    searchFolder string = "./"
end

% Little trick to ensure the folders have the correct separators
libFolder = fullfile(libFolder);
searchFolder = fullfile(searchFolder);

foundLibraries= string.empty;
foundSourceBlocks = string.empty;
foundLibLinkBlocks = string.empty;

% First find all SLX files in the search folder

slxSearchExpression = fullfile(searchFolder,'**','*.slx');
dirEntries = dir(slxSearchExpression);

F = Simulink.FindOptions("FollowLinks",true,"RegExp",true);

% For each discovered SLX file, search for library links
for i=1:numel(dirEntries)
    anEntry = dirEntries(i);
    aFile = fullfile(anEntry.folder,anEntry.name);
    if (nargout == 0)
        fprintf('-------------\n%s ...\n',aFile);
    end

    % Load the diagram and search for library links
    aModel = load_system(aFile);
    activeLinks = Simulink.findBlocks(aModel,'LinkStatus','resolved',F);

    if (false == isempty(activeLinks))
        % Acquire the sources for all the links present
        activeLinkSources = string(get(activeLinks,'ReferenceBlock'));
    
        [foundLinkBlocks,toTheseSourceBlocks,inTheseLibraries] = ...
            FindMatchingBlocks(activeLinks,activeLinkSources,libFolder);
        if (false == isempty(foundLinkBlocks))
            foundLibraries = vertcat(foundLibraries,inTheseLibraries);
            foundSourceBlocks = vertcat(foundSourceBlocks,toTheseSourceBlocks);
            foundLibLinkBlocks = vertcat(foundLibLinkBlocks,foundLinkBlocks);
            if (nargout == 0)
                fprintf("Found references to these libraries:\n");
                fprintf('  %s\n',inTheseLibraries);
                fprintf("Specifically these blocks\n")
                fprintf('  %s\n',toTheseSourceBlocks);
                fprintf("From these ACTIVE lib links\n")
                fprintf('  %s\n',string(foundLinkBlocks));
            end
        end
    end

    brokenLinks = Simulink.findBlocks(aModel,'LinkStatus','inactive',F);
    if (false == isempty(brokenLinks))
        brokenLinkSources = string(get(brokenLinks,'AncestorBlock'));
        [foundLinkBlocks,toTheseSourceBlocks,inTheseLibraries] = ...
            FindMatchingBlocks(brokenLinks,brokenLinkSources,libFolder);
        if (false == isempty(foundLinkBlocks))
            foundLibraries = vertcat(foundLibraries,inTheseLibraries);
            foundSourceBlocks = vertcat(foundSourceBlocks,toTheseSourceBlocks);
            foundLibLinkBlocks = vertcat(foundLibLinkBlocks,foundLinkBlocks);
            if (nargout == 0)
                fprintf("Found references to these libraries:\n");
                fprintf('  %s\n',string(inTheseLibraries));
                fprintf("Specifically these blocks\n")
                fprintf('  %s\n',string(toTheseSourceBlocks));
                fprintf("From these DISABLED lib links\n")
                fprintf('  %s\n',string(foundLinkBlocks));
            end
        end
    end
end
  foundLibraries    = unique(foundLibraries    );
  foundSourceBlocks = unique(foundSourceBlocks );
  foundLibLinkBlocks= unique(foundLibLinkBlocks); 
end
function [foundLinkBlocks,toTheseSourceBlocks,inTheseLibraries] = ...
    FindMatchingBlocks(linkBlocks,linkSources,libFolder)

    % Simplify to just a unique library source block list
    [uniqueSourceBlocks,~,sourceBlockIndex] = SafeUnique(linkSources);

    % Acquire the source SLX file for all the links
    sourceBlockLibs = extractBefore(uniqueSourceBlocks,'/');

    % Simplify to just the unique libraries
    [uniqueSourceLibs,~,sourceLibIndex] = SafeUnique(sourceBlockLibs);

    % Find each library on the path
    sourcePaths = cellfun(@which,uniqueSourceLibs,'UniformOutput',false);

    % Determine if any are contained in the libFolder
    foundInLibFolderIndex = contains(sourcePaths,libFolder,'IgnoreCase',true);

    % If any were found, back-trace to their sources
    if (any(foundInLibFolderIndex))
        % Gather names of which libraries were found
        inTheseLibraries = uniqueSourceLibs(foundInLibFolderIndex);

        % Gather names of which source blocks in those found libraries
        foundLibIndexes = find(foundInLibFolderIndex);
        isUniqueBlockFound = ismember(sourceLibIndex,foundLibIndexes);
        toTheseSourceBlocks = uniqueSourceBlocks(isUniqueBlockFound);

        % Gather the names of which original blocks linked to those sources
        foundBlockIndexes = find(isUniqueBlockFound);
        isBlockFound = ismember(sourceBlockIndex,foundBlockIndexes);
        foundLinkBlocks = getfullname(linkBlocks(isBlockFound));
    else
        foundLinkBlocks = [];
        toTheseSourceBlocks = [];
        inTheseLibraries = [];
    end
    foundLinkBlocks = string(foundLinkBlocks);
    toTheseSourceBlocks = string(toTheseSourceBlocks);
    inTheseLibraries = string(inTheseLibraries);
end
function [A,IA,IC] = SafeUnique(items)
% Acquires unique list - correctly handling lists with only one item
    if (1 == numel(items))
        A = items;
        IA = 1;
        IC = 1;
    else
        [A,IA,IC] = unique(items);
    end
end
classdef SaveInactiveBlockPaths
% SaveInactiveBlockPaths Creates a base workspace listing of all inactive
%  blocks to be used for derivative model creation - e.g. engine models.
%  This list is necessary for partitioning because every time a model is
%  opened, the CompiledIsActive states revert to default 'on'. As the
%  translation and interpretation engine models are created, the variant
%  active state is critical to determine whether blocks should be allowed to
%  remain in the model or not.
%
%    Documentation for SaveInactiveBlockPaths
%       doc SaveInactiveBlockPaths
%
%
methods
function GetList
   %  GetList retrieves the list - if not found, it creates it first
end
function CreateList
   % CreateList analyzes provided model and save list to workspace
end
function SaveInactiveBlockPaths
   % SaveInactiveBlockPaths Creates a base workspace listing of all inactive
   %  blocks to be used for derivative model creation - e.g. engine models.
   %  This list is necessary for partitioning because every time a model is
   %  opened, the CompiledIsActive states revert to default 'on'. As the
   %  translation and interpretation engine models are created, the variant
   %  active state is critical to determine whether blocks should be allowed to
   %  remain in the model or not.
   %
   %    Documentation for SaveInactiveBlockPaths/SaveInactiveBlockPaths
   %       doc SaveInactiveBlockPaths
   %
end
end
end

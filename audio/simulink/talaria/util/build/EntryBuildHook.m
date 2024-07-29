classdef EntryBuildHook < BuildHook
%ENTRYBUILDHOOK 
%   Manages build hooks for the 'entry' phase

%   Copyright 2018-2019 Bose Corporation
    properties (Constant)
        event = 'entry';
    end
        
    methods (Static)
        function register(bh, hook, varargin)
            BuildHook.add_to_list(EntryBuildHook.event, bh, hook, varargin);
        end
        
        function deregister(bh, varargin)
            BuildHook.remove_from_list(EntryBuildHook.event, bh, varargin);
        end
        
        function execute(bh)
            BuildHook.execute_hook_list(EntryBuildHook.event, bh);
        end
        
        function prune(bh)
            BuildHook.prune_hook_list(EntryBuildHook.event, bh);
        end
    end
    
end

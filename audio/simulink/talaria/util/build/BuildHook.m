classdef BuildHook < handle
%BUILD_HOOK
%   See code below :)

%   Copyright 2018-2019 Bose Corporation
    properties
   
    end
    
    methods (Static)
        function add_to_list(build_event, bh, this_hook, varargin)
            if ~isempty(varargin)
                cleanup_hook = cell2mat(varargin{1});
            else
                cleanup_hook = [];
            end

            % Make sure it is a function handle
            if ~isa(this_hook, 'function_handle')
                warning('Invalid function handle type passed as build hook');
                return;
            end
            
            % Fetch the list from memory
            hooks = BuildHook.get_hooks(build_event, bh);
            
            % Get name and field used to determine name
            [name, field_used] = BuildHook.get_name(bh);
            
            % Assemble struct
            hook.handle = this_hook;
            hook.name = name;
            hook.cleanup_hook = cleanup_hook;
            hook.field_used = field_used;            
            
            % Check for duplicates
            is_duplicate = 0;
            for i = 1:length(hooks)
                if isequal(hooks(i), hook)
                    is_duplicate = 1;
                    break;
                end
            end
            
            % Store if it is not duplicate
            if ~is_duplicate
                if isempty(hooks)
                    hooks = hook;
                else
                    hooks(end + 1) = hook;
                end
                BuildHook.set_hooks(build_event, bh, hooks);
            end

        end
        
        function remove_from_list(build_event, bh, varargin)
            if ~isempty(varargin)
                this_hook = varargin{1};
            else
                this_hook = [];
            end
            
            % Grab the list
            hooks = BuildHook.get_hooks(build_event, bh);
            
            % Loop through and remove if the entry matches
            indices_to_remove = [];
            for i = 1:length(hooks)
                hook = hooks(i).handle;
                name = hooks(i).name;
                [name_to_remove, field_used] = BuildHook.get_name(bh);
                
                is_a_match = strcmp(name, name_to_remove) && strcmp(field_used, hooks(i).field_used);
                if ~isempty(this_hook)
                    is_a_match = is_a_match && isequal(hook, this_hook);
                end

                if is_a_match
                    % Remove it as long as no other blocks of this type
                    % exist in the top level system.  Note that this runs
                    % prior to the block being removed, so at least one
                    % still exists on the canvas, hence length(x < 2)
                    
                    blocks = BuildHook.find_blocks(name_to_remove, field_used);
                    if (length(blocks) < 2)
                        indices_to_remove = [indices_to_remove i];
                        f = hooks(i).cleanup_hook;
                        if ~isempty(f)
                            f();
                        end
                    end
                end
                if ~isempty(indices_to_remove)
                    hooks(indices_to_remove) = [];
                end
            end
            
            % Write the list back out
            BuildHook.set_hooks(build_event, bh, hooks);
        end
        
        function execute_hook_list(build_event, bh)
            % Loop through the list of hooks and executes each
            hooks = BuildHook.get_hooks(build_event, bh);
            for i = 1:length(hooks)
                f = hooks(i).handle;
                f();
            end
        end
        
        function prune_hook_list(build_event, bh)
            % Garbage collection for the build hook list
            indices_to_prune = [];
            hooks = BuildHook.get_hooks(build_event, bh);
            for i = 1:length(hooks)
                % Make sure there are objects in the system
                name = hooks(i).name;
                field_used = hooks(i).field_used;
                
                blocks = BuildHook.find_blocks(name, field_used);
                if isempty(blocks)
                    indices_to_prune = [indices_to_prune i];
                    cleanup = hooks(i).cleanup_hook;
                    if ~isempty(cleanup)
                        cleanup();
                    end
                end
            end
            
            if ~isempty(indices_to_prune)
                hooks(indices_to_prune) = [];
                BuildHook.set_hooks(build_event, bh, hooks);
            end
                   
        end
    end
    
    methods (Static, Access = protected)
        
        function hooks = get_hooks(event, bh)
            % Grab hook list from workspace. Initialize it if it doesn't
            % exist yet
            
            name = [event '_build_hooks_' bdroot];
            try
                hooks = evalin('base', name);
            catch
                assignin('base', name, []);
                hooks = evalin('base', name);
            end
        end
        
        function set_hooks(event, bh, hook)
            % Write hook list to workspace
            name = [event '_build_hooks_' bdroot];
            assignin('base', name, hook);
        end
        
        function [name, field] = get_name(bh)
            params = {'MaskType', 'ReferenceBlock', 'AncestorBlock'};
            name = '';
            for i = 1:length(params)
                field = params{i};
                name = get_param(bh, field);
                if ~isempty(name)
                    return;
                end
            end
            error('Could not find name associated with this block');
        end
        
        function blocks = find_blocks(name, field_used)
            blocks = find_system(bdroot(gcbh), field_used, name);
        end      
            
    end
    
end


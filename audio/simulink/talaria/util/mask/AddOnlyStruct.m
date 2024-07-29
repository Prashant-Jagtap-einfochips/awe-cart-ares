classdef AddOnlyStruct
%AddOnlyStruct build-time configuration class (Project Talaria)
%   This class assists with cultivating a full set of configuration
%   data in the form of a data structure (aka a MATLAB struct). It has
%   special behavior in that once an element of its internal "Defs"
%   struct has been set, that element will no longer accept new values.
%   This behavior is particularly useful for "filling in" any missing
%   configuration settings without overwriting those settings that have
%   been already provided.
% Usage:
%   sys = AddOnlyStruct()
%   sys.Defs.setting1 = 1.0;
%   sys.Defs.otherValue = 5;
%   sys.Defs.otherValue = 'something new';
%   disp(sys.Defs.otherValue)
%      ... returns "5"

%   Copyright 2018-2019 Bose Corporation
    
    properties
        Defs
    end
    properties (Access = protected, Dependent = true)
        theName
    end
    properties
        debugName
    end
    
    methods
        function obj = AddOnlyStruct(startingStruct,debugName)
            %AddOnlyStruct Construct the "add only" wrapper
            
            % Do not print debug message for the initial loading
            obj.debugName = [];
            
            % Ensure a starting struct is defined
            if (nargin < 1 || nargin > 0 && isempty(startingStruct))
                startingStruct = struct;
            end
            % Disallow any non-struct initial offerings
            if (false == isstruct(startingStruct))
                error('AddOnlyStruct constructor accepts a struct, not a [%s].', ...
                    class(startingStruct));
            else
                % LOAD UP THE INITIAL STRUCTURE
                obj.Defs = startingStruct;
            end
            % Setup debug messages for additions from this point onward
            if (nargin >= 2)
                obj.debugName = debugName;
            end
            
        end
        
        function theName = get.theName(obj)
            if (ishandle(obj.debugName))
                theName = getfullname(obj.debugName);
            else
                theName = obj.debugName;
            end
        end
        
        function obj = set.Defs(obj,newDefs)
            %set.Defs Special "only set if not already there" trick.
            %   This routine ensures that once an element of the Defs
            %   struct is set, that value will remain unchanged even though
            %   attempts to overwrite it.
            if (isempty(obj.Defs))
                obj.Defs = struct;
            end
            
            [obj.Defs,acceptNew] = AnyNewFields(obj,obj.Defs, newDefs, 'Defs.');
            if (acceptNew)
                % A decision was made to just grab it all - let's do it. 
                obj.Defs = newDefs;
            end
        end
    end
    methods (Access = protected)
        function [oldStruct,acceptNew] = AnyNewFields(obj,oldStruct, newStruct, prefix)
            acceptNew = false;
            newFields = fields(newStruct);
            oldFields = fields(oldStruct);
            theNew = setdiff(newFields,oldFields);
            if (false == isempty(theNew))
                % Something new was found!
                if (isempty(obj.theName))
                    % Nobody is watching. Just take the whole new struct
                    acceptNew = true;
                    return;
                else
                    % Debug is on. They are watching us. Let'em know.
                    for i=1:numel(theNew)
                        newVal = newStruct.(theNew{i});
                        if (isnumeric(newVal) || ischar(newVal) || isstring(newVal))
                            fprintf('%s adding [%s%s] as [%s]\n', ...
                                obj.theName, prefix, theNew{i}, mat2str(newVal));
                        else
                            fprintf('%s adding [%s%s] as <%s>\n', ...
                                obj.theName, prefix, theNew{i}, class(newVal));
                        end
                        oldStruct.(theNew{i}) = newVal;
                    end
                end
            end
            % If we made it down here, we have to check for sub-structs
            nonNew = setdiff(newFields, theNew);
            for i=1:numel(nonNew)
                if (isstruct(oldStruct.(nonNew{i})))
                    if (false == isstruct(newStruct.(nonNew{i})))
                        error('Attempt to replace struct with non-struct [%s%s]', ...
                            prefix,nonNew{i});
                    end
                    [oldStruct.(nonNew{i}),acceptNew] = AnyNewFields(obj, ...
                        oldStruct.(nonNew{i}), ...
                        newStruct.(nonNew{i}), ...
                        [prefix nonNew{i} '.']);
                    if (acceptNew)
                        return;
                    end
                end
            end
        end
    end
end


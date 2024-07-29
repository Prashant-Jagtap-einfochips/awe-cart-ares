classdef UpdateOnlyStruct
%UpdateOnlyStruct build-time configuration class (Project Talaria)
%   This class assists with cultivating a full set of configuration
%   data in the form of a data structure (aka a MATLAB struct). It has
%   special behavior in that once its internal "Defs" struct has been set,
%   no new structure elements can be added. This behavior is particularly
%   useful for modifying settings without accidentally mispelling a
%   structure field name or specifying out-of-date settings that are no
%   longer used.
%
% Usage:
%   avowedStruct.field = 1;
%   sys = UpdateOnlyStruct(avowedStruct)
%   sys.Defs.field = 1;
%   sys.Defs.fieeld= 2; <--- returns error since "fieeld" is not recognized

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
        function obj = UpdateOnlyStruct(startingStruct,debugName)
            %UpdateOnlyStruct Construct an empty instance
            %   Sets up instance for normal operation
            if (nargin < 2)
                obj.debugName = [];
            else
                obj.debugName = debugName;
            end
            if (false == isstruct(startingStruct))
                error('UpdateOnlyStruct constructor accepts a struct, not a [%s].', ...
                    class(startingStruct));
            else
                obj.Defs = startingStruct;
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
            %set.Defs Special "only set if already there" trick.
            %   This routine ensures that no new elements can be added to 
            %   the Defs struct.
            NoNewFields(obj,obj.Defs, newDefs, 'Defs.');
            % if no error was thrown then let's do it:
            obj.Defs = newDefs;
        end
    end
    methods (Access = protected)
        function NoNewFields(obj,oldStruct, newStruct, prefix)
            newFields = fields(newStruct);
            if (isempty(oldStruct))
                % the old struct has never been set. approve this first one
                if (numel(newFields) == 0)
                    error('Starting structure cannot be empty');
                end
                return;
            end
            oldFields = fields(oldStruct);
            theNew = setdiff(newFields,oldFields);
            if (false == isempty(theNew))
                % Something new was found!
                error('Unrecognized field encountered: [%s]', strjoin(string(prefix) + theNew));
            end
            % If we made it down here, we have to check for sub-structs
            for i=1:numel(oldFields)
                % Ensure that the datatype does not change
                oldElem = oldStruct.(oldFields{i});
                if (false == isfield(newStruct,oldFields{i}))
                    error('Attempt to remove field [%s%s]', ...
                        prefix,oldFields{i});
                end
                newElem = newStruct.(oldFields{i});
                if (false == strcmp(class(oldElem),class(newElem)))
                    error('Attempt to replace a [%s] with a [%s] for [%s%s]', ...
                        class(oldElem), class(newElem), ...
                        prefix,oldFields{i});
                end
                % Look deeper for any sub-structures
                if (isstruct(oldElem))
                    NoNewFields(obj, ...
                        oldElem, ...
                        newElem, ...
                        [prefix oldFields{i} '.']);
                end
            end
        end
    end
end


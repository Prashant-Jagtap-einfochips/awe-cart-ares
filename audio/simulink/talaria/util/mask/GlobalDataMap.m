classdef GlobalDataMap < DataMapUtil
%GlobalDataMap Utility class to map block-url to *global* workspace map
%   Using the block's full name as a key (with adjustments to support
%   using ModelRootName, saves a single value into a base workspace
%   containers.Map variable whose name does *NOT* depend on the containing model. 

%   Copyright 2018-2019 Bose Corporation

    methods (Static)
    end
    methods
        %--------------------------------------------------
        function this = GlobalDataMap(mapName)
            this@DataMapUtil(mapName,false);
        end
        %--------------------------------------------------
        function [mapName] = GetDataMapName(this,~)
            % This map is GLOBAL so the 
            mapName = this.MapNameRoot;
        end
        %--------------------------------------------------
    end
end


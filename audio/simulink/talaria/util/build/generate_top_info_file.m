function generate_top_info_file( model_name )
%

%   Copyright 2018-2019 Bose Corporation
% Handle input arguments
if nargin < 1
    model_name = 'test';
end

% place output file in same directory
fileName = get_param(model_name,'FileName');
[filepath,name,ext] = fileparts(fileName);
filename = fullfile(filepath,[model_name,'.yml']);
% filename = ['model/parts/' model_name '.yml'];

% Open the info file and fill out the header/initial text
fh = fopen(filename, 'w');
fprintf(fh, '# This file is auto-generated from %s.  Do not edit', ...
    mfilename);
fprintf(fh, '\r\n\r\n');
fprintf(fh, 'group list:\r\n');
fprintf(fh, '    list: True\r\n');
fprintf(fh, '    value:\r\n');

% Create the initial map data structure
tops = containers.Map();

% Find all the groups and their packages
group_list = find_system(model_name, 'LookUnderMasks','on', ...
    'MaskType', 'Bose Feature Path', 'isGroup', 'on');

for i = 1:length(group_list)
    % Get the list of packages
    packages = get_param(group_list{i}, 'packageNames');
    packages = strsplit(packages, ',');
    
    % Print the group name to file
    groupName = FeaturePathUtil.GetPath(group_list{i});
    fprintf(fh, '        - %s\r\n', groupName);
    
    % Add to the data struct
    tops(groupName) = containers.Map(packages,cell(size(packages)));
end
fprintf(fh,'\r\n');
    
% Initialize the string that will hold the top list, which will be printed
% to file later
top_list_string = 'top list:\r\n    value:\r\n';

% Find all the tops and add them to the correct structure
top_list = find_system(model_name, 'LookUnderMasks', 'all', ...
                       'MaskType', TOP_Mask.MaskType);

for i = 1:length(top_list)
    [featurePath, group, sub] = FeaturePathUtil.GetPath(top_list{i});
    package = packages{str2num(get_param(top_list{i}, 'PackageNumber'))};
    name = get_param(top_list{i}, 'TopName');
    if isKey(tops, group)
        package_map = tops(group);
        if isKey(package_map, package)
            % Add to the top data struct
            fullName = strcat(group, '_', package, '.', sub, name);
            contents = package_map(package);
            contents{end+1} = fullName;
            package_map(package) = contents;
            
            % Add to the top list string
            string = ['        ' fullName ':\r\n'];
            string = [string '            group: ' group '\r\n'];
            string = [string '            package: ' package '\r\n'];
            top_list_string = [top_list_string string];
        else
            error(['Cannot find package ' package ' in group ' featurePath]);
        end
    else
        error(['Cannot find group ' featurePath]);
    end
            
end

% Loop through the group list again to print packages/tops
k = keys(tops);
v = values(tops);
for i = 1:length(tops)
    fprintf(fh, '%s package list:\r\n', k{i});
    fprintf(fh, '    value:\r\n');  
    
    packages = keys(v{i});
    vars = values(v{i});
    for i = 1:length(packages)
        package = packages{i};
        names = vars{i};
        fprintf(fh, '        %s:\r\n', package);
        for j = 1:length(names)
            fprintf(fh, '            - %s\r\n', names{j});
        end
    end
    fprintf(fh,'\r\n');
end

fprintf(fh, top_list_string);
fclose(fh);
end


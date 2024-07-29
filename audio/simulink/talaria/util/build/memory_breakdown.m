function [ table, num_bytes ] = memory_breakdown( map_file, baseline, reset_baseline )
%MEMORY_BREAKDOWN Breakdown memory usage of model based on XML map file

%   Copyright 2018-2019 Bose Corporation
theStruct = parseXML(map_file);
link_project = theStruct(3).Children(2);
dxe_name = get_value_from_struct_array(link_project.Attributes, 'orig_file_name');

[~, name, ~] = fileparts(map_file);
if ~strcmp(dxe_name, [name '.dxe'])
    error('Unexpected data pulled from xml file');
end

nodes = link_project.Children;
memory_sections = [];
for i = 1:length(nodes)
    node = nodes(i);
    if strcmp(node.Name, 'MEMORY')
        memory_sections = [memory_sections node];
    end
end
mem_breakdown = get_memory_breakdown(memory_sections);
mem_breakdown = normalize_mem_breakdown(mem_breakdown, baseline, reset_baseline);
num_bytes = compute_total_bytes_used(mem_breakdown);
table = create_table(mem_breakdown);    

function mem_breakdown = get_memory_breakdown(memory_sections)
% Extracts memory information from xml data object of memory sections
% Output format is struct with the following fields:
% name, type, width, used, unused
mem_breakdown = struct;
for i = 1:length(memory_sections)
    mem = memory_sections(i);
    name = get_value_from_struct_array(mem.Attributes, 'name');
    typ = get_value_from_struct_array(mem.Attributes, 'type');
    
    width = get_value_from_struct_array(mem.Attributes, 'width');
    used = get_value_from_struct_array(mem.Attributes, 'words_used');
    unused = get_value_from_struct_array(mem.Attributes, 'words_unused');
    
    mem_breakdown(i).name = name;
    mem_breakdown(i).type = typ;
    mem_breakdown(i).width = hex2dec(width(3:end));
    mem_breakdown(i).used = hex2dec(used(3:end));
    mem_breakdown(i).unused = hex2dec(unused(3:end));
end

function num_bytes = compute_total_bytes_used(mem_breakdown)
bits = 0;
for i = 1:numel(mem_breakdown)
    bits = bits + mem_breakdown(i).width * mem_breakdown(i).used;
end
num_bytes = bits / 8;

function mem = normalize_mem_breakdown(mem, baseline_file, reset)
% The memory breakdown contains everything, including c run time and
% various standard libraries. Remove this by subtracting out a memory
% breakdown taken from everything but calling into the model
if nargin < 3
    reset = 0;
end

if reset
    memory_baseline = mem;
    save(baseline_file, 'memory_baseline');
else
    load(baseline_file, 'memory_baseline');
    
    for i = 1:numel(memory_baseline)
        baseline = memory_baseline(i);
        test_name = strcmp(baseline.name, mem(i).name);
        test_type = strcmp(baseline.type, mem(i).type);
        test_width = (baseline.width == mem(i).width);
        
        if (test_name && test_type && test_width)
            mem(i).used = mem(i).used - baseline.used;
        else
            error('Memory profile mismatch with baseline');
        end
    end
end

function table = create_table(mem_breakdown)

pm_used = 0;
dm_used = 0;
pm_unused = 0;
dm_unused = 0;

table = sprintf('       Memory       |  Type | Width | Used | Unused (Mbits)\n');
delim_line = '-----------------------------------------------------------\n';
table = [table delim_line];
fmt_str = ' %18s | %5s | %5s | %4.2f | %4.2f\n';
for i = 1:length(mem_breakdown)
    name = mem_breakdown(i).name;
    typ = mem_breakdown(i).type;
    
    width = mem_breakdown(i).width;
    used = mem_breakdown(i).used;
    unused = mem_breakdown(i).unused;
    bits_used = width * used;
    bits_unused = width * unused;
    mbits_used = bits_used / 2^20;
    mbits_unused = bits_unused / 2^20;
    if (strcmp(typ, 'PM') | strcmp(typ, 'SW'))
        pm_used = pm_used + mbits_used;
        pm_unused = pm_unused + mbits_unused;
    else
        dm_used = dm_used + mbits_used;
        dm_unused = dm_unused + mbits_unused;
    end
    
    line = sprintf(fmt_str, name, typ, width, mbits_used, mbits_unused);   
    table = [table line];
end
table = [table delim_line];
line = sprintf(fmt_str, 'Total Program', 'PM/SW', 'N/A', pm_used, pm_unused);
table = [table line];
line = sprintf(fmt_str, 'Total Data', 'DM', 'N/A', dm_used, dm_unused);
table = [table line];

function value = get_value_from_struct_array( struct_array, element )
value = [];
for i = 1:length(struct_array)
    s = struct_array(i);
    if strcmpi(s.Name, element)
        value = s.Value;
    end
end


% script replaces pcpreq pq4paramVector blocks with TOPs
% This script is out-of-date and should use TuneTOPs instead of TOPS

%   Copyright 2018-2019 Bose Corporation
maskType = 'pq4paramVector';
blks = find_system(bdroot(gcbh), 'MaskType', maskType);

for i = 1:length(blks)
    blk = blks(i);
    blk_name = get_param(blk, 'Name');
    top_name = erase(get_param(blk, 'rtpName'), '''');
    dataType = get_param(blk, 'rtpDataType');
    value = get_param(blk, 'rtpInit');
    path = get_param(blk, 'Parent');
    disp([num2str(i) ': ' top_name ' | ' dataType ' | ' value]);
        
    % keyboard
    this = [path '/' blk_name];
    replaced_blocks = replace_block(path, 'Name', blk_name, 'TOP/TOP', 'noprompt');
    set_param(this, 'Value', [ 'single(' value ')']);
    set_param(this, 'TopName', top_name);
    set_param(this, 'Name', top_name);
    
end

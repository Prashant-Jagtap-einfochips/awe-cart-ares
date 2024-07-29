% script adds trivial tune to TOP block

%   Copyright 2018-2019 Bose Corporation
% Find all the TOPs
maskType = 'Bose Target Overlay Parameter';
blks = find_system(bdroot(gcbh), 'LookUnderMasks', 'on', 'MaskType', maskType);

% If this is a library, unlock it
isLibrary = 0;
if strcmp(get_param(bdroot, 'Lock'), 'on')
    isLibrary = 1;
    set_param(bdroot, 'Lock', 'off');
end

% Step through each TOP
for i = 1:length(blks)
    % Get information about the existing block
    blk = blks(i);
    blk_name = get_param(blk, 'Name');
    ss_name = get_param(blk, 'Parent');
    position = get(blk, 'Position');
    value = get(blk, 'Value');
    set_param(blk, 'Role', 'Automatic');
    
    % Add a new translation block, set properties, and move it to the right place
    tx_blk_name = ['translation' num2str(i)];
    tx_block = add_block('TuneVar/TrivialTranslation', [ss_name '/' tx_blk_name], 'MakeNameUnique', 'on');
    tx_position = get(tx_block, 'Position');
    width = tx_position(3) - tx_position(1);
    heigth = tx_position(4) - tx_position(2);
    
    left_offset = 80;
    new_position = position;
    new_position(1) = new_position(1) - left_offset;
    new_position(3) = new_position(1) + width;
    new_position(4) = new_position(2) + heigth;
    set(tx_block, 'Position', new_position);
    tx_position = new_position;
    
    % Add a new tune var block, set properties, and move it to the right
    % place
    tune_blk_name = ['tune var' num2str(i)];
    tune_block = add_block('TuneVar/TuneVar', [ss_name '/' tune_blk_name], 'MakeNameUnique', 'on');
    tune_position = get(tune_block, 'Position');
    width = tune_position(3) - tune_position(1);
    heigth = tune_position(4) - tune_position(2);
    
    left_offset = 200;
    new_position = tx_position;
    new_position(1) = new_position(1) - left_offset;
    new_position(3) = new_position(1) + width;
    new_position(4) = new_position(2) + heigth;
    set(tune_block, 'Position', new_position);
    set(tune_block, 'TuneName', [blk_name '_tune_var']);
    set(tune_block, 'MaskValue', value);
    
    add_line(ss_name, [tune_blk_name '/1'], [tx_blk_name '/1']);
    add_line(ss_name, [tx_blk_name '/1'], [blk_name '/1']);
    
end

if isLibrary == 1
    set_param(bdroot, 'Lock', 'on');
end

    

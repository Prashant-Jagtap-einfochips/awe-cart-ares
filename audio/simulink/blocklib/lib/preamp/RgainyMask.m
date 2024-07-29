classdef RgainyMask
%RgainyMask This class performs mask operations for the Rgainy block
%   To aid with debugging and change management, the code associated
%   with the rgainy mask has been placed here instead of in the mask
%   dialog itself.

%   Copyright 2018-2019 Bose Corporation
  methods (Static)
    function Init(H,gainBlockName,gainOutEnable)
       
        % Set Gain output port enable/disable
        EnableBlockPort(H,gainBlockName,gainOutEnable);
        
        % Create rgain_bus datatype for our ramper DSM
        RgainBusUtil.Create(H);
        
        % Setup DataStoreMemories
        theDSMs = {'freeze control','ramp control'};
        CscUtil.SetupDsmsForStaticInit(H,theDSMs);
    end
	%----------------------------------------------------------------------
    function rampToChanMap = ConvertChan2RampIntoRamp2Chan(ChannelCount,MaskBlockH,Config)
        % Convert a channel-based map that lists rampers into a
        % ramper-based map that lists channel counts and channels. If the
        % provided channel map does not have enough values to specify the
        % ramper for every channel, just repeat the last channel's ramp
        % association for the rest of the unspecified channels.
        % UNLESS... error has specified to emit an error if the size of the
        % map does not strictly match the number of channels we have.
        emitError = Config.EnableStrictSizeMatch;
        MaskNumRampers = Config.NumRampers;
        MaskChan2RamperMap = Config.Chan2RamperMap;
       
        rampToChanMap = zeros(1,MaskNumRampers+ChannelCount,'uint8');

        % count how many channels each ramper has - including the "no ramper" #0
        numChan = zeros(1,MaskNumRampers+1);

        % create a full-sized matrix to hold channel indexes for each ramper...
        % with enough room for each one of them to own all the channels:
        ramperChannelMatrix = zeros(MaskNumRampers+1,ChannelCount);

        % loop through the tune variable that lists which ramper each channel uses.
        % tabulate the count for each ramper and place the channel index into the
        % ramperChannelMatrix
        inputMapWidth = numel(MaskChan2RamperMap);
        if (emitError && inputMapWidth ~= ChannelCount)
            error(['Channel-to-ramper map width [%d] does not match number ' ...
                'audio channels [%d] for block %s. (Disable this block''s option ' ...
                'to emit an error if you do not care about this mismatch)'], ...
                inputMapWidth, ChannelCount, MaskUtil.GetBlockLinkString(MaskBlockH));
        end
        for chan=1:ChannelCount
            if (chan <= inputMapWidth)
                r = MaskChan2RamperMap(chan);
            else
                % If not enough channels were provided, repeat the last
                % channel's mapping for the remaining unspecified channels.
                r = MaskChan2RamperMap(inputMapWidth);
            end

            % if channel is assigned not to ramp (ramper #0), then place it in the
            % last row of our matrix.
            if (r == 0)
                r = MaskNumRampers + 1;
            end

            numChan(r) = numChan(r) + 1;
            ramperChannelMatrix(r,numChan(r)) = chan;
        end

        % Create the output RamperMap. It starts with a count of channels for each
        % ramper, and then sequentially lists each ramper's channels. Note it also
        % includes the last row of the matrix to dump out non-ramping channels.
        % There is no count of these - they are just the left-overs.
        startIdx = MaskNumRampers;
        for r=1:(MaskNumRampers+1)
            if (r <= MaskNumRampers)
                rampToChanMap(r) = numChan(r);
            end
            for i=1:numChan(r)
                rampToChanMap(startIdx + i) = ramperChannelMatrix(r,i);
            end
            startIdx = startIdx + numChan(r);
        end        
    end
    %----------------------------------------------------------------------
    
    % Note - the mask for the Rgainy block was created interactively with
    % the Simulink GUI instead of being scripted as is common with more
    % complex blocks.
    
  end % methods (Static)
    
end



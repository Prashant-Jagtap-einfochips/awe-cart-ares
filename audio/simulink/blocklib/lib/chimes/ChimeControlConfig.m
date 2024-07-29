classdef ChimeControlConfig < BtcBaseClass
% ChimeControlConfig Build-time configuration class for the ChimeControl block
% Usage:
%    aConfig = ChimeControlConfig
%

%   Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)
    CheckGroupCount = 5 % = CHECK_GROUP_COUNT
    CheckPulseCount = 3 % = CHECK_PULSE_COUNT
    ChimeCancel = 1 % = chimeCancel
    ChimeEndNotification = [1 1 1 1] % = chimeEndNotification
    ChimeInputIndex = [1 2 3 4] % = chime_input_index
    ChimePlayPending = [0 0 0 0] % = chimePlayPending
    ChimeRouting = 0 % = chimeRouting
    ChimeType = [2 2 2 2] % = chimeType
    ChimecontrolNstates = 6 % = CHIMECONTROL_NSTATES
    Count = zeros(1,24) % = count
    CountDecrement = repmat([0 1 1 1 1 1],4) % = countDecrement
    CountState = zeros(1,24) % = countState
    DecrementVals = repmat([0 1 1 1 1 1],4) % = decrementVals
    EndOfFinalChime = [0 0 0 0] % = endOfFinalChime
    Fall = 2  % = FALL
    FallTime = [0.0450    0.0450    0.0450    0.0450]% = fallTime
    Forever = [1 1 1 1] % = forever
    FrameSizeSamples = 32 % = sys_defs.frameSize_samples
    FsHz = 480000 % = sys_defs.Fs_Hz
    FwAppendHostNotification = [-1 -1 -1 -1] % = FW_AppendHostNotification
    Idle = 0 % = IDLE
    Immediately = [0 0 0 0] % = immediately
    IncludeInternalChimeFlag = [1 1 1 1] % = include_internal_chime_flag
    IncludeInternalClickClack = 0 % = include_internal_click_clack
    NInternalBeepsGongs   % = nInternalBeepsGongs
    NStates = 6 % = NSTATES
    Nch = 4 % = nch
    NewProfile = 0 % = newProfile
    NextStateExpired = [0 2 3 4 5 0]  % = nextState_expired
    NextStateNotExpired = [0 1 2 1 4 1] % = nextState_notExpired
    NumChimeCount = 8 % = num_chime_count
    NumClickClack = 1 % = num_click_clack
    NumInpChs = 4 % = num_inp_chs
    NumInternalChime = 4 % = num_internal_chime
    NumPulses =  [3 3 3 3] % = numPulses
    OffTime = [0.5000    0.5000    0.5000    0.5000] % = offTime
    OffsetIntChime = 1 % = OffsetIntChime
    OnTime = [0.0300    0.0300    0.0300    0.0300] % = onTime
    OutputState = [1 1 1 0 1 0] % = outputState
    Period = [1 1 1 1]  % = period
    PlaybackFlag = [1 1 1 1] % = playback_flag
    RemainingCount = 0  % = remainingCount
    Rise = 1 % = RISE
    RiseTime = [0.0100    0.0100    0.0100    0.0100] % = riseTime
    SelectNumIntChimes = [1 2 3 4] % = SelectNumIntChimes
    SineTableLength = 1024 % = sys_defs.sineTableLength
    StartOfFinalChime = [0 0 0 0] % = startOfFinalChime
    State = [0 0 0 0] % = state
    ThresholdNumPulse = 1 % = thresholdNumPulse
    Volume = [224 224 224 224] % = volume
    WaitForPeriod = 4 % = WAIT_FOR_PERIOD
  end
  methods
    %----------------------------------------------------------------------
    function this = ChimeControlConfig()
        % ChimeControlConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClass(this);

        % If there are no cross-property interactions, then this entire
        % method can be deleted!
    end

  end
end

%        If nIn==2: L, R
%        If nIn==6: L, R, Ls, Rs, C, LFE
%        If nIn==8: L, R, Ls, Rs, C, LFE, Lrs, Rrs

InputMixerConfig.num_in_channels = numInputSelectEnt_out;
InputMixerConfig.num_out_channels = numInputSelectEnt_out;
InputMixerConfig.input_mix_weights_table =[
%L2L/R  ,C2L/R,  LFE2L/R,  Ls2Ls,  Rs2Rs,  Rs2Ls,  Ls2Rs,  Lrs2Ls,  Rrs2Rs,  Rrs2Ls,  Lrs2Rs  
    % BI_INPUT_MIXER_1:  (C) to (L, R)
    0    ,0.5  ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0   ... 
    % BI_INPUT_MIXER_2:  (C, LFE) to (L, R)
    0    ,0.5  ,0.5  ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ...
    % BI_INPUT_MIXER_3:  (L, R) to (L, R)
    1.0  ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0    ,0   ...
    % BI_INPUT_MIXER_4:  (L, R, C) to (L, R)
    0.66666666,0.33333333, 0   ,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0 ...
    % BI_INPUT_MIXER_5:  (L, R, LFE) to (L, R)
    0.66666666,0   ,0.33333333,0   ,0   ,0   ,0   ,0   ,0   ,0   ,0 ...
    % BI_INPUT_MIXER_6:  (L, R, C, LFE) to (L, R)
    0.5  ,0.25 ,0.25 ,0    ,0    ,0    ,0    ,0    ,0     ,0    ,0 ...
    % BI_INPUT_MIXER_7:  (L, R, Ls) to (L, R, Ls, Rs)
    1.0  ,0    ,0    ,0.5  ,0    ,0    ,0.5  ,0    ,0     ,0    ,0 ...
    % BI_INPUT_MIXER_8:  (L, R, Ls, Rs) to (L, R, Ls, Rs)
    1.0  ,0    ,0    ,1.0  ,1.0  ,0    ,0    ,0    ,0     ,0    ,0  ...
    % BI_INPUT_MIXER_9:  (L, R, Ls, Rs, Lrs) to (L, R, Ls, Rs)
    1.0  ,0    ,0    ,0.5  ,0.5  ,0    ,0    ,0.5  ,0     ,0    ,0.5 ...
    % BI_INPUT_MIXER_10: (L, R, Ls, Rs, Lrs, Rrs) to (L, R, Ls, Rs)
    1.0  ,0    ,0    ,0.5  ,0.5  ,0    ,0    ,0.5  ,0.5   ,0    , 0 ];

InputMixerConfig.default_mixer_type = 8;
InputMixerConfig.input_mixer_type  = InputMixerConfig.default_mixer_type;
InputMixerConfig.assigned_mixer_type = InputMixerConfig.input_mixer_type;
index = ((InputMixerConfig.input_mixer_type -1) * size(InputMixerConfig.input_mix_weights_table,2)) + 1;
InputMixerConfig.index = index;
InputMixerConfig.input_mix_weights_table = reshape(InputMixerConfig.input_mix_weights_table',1,numel(InputMixerConfig.input_mix_weights_table));
InputMixerConfig.InMixCoeffs = InputMixerConfig.input_mix_weights_table(InputMixerConfig.index:InputMixerConfig.index+10);

InputMixerConfig.INMIX_L2L_R2R     = 1.0;
InputMixerConfig.INMIX_LFE2L_LFE2R = 0; 
InputMixerConfig.INMIX_C2L_C2R     = 0; 
InputMixerConfig.INMIX_Rs2Rs       = 1.0; 
InputMixerConfig.INMIX_Ls2Rs       = 0; 
InputMixerConfig.INMIX_Ls2Ls       = 1.0; 
InputMixerConfig.INMIX_Rs2Ls       = 0; 
InputMixerConfig.INMIX_Rrs2Rs      = 1.0; 
InputMixerConfig.INMIX_Lrs2Rs      = 0; 
InputMixerConfig.INMIX_Lrs2Ls      = 1.0; 
InputMixerConfig.INMIX_Rrs2Ls      = 0; 

if(InputMixerConfig.num_in_channels > 2)
    InputMixerConfig.INMIX_LFE2L_LFE2R = InputMixerConfig.InMixCoeffs(3);
    InputMixerConfig.INMIX_C2L_C2R     = InputMixerConfig.InMixCoeffs(2);

    InputMixerConfig.INMIX_Rs2Rs       = InputMixerConfig.InMixCoeffs(5);
    InputMixerConfig.INMIX_Ls2Rs       = InputMixerConfig.InMixCoeffs(7);
    InputMixerConfig.INMIX_Ls2Ls       = InputMixerConfig.InMixCoeffs(4);
    InputMixerConfig.INMIX_Rs2Ls       = InputMixerConfig.InMixCoeffs(6);

end

if (InputMixerConfig.num_in_channels > 6)
    InputMixerConfig.INMIX_Rrs2Rs       = InputMixerConfig.InMixCoeffs(9);
    InputMixerConfig.INMIX_Lrs2Rs       = InputMixerConfig.InMixCoeffs(11);
    InputMixerConfig.INMIX_Lrs2Ls       = InputMixerConfig.InMixCoeffs(8);
    InputMixerConfig.INMIX_Rrs2Ls       = InputMixerConfig.InMixCoeffs(10);
else
    InputMixerConfig.INMIX_Rrs2Rs       = 0;
    InputMixerConfig.INMIX_Lrs2Rs       = 0;
    InputMixerConfig.INMIX_Lrs2Ls       = 0;
    InputMixerConfig.INMIX_Rrs2Ls       = 0;
end

MixerMatrix = ...
... %  L                    R                        Ls                      Rs                         C                        BASS                           Lrs                       Rrs                    OUTPUTS
[ InputMixerConfig.INMIX_L2L_R2R  0                        0                       0                         InputMixerConfig.INMIX_C2L_C2R  InputMixerConfig.INMIX_LFE2L_LFE2R   0                         0;                        %L
       0                    InputMixerConfig.INMIX_L2L_R2R 0                       0                         InputMixerConfig.INMIX_C2L_C2R  InputMixerConfig.INMIX_LFE2L_LFE2R   0                         0;                        %R
       0                    0                        InputMixerConfig.INMIX_Ls2Ls  InputMixerConfig.INMIX_Rs2Ls    0                         0                              InputMixerConfig.INMIX_Lrs2Ls  InputMixerConfig.INMIX_Rrs2Ls;   %Ls
       0                    0                        InputMixerConfig.INMIX_Ls2Rs  InputMixerConfig.INMIX_Rs2Rs    0                         0                              InputMixerConfig.INMIX_Lrs2Rs  InputMixerConfig.INMIX_Rrs2Rs;   %Rs
       0                    0                        0                       0                         1.0                       0                              0                         0;                        %C
       0                    0                        0                       0                         0                         1.0                            0                         0];                       %Bass

fMixerMatrix = MixerMatrix(:,1:InputMixerConfig.num_out_channels);
%For Bernstein testing, we use simple Identity matrix
% fMixerMatrix = eye(InputMixerConfig.num_in_channels,InputMixerConfig.num_out_channels);
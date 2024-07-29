function Config = ConvertLegacyPoolIirAccelToVariant(Variant, FrameSize, NumChannels, MaxNumStages, NumStages, SosCoefficients)
    % Construct a new object and port over the computed values from old instance of PooliirAccel block
    Config = PoolIirTunableConfig;
    Config.FrameSize = FrameSize;
    Config.NumChannels = NumChannels;
    Config.MaxNumStages = MaxNumStages;
    Config.TuneDefaults.NumStages = NumStages;
    Config.TuneDefaults.SosCoefficients = SosCoefficients;
    
    if(strcmp(Variant, "Core"))
            Config.SelectVariant('SharcPlusCore');
        elseif(strcmp(Variant, "Accel"))
            Config.SelectVariant('SharcPlusAccelGul');
        else
            error('Unidentified variant encountered in PoolIirAccel instance at "%s"', gcb);
    end
    
    % Notify the user about the BTC upgrade of PooliirAccel and request the
    % user to complete the remaining aspects of BTC upgrade which involves
    % switching from init script/function to using Config classes for all
    % library blocks and Models
    warning(['''%s'' has been upgraded to use the ''Sharc'' variant of PoolIirTunable. Since PoolIirTunable is a BTC block,' ...
        ' please upgrade the initialization associated with ''%s'' to a Config class at the earliest.'], gcb, gcb);
    
end
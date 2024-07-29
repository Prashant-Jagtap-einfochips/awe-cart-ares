function pgo = partitionGenOptCfg()
    %hifi4Config = ConfigSetHifi4()
    %sharcConfig = ConfigSetSharc()
    %sharcPlusConfig = ConfigSetSharcPlus()
    %sharcPlusGXPConfig = ConfigSetSharcPlusGXP()
    x86Config = ConfigSetx86();

    pgo = PartitionGenerateOptions('-i32 -t32');
    pgo.CoreTagToConfigSetMap("default") = x86Config;
end


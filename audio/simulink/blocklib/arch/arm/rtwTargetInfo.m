function rtwTargetInfo(cm)

cm.registerTargetInfo(@loc_register_crl);

function this = loc_register_crl

this(1) = RTW.TflRegistry;
this(1).Name = 'ARM Neon V7/V8 code replacement library';
this(1).TableList = {'ArmCodeReplacementLibraryTable'};
this(1).BaseTfl = '';
this(1).TargetHWDeviceType = {'*'};
this(1).Description = 'Code replacement library for ARM Cortex A Processors';

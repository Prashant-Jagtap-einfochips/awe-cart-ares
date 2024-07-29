function rtwTargetInfo(cm)

cm.registerTargetInfo(@loc_register_crl);

function this = loc_register_crl

this(1) = RTW.TflRegistry;
this(1).Name = 'Qualcomm Hexagon DSP code replacement library';
this(1).TableList = {'HexagonCodeReplacementLibraryTable'};
this(1).BaseTfl = '';
this(1).TargetHWDeviceType = {'*'};
this(1).Description = 'Code replacement library for Qualcomm Hexagon DSP';

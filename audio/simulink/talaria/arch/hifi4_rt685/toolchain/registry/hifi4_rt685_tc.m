function tc = hifi4_rt685_tc(ToolChainName)
%INTEL_TC Creates an Intel v12.1 ToolchainInfo object.
% This file can be used as a template to define other toolchains on Windows.

% Copyright 2012-2013 The MathWorks, Inc.

tc = coder.make.ToolchainInfo('BuildArtifact', 'gmake makefile', 'SupportedLanguages', {'Asm/C/C++'});
tc.Name             = ToolChainName;
tc.Platform         = 'win64';
tc.SupportedVersion = '2.3';

% Setting this attribute appears to prevent Simulink from including
% several unused/unrelated *.c files in the project source dir
tc.addAttribute('TransformPathsWithSpaces');

% ------------------------------
% Macros
% ------------------------------
tc.addMacro('SHELL','C:\Windows\System32\cmd.exe');

tc.addIntrinsicMacros({'XTENSA_DIR'});

% ------------------------------
% C Compiler
% ------------------------------
tool = tc.getBuildTool('C Compiler');

tool.setName(           'Xtensa GNU C Compiler');
tool.setCommand(        'xt-xcc');
tool.setPath(           '$(XTENSA_DIR)');

tool.setDirective(      'IncludeSearchPath',    '-I');
tool.setDirective(      'PreprocessorDefine',   '-D');
tool.setDirective(      'OutputFlag',           '-o');
tool.setDirective(      'Debug',                '-g');

tool.setFileExtension(  'Source',               '.c');
tool.setFileExtension(  'Header',               '.h');
tool.setFileExtension(  'Object',               '.o');

% ------------------------------
% C++ Compiler
% ------------------------------

tool = tc.getBuildTool('C++ Compiler');

tool.setName(           'Xtensa GNU C++ Compiler');
tool.setCommand(        'xt-xcc');
tool.setPath(           '$(XTENSA_DIR)');

tool.setDirective(      'IncludeSearchPath',  	'-I');
tool.setDirective(      'PreprocessorDefine', 	'-D');
tool.setDirective(      'OutputFlag',           '-o');
tool.setDirective(      'Debug',                '-g');

tool.setFileExtension(  'Source',               '.cpp');
tool.setFileExtension(  'Header',               '.h');
tool.setFileExtension(  'Object',               '.o');

% ------------------------------
% Assembler
% ------------------------------

tool = tc.getBuildTool('Assembler');

tool.setName(           'Xtensa GNU Assembler');
tool.setCommand(        'xt-as');
tool.setPath(           '$(XTENSA_DIR)');

tool.setDirective(      'IncludeSearchPath',  	'-I');
tool.setDirective(      'PreprocessorDefine', 	'-D');
tool.setDirective(      'OutputFlag',           '-o');
tool.setDirective(      'Debug',                '-g');

tool.setFileExtension(  'Source',               '.asm');
tool.setFileExtension(  'Object',               '.o');


% ------------------------------
% Linker
% ------------------------------

tool = tc.getBuildTool('Linker');

tool.setName(           'Xtensa GNU C/C++ Linker');
tool.setCommand(        'xt-ld');
tool.setPath(           '$(XTENSA_DIR)');

tool.setDirective(      'Library',              '-L');
tool.setDirective(      'LibrarySearchPath',    '-l');
tool.setDirective(      'OutputFlag',           '-o');
tool.setDirective(      'Debug',                '-g');
tool.addDirective(      'LinkerFile',           {''});
tool.setDirective(      'LinkerFile',           '-T');

tool.setFileExtension(  'Executable',           '.elf');
tool.setFileExtension(  'Shared Library',       '.a');

% ------------------------------
% C++ Linker
% ------------------------------

tool = tc.getBuildTool('C++ Linker');

tool.setName(           'Xtensa GNU C/C++ Linker');
tool.setCommand(        'xt-ld');
tool.setPath(           '$(XTENSA_DIR)');

tool.setDirective(      'Library',              '-L');
tool.setDirective(      'LibrarySearchPath',    '-l');
tool.setDirective(      'OutputFlag',           '-o');
tool.setDirective(      'Debug',                '-g');
tool.addDirective(      'LinkerFile',           {''});
tool.setDirective(      'LinkerFile',           '-T');

tool.setFileExtension(  'Executable',           '.elf');
tool.setFileExtension(  'Shared Library',       '.a');

% ------------------------------
% Archiver
% ------------------------------

tool = tc.getBuildTool('Archiver');

tool.setName(           'Xtensa GNU Archiver');
tool.setCommand(        'xt-ar');
tool.setPath(           '$(XTENSA_DIR)');
tool.setDirective(      'OutputFlag',           '');
tool.setFileExtension(  'Static Library',       '.a');

% ------------------------------
% Download
% ------------------------------

% ------------------------------
% Builder
% ------------------------------

tc.setBuilderApplication(tc.Platform);

% --------------------------------------------
% BUILD CONFIGURATIONS
% --------------------------------------------

optimsOffOpts    = {'-O0'};
optimsOnOpts     = {'-O3'}; % O3 allows auto vectorization with -LNO:simd
cCompilerOpts    = {'-c'};
cppCompilerOpts  = cCompilerOpts;
assemblerOpts    = {''};
linkerOpts       = {'-static', ...
                    '-mlsp=C:/nxp/SDK_RT685/devices/MIMXRT685S/xtensa/gdbio'
                    };
sharedLinkerOpts = {'-shared'};
archiverOpts     = {''};

defineOpts = {'-DSDK_DEBUGCONSOLE=1',...
              '-DXOS_CLOCK_FREQ=594000000',...
              '-DCPU_MIMXRT685SFVKB_dsp',...
              '-DMCUXPRESSO_SDK',...
              '-LNO:simd'};

xtensaConfigOpts = {'--xtensa-system=C:/usr/xtensa/XtDevTools/install/builds/RI-2020.5-win32/nxp_rt600_RI2020_5_newlib/config/', ...
                    '--xtensa-core=nxp_rt600_RI2020_5_newlib'};

includeDirOpts = {'-IC:/nxp/SDK_RT685/devices/MIMXRT685S/drivers',...
                  '-IC:/nxp/SDK_RT685/components/lists',...
                  '-IC:/nxp/SDK_RT685/devices/MIMXRT685S',...
                  '-IC:/nxp/SDK_RT685/CMSIS/Core/Include',...
                  '-IC:/nxp/SDK_RT685/middleware/dsp/naturedsp/hifi4/include',...
                  '-IC:/nxp/SDK_RT685/middleware/dsp/naturedsp/hifi4/include_private'};

% Get the debug flag per build tool
debugFlag.CCompiler   = '-g -DDEBUG';   
debugFlag.CppCompiler = '-g -DDEBUG';
debugFlag.Assembler   = '-g -DDEBUG';
debugFlag.Linker      = '-g';  
debugFlag.CppLinker   = '-g';
debugFlag.Archiver    = '';

% Set the toolchain flags for build configurations
cfg = tc.getBuildConfiguration('Faster Builds');
cfg.setOption( 'C Compiler',                horzcat(cCompilerOpts, defineOpts,xtensaConfigOpts, includeDirOpts,  optimsOnOpts));
cfg.setOption( 'C++ Compiler',              horzcat(cppCompilerOpts, defineOpts,xtensaConfigOpts, includeDirOpts, optimsOnOpts));
cfg.setOption( 'Assembler',                 horzcat(assemblerOpts, defineOpts,xtensaConfigOpts, includeDirOpts));
cfg.setOption( 'Linker',                    horzcat(xtensaConfigOpts, linkerOpts));
cfg.setOption( 'C++ Linker',                horzcat(xtensaConfigOpts, linkerOpts));
cfg.setOption( 'Shared Library Linker',     horzcat(xtensaConfigOpts, sharedLinkerOpts));
cfg.setOption( 'C++ Shared Library Linker', horzcat(xtensaConfigOpts, sharedLinkerOpts));
cfg.setOption( 'Archiver',                  archiverOpts);

% Set the toolchain flags for build configurations
cfg = tc.getBuildConfiguration('Faster Runs');
cfg.setOption( 'C Compiler',                horzcat(cCompilerOpts, defineOpts,xtensaConfigOpts, includeDirOpts,  optimsOnOpts));
cfg.setOption( 'C++ Compiler',              horzcat(cppCompilerOpts, defineOpts,xtensaConfigOpts, includeDirOpts, optimsOnOpts));
cfg.setOption( 'Assembler',                 horzcat(assemblerOpts, defineOpts,xtensaConfigOpts, includeDirOpts));
cfg.setOption( 'Linker',                    horzcat(xtensaConfigOpts, linkerOpts));
cfg.setOption( 'C++ Linker',                horzcat(xtensaConfigOpts, linkerOpts));
cfg.setOption( 'Shared Library Linker',     horzcat(xtensaConfigOpts, sharedLinkerOpts));
cfg.setOption( 'C++ Shared Library Linker', horzcat(xtensaConfigOpts, sharedLinkerOpts));
cfg.setOption( 'Archiver',                  archiverOpts);

cfg = tc.getBuildConfiguration('Debug');
cfg.setOption( 'C Compiler',                horzcat(cCompilerOpts,defineOpts, xtensaConfigOpts, includeDirOpts, optimsOffOpts, debugFlag.CCompiler));
cfg.setOption( 'C++ Compiler',              horzcat(cppCompilerOpts,defineOpts, xtensaConfigOpts, includeDirOpts, optimsOffOpts, debugFlag.CppCompiler));
cfg.setOption( 'Assembler',                 horzcat(assemblerOpts, defineOpts, xtensaConfigOpts, includeDirOpts, debugFlag.Assembler));
cfg.setOption( 'Linker',                    horzcat(xtensaConfigOpts, linkerOpts, debugFlag.Linker));
cfg.setOption( 'C++ Linker',                horzcat(xtensaConfigOpts, linkerOpts, debugFlag.CppLinker));
cfg.setOption( 'Shared Library Linker',     horzcat(xtensaConfigOpts, sharedLinkerOpts, debugFlag.Linker));
cfg.setOption( 'C++ Shared Library Linker', horzcat(xtensaConfigOpts, sharedLinkerOpts, debugFlag.Linker));
cfg.setOption( 'Archiver',                  horzcat(archiverOpts,     debugFlag.Archiver));

tc.setBuildConfigurationOption('all', 'Make Tool',     '-f $(MAKEFILE)');

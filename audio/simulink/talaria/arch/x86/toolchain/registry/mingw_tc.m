function tc = mingw_tc(ToolChainName)
%INTEL_TC Creates an Intel v12.1 ToolchainInfo object.
% This file can be used as a template to define other toolchains on Windows.

% Copyright 2012-2013 The MathWorks, Inc.

tc = coder.make.ToolchainInfo('BuildArtifact', 'gmake makefile', 'SupportedLanguages', {'Asm/C/C++'});
tc.Name             = ToolChainName;
tc.Platform         = 'win64';
tc.SupportedVersion = '5.3';

% Setting this attribute appears to prevent Simulink from including
% several unused/unrelated *.c files in the project source dir
tc.addAttribute('TransformPathsWithSpaces');

% ------------------------------
% Macros
% ------------------------------
tc.addMacro('SHELL','C:\Windows\System32\cmd.exe');

tc.addIntrinsicMacros({'MW_MINGW64_LOC'});

% ------------------------------
% C Compiler
% ------------------------------
tool = tc.getBuildTool('C Compiler');

tool.setName(           'Mingw C Compiler');
tool.setCommand(        'gcc');
tool.setPath(           '$(MW_MINGW64_LOC)\bin');

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

tool.setName(           'Mingw C++ Compiler');
tool.setCommand(        'g++');
tool.setPath(           '$(MW_MINGW64_LOC)\bin');

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

tool.setName(           'Mingw Assembler');
tool.setCommand(        'as');
tool.setPath(           '$(MW_MINGW64_LOC)');

tool.setDirective(      'IncludeSearchPath',  	'-I');
tool.setDirective(      'PreprocessorDefine', 	'-D');
tool.setDirective(      'OutputFlag',           '-o');
tool.setDirective(      'Debug',                '-g');

tool.setFileExtension(  'Source',               '.s');
tool.setFileExtension(  'Object',               '.o');


% ------------------------------
% Linker
% ------------------------------

tool = tc.getBuildTool('Linker');

tool.setName(           'Mingw C/C++ Linker');
tool.setCommand(        'gcc');
tool.setPath(           '$(MW_MINGW64_LOC)\bin');

tool.setDirective(      'Library',              '-L');
tool.setDirective(      'LibrarySearchPath',    '-l');
tool.setDirective(      'OutputFlag',           '-o');
tool.setDirective(      'Debug',                '-g');

tool.setFileExtension(  'Executable',           '.exe');
tool.setFileExtension(  'Shared Library',       '.dll');

% ------------------------------
% C++ Linker
% ------------------------------

tool = tc.getBuildTool('C++ Linker');

tool.setName(           'Mingw C/C++ Linker');
tool.setCommand(        'g++');
tool.setPath(           '$(MW_MINGW64_LOC)\bin');

tool.setDirective(      'Library',              '-L');
tool.setDirective(      'LibrarySearchPath',    '-l');
tool.setDirective(      'OutputFlag',           '-o');
tool.setDirective(      'Debug',                '-g');

tool.setFileExtension(  'Executable',           '.exe');
tool.setFileExtension(  'Shared Library',       '.dll');

% ------------------------------
% Archiver
% ------------------------------

tool = tc.getBuildTool('Archiver');

tool.setName(           'Mingw Archiver');
tool.setCommand(        'ar');
tool.setPath(           '$(MW_MINGW64_LOC)\bin');
tool.setDirective(      'OutputFlag',           '');
tool.setFileExtension(  'Static Library',       '.a');

% ------------------------------
% Builder
% ------------------------------

tc.setBuilderApplication(tc.Platform);

% --------------------------------------------
% BUILD CONFIGURATIONS
% --------------------------------------------

optimsOffOpts    = {'-O0'};
optimsOnOpts     = {'-O3'};
cCompilerOpts    = {'-c', '-g', ...
                    '-m32'};
cppCompilerOpts  = cCompilerOpts;
assemblerOpts    = {''};
linkerOpts       = {'-static', '-m32'};
sharedLinkerOpts = {'-shared', '-m32'};
archiverOpts     = {'-ruvs'};

% Set the toolchain flags for build configurations
cfg = tc.getBuildConfiguration('Faster Builds');
cfg.setOption( 'C Compiler',                horzcat(cCompilerOpts,   optimsOffOpts));
cfg.setOption( 'C++ Compiler',              horzcat(cppCompilerOpts, optimsOffOpts));
cfg.setOption( 'Assembler',                 assemblerOpts);
cfg.setOption( 'Linker',                    linkerOpts);
cfg.setOption( 'C++ Linker',                linkerOpts);
cfg.setOption( 'Shared Library Linker',     sharedLinkerOpts);
cfg.setOption( 'C++ Shared Library Linker', sharedLinkerOpts);
cfg.setOption( 'Archiver',                  archiverOpts);

cfg = tc.getBuildConfiguration('Faster Runs');
cfg.setOption( 'C Compiler',                horzcat(cCompilerOpts,   optimsOnOpts));
cfg.setOption( 'C++ Compiler',              horzcat(cppCompilerOpts, optimsOnOpts));
cfg.setOption( 'Assembler',                 assemblerOpts);
cfg.setOption( 'Linker',                    linkerOpts);
cfg.setOption( 'C++ Linker',                linkerOpts);
cfg.setOption( 'Shared Library Linker',     sharedLinkerOpts);
cfg.setOption( 'C++ Shared Library Linker', sharedLinkerOpts);
cfg.setOption( 'Archiver',                  archiverOpts);

cfg = tc.getBuildConfiguration('Debug');
cfg.setOption( 'C Compiler',              	horzcat(cCompilerOpts,    optimsOffOpts));
cfg.setOption( 'C++ Compiler',          	horzcat(cppCompilerOpts,  optimsOffOpts));
cfg.setOption( 'Assembler',             	horzcat(assemblerOpts));
cfg.setOption( 'Linker',                	horzcat(linkerOpts));
cfg.setOption( 'C++ Linker',               	horzcat(linkerOpts));
cfg.setOption( 'Shared Library Linker',  	horzcat(sharedLinkerOpts));
cfg.setOption( 'C++ Shared Library Linker',     horzcat(sharedLinkerOpts));
cfg.setOption( 'Archiver',              	horzcat(archiverOpts));

tc.setBuildConfigurationOption('all', 'Make Tool',     '-f $(MAKEFILE)');

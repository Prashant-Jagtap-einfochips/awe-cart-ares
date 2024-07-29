function tc = vdsp_tc(ToolChainName)
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

tc.addIntrinsicMacros({'VDSP_DIR'});

% ------------------------------
% C Compiler
% ------------------------------
tool = tc.getBuildTool('C Compiler');

tool.setName(           'VisualDSP C Compiler');
tool.setCommand(        'cc21k');
tool.setPath(           '$(VDSP_DIR)');

tool.setDirective(      'IncludeSearchPath',    '-I');
tool.setDirective(      'PreprocessorDefine',   '-D');
tool.setDirective(      'OutputFlag',           '-o');
tool.setDirective(      'Debug',                '-g');

tool.setFileExtension(  'Source',               '.c');
tool.setFileExtension(  'Header',               '.h');
tool.setFileExtension(  'Object',               '.doj');

% ------------------------------
% C++ Compiler
% ------------------------------

tool = tc.getBuildTool('C++ Compiler');

tool.setName(           'VisualDSP C++ Compiler');
tool.setCommand(        'cc21k');
tool.setPath(           '$(VDSP_DIR)');

tool.setDirective(      'IncludeSearchPath',  	'-I');
tool.setDirective(      'PreprocessorDefine', 	'-D');
tool.setDirective(      'OutputFlag',           '-o');
tool.setDirective(      'Debug',                '-g');

tool.setFileExtension(  'Source',               '.cpp');
tool.setFileExtension(  'Header',               '.h');
tool.setFileExtension(  'Object',               '.doj');

% ------------------------------
% Assembler
% ------------------------------

tool = tc.getBuildTool('Assembler');

tool.setName(           'VisualDSP Assembler');
tool.setCommand(        'easm21k');
tool.setPath(           '$(VDSP_DIR)');

tool.setDirective(      'IncludeSearchPath',  	'-I');
tool.setDirective(      'PreprocessorDefine', 	'-D');
tool.setDirective(      'OutputFlag',           '-o');
tool.setDirective(      'Debug',                '-g');

tool.setFileExtension(  'Source',               '.asm');
tool.setFileExtension(  'Object',               '.doj');


% ------------------------------
% Linker
% ------------------------------

tool = tc.getBuildTool('Linker');

tool.setName(           'VisualDSP C/C++ Linker');
tool.setCommand(        'cc21k');
tool.setPath(           '$(VDSP_DIR)');

tool.setDirective(      'Library',              '-L');
tool.setDirective(      'LibrarySearchPath',    '-l');
tool.setDirective(      'OutputFlag',           '-o');
tool.setDirective(      'Debug',                '-g');
tool.addDirective(      'LinkerFile',           {''});
tool.setDirective(      'LinkerFile',           '-T');

tool.setFileExtension(  'Executable',           '.dxe');
tool.setFileExtension(  'Shared Library',       '.dlb');

% ------------------------------
% C++ Linker
% ------------------------------

tool = tc.getBuildTool('C++ Linker');

tool.setName(           'VisualDSP C/C++ Linker');
tool.setCommand(        'cc21k');
tool.setPath(           '$(VDSP_DIR)');

tool.setDirective(      'Library',              '-L');
tool.setDirective(      'LibrarySearchPath',    '-l');
tool.setDirective(      'OutputFlag',           '-o');
tool.setDirective(      'Debug',                '-g');
tool.addDirective(      'LinkerFile',           {''});
tool.setDirective(      'LinkerFile',           '-T');

tool.setFileExtension(  'Executable',           '.dxe');
tool.setFileExtension(  'Shared Library',       '.dlb');

% ------------------------------
% Archiver
% ------------------------------

tool = tc.getBuildTool('Archiver');

tool.setName(           'VisualDSP Archiver');
tool.setCommand(        'elfar');
tool.setPath(           '$(VDSP_DIR)');
tool.setDirective(      'OutputFlag',           '');
tool.setFileExtension(  'Static Library',       '.dlb');

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
optimsOnOpts     = {'-Ov100'};
cCompilerOpts    = {'-c', '-g', ...
                    '-flags-compiler',...
                    '--no_wrap_diagnostics',...
                    '-structs-do-not-overlap',...
                    '-MM',...    
                    '-no-const-strings',...
                    '-no-multiline',...
                    '-warn-protos',...
                    '-double-size-32',...
                    '-swc',...
                    '-proc ADSP-21478'};
cppCompilerOpts  = cCompilerOpts;
assemblerOpts    = {'-swc', '-proc ADSP-21478', '$(INCLUDES)'};
linkerOpts       = {'-no-mem', '-proc ADSP-21478'};
sharedLinkerOpts = {'-no-mem', '-build-lib', '-proc ADSP-21478'};
archiverOpts     = {'-c'};

% Get the debug flag per build tool
debugFlag.CCompiler   = '-g -DDEBUG';   
debugFlag.CppCompiler = '-g -DDEBUG';
debugFlag.Assembler   = '-g -DDEBUG';
debugFlag.Linker      = '-add-debug-libpaths';  
debugFlag.CppLinker   = '-add-debug-libpaths';
debugFlag.Archiver    = '';

% Set the toolchain flags for build configurations
cfg = tc.getBuildConfiguration('Faster Builds');
cfg.setOption( 'C Compiler',                horzcat(cCompilerOpts,   optimsOnOpts));
cfg.setOption( 'C++ Compiler',              horzcat(cppCompilerOpts, optimsOnOpts));
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
cfg.setOption( 'C Compiler',              	horzcat(cCompilerOpts,    optimsOffOpts, debugFlag.CCompiler));
cfg.setOption( 'C++ Compiler',          	horzcat(cppCompilerOpts,  optimsOffOpts, debugFlag.CppCompiler));
cfg.setOption( 'Assembler',             	horzcat(assemblerOpts,    debugFlag.Assembler));
cfg.setOption( 'Linker',                	horzcat(linkerOpts,       debugFlag.Linker));
cfg.setOption( 'C++ Linker',               	horzcat(linkerOpts,       debugFlag.CppLinker));
cfg.setOption( 'Shared Library Linker',  	horzcat(sharedLinkerOpts, debugFlag.Linker));
cfg.setOption( 'C++ Shared Library Linker', horzcat(sharedLinkerOpts, debugFlag.Linker));
cfg.setOption( 'Archiver',              	horzcat(archiverOpts,     debugFlag.Archiver));

tc.setBuildConfigurationOption('all', 'Download',      '');
tc.setBuildConfigurationOption('all', 'Execute',       '');
tc.setBuildConfigurationOption('all', 'Make Tool',     '-f $(MAKEFILE)');

function tc = adsp_sc58x_tc(ToolChainName)
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

tc.addIntrinsicMacros({'CCES_2_6'});
tc.addIntrinsicMacros({'ADSP_SC58X_TOOLCHAIN'});
tc.addIntrinsicMacros({'CCES_LDF_DIR'});

% ------------------------------
% C Compiler
% ------------------------------
tool = tc.getBuildTool('C Compiler');

tool.setName(           'ADSP-SC58x C Compiler');
tool.setCommand(        'cc21k');
tool.setPath(           '$(CCES_2_6)');

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

tool.setName(           'ADSP-SC58x C++ Compiler');
tool.setCommand(        'cc21k');
tool.setPath(           '$(CCES_2_6)');

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

tool.setName(           'ADSP-SC58x Assembler');
tool.setCommand(        'easm21k');
tool.setPath(           '$(CCES_2_6)');

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

tool.setName(           'ADSP-SC58x C/C++ Linker');
tool.setCommand(        'cc21k');
tool.setPath(           '$(CCES_2_6)');

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

tool.setName(           'ADSP-SC58x C/C++ Linker');
tool.setCommand(        'cc21k');
tool.setPath(           '$(CCES_2_6)');

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

tool.setName(           'ADSP-SC58x Archiver');
tool.setCommand(        'elfar');
tool.setPath(           '$(CCES_2_6)');
tool.setDirective(      'OutputFlag',           '');
tool.setFileExtension(  'Static Library',       '.dlb');

% ------------------------------
% Download
% ------------------------------

tool = tc.getPostbuildTool('Download');

tool.setName(           'ADSP-SC58x Download');
tool.setCommand(        'load.bat');
tool.setPath(           '$(ADSP_SC58X_TOOLCHAIN)');


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
                    '-swc'};
cppCompilerOpts  = cCompilerOpts;
assemblerOpts    = {'-swc', '-D_INCLUDE_LEGACY_SYSREG_NAMES', '$(INCLUDES)'};
linkerOpts       = {'-no-mem', '-T $(CCES_LDF_DIR)/test.ldf'};
sharedLinkerOpts = {'-no-mem', '-build-lib'};
archiverOpts     = {'-c'};

sc584Opts = {'-proc ADSP-SC584'};

% Get the debug flag per build tool
debugFlag.CCompiler   = '-DDEBUG';   
debugFlag.CppCompiler = '-DDEBUG';
debugFlag.Assembler   = '-DDEBUG';
debugFlag.Linker      = '-add-debug-libpaths';  
debugFlag.CppLinker   = '-add-debug-libpaths';
debugFlag.Archiver    = '';  

% Create new build configurations
tc.addBuildConfiguration('Release - ADSP-SC584', 'Release configuration for ADSP-SC584');

% Set the toolchain flags for build configurations
cfg = tc.getBuildConfiguration('Faster Builds');
cfg.setOption( 'C Compiler',                horzcat(cCompilerOpts,   optimsOnOpts, sc584Opts));
cfg.setOption( 'C++ Compiler',              horzcat(cppCompilerOpts, optimsOnOpts, sc584Opts));
cfg.setOption( 'Assembler',                 horzcat(assemblerOpts,   sc584Opts));
cfg.setOption( 'Linker',                    horzcat(linkerOpts, sc584Opts));
cfg.setOption( 'C++ Linker',                horzcat(linkerOpts, sc584Opts));
cfg.setOption( 'Shared Library Linker',     horzcat(sharedLinkerOpts, sc584Opts));
cfg.setOption( 'C++ Shared Library Linker', horzcat(sharedLinkerOpts, sc584Opts));
cfg.setOption( 'Archiver',                  archiverOpts);

cfg = tc.getBuildConfiguration('Faster Runs');
cfg.setOption( 'C Compiler',                horzcat(cCompilerOpts,   optimsOnOpts, sc584Opts));
cfg.setOption( 'C++ Compiler',              horzcat(cppCompilerOpts, optimsOnOpts, sc584Opts));
cfg.setOption( 'Assembler',                 horzcat(assemblerOpts,   sc584Opts));
cfg.setOption( 'Linker',                    horzcat(linkerOpts, sc584Opts));
cfg.setOption( 'C++ Linker',                horzcat(linkerOpts, sc584Opts));
cfg.setOption( 'Shared Library Linker',     horzcat(sharedLinkerOpts, sc584Opts));
cfg.setOption( 'C++ Shared Library Linker', horzcat(sharedLinkerOpts, sc584Opts));
cfg.setOption( 'Archiver',                  archiverOpts);

cfg = tc.getBuildConfiguration('Debug');
cfg.setOption( 'C Compiler',                 horzcat(cCompilerOpts,    optimsOffOpts, debugFlag.CCompiler, sc584Opts));
cfg.setOption( 'C++ Compiler',               horzcat(cppCompilerOpts,  optimsOffOpts, debugFlag.CppCompiler, sc584Opts));
cfg.setOption( 'Assembler',                  horzcat(assemblerOpts,    debugFlag.Assembler, sc584Opts));
cfg.setOption( 'Linker',                     horzcat(linkerOpts, debugFlag.Linker, sc584Opts));
cfg.setOption( 'C++ Linker',                 horzcat(linkerOpts,       debugFlag.CppLinker, sc584Opts));
cfg.setOption( 'Shared Library Linker',      horzcat(sharedLinkerOpts, debugFlag.Linker, sc584Opts));
cfg.setOption( 'C++ Shared Library Linker', horzcat(sharedLinkerOpts, debugFlag.Linker, sc584Opts));
cfg.setOption( 'Archiver',                   horzcat(archiverOpts,     debugFlag.Archiver));

cfg = tc.getBuildConfiguration('Release - ADSP-SC584');
cfg.setOption( 'C Compiler',                horzcat(cCompilerOpts,   optimsOnOpts, sc584Opts));
cfg.setOption( 'C++ Compiler',              horzcat(cppCompilerOpts, optimsOnOpts, sc584Opts));
cfg.setOption( 'Assembler',                 horzcat(assemblerOpts,   sc584Opts));
cfg.setOption( 'Linker',                    horzcat(linkerOpts, sc584Opts));
cfg.setOption( 'C++ Linker',                horzcat(linkerOpts, sc584Opts));
cfg.setOption( 'Shared Library Linker',     horzcat(sharedLinkerOpts, sc584Opts));
cfg.setOption( 'C++ Shared Library Linker', horzcat(sharedLinkerOpts, sc584Opts));
cfg.setOption( 'Archiver',                  archiverOpts);


tc.setBuildConfigurationOption('all', 'Download',      '$(PRODUCT)');
tc.setBuildConfigurationOption('all', 'Execute',       '');
tc.setBuildConfigurationOption('all', 'Make Tool',     '-f $(MAKEFILE)');

end


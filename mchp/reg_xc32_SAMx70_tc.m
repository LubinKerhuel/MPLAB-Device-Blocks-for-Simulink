function config = reg_xc32_SAMx70_tc()
% Creates an SAMx7 ToolchainInfo object.
% This file can be used as a template to define other toolchains on Windows.

% Copyright 2012-2016 The MathWorks, Inc.

[isOk XCxxPath_out cmplrversions compilerexe] = MCHP_findCompilerPath('c32','');
config(1)                       = coder.make.ToolchainInfoRegistry;

try
    CHIP_REF = get_param(gcs,'PIC_REF');
catch
    CHIP_REF = 'ATSAME70Q21B';
end

PICREF_XC32 = regexprep(CHIP_REF,'^PIC','');   % for PIC32C, remove trailing PIC

%regexprep()

for iC = 1:numel(XCxxPath_out)    
    
    % for each compiler detected, we generate the appropriate .mat file and create one new entry in config to register 
    % the mat file as a toolchain.
    
    %% Step 1: Generate .mat file related to each Compiler detected
    clear tc;
    
    tcFileName = [tempdir 'xc32_SAMx70_' cmplrversions{iC}{1} '.mat'];
    
    tc = coder.make.ToolchainInfo;%('BuildArtifact', 'nmake makefile');
    tc.Name             = ['xc32 - v' cmplrversions{iC}{1} ' for Mirochip SAMx7 device'];
    tc.Platform         = computer('arch');
    tc.SupportedVersion = '';

    tc.addAttribute('TransformPathsWithSpaces');
    %tc.addAttribute('RequiresCommandFile');	% This will place list all object files into a rsp file to be used by linker. Do not work with XC32
    tc.addAttribute('RequiresBatchFile');

    % ------------------------------
    % Setup
    % ------------------------------
    % Below we are using %ICPP_COMPILER14% as root folder where Intel Compiler is installed.
    % You can either set an environment variable or give full path to the
    % compilervars.bat file
    %tc.ShellSetup{1} = 'call %ICPP_COMPILER14%\bin\compilervars.bat intel64';


    % ------------------------------
    % Macros
    % ------------------------------
    %tc.addMacro('MW_EXTERNLIB_DIR',     ['$(MATLAB_ROOT)\extern\lib\' tc.Platform '\microsoft']);
    %tc.addMacro('MW_LIB_DIR',           ['$(MATLAB_ROOT)\lib\' tc.Platform]);
    %tc.addMacro('CFLAGS_ADDITIONAL',    '-D_CRT_SECURE_NO_WARNINGS');
    %tc.addMacro('CPPFLAGS_ADDITIONAL',  '-EHs -D_CRT_SECURE_NO_WARNINGS');
    %tc.addMacro('LIBS_TOOLCHAIN',       '$(conlibs)');
    %tc.addMacro('CVARSFLAG',            '');

    %tc.addIntrinsicMacros({'ldebug', 'conflags', 'cflags'});

    % ------------------------------
    % C Compiler
    % ------------------------------

    tool = tc.getBuildTool('C Compiler');

    %tool.OptionsRegistry{end+1} = 'pouet';
    %tool.addDirective('pouet','-O');


    tool.setName(           'XC32 C Compiler');
    tool.setCommand(        compilerexe{iC}{1});
    tool.setPath(           fullfile(XCxxPath_out{iC},'bin') );

    tool.setDirective(      'IncludeSearchPath',    '-I');
    tool.setDirective(      'PreprocessorDefine',   '-D');
    tool.setDirective(      'OutputFlag',           '-o');
    tool.setDirective(      'Debug',                '-g');

    tool.setFileExtension(  'Source',               '.c');
    tool.setFileExtension(  'Header',               '.h');
    tool.setFileExtension(  'Object',               '.o');

    tool.setCommandPattern('|>TOOL<| |>TOOL_OPTIONS<| |>OUTPUT_FLAG<| |>OUTPUT<|');

    % ------------------------------
    % C++ Compiler
    % ------------------------------

    tool = tc.getBuildTool('C++ Compiler');
    %
    tool.setName(           'XC32 C++ Compiler');
    tool.setCommand(        compilerexe{iC}{1});
    tool.setPath(           fullfile(XCxxPath_out{iC},'bin') );

    tool.setDirective(      'IncludeSearchPath',    '-I');
    tool.setDirective(      'PreprocessorDefine',   '-D');
    tool.setDirective(      'OutputFlag',           '-o');
    tool.setDirective(      'Debug',                '-g');

    tool.setFileExtension(  'Source',               '.cpp');
    tool.setFileExtension(  'Header',               '.hpp');
    tool.setFileExtension(  'Object',               '.obj');

    tool.setCommandPattern('|>TOOL<| |>TOOL_OPTIONS<| |>OUTPUT_FLAG<| |>OUTPUT<|');


    % ------------------------------
    % Linker
    % ------------------------------

    tool = tc.getBuildTool('Linker');

    tool.setName(           'xc32 Linker');
    tool.setCommand(        compilerexe{iC}{1});
    tool.setPath(           fullfile(XCxxPath_out{iC},'bin') );

    tool.setDirective(      'Library',              '-L');
    tool.setDirective(      'LibrarySearchPath',    '-I');
    tool.setDirective(      'OutputFlag',           '-o');
    tool.setDirective(      'Debug',                '-g');

    tool.setFileExtension(  'Executable',           '.elf');
    tool.setFileExtension(  'Shared Library',       '.a');

    tool.setCommandPattern('|>TOOL<| |>TOOL_OPTIONS<| |>OUTPUT_FLAG<| |>OUTPUT<|');

    % ------------------------------
    % C++ Linker
    % ------------------------------

    %tool = tc.getBuildTool('C++ Linker');
    %
    %tool.setName(           'Intel C/C++ Linker');
    %tool.setCommand(        'xilink');
    %tool.setPath(           fullfile(XCxxPath_out{1},'bin') );
    %
    %tool.setDirective(      'Library',              '-L');
    %tool.setDirective(      'LibrarySearchPath',    '-I');
    %tool.setDirective(      'OutputFlag',           '-out:');
    %tool.setDirective(      'Debug',                '');
    %
    %tool.setFileExtension(  'Executable',           '.exe');
    %tool.setFileExtension(  'Shared Library',       '.dll');
    %
    %tool.setCommandPattern('|>TOOL<| |>TOOL_OPTIONS<| |>OUTPUT_FLAG<||>OUTPUT<|');

    % ------------------------------
    % Archiver
    % ------------------------------

    tool = tc.getBuildTool('Archiver');

    tool.setName(           'xc32 Archiver');

    tool.setCommand(        compilerexe{iC}{2});
    tool.setPath(           fullfile(XCxxPath_out{iC},'bin') );
    tool.setDirective(      'OutputFlag',           '-out:');
    tool.setFileExtension(  'Static Library',       '.a');
    tool.setCommandPattern('|>TOOL<| |>TOOL_OPTIONS<| |>OUTPUT_FLAG<||>OUTPUT<|');

    % ------------------------------
    % Builder
    % ------------------------------

    tc.setBuilderApplication(tc.Platform);

    % --------------------------------------------
    % BUILD CONFIGURATIONS
    % --------------------------------------------

    optimsOffOpts    = {'-O0'};
    optimsOnOpts     = {'-O1'};
    cCompilerOpts    = '-x c -c -mprocessor=$(CHIP_REF) $(cflags) $(CVARSFLAG) $(CFLAGS_ADDITIONAL)';
    cppCompilerOpts  = '$(cflags) $(CVARSFLAG) $(CPPFLAGS_ADDITIONAL)';
    linkerOpts       = {'-mprocessor=$(CHIP_REF) $(ldebug) $(conflags) $(LIBS_TOOLCHAIN)'};
    sharedLinkerOpts = horzcat(linkerOpts, '-dll -def:$(DEF_FILE)');
    archiverOpts     = {''};

    % Get the debug flag per build tool
    debugFlag.CCompiler   = '$(CDEBUG)';   
    debugFlag.CppCompiler = '$(CPPDEBUG)';
    debugFlag.Linker      = '$(LDDEBUG)';  
    debugFlag.CppLinker   = '$(CPPLDDEBUG)';  
    debugFlag.Archiver    = '$(ARDEBUG)';  
        
   % Faster Builds = Faster run. Default MAthWorks menue. Can't modify.
    % Set the toolchain flags for 'Faster Builds' build configuration   
    cfg = tc.getBuildConfiguration('Faster Builds');
    cfg.setOption( 'C Compiler',                horzcat(cCompilerOpts,   optimsOnOpts));
    cfg.setOption( 'C++ Compiler',              horzcat(cppCompilerOpts, optimsOnOpts));
    cfg.setOption( 'Linker',                    linkerOpts);
    cfg.setOption( 'C++ Linker',                linkerOpts);
    cfg.setOption( 'Shared Library Linker',     sharedLinkerOpts);
    cfg.setOption( 'Archiver',                  archiverOpts);
    ChipRef_Opt = coder.make.BuildItem('CHIP_REF',PICREF_XC32);
    cfg.addOption('Chip reference',ChipRef_Opt);

    % Set the toolchain flags for 'Faster Runs' build configuration   
    cfg = tc.getBuildConfiguration('Faster Runs');
    cfg.setOption( 'C Compiler',                horzcat(cCompilerOpts,   optimsOnOpts));
    cfg.setOption( 'C++ Compiler',              horzcat(cppCompilerOpts, optimsOnOpts));
    cfg.setOption( 'Linker',                    linkerOpts);
    cfg.setOption( 'C++ Linker',                linkerOpts);
    cfg.setOption( 'Shared Library Linker',     sharedLinkerOpts);
    cfg.setOption( 'Archiver',                  archiverOpts);
    ChipRef_Opt = coder.make.BuildItem('CHIP_REF',PICREF_XC32);
    cfg.addOption('Chip reference',ChipRef_Opt);
    
    
    % Set the toolchain flags for 'Debug' build configuration
    cfg = tc.getBuildConfiguration('Debug');
    cfg.setOption( 'C Compiler',              	horzcat(cCompilerOpts,   optimsOffOpts, debugFlag.CCompiler));
    cfg.setOption( 'C++ Compiler',          	horzcat(cppCompilerOpts, optimsOffOpts, debugFlag.CppCompiler));
    cfg.setOption( 'Linker',                	horzcat(linkerOpts,       debugFlag.Linker));
    cfg.setOption( 'C++ Linker',               	horzcat(linkerOpts,       debugFlag.CppLinker));
    cfg.setOption( 'Shared Library Linker',  	horzcat(sharedLinkerOpts, debugFlag.Linker));
    cfg.setOption( 'Archiver',              	horzcat(archiverOpts,     debugFlag.Archiver));
    ChipRef_Opt = coder.make.BuildItem('CHIP_REF',PICREF_XC32);
    cfg.addOption('Chip reference',ChipRef_Opt);

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % Adding a post-build tool to ToolchainInfo
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    % ELF (executable and linkable format) to hexadecimal converter 
    postbuildToolName = 'elf2hex converter';
    % Create and populate a post-build tool
    postbuild = tc.addPostbuildTool(postbuildToolName);              
    % Set command macro and value for tool
    postbuild.setCommand('OBJCOPY', compilerexe{iC}{3});	% xc32-bin2hex
    % Set path for tool
    postbuild.setPath('OBJCOPYPATH',fullfile(XCxxPath_out{iC},'bin') );
    % Set options for tool
    postbuild.OptionsRegistry = {postbuildToolName, 'OBJCOPYFLAGS_HEX'};
    % Set output type from tool
    postbuild.SupportedOutputs = {coder.make.enum.BuildOutput.EXECUTABLE};
    % Create build configuration for tool
    tc.addBuildConfigurationOption(postbuildToolName, postbuild);
    % Set build configuration for tool
    tc.setBuildConfigurationOption('all', postbuildToolName, ' $(PRODUCT)'); % $(PRODUCT_HEX)

    % listing assembly $(OBJDUMP) -S -f -Msymbolic $(PRODUCT) > $(MODEL).lst
    % ELF (executable and linkable format) to hexadecimal converter 
    postbuildToolName = 'Assembly listing';
    % Create and populate a post-build tool
    postbuild = tc.addPostbuildTool(postbuildToolName);              
    % Set command macro and value for tool
    postbuild.setCommand('LSTCOPY', [compilerexe{iC}{4}]);	% xc32-bin2hex
    % Set path for tool
    postbuild.setPath('LSTCOPYPATH',fullfile(XCxxPath_out{iC},'bin') );
    % Set options for tool
    postbuild.OptionsRegistry = {postbuildToolName, 'OBJCOPYFLAGS_LST'};
    % Set output type from tool
    postbuild.SupportedOutputs = {coder.make.enum.BuildOutput.EXECUTABLE};
    % Create build configuration for tool
    tc.addBuildConfigurationOption(postbuildToolName, postbuild);
    % Set build configuration for tool
    tc.setBuildConfigurationOption('all', postbuildToolName, ' -S $(PRODUCT) >> $(PRODUCT).lst'); % $(PRODUCT_HEX)
    

    %%
    % ------------------------------
    % Download Tool
    % ------------------------------
    % Adding a new download configuration to download and run using DSS
    % This is not ok

    % tc.addMacro('DOWNLOAD_ARGS', [' >tmp.trash 2>&1 ', ...
    %     '-P$(ARDUINO_PORT) ',...
    %     '-V -q -q -q -q -F ',...
    %     '-C$(ARDUINO_ROOT)/hardware/tools/avr/etc/avrdude.conf ',...
    %     '-p$(ARDUINO_MCU) ',...
    %     '-c$(ARDUINO_PROTOCOL) ',...
    %     '-b$(ARDUINO_BAUD) ',...
    %     '-D ',...
    %             '-Uflash:w:']);
    % downloadTool = tc.getPostbuildTool('Download');
    % downloadTool.setCommand('picflash');       % Macro name & Tool name
    % downloadTool.setPath('$(ARDUINO_TOOLS)');
    %tc.setBuildConfigurationOption('all', 'Download', '$(DOWNLOAD_ARGS)$(PRODUCT_HEX):i'); 
    % tc.setPostbuildTool('Download',downloadTool);

    tc.setBuildConfigurationOption('all', 'Download', ''); 
    %tc.setBuildConfigurationOption('all', 'Download',      'picflash($(PRODUCT))');
    tc.setBuildConfigurationOption('all', 'Execute',       '');
    tc.setBuildConfigurationOption('all', 'Make Tool',     '-f $(MAKEFILE)');
   
    save(tcFileName,'tc'); 
    
    %% Step 2: Register toolchain (with Generate .mat file)
    config(iC)                       = coder.make.ToolchainInfoRegistry;
    config(iC).Name                  = tc.Name ;
    config(iC).FileName              = tcFileName;
    config(iC).TargetHWDeviceType    = {'*'};
    config(iC).Platform              =  {computer('arch')};
    
end

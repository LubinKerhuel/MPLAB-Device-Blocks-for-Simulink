function MCHP_dsPIC_stf_make_rtw_hook(hookMethod,modelName,rtwroot,templateMakefile,buildOpts,buildArgs,buildInfo)
% ERT_MAKE_RTW_HOOK - This is the standard ERT hook file for the build
% process (make_rtw), and implements automatic configuration of the
% models configuration parameters.  When the buildArgs option is specified
% as 'optimized_fixed_point=1' or 'optimized_floating_point=1', the model
% is configured automatically for optimized code generation.
%
% This hook file (i.e., file that implements various codegen callbacks) is
% called for system target file ert.tlc.  The file leverages
% strategic points of the build process.  A brief synopsis of the callback
% API is as follows:
%
% ert_make_rtw_hook(hookMethod, modelName, rtwroot, templateMakefile,
%                   buildOpts, buildArgs)
%
% hookMethod:
%   Specifies the stage of the build process.  Possible values are
%   entry, before_tlc, after_tlc, before_make, after_make and exit, etc.
%
% modelName:
%   Name of model.  Valid for all stages.
%
% rtwroot:
%   Reserved.
%
% templateMakefile:
%   Name of template makefile.  Valid for stages 'before_make' and 'exit'.
%
% buildOpts:
%   Valid for stages 'before_make' and 'exit', a MATLAB structure
%   containing fields
%
%   modules:
%     Char array specifying list of generated C files: model.c, model_data.c,
%     etc.
%
%   codeFormat:
%     Char array containing code format: 'RealTime', 'RealTimeMalloc',
%     'Embedded-C', and 'S-Function'
%
%   noninlinedSFcns:
%     Cell array specifying list of non-inlined S-Functions.
%
%   compilerEnvVal:
%     String specifying compiler environment variable value, e.g.,
%     D:\Applications\Microsoft Visual
%
% buildArgs:
%   Char array containing the argument to make_rtw.  When pressing the build
%   button through the Configuration Parameter Dialog, buildArgs is taken
%   verbatim from whatever follows make_rtw in the make command edit field.
%   From MATLAB, it's whatever is passed into make_rtw.  For example, its
%   'optimized_fixed_point=1' for make_rtw('optimized_fixed_point=1').
%
%   This file implements these buildArgs:
%     optimized_fixed_point=1
%     optimized_floating_point=1
%
% You are encouraged to add other configuration options, and extend the
% various callbacks to fully integrate ERT into your environment.

% Copyright 1996-2010 The MathWorks, Inc.
% $Revision: 1.1.6.1 $ $Date: 2010/11/17 11:14:07 $



% interresting instructions:
% notMdlRef = strcmpi(h.MdlRefBuildArgs.ModelReferenceTargetType,'NONE');
% isMdlRefSim = strcmpi(h.MdlRefBuildArgs.ModelReferenceTargetType,'SIM');
% if createSILPILBlock
% % early check for a valid SIL/PIL configuration
% if silPILBlockIsSILMode
% rtw.pil.ModelBlockPIL.getSilConnectivityConfig(h.ModelName);
% else
% rtw.pil.ModelBlockPIL.getPilConnectivityConfig(h.ModelName);
% end
% end

if 0
    try
        load('c:\tmp.mat');
    catch
    end
    [c d] = Simulink.BlockDiagram.getChecksum(gcs);
    s.c.(hookMethod) = c;
    s.d.(hookMethod) = d;
    save c:\tmp.mat s;
end

switch hookMethod
    case 'error'
        % Called if an error occurs anywhere during the build.  If no error occurs
        % during the build, then this hook will not be called.  Valid arguments
        % at this stage are hookMethod and modelName. This enables cleaning up
        % any static or global data used by this hook file.
        disp(['### Build procedure for model: ''' modelName...
            ''' aborted due to an error.']);

        %% Give a chance to MPLAB project: if error appears during compilation, this might help.
        try
            %ProjectRelativePath_Root = pwd;
            %ProjectRelativePath_Root = fileparts(ProjectRelativePath_Root);  % up one folder            
			%MCHP_CreateMPLAB(modelName,'dsPIC',ProjectRelativePath_Root,buildInfo);
            [MPLAB_Link AutoProgram] = MCHP_MplabLink(modelName);
            disp([ MPLAB_Link 10 10 ' ***** Error detected. Links above are provided for debug purpose only (.elf file might not be current).']);
        catch
        end

    case 'entry'
        % Called at start of code generation process (before anything happens.)
        % Valid arguments at this stage are hookMethod, modelName, and buildArgs.
        NbrPins = MCHP_model_update(modelName);
        
        if 0
            RTWGenSettings = get_param(modelName,'RTWGenSettings');
            myData = MCHP_Fun.GetRTWGenSettings(modelName);
            myDataFields = fieldnames(myData);
            for iF = 1:numel(myDataFields)
                RTWGenSettings.(myDataFields{iF}) = myData.(myDataFields{iF});
            end
            set_param(modelName,'RTWGenSettings',RTWGenSettings);
        end


    case 'before_tlc'
        % Called just prior to invoking TLC Compiler (actual code generation.)
        % Valid arguments at this stage are hookMethod, modelName, and
        % buildArgs
       
        % RTWgenSettings is changed during compilation and revert back then.
        % model Checksum stored within C code include theses RTWgenSettings changes.
        % Thus we track the change and update the model at end of compilation with theses to get a checksum match.
        % Alternatively, we might usethe model parameters: get_param(gcs,'ExtModeSkipChecksumValidation')
        %        if strcmp(get_param(gcs,'ExtMode'),'on')
        %if i_isModelRef == 0
        if 0
            MCHP_RTWgenSettings = get_param(gcs,'RTWgenSettings');
            if isfield(MCHP_RTWgenSettings,'RelativeBuildDir')
                save(['.' filesep MCHP_RTWgenSettings.RelativeBuildDir filesep 'MCHP_RTWgenSettings.mat'],'MCHP_RTWgenSettings');
            end
        end
        %end

        MCHP_isPilSim = i_isPilSim(buildOpts);

        % right click to compile subsystem: Create a new simulink file (not stored to DD)
        % there is no Master block in that subsystem ==> we create a library
        MCHP_isSubSystem = 0;
        strSubModel = which(modelName); % return "is a new simulink model file for a subsystem
        if strcmpi(strSubModel,'new Simulink model');
            MCHP_MASTERLINK =  find_system(modelName,'LoadFullyIfNeeded','on','SearchDepth',3,'RegExp', 'on','PICREF','.*'); %% 3 variable characteristics of Master Block
            if isempty(MCHP_MASTERLINK)
                MCHP_isSubSystem = 1;
            end
        end


        RTWGenSettings = get_param(modelName,'RTWGenSettings');
        myData = MCHP_Fun.GetRTWGenSettings(modelName);
        % Add 2 fields to make out compilation situation in TLC script
        % detect PIL submodel
        % Detect subsystem (not submodel)
        myData.MCHP_isPilSim = int2str(MCHP_isPilSim);
        myData.MCHP_isSubSystem = int2str(MCHP_isSubSystem);
        myDataFields = fieldnames(myData);

        ident = '  ';
        identI = [09 '   '];
        rtw_txt = '';
        for iF = 1:numel(myDataFields)
             %disp([ myDataFields{iF} ' => ']);
             %disp(['                       ' myData.(myDataFields{iF}) ]);
            %rtw_txt = [ rtw_txt 13 10 ident myDataFields{iF} identI '"' myData.(myDataFields{iF}) '"'  ];
            rtw_txt = [ rtw_txt 13 10 ident myDataFields{iF} identI '"' strrep(myData.(myDataFields{iF}),'\\','\') '"'  ];
        end

        if isstruct(RTWGenSettings) && isfield(RTWGenSettings,'RelativeBuildDir')
            RelativeBuildDir = RTWGenSettings.RelativeBuildDir;
        else
            pX = mchp.prefs.getFolderNameExtension;
            RelativeBuildDir = [modelName pX];    % Plan B it might happend that that structure is not available
        end
        
        if i_isModelRef(modelName) || i_isPilSim(buildOpts)
            rtwFile = ['slprj' filesep 'MCHP_dsPIC_stf' filesep modelName filesep modelName '.rtw'];
        else
            rtwFile = [RelativeBuildDir filesep modelName '.rtw'];
        end
        if exist(rtwFile,'file')    % if no need to rebuild, rtw file is not re-created
            fid = fopen(rtwFile,'r');
            r = fread(fid);
            fclose(fid);
            r = char(r');
            %        rtw_txt = regexptranslate('escape', rtw_txt)
            %        r2 = regexprep(r,'RTWGenSettings \{',rtw_txt);
            r2 = strrep(r,'RTWGenSettings {',['RTWGenSettings {' rtw_txt]);

            fid = fopen(rtwFile,'w');
            fwrite(fid,r2);
            fclose(fid);
        end

        if 0
            RTWGenSettings = get_param(modelName,'RTWGenSettings');
            myData = MCHP_Fun.GetRTWGenSettings(modelName);
            myDataFields = fieldnames(myData);
            for iF = 1:numel(myDataFields)
                RTWGenSettings.(myDataFields{iF}) = myData.(myDataFields{iF});
            end
            set_param(modelName,'RTWGenSettings',RTWGenSettings);
        end



    case 'after_tlc'
        % Called just after to invoking TLC Compiler (actual code generation.)
        % Valid arguments at this stage are hookMethod, modelName, and
        % buildArgs

        % checks must be done after the model has been compiled otherwise
        % sample time may not be valid
    
        if ~isempty(buildInfo)  %% buildInfo not available in older matlab release
            try
                setTargetProvidesMain(buildInfo,true);        % Function not available in older matlab release
            catch
            end
            
            % addSourcePaths(buildInfo,'/dsPIC33CH512MP508_Slave.X/slave1_image','Slave core');
            %addSourceFiles(buildInfo,'slave1_image.s','','Slave core');
            % dsPIC33CH512MP508_Slave.X\slave1_image
        %    addIncludePaths(buildInfo,'C:\M91449\MCHP_Blockset\Developpements\2019_08_dsPIC_CH_DualCore\dsPIC33CH512MP508_Slave.X\slave1_image');
        %    addIncludeFiles(buildInfo, 'slave1_image.h','C:\M91449\MCHP_Blockset\Developpements\2019_08_dsPIC_CH_DualCore\dsPIC33CH512MP508_Slave.X\slave1_image');
        %    addSourceFiles(buildInfo,'slave1_image.s','C:\M91449\MCHP_Blockset\Developpements\2019_08_dsPIC_CH_DualCore\dsPIC33CH512MP508_Slave.X\slave1_image');            
        end

        %buildInfo.getPaths('include','')   % relative paths
        %buildInfo.getPaths('include',1)    % full paths
        
    case 'before_make'
        % Called after code generation is complete, and just prior to kicking
        % off make process (assuming code generation only is not selected.)  All
        % arguments are valid at this stage.

        if ~isempty(buildInfo)  %% buildInfo not available in older matlab release
            try
                setTargetProvidesMain(buildInfo,true);        % Function not available in older matlab release
            catch
            end
        end        
               
        % Could add Makefile additional files here
        if 1
        	try        		
        		CurrentFolder = pwd;
		        if i_isModelRef(modelName) == 0
		            MCHP_CreateMPLAB(modelName,'dsPIC',buildInfo);
		        end
	    	catch
	    		disp('Problem while Creating MPLAB project. MPLAB Project skipped.');
	    		cd(CurrentFolder);
	    	end
        end
        
    case 'after_make'
        % Called after make process is complete. All arguments are valid at
        % this stage.
        if 1
        	try
        		CurrentFolder = pwd;
		        if i_isModelRef(modelName) == 0
					MCHP_CreateMPLAB(modelName,'dsPIC',buildInfo);                   

%                     get_param(modelName,'PIC_REF')
%                     OPTIM_GCC
%                     LDFLAGS
%                     GCCPATH
%                     GCC1                    
%                     LINKER_SCRIPT
%                     MCHP_PROGRAMMER
                  		            
		        end
	    	catch
	    		disp('Problem while Creating MPLAB project. MPLAB Project skipped.');
	    		cd(CurrentFolder);
	    	end	        
   		end
        %% Early version, generate MPLAB X project here.

    case 'exit'
        % Called at the end of the build process.  All arguments are valid
        % at this stage.
        % if i_isModelRef == 0 %% not a model ref
        
        Flag = (i_isPilSim(buildOpts) == 0);     % Flag is 1 if system is not PIL

        if Flag == 1
            [MPLAB_Link AutoProgram] = MCHP_MplabLink(modelName);
            disp(MPLAB_Link);
            % Trick for Matlab 2014a
            %         assignin('base','tmp_MPLAB_Link',MPLAB_Link);
            %         evalin('base','disp(tmp_MPLAB_Link);');
            %         evalin('base','clear(tmp_MPLAB_Link);');

            disp(['### Successful completion of build procedure for model: ', modelName]);

            if ~isempty(AutoProgram)
                disp(['   Flash After Compilation option is checked. Start programming.' 10 '    To remove this option, go to Simulation --> Model Configuration Parameters --> dsPIC Options tab, and uncheck the option.'])
                try
                    evalin('base',AutoProgram);
                catch
                end
            end
        end
        
        if 0
        	try
        		CurrentFolder = pwd;
		        if i_isModelRef(modelName) == 0
					MCHP_CreateMPLAB(modelName,'dsPIC',buildInfo);                   		            
		        end
	    	catch
	    		disp('Problem while Creating MPLAB project. MPLAB Project skipped.');
	    		cd(CurrentFolder);
	    	end		        
   		end
end


function isMdlRef = i_isModelRef(modelName)
MCHP_RTWgenSettings = get_param(modelName,'RTWgenSettings');
if ~isempty(MCHP_RTWgenSettings) && ~isfield(MCHP_RTWgenSettings,'RelativeBuildDir')  % This field does not exist with submodel, seems not OK with R2015b
    isMdlRef = 1;
else
    isMdlRef = 0;   % is a model ref
end

function isPilSim = i_isPilSim(buildOpts)

isPilSim = false;
if isfield(buildOpts,'XilInfo') && (isfield(buildOpts.XilInfo,'IsStandaloneBuild')) && (buildOpts.XilInfo.IsStandaloneBuild == 1);  %% quick test, we know it is not PIl
    return;
end

s = dbstack;
%for k = 1:numel(s) disp(s(k).name); end

if verLessThan('Simulink','8.6')    % if before R2015b (= 8.6)
    for i=3:length(s)   % does not take into account this call
        if strfind(s(i).name,'loc_PIL_Update')  %  build_model_reference_target ; % build_standalone_rtw_target
            isPilSim=true;
            break;
        end
    end
else    
    for i=length(s):-1:3   % does not take into account this call
        if strfind(s(i).name,'loc_PIL_Update')  %  build_model_reference_target ; % build_standalone_rtw_target
            isPilSim=true;
            break;
        elseif strfind(s(i).name,'build_model_reference_target')    % From Matlab R2015b, however that exist with non PIL with previous matlab release !
            isPilSim=true;
            break;
        elseif strfind(s(i).name,'build_standalone_rtw_target')     % From Matlab R2015b
            isPilSim=false;
            break;
        end
    end
end



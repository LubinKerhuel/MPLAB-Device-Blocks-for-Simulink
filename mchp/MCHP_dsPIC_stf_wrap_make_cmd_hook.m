function makeCmdOut = MCHP_dsPIC_stf_wrap_make_cmd_hook(args)
% thanks to this, we do not have problems with LCC and so on...
% Script name is derived from target file MCHP_dsPIC_stf
makeCmd        = args.makeCmd;
modelName      = args.modelName;
verbose        = args.verbose;


for i=1:100 % remove duplicated -menable-fixed elements
    makeCmdS = regexprep(makeCmd,'-menable-fixed -menable-fixed ','-menable-fixed ');   % remove duplicated option (each CRL trig add a new one)
    if strcmp(makeCmdS,makeCmd)
        makeCmdS = regexprep(makeCmd,'-menable-fixed -menable-fixed','-menable-fixed'); % last one Watch out: no space here !
        makeCmd = makeCmdS;
        break;
    else
        makeCmd = makeCmdS;
    end
end

makeCmdOut = makeCmd;  % default, will be replaced

if exist(modelName,'file') ~= 4  % for rtwshared. Not a model. Library is already compiled at this point. No makefile required    
    return;
end

% modelParamList = get_param(modelName,'objectParameters');
% if isfield(modelParamList,'MasterModelReference')   % This field does not exist, sometime model name is rtwshared when using a subsystem. 
%                                                     % it is not a model ! but the library is already compiled at this point.
%     MasterModelReference = get_param(modelName,'MasterModelReference'); % This cause error with subsystems !
% else
%     MasterModelReference = '';
% end

debug = '';
% adding -d print a lot of debug info

jn = ''; % default value is no parallelization

MCHP_MULTITHREAD_COMPILATION = get_param(modelName,'MCHP_MULTITHREAD_COMPILATION');
if strcmp(MCHP_MULTITHREAD_COMPILATION,'on')
    % parallelize compilation: (Speed improvement by ~10%, not that big)
    NUMBER_OF_PROCESSORS = getenv('NUMBER_OF_PROCESSORS');
    if ~isempty(NUMBER_OF_PROCESSORS)
        NUMBER_OF_PROCESSORS = str2num(NUMBER_OF_PROCESSORS);
        if ~isempty(NUMBER_OF_PROCESSORS)
            NUMBER_OF_PROCESSORS = NUMBER_OF_PROCESSORS+1;
        else
            NUMBER_OF_PROCESSORS = 2;   % If number of processor is not found, Default value
        end
    else
        try 
            NUMBER_OF_PROCESSORS = feature('numcores')+1;
        catch 
            NUMBER_OF_PROCESSORS = 2;   % If number of processor is not found, Default value
        end
    end

    if NUMBER_OF_PROCESSORS > 1
        disp(['COMPILATION using ' int2str(NUMBER_OF_PROCESSORS) ' thread.']);
        jn = [' -j ' int2str(NUMBER_OF_PROCESSORS) ' -l ' int2str(NUMBER_OF_PROCESSORS) ];		% reported to works well with n+1 for the number of thread
    end
end

if isunix()
    BatchExt = '.sh';
    UnixExtra = ' 2>&1';
else
    BatchExt = '.bat';
    UnixExtra = '';
end
ForceMake = '';
    
LinkWithMaster = strcmp(get_param(modelName,'LinkWithMaster'),'on');
if LinkWithMaster == 1
    ForceMake = ' --always-make';
end

try
    cmdFile = [modelName, BatchExt];    
    cmdFileFid = fopen(cmdFile,'w');
    fprintf(cmdFileFid, '%s\n', [makeCmd ForceMake jn debug UnixExtra]);
    fclose(cmdFileFid);   
    if isunix() 
        unix(['chmod +x ' cmdFile]);    
    end
   makeCmdOut = [ '"' pwd filesep cmdFile '"'  UnixExtra];  % cmdFile or makeCmd directly; 
   % this make works but output error messages goes in matlab cmd line    
   % makeCmdOut = [makeCmd ForceMake jn debug UnixExtra];
catch
    disp(['Make cmd: ' makeCmdOut]);
    warning('Error while creating Batch file starting compilation');
    return;        
end


% Handle Main-Secondary for dsPIC dual core
MasterModelReference = get_param(modelName,'MasterModelReference'); % This cause error with subsystems !

if ~isempty(MasterModelReference)
    [MasterPath,MasterName,MasterExt] = fileparts(MasterModelReference);     
    MasterExt = lower(MasterExt);
    switch MasterExt
        case {'.bat','.sh','.elf'}
            MasterPath = fileparts(MasterPath);  % goes one folder up
        case {'.mdl','.slx'} 
            % do nothing
    end



    cmdFile = [modelName, '_wMaster', BatchExt];
    cmdFileFid = fopen(cmdFile,'w');

    pX = mchp.prefs.getFolderNameExtension;

    if isunix()
        fprintf(cmdFileFid, '%s\n', 'echo Compiling Secondary core Image ...');
        fprintf(cmdFileFid, '%s\n', ['sh ' makeCmdOut]);
        
        fprintf(cmdFileFid, '%s\n', ['FILE=./' modelName '.elf']);
        fprintf(cmdFileFid, '%s\n', ['if test -f "$FILE"; then']);

        fprintf(cmdFileFid, '%s\n', ['    echo ---']);
        fprintf(cmdFileFid, '%s\n', ['    echo Compiling Master Image ...']);            
        fprintf(cmdFileFid, '%s\n', ['    cd ' MasterPath filesep MasterName pX ]);
        fprintf(cmdFileFid, '%s\n', ['    sh ' MasterName '.sh']);
        fprintf(cmdFileFid, '%s\n', ['    cd ' pwd ]);
        fprintf(cmdFileFid, '%s\n', ['    cd ' pwd ]);

        fprintf(cmdFileFid, '%s\n', 'else');
        fprintf(cmdFileFid, '%s\n', '    echo "Creating Secondary core image failed." ');
        fprintf(cmdFileFid, '%s\n', 'fi');        

        fclose(cmdFileFid);
        unix(['chmod +x ' cmdFile]);  

    else
        fprintf(cmdFileFid, '%s\n', '@echo Compiling Secondary core Image ...');
        fprintf(cmdFileFid, '%s\n', ['@call ' makeCmdOut]);
        fprintf(cmdFileFid, '%s\n', ['@if exist ' modelName '.elf (']);

        fprintf(cmdFileFid, '%s\n', ['    @echo ---']);
        fprintf(cmdFileFid, '%s\n', ['    @echo Compiling Master Image ...']);            
        fprintf(cmdFileFid, '%s\n', ['    @cd /d ' MasterPath filesep MasterName pX ]);
        fprintf(cmdFileFid, '%s\n', ['    @call ' MasterName '.bat']);
        fprintf(cmdFileFid, '%s\n', ['    @cd /d ' pwd ]);

        fprintf(cmdFileFid, '%s\n', ') else (');
        fprintf(cmdFileFid, '%s\n', '    @echo Creating Secondary core image failed.');
        fprintf(cmdFileFid, '%s\n', ')');
        fclose(cmdFileFid);
    end
        %            fprintf(cmdFileFid, '%s\n', ['"' MasterModelReference '"']);

        
        makeCmdOut = ['"' pwd filesep cmdFile '"' UnixExtra];  % cmdFile or makeCmd directly
end

% @echo Compiling Slave Image ...
% @call C:\M91449\MCHP_Blockset\Developpements\2019_08_dsPIC_CH_DualCore\dsPIC33CH512MP508_Slave.X\dsPIC33CH512MP508_Slave.bat
% @if exist C:\M91449\MCHP_Blockset\Developpements\2019_08_dsPIC_CH_DualCore\dsPIC33CH512MP508_Slave.X\dsPIC33CH512MP508_Slave.elf (
%     @echo ---
%     @echo Compiling Master Image ...
%     @cd C:\M91449\MCHP_Blockset\Developpements\2019_08_dsPIC_CH_DualCore\dsPIC33CH512MP508_CuriosityBoard.X
%     @call C:\M91449\MCHP_Blockset\Developpements\2019_08_dsPIC_CH_DualCore\dsPIC33CH512MP508_CuriosityBoard.X\dsPIC33CH512MP508_CuriosityBoard.bat
%     @cd C:\M91449\MCHP_Blockset\Developpements\2019_08_dsPIC_CH_DualCore    
% ) else (
%     @echo Creating Slave image failed.
% )    



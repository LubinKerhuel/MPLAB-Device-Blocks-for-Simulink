function makeCmdOut = MCHP_ARM7_stf_wrap_make_cmd_hook(args)
% thanks to this, we do not have problems with LCC and so on...
% Script name is derived from target file MCHP_ARM7_stf
makeCmd        = args.makeCmd;
modelName      = args.modelName;
verbose        = args.verbose;

makeCmdOut = makeCmd;  % default, will be replaced

if exist(modelName,'file') ~= 4  % for rtwshared. Not a model. Library is already compiled at this point. No makefile required    
    return;
end

if ~ispc

    if isunix()
        MCHP_MULTITHREAD_COMPILATION = get_param(modelName,'MCHP_MULTITHREAD_COMPILATION');
        if strcmp(MCHP_MULTITHREAD_COMPILATION,'on')
            [w s] = system('nproc');
            s = strtrim(s);
            if isempty(s)
                s = '1';
            end
            NUMBER_OF_PROCESSORS = str2num(s)+1;
            disp(['COMPILATION using ' int2str(NUMBER_OF_PROCESSORS) ' thread.']);
            jn = [' -j ' int2str(NUMBER_OF_PROCESSORS) ' -l ' int2str(NUMBER_OF_PROCESSORS) ];		% reported to works well with n+1 for the number of thread            
            makeCmdOut = [makeCmdOut jn];
        end
    end

    
    % UNIX: directly call the gmake function (avoid problem with creating script : right problems...)    
    return;
end
    

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
        NUMBER_OF_PROCESSORS = 2;   % If number of processor is not found, Default value
    end

    if NUMBER_OF_PROCESSORS > 1
        disp(['COMPILATION using ' int2str(NUMBER_OF_PROCESSORS) ' thread.']);
        jn = [' -j ' int2str(NUMBER_OF_PROCESSORS) ' -l ' int2str(NUMBER_OF_PROCESSORS) ];		% reported to works well with n+1 for the number of thread
    end
end

cmdFile = [modelName, '.bat'];
cmdFileFid = fopen(cmdFile,'w');
%    if ~verbose
% fprintf(cmdFileFid, '@echo off\n');
%     else
%         fprintf(cmdFileFid, '@echo on\n');
%    end

debug = '';%' --debug=v';

fprintf(cmdFileFid, '%s\n', [makeCmd jn debug]);
fclose(cmdFileFid);
makeCmdOut = ['"' pwd filesep cmdFile '"'];  % cmdFile or makeCmd directly

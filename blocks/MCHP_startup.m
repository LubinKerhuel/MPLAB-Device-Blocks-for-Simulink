function MCHP_startup()


try
    %blkPath = which('MCHP_Master.tlc');
    blkPath = mfilename('fullpath');
    if ~isempty(blkPath)
        blkPath = fileparts(blkPath);
    else
        disp('Warning: Problem with the MCHP Blockset. Please reinstall the tool.');
        return;
    end
    
    if verLessThan('MATLAB','8.4')
        % for version equal and prior to R2014a, use .mdl file
        slxPath = which('MCHP_Blockset.slx');
        if ~isempty(slxPath)
            if strcmpi(blkPath,fileparts(slxPath)) ~= 1
                rmpath(fileparts(slxPath));  % remove folder containing this file from the path
            else
                disp('Warning: The MCHP blockset should be removed and re-installed');
            end
        end
        
        mdlPath = which('MCHP_Blockset.mdl');
        if isempty(mdlPath)
            addpath( [blkPath filesep 'blk_mdl'] );  % Add folder containing mdl file.
        else
            mdlPath = fileparts(mdlPath);
            if strcmpi(blkPath,mdlPath) == 1
                addpath( [blkPath filesep 'blk_mdl'] );  % Add folder containing mdl file.
                disp(['Warning: The MCHP blockset should be removed and re-installed because the file ' mdlPath ' belong to an old version of the blockset.']);
            end
            if strcmpi([blkPath filesep 'blk_mdl'],mdlPath) ~= 1
                disp(['Warning: The MCHP blockset find an old path which will be removed: ' mdlPath]);
                rmpath(mdlPath);
                addpath( [blkPath filesep 'blk_mdl'] );  % Add folder containing mdl file.
            end
        end
        
    else
        % for version above and equal to R2014b, use .slx file
        mdlPath = which('MCHP_Blockset.mdl');
        if ~isempty(mdlPath)
            if strcmpi(blkPath,fileparts(mdlPath)) ~= 1
                rmpath(fileparts(mdlPath));  % remove folder containing this file from the path
            else
                disp(['Warning: The MCHP blockset should be removed and re-installed because the file ' mdlPath ' belong to an old version of the blockset.']);
            end
        end
        
        slxPath = which('MCHP_Blockset.slx');
        if isempty(slxPath)
            addpath( [blkPath filesep 'blk_slx'] );  % Add folder containing mdl file.
        else
            slxPath = fileparts(slxPath);
            if strcmpi([blkPath filesep 'blk_slx'],slxPath) ~= 1
                disp(['Warning: The MCHP blockset find an old path which will be removed: ' slxPath]);
                rmpath(slxPath);
                addpath( [blkPath filesep 'blk_slx'] );  % Add folder containing mdl file.
            end
        end
        
    end
catch
end

try
    picInfo
catch
end

end

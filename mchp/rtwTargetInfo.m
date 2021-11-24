function rtwTargetInfo(tr)
%RTWTARGETINFO Registration file for custom toolchains.

% Copyright 2012-2016 The MathWorks, Inc.

tr.registerTargetInfo(@loc_createToolchain);

end

% -------------------------------------------------------------------------
% Create the ToolchainInfoRegistry entries
% -------------------------------------------------------------------------
function config = loc_createToolchain

config = reg_xc32_SAMx70_tc();

% config(1)                       = coder.make.ToolchainInfoRegistry;
% config(1).Name                  = 'xc32 for SAMx70';
% config(1).FileName              = fullfile(fileparts(mfilename('fullpath')), 'xc32_SAMx70_tc.mat');
% config(1).TargetHWDeviceType    = {'*'};
% config(1).Platform              =  {computer('arch')};

end

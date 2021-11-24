function Target = mcb_SetProcessorDetails(Board_type,PWM_frequency)
%% ************************************************************************
% File          :   Target HW Parameter File
% Description   :   Parameters for Targets supported by MCB
% Authors       :   A. Selvaraj, H. Gulhane, D. Pandit, S. Narayan
% Copyright 2020 The MathWorks, Inc.
%% ************************************************************************
% Get basic parameters
    switch Board_type 
        case {'F28379D','F28377S'}
            Target.model                = 'LAUNCHXL-F28379D';% Manufacturer Model Number
            Target.sn                   = '123456';          % Manufacturer Serial Number
            Target.CPU_frequency        = 200e6;    %(Hz)   // Clock frequency
            Target.PWM_frequency        = PWM_frequency;   %// PWM frequency
            Target.PWM_Counter_Period   = round(Target.CPU_frequency/Target.PWM_frequency/2); %(PWM timer counts)    
 
        case {'F28069M','F28069'}
            Target.model                = 'LAUNCHXL-F28069M';% Manufacturer Model Number
            Target.sn                   = '123456';          % Manufacturer Serial Number
            Target.CPU_frequency        = 90e6;    %(Hz)   // Clock frequency
            Target.PWM_frequency        = PWM_frequency;   %// PWM frequency
            Target.PWM_Counter_Period   = round(Target.CPU_frequency/Target.PWM_frequency/2); %(PWM timer counts)
            
        case {'SAME70Q','SAME70'}
            Target.model                = 'ATSAME70Q21';% Manufacturer Model Number
            Target.sn                   = '123456';          % Manufacturer Serial Number
            Target.CPU_frequency        = 300e6;    %(Hz)   // Clock frequency
            Target.PWM_frequency        = PWM_frequency;   %// PWM frequency
            Target.PWM_Counter_Period   = round(Target.CPU_frequency/Target.PWM_frequency/2); %(PWM timer counts)
        otherwise
            disp('Error: Invalid Target!');
	end
    
end
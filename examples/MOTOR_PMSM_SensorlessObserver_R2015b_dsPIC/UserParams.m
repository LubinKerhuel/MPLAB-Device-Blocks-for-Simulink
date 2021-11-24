POLEPAIRS = 5;
PHASERES = 2.03;    % Phase resistance (Ohms)
PHASEIND = 0.0023;  % Phase inductance (Henrys)

NOMINALSPEEDINRPM = 2700;
MINSPEEDINRPM = 500;
FIELDWEAKSPEEDRPM = 5300;

LOCKTIMEINSEC = 0.25;
OPENLOOPTIMEINSEC = 3;
INITIALTORQUE = 1;

PWMFREQUENCY = 20e3;    
LOOPTIMEINSEC = 1/PWMFREQUENCY;
TS = LOOPTIMEINSEC;


DELTA_STARTUP_RAMP = round( (MINSPEEDINRPM*POLEPAIRS*LOOPTIMEINSEC*LOOPTIMEINSEC*65536*65536/(60*OPENLOOPTIMEINSEC)) );

ACCEL_STARTUP_RPM = (MINSPEEDINRPM*POLEPAIRS*LOOPTIMEINSEC)/(60*OPENLOOPTIMEINSEC);



%% Peripheral Parameters
% PWM settings:
DEADTIME = 1e-6;
% Current settling time is about 2us after PWM edge (i.e. after daedtime).
%TRIGs = [41.4 45.9];

TConv = 8.915*1e-6 * 1.05; % TConv is total time, here for 2 conversions; take 5% margin (required)
TSmp =  0.343*1e-6;

%TRIGS = [-8.6 -3.6] * 1e-6;
PSHIFT = TConv/2;   % Time for 1 conversion to complete.
TRIGS = [-PSHIFT 0] - (DEADTIME + TSmp);

%TRIGS3 for Single & Dual Shunt at the same time, edge aligned.
TRIGS3 = [-2*PSHIFT -PSHIFT 0] - (DEADTIME + TSmp); 

%% Center Aligned
TSETTLE = 2e-6;  % Settling Time for current (after dead time)





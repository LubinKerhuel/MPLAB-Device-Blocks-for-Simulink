%% ************************************************************************
% Model         :   PMSM Field Oriented Control
% Description   :   Set Parameters for PMSM Field Oriented Control
% File name     :   mcb_pmsm_foc_sensorless_f28069MLaunchPad_data.m
% Copyright 2020 The MathWorks, Inc.

%% Simulation Parameters

%% Set PWM Switching frequency
PWM_frequency 	= 20e3;    %Hz          // converter s/w freq
T_pwm           = 1/PWM_frequency;  %s  // PWM switching time period

%% Set Sample Times
Ts          	= T_pwm;        %sec        // simulation time step for controller
Ts_simulink     = T_pwm/2;      %sec        // simulation time step for model simulation
Ts_motor        = T_pwm/2;      %Sec        // Simulation sample time
Ts_inverter     = T_pwm/2;      %sec        // simulation time step for average value inverter
Ts_speed        = 10*Ts;        %Sec        // Sample time for speed controller

%% Set data type for controller & code-gen
%dataType = fixdt(1,32,17);    % Fixed point code-generation
dataType = 'single';            % Floating point code-generation

%% System Parameters // Hardware parameters

pmsm = mcb_SetPMSMMotorParameters('Microchip_motor');
%pmsm = mcb_SetPMSMMotorParameters('BLY171D');
%pmsm = mcb_SetPMSMMotorParameters('BLY172S');

%% Parameters below are not mandatory for offset computation

inverter = mcb_SetInverterParameters('Microchip_inverter');
inverter.ADCOffsetCalibEnable = 1; % Enable: 1, Disable: 0

smo = mcb_SetSMOParameters;

inverter.CtSensAOffset = 2048;
inverter.CtSensBOffset = 2048;

% Update ADC Gain for DRV8305
% if pmsm.I_rated < 5
%     inverter.ADCGain = 4;   % ADC Range = +- 4.825A wrto 0-4095 counts
%     inverter.SPI_Gain_Setting = 0x502A;
% 
% elseif pmsm.I_rated < 7
%     inverter.ADCGain = 2;   % ADC Range = +- 9.650A wrto 0-4095 counts
%     inverter.SPI_Gain_Setting = 0x5015;
% 
% else
    inverter.ADCGain = 1;   % ADC Range = +- 19.300A wrto 0-4095 counts
    inverter.SPI_Gain_Setting = 0x5000;

%end

% ADC Gain correction to scale for 3.3V ADC
inverter.ADC_Correction = (3.3 - 1.65)/(3.0 - 1.65);  % ADC Gain correction for 3.3V ADC
      
%inverter.I_max = (inverter.I_max*inverter.ADC_Correction)/inverter.ADCGain;

target = mcb_SetProcessorDetails('SAME70Q',PWM_frequency);
target.ADCCalibEnable = 1;  % Enable : 1, Disable :0

%% Derive Characteristics
pmsm.N_base = mcb_getBaseSpeed(pmsm,inverter); %rpm // Base speed of motor at given Vdc
% mcb_getCharacteristics(pmsm,inverter);

% Display parameters
pmsm
inverter

%% PU System details // Set base values for pu conversion

PU_System = mcb_SetPUSystem(pmsm,inverter);

%% Open loop d-axis per unit reference voltage
% Vd_Ref_openLoop_PU = 0.1; % Use 0.12 for Dyno setup and 0.1 for others

%% Controller design // Get ballpark values!

PI_params = mcb.internal.SetControllerParameters(pmsm,inverter,PU_System,T_pwm,2*Ts,Ts_speed);

%Updating delays for simulation
PI_params.delay_Currents    = 1; %No of samples delayed for current sensing

% mcb_getControlAnalysis(pmsm,inverter,PU_System,PI_params,Ts,Ts_speed);

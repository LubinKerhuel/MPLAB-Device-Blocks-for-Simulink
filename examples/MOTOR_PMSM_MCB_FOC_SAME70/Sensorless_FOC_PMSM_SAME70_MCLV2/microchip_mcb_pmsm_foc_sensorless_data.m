%% ************************************************************************
% Model         :   PMSM Field Oriented Control
% Description   :   Set Parameters for PMSM Field Oriented Control
% File name     :   microchip_mcb_pmsm_foc_sensorless_data.m
% Copyright 2021 The MathWorks, Inc.

%% Detect MATLAB version
release = version('-release');
release = sscanf(release,'%d');

%% Set PWM Switching frequency
PWM_frequency 	= 20e3;             %Hz     // converter s/w freq
T_pwm           = 1/PWM_frequency;  %s      // PWM switching time period

%% Set Sample Times
Ts          	= T_pwm;        %sec        // simulation time step for controller
Ts_simulink     = T_pwm/2;      %sec        // simulation time step for model simulation
Ts_motor        = T_pwm/2;      %Sec        // Simulation sample time
Ts_inverter     = T_pwm/2;      %sec        // simulation time step for average value inverter
Ts_speed        = 20*Ts;        %Sec        // Sample time for speed controller

%% Set data type for controller & code-gen
%dataType = fixdt(1,32,17);     % Fixed point code-generation
dataType = 'single';            % Floating point code-generation

%% System Parameters // Hardware parameters
pmsm.model  = 'Long_Hurst';     %           // Manufacturer Model Number
pmsm.sn     = '005';            %           // Manufacturer Model Number
pmsm.p      = 5;                %           // Pole Pairs for the motor
pmsm.Rs     = 0.285;            %Ohm        // Stator Resistor
pmsm.Ld     = 0.00032;          %H          // D-axis inductance value
pmsm.Lq     = 0.00032;          %H          // Q-axis inductance value
pmsm.J      = 7.061551833333e-6;%Kg-m2      // Inertia in SI units
pmsm.B      = 2.636875217824e-6;%Kg-m2/s    // Friction Co-efficient
pmsm.Ke     = 7.24;             %Bemf Const	// Vline_peak/krpm
pmsm.Kt     = 0.274;            %Nm/A       // Torque constant
pmsm.I_rated= 3.42*sqrt(2);     %A      	// Rated current (phase-peak)
pmsm.N_max  = 3644;             %rpm        // Max speed
pmsm.PositionOffset = 0.1712;	%PU position// Position Offset
pmsm.QEPSlits       = 250;      %           // QEP Encoder Slits
pmsm.FluxPM     = (pmsm.Ke)/(sqrt(3)*2*pi*1000*pmsm.p/60); %PM flux computed from Ke
pmsm.T_rated    = (3/2)*pmsm.p*pmsm.FluxPM*pmsm.I_rated;   %Get T_rated from I_rated

%% Inverter Parameters
inverter.model          = 'Microchip_inverter';% Manufacturer Model Number
inverter.sn             = 'INV_XXXX';         % Manufacturer Serial Number
inverter.V_dc           = 24;       %V      // DC Link Voltage of the Inverter

if(release<2021)
    inverter.I_max = 4.4;           %Amps   // Max current that can be measured by 3.3V ADC
else
    inverter.ISenseMax = 4.4;       %Amps   // Max current that can be measured by 3.3V ADC
end
inverter.I_trip         = 10;       %Amps   // Max current for trip
inverter.Rds_on         = 6.5e-3;   %Ohms   // Rds ON for BoostXL-DRV8301
inverter.Rshunt         = 0.025;    %Ohms   // Rshunt for BoostXL-DRV8301
inverter.MaxADCCnt      = 4095;     %Counts // ADC Counts Max Value
inverter.CtSensAOffset  = 2048;     %Counts // ADC Offset for phase-A
inverter.CtSensBOffset  = 2048;     %Counts // ADC Offset for phase-B
inverter.ADCGain        = 1;        %       // ADC Gain factor scaled by SPI
inverter.ADCOffsetCalibEnable = 1;  % Enable: 1, Disable: 0
inverter.CtSensAOffset = 2048;
inverter.CtSensBOffset = 2048;
inverter.ADCGain = 1;               % ADC Range = +- 19.300A wrto 0-4095 counts
inverter.SPI_Gain_Setting = 0x5000;
inverter.R_board        = inverter.Rds_on + inverter.Rshunt/3;  %Ohms
%% Target hardware parameters
target.model                = 'ATSAME70Q21';% Manufacturer Model Number
target.sn                   = '123456'; % Manufacturer Serial Number
target.CPU_frequency        = 300e6;    %(Hz)   // Clock frequency
target.PWM_frequency        = PWM_frequency;   %// PWM frequency
target.PWM_Counter_Period   = round(target.CPU_frequency/target.PWM_frequency/2); %(PWM timer counts)
target.ADCCalibEnable = 1;  % Enable : 1, Disable :0

%% Derive Characteristics
pmsm.N_base = mcb_getBaseSpeed(pmsm,inverter); %rpm // Base speed of motor at given Vdc
% mcb_getCharacteristics(pmsm,inverter);

%% PU System details // Set base values for pu conversion
PU_System = mcb_SetPUSystem(pmsm,inverter);

%% SMO parameters
Vd_Ref_openLoop_PU = 0.1; % Use 0.12 for Dyno setup and 0.1 for others
smo = mcb_SetSMOParameters;
smo.CurrentObsGain = 0.5;

%% Controller design // Get ballpark values!

PI_params = mcb.internal.SetControllerParameters(pmsm,inverter,PU_System,T_pwm,2*Ts,Ts_speed);

%Updating delays for simulation
PI_params.delay_Currents    = 1; %No of samples delayed for current sensing

% mcb_getControlAnalysis(pmsm,inverter,PU_System,PI_params,Ts,Ts_speed);

%% Display parameters
disp(pmsm);
disp(inverter);
clear release;
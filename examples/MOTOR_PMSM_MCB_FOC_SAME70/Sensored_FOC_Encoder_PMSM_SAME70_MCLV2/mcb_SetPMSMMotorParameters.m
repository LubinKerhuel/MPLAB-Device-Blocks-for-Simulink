function pmsm = mcb_SetPMSMMotorParameters(pmsm_type)
%% ************************************************************************
% File          :   PMSM Parameter File
% Description   :   Parameters for PMSMs supported by MCB
% Authors       :   A. Selvaraj, H. Gulhane, D. Pandit, S. Narayan
% Copyright 2020 The MathWorks, Inc.
%% ************************************************************************
% Get basic parameters
switch pmsm_type
    case 'BLY171D'
        pmsm.model  = 'Anaheim-BLY171D-24V-4000';%  // Manufacturer Model Number
        pmsm.sn     = '001';         %           // Manufacturer Model Number
        pmsm.p      = 4;                %           // Pole Pairs for the motor
        pmsm.Rs     = 0.75;             %Ohm        // Stator Resistor
        pmsm.Ld     = 1.05e-3;          %H          // D-axis inductance value
        pmsm.Lq     = 1.05e-3;          %H          // Q-axis inductance value
        pmsm.J      = 2.4018552467e-06; %Kg-m2      // Inertia in SI units
        pmsm.B      = 11.603710493e-06; %Kg-m2/s    // Friction Co-efficient
        pmsm.Ke     = 3.8;              %Bemf Const	// Vline_peak/krpm
        pmsm.Kt     = 0.03396;          %Nm/A       // Torque constant
        pmsm.I_rated= 1.8;              %A      	// Rated current (phase-peak)
        pmsm.N_max  = 10000;            %rpm        // Max speed
        pmsm.PositionOffset = 0.2016/4;	%PU position// Position Offset 
        pmsm.QEPSlits       = 1250;     %           // QEP Encoder Slits
        pmsm.FluxPM     = (pmsm.Ke)/(sqrt(3)*2*pi*1000*pmsm.p/60); %PM flux computed from Ke
        pmsm.T_rated    = (3/2)*pmsm.p*pmsm.FluxPM*pmsm.I_rated;   %Get T_rated from I_rated
        
    case 'BLY172S'
        pmsm.model  = 'Anaheim-BLY172S-24V-4000';%  // Manufacturer Model Number
        pmsm.sn     = '002';         %           // Manufacturer Model Number
        pmsm.p      = 4;                %           // Pole Pairs for the motor
        pmsm.Rs     = 0.4;              %Ohm        // Stator Resistor
        pmsm.Ld     = 0.6e-3;           %H          // D-axis inductance value
        pmsm.Lq     = 0.6e-3;           %H          // Q-axis inductance value
        pmsm.J      = 4.8018552467e-06; %Kg-m2      // Inertia in SI units
        pmsm.B      = 9.6037104933e-06; %Kg-m2/s    // Friction Co-efficient
        pmsm.Ke     = 4;              %Bemf Const	// Vline_peak/krpm
        pmsm.Kt     = 0.04103;          %Nm/A       // Torque constant
        pmsm.I_rated= 3.5;              %A      	// Rated current (phase-peak)
        pmsm.N_max  = 10000;            %rpm        // Max speed
        pmsm.PositionOffset = 0.1712;	%PU position// Position Offset
        pmsm.QEPSlits       = 1250;     %           // QEP Encoder Slits
        pmsm.FluxPM     = (pmsm.Ke)/(sqrt(3)*2*pi*1000*pmsm.p/60); %PM flux computed from Ke
        pmsm.T_rated    = (3/2)*pmsm.p*pmsm.FluxPM*pmsm.I_rated;   %Get T_rated from I_rated
        
    case 'Teknic2310P'
        pmsm.model  = 'Teknic-2310P';%              // Manufacturer Model Number
        pmsm.sn     = '003';         %           // Manufacturer Model Number
        pmsm.p      = 4;                %           // Pole Pairs for the motor
        pmsm.Rs     = 0.36;             %Ohm        // Stator Resistor
        pmsm.Ld     = 0.2e-3;           %H          // D-axis inductance value
        pmsm.Lq     = 0.2e-3;           %H          // Q-axis inductance value
        pmsm.J      = 7.061551833333e-6;%Kg-m2      // Inertia in SI units
        pmsm.B      = 2.636875217824e-6;%Kg-m2/s    // Friction Co-efficient
        pmsm.Ke     = 4.64;             %Bemf Const	// Vline_peak/krpm
        pmsm.Kt     = 0.274;            %Nm/A       // Torque constant
        pmsm.I_rated= 7.1;              %A      	// Rated current (phase-peak)
        pmsm.N_max  = 6000;             %rpm        // Max speed
        pmsm.PositionOffset = 0.1712;	%PU position// Position Offset
        pmsm.QEPSlits       = 1000;     %           // QEP Encoder Slits
        pmsm.FluxPM     = (pmsm.Ke)/(sqrt(3)*2*pi*1000*pmsm.p/60); %PM flux computed from Ke
        pmsm.T_rated    = (3/2)*pmsm.p*pmsm.FluxPM*pmsm.I_rated;   %Get T_rated from I_rated
        
    case 'Teknic2310P_IPM'
        pmsm.model  = 'Teknic-2310P';%              // Manufacturer Model Number
        pmsm.sn     = '004';         %           // Manufacturer Model Number
        pmsm.p      = 4;                %           // Pole Pairs for the motor
        pmsm.Rs     = 0.36;             %Ohm        // Stator Resistor
        pmsm.Ld     = 0.2e-3;           %H          // D-axis inductance value
        pmsm.Lq     = 0.4e-3;           %H          // Q-axis inductance value
        pmsm.J      = 7.061551833333e-6;%Kg-m2      // Inertia in SI units
        pmsm.B      = 2.636875217824e-6;%Kg-m2/s    // Friction Co-efficient
        pmsm.Ke     = 4.64;             %Bemf Const	// Vline_peak/krpm
        pmsm.Kt     = 0.274;            %Nm/A       // Torque constant
        pmsm.I_rated= 7.1;              %A      	// Rated current (phase-peak)
        pmsm.N_max  = 6000;             %rpm        // Max speed
        pmsm.PositionOffset = 0.1712;	%PU position// Position Offset
        pmsm.QEPSlits       = 1000;     %           // QEP Encoder Slits
        pmsm.FluxPM     = (pmsm.Ke)/(sqrt(3)*2*pi*1000*pmsm.p/60); %PM flux computed from Ke
        pmsm.T_rated    = (3/2)*pmsm.p*pmsm.FluxPM*pmsm.I_rated;   %Get T_rated from I_rated
        
        case 'Microchip_motor'
        pmsm.model  = 'Long_Hurst';%              // Manufacturer Model Number
        pmsm.sn     = '005';         %           // Manufacturer Model Number
        pmsm.p      = 5;                %           // Pole Pairs for the motor
        pmsm.Rs     = 0.285;             %Ohm        // Stator Resistor
        pmsm.Ld     = 0.00032;           %H          // D-axis inductance value
        pmsm.Lq     = 0.00032;           %H          // Q-axis inductance value
        pmsm.J      = 7.061551833333e-6;%Kg-m2      // Inertia in SI units
        pmsm.B      = 2.636875217824e-6;%Kg-m2/s    // Friction Co-efficient
        pmsm.Ke     = 7.24;             %Bemf Const	// Vline_peak/krpm
        pmsm.Kt     = 0.274;            %Nm/A       // Torque constant
        pmsm.I_rated= 3.42*sqrt(2);              %A      	// Rated current (phase-peak)
        pmsm.N_max  = 3644;             %rpm        // Max speed
        pmsm.PositionOffset = 0.1712;	%PU position// Position Offset
        pmsm.QEPSlits       = 250;     %           // QEP Encoder Slits
        pmsm.FluxPM     = (pmsm.Ke)/(sqrt(3)*2*pi*1000*pmsm.p/60); %PM flux computed from Ke
        pmsm.T_rated    = (3/2)*pmsm.p*pmsm.FluxPM*pmsm.I_rated;   %Get T_rated from I_rated
        
    otherwise
        disp('Creating new parameter structure.');
        pmsm.model  = 'new';%                       // Manufacturer Model Number
        pmsm.sn     = '005';         %           // Manufacturer Model Number
        pmsm.p      = 1;                %           // Pole Pairs for the motor
        pmsm.Rs     = 1;                %Ohm        // Stator Resistor
        pmsm.Ld     = 1e-3;             %H          // D-axis inductance value
        pmsm.Lq     = 1e-3;             %H          // Q-axis inductance value
        pmsm.J      = 1e-6;             %Kg-m2      // Inertia in SI units
        pmsm.B      = 1e-6;             %Kg-m2/s    // Friction Co-efficient
        pmsm.Ke     = 1;                %Bemf Const	// Vline_peak/krpm
        pmsm.Kt     = 1;                %Nm/A       // Torque constant
        pmsm.I_rated= 1;                %A      	// Rated current (phase-peak)
        pmsm.N_max  = 1;                %rpm        // Max speed
        pmsm.PositionOffset = 1;		%PU position// Position Offset
        pmsm.QEPSlits       = 1;        %           // QEP Encoder Slits
        pmsm.FluxPM     = (pmsm.Ke)/(sqrt(3)*2*pi*1000*pmsm.p/60); %PM flux computed from Ke
        pmsm.T_rated    = (3/2)*pmsm.p*pmsm.FluxPM*pmsm.I_rated;   %Get T_rated from I_rated
end
if evalin('base', 'exist(''motorParam'', ''var'')')
    if evalin('base', 'numel(fieldnames(motorParam))') >= 9
        motorParam = evalin('base', 'motorParam');
        pmsm.p      = motorParam.p;     %           // Pole Pairs for the motor
        pmsm.Rs     = motorParam.Rs;	%Ohm        // Shunt Resistor
        pmsm.Ld     = motorParam.Ld;	%H          // D-axis inductance value
        pmsm.Lq     = motorParam.Lq;	%H          // Q-axis inductance value
        pmsm.J      = motorParam.J;     %Kg-m2      // Inertia in SI units
        pmsm.B      = motorParam.B;     %Kg-m2/s    // Friction Co-efficient
        pmsm.Ke     = motorParam.Ke;	%Bemf Const	// Vline_peak/krpm
        pmsm.FluxPM     = (pmsm.Ke)/(sqrt(3)*2*pi*1000*pmsm.p/60);  %PM flux computed from Ke
        pmsm.I_rated= motorParam.nomCurrent / sqrt(2);                %A      	// Rated current (phase-peak)
		pmsm.T_rated    = (3/2)*pmsm.p*pmsm.FluxPM*pmsm.I_rated;    %Get T_rated from I_rated
        pmsm.N_max  = motorParam.ratedSpeed;                %rpm       // Max speed
        pmsm.PositionOffset = motorParam.PositionOffset;          % PU angle for Hall Offset
    end
end
end
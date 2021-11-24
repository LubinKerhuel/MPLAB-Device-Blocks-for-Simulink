function inverter = mcb_SetInverterParameters(inverter_type)
%% ************************************************************************
% File          :   Inverter Parameter File
% Description   :   Parameters for Inverters supported by MCB
% Authors       :   A. Selvaraj, H. Gulhane, D. Pandit, S. Narayan
% Copyright 2020 The MathWorks, Inc.
%% ************************************************************************
% Get basic parameters
    switch inverter_type 
        case 'BoostXL-DRV8301'
            inverter.model          = 'BoostXL-DRV8301';% Manufacturer Model Number
            inverter.sn             = 'INV_XXXX';         % Manufacturer Serial Number
            inverter.V_dc           = 24;       %V      // DC Link Voltage of the Inverter
            %Note: inverter.I_max = 1.65V/(Rshunt*10V/V) for 3.3V ADC with offset of 1.65V
            %Requires modification to match 3V ADC with 1.65V offset value for %LaunchXL-F28379D
            inverter.I_max          = 16.5;     %Amps   // Max current that can be measured by 3.3V ADC
            inverter.I_trip         = 10;       %Amps   // Max current for trip
            inverter.Rds_on         = 6.5e-3;   %Ohms   // Rds ON for BoostXL-DRV8301
            inverter.Rshunt         = 0.010;    %Ohms   // Rshunt for BoostXL-DRV8301
            inverter.MaxADCCnt      = 4095;     %Counts // ADC Counts Max Value
            inverter.CtSensAOffset  = 2048;     %Counts // ADC Offset for phase-A 
            inverter.CtSensBOffset  = 2048;     %Counts // ADC Offset for phase-B
            inverter.ADCGain        = 1;        %       // ADC Gain factor scaled by SPI

        case 'BoostXL-DRV8305'
            inverter.model          = 'BoostXL-DRV8305';        % Manufacturer Model Number
            inverter.sn             = 'INV_XXXX';               % Manufacturer Serial Number
            inverter.V_dc           = 24;                       %V      // DC Link Voltage of the Inverter
            %Note: inverter.I_max = 1.65V/(Rshunt*10V/V) for 3.3V ADC with offset of 1.65V
            %This is modified to match 3V ADC with 1.65V offset value for %LaunchXL-F28379D
            inverter.I_max          = 23.5714*((3-1.65)/1.65);  %Amps   // Max current that can be measured by 3.0V ADC
            inverter.I_trip         = 10;                       %Amps   // Max current for trip
            inverter.Rds_on         = 2e-3;                     %Ohms   // Rds ON for BoostXL-DRV8305
            inverter.Rshunt         = 0.007;                    %Ohms   // Rshunt for BoostXL-DRV8305
            inverter.MaxADCCnt      = 4095;                     %Counts // ADC Counts Max Value
            inverter.CtSensAOffset  = 2295;                     %Counts // ADC Offset for phase-A 
            inverter.CtSensBOffset  = 2286;                     %Counts // ADC Offset for phase-B
            inverter.ADCGain        = 1;                        %       // ADC Gain factor scaled by SPI
            
        case 'DRV8312-C2-KIT'
            inverter.model          = 'DRV8312-C2-KIT'; % Manufacturer Model Number
            inverter.sn             = 'INV_XXXX';         % Manufacturer Serial Number
            inverter.V_dc           = 24;       %V      // DC Link Voltage of the Inverter
            inverter.I_max          = 8.65;     %Amps   // Max current that can be measured by 3.3V ADC
            inverter.I_trip         = 9.7;      %Amps   // Max current for trip
            inverter.Rds_on         = 80e-3;    %Ohms   // Rds ON for BoostXL-DRV8312
            inverter.Rshunt         = 0.010;    %Ohms   // Rshunt for BoostXL-DRV8318
            inverter.MaxADCCnt      = 4095;     %Counts // ADC Counts Max Value
            inverter.CtSensAOffset  = 2052;     %Counts // ADC Offset for phase-A 
            inverter.CtSensBOffset  = 2043;     %Counts // ADC Offset for phase-B
            inverter.ADCGain        = 1;        %       // ADC Gain factor scaled by SPI
            
        case {'GaN','BoostXL-3PHGANINV'}
            inverter.model          = 'BOOSTXL-3PhGaNInv';  % Manufacturer Model Number
            inverter.sn             = 'INV_XXXX';           % Manufacturer Serial Number
            inverter.V_dc           = 24;                   %V  // DC Link Voltage of the Inverter
            %Note: inverter.I_max = 1.65V/(Rshunt*20V/V) for 3.3V ADC with offset of 1.65V
            %This is modified to match 3V ADC with 1.65V offset value for %LaunchXL-F28379D
            inverter.I_max          = 16.5*((3-1.65)/1.65); %// Max current that can be measured by 3.0V ADC
            inverter.I_trip         = 12;                   %Amps   // Max current for trip
            inverter.Rds_on         = 20e-3;                %Ohms   // Rds ON for BoostXL-DRV8312
            inverter.Rshunt         = 0.005;                %Ohms   // Rshunt for BoostXL-DRV8312
            inverter.MaxADCCnt      = 4095;                 %Counts // ADC Counts Max Value
            inverter.CtSensAOffset  = 2247;                 %Counts // ADC Offset for phase-A 
            inverter.CtSensBOffset  = 2245;                 %Counts // ADC Offset for phase-B
            inverter.ADCGain        = 1;                    %       // ADC Gain factor scaled by SPI
            
         case 'Microchip_inverter'
            inverter.model          = 'Microchip_inverter';% Manufacturer Model Number
            inverter.sn             = 'INV_XXXX';         % Manufacturer Serial Number
            inverter.V_dc           = 24;       %V      // DC Link Voltage of the Inverter
            %Note: inverter.I_max = 1.65V/(Rshunt*10V/V) for 3.3V ADC with offset of 1.65V
            %Requires modification to match 3V ADC with 1.65V offset value for %LaunchXL-F28379D
            inverter.ISenseMax      = 4.4;     %Amps   // Max current that can be measured by 3.3V ADC
            inverter.I_trip         = 10;       %Amps   // Max current for trip
            inverter.Rds_on         = 6.5e-3;   %Ohms   // Rds ON for BoostXL-DRV8301
            inverter.Rshunt         = 0.025;    %Ohms   // Rshunt for BoostXL-DRV8301
            inverter.MaxADCCnt      = 4095;     %Counts // ADC Counts Max Value
            inverter.CtSensAOffset  = 2048;     %Counts // ADC Offset for phase-A 
            inverter.CtSensBOffset  = 2048;     %Counts // ADC Offset for phase-B
            inverter.ADCGain        = 1;        %       // ADC Gain factor scaled by SPI
        
        otherwise
            disp('Creating new inverter parameter structure.');
            inverter.model          = 'new'; % Manufacturer Model Number
            inverter.sn             = 'INV_XXXX';         % Manufacturer Serial Number
            inverter.V_dc           = 1;        %V      // DC Link Voltage of the Inverter
            inverter.I_max          = 1;        %Amps   // Max current that can be measured by ADC
            inverter.I_trip         = 1;        %Amps   // Max current for trip
            inverter.Rds_on         = 1e-3;     %Ohms   // Rds ON for BoostXL-DRV8312
            inverter.Rshunt         = 1e-3;     %Ohms   // Rshunt for BoostXL-DRV8312
            inverter.MaxADCCnt      = 4095;     %Counts // ADC Counts Max Value
            inverter.CtSensAOffset  = 2048;     %Counts // ADC Offset for phase-A 
            inverter.CtSensBOffset  = 2048;     %Counts // ADC Offset for phase-B
            inverter.ADCGain        = 1;        %       // ADC Gain factor scaled by SPI
    end
    
    inverter.R_board        = inverter.Rds_on + inverter.Rshunt/3;  %Ohms

end






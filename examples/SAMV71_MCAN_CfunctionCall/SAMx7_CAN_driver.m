function SAMx7_CAN_driver() 

% parse .h file for structs to 
% create Simulink BUS Object for corresponding structs.

try
    % This function seems not Ok on windows setup. ok with linux
    % reported on 10/09/2021 - R2021a
    evalin('base','Simulink.importExternalCTypes(''SAMx7_CAN_driver.h'')');
catch
    % in case bug is not solved 
    % load('MCHP_MCAN_Frame_t.mat'); Let's create it for more flexibility    

    clear elems;
    elems(1) = Simulink.BusElement;
    elems(1).Name = 'ID';
    elems(1).Dimensions = 1;
    elems(1).DimensionsMode = 'Fixed';
    elems(1).DataType = 'uint32';
    elems(1).SampleTime = -1;
    elems(1).Complexity = 'real';
    elems(1).Min = [];
    elems(1).Max = [];

    elems(2) = Simulink.BusElement;
    elems(2).Name = 'buffer';
    elems(2).Dimensions = 8;
    elems(2).DimensionsMode = 'Fixed';
    elems(2).DataType = 'uint8';
    elems(2).SampleTime = -1;
    elems(2).Complexity = 'real';
    elems(2).Min = [];
    elems(2).Max = [];


    elems(3) = Simulink.BusElement;
    elems(3).Name = 'DLC';
    elems(3).Dimensions = 1;
    elems(3).DimensionsMode = 'Fixed';
    elems(3).DataType = 'int16';
    elems(3).SampleTime = -1;
    elems(3).Complexity = 'real';
    elems(3).Min = [];
    elems(3).Max = [];    
    
    MCHP_MCAN_Frame_t = Simulink.Bus;
    MCHP_MCAN_Frame_t.HeaderFile = '';
    MCHP_MCAN_Frame_t.Description = sprintf('');
    MCHP_MCAN_Frame_t.DataScope = 'Auto';
    MCHP_MCAN_Frame_t.Alignment = -1;
    MCHP_MCAN_Frame_t.Elements = elems;
    assignin('base', 'MCHP_MCAN_Frame_t', MCHP_MCAN_Frame_t) ;   
end


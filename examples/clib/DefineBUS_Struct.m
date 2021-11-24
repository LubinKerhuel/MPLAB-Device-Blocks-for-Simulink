function DefineBUS_Struct() 
% Initialize a bus object in the MATLAB base workspace for use with C function Call using struct

% Bus object: BM180_Caldata 
clear elems;
elems(1) = Simulink.BusElement;
elems(1).Name = 'Val1';
elems(1).Dimensions = 1;
elems(1).DimensionsMode = 'Fixed';
elems(1).DataType = 'uint16';
elems(1).SampleTime = -1;
elems(1).Complexity = 'real';
elems(1).SamplingMode = 'Sample based';
elems(1).Min = [];
elems(1).Max = [];

elems(2) = Simulink.BusElement;
elems(2).Name = 'Val2';
elems(2).Dimensions = 1;
elems(2).DimensionsMode = 'Fixed';
elems(2).DataType = 'uint16';
elems(2).SampleTime = -1;
elems(2).Complexity = 'real';
elems(2).SamplingMode = 'Sample based';
elems(2).Min = [];
elems(2).Max = [];

myStruct = Simulink.Bus;
myStruct.HeaderFile = '';
myStruct.Description = sprintf('');
myStruct.DataScope = 'Auto';
myStruct.Alignment = -1;
myStruct.Elements = elems;
assignin('base', 'myStruct', myStruct)


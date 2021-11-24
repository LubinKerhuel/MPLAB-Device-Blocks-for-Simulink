function sl_customization(cm)
% MCHP Connectivity Config

%cm.registerTargetInfo(@loc_register_device);
cm.registerTargetInfo(@loc_createConfig_PIC32);
cm.registerTargetInfo(@loc_createConfig_SAM7);
cm.registerTargetInfo(@loc_createConfig);

% Register Optimisation options for PIC32
%cm.registerTargetInfo(@TfldsPIC_OperationEntry_PIC32_C_src);   %Include C based Code Replacement Libraries for PIC32

% Register Optimisation options for dsPIC
%cm.registerTargetInfo(@TfldsPIC_OperationEntry_ASM);            %Include ASM based Code Replacement Libraries   
cm.registerTargetInfo(@TfldsPIC_OperationEntry_MACRO);          %Include MACRO based Code Replacement Libraries
%cm.registerTargetInfo(@TfldsPIC_OperationEntry_C_DSP);          %Include C-DSP based Code Replacement Libraries


% 1.  ext_serial_win32_comm
%     a.  This is the officially supported host-side classic External mode serial protocol
%     b.  This protocol uses some flow control hand shaking to avoid receive buffer overruns
% 2.  ext_serial_win32_comm_no_acks
%     a.  This is a version of the host-side classic External mode without the flow control hand shaking
%     b.  It can be useful in circumstances where the flow control is not required, but it is not documented
% 3.  ext_serial_win32_comm_no_acks_rtt
%     a.  This is a customized build of #2 above to support some of our internal targets
%     b.  I would definitely not recommend using this one

% ext_serial_win32_comm_no_acks Ok
% ext_serial_win32_comm NOk
%External mode
%cm.ExtModeTransports.add('MCHP_dsPIC_stf.tlc', 'serial', 'ext_serial_win32_comm', 'Level1'); 
%cm.ExtModeTransports.add('MCHP_pic32_stf.tlc', 'serial', 'ext_serial_win32_comm', 'Level1');
%cm.ExtModeTransports.add('MCHP_ARM7_stf.tlc', 'serial', 'ext_serial_win32_comm', 'Level1');
%cm.ExtModeTransports.add('MCHP_ARM4F_stf.tlc', 'serial', 'ext_serial_win32_comm', 'Level1');
%cm.ExtModeTransports.add('MCHP_ARM0p_stf.tlc', 'serial', 'ext_serial_win32_comm', 'Level1');

cm.ExtModeTransports.add('MCHP_dsPIC_stf.tlc', 'serialNoAck', 'ext_serial_win32_comm_no_acks', 'Level1'); 
cm.ExtModeTransports.add('MCHP_pic32_stf.tlc', 'serialNoAck', 'ext_serial_win32_comm_no_acks', 'Level1');
cm.ExtModeTransports.add('MCHP_ARM7_stf.tlc', 'serialNoAck', 'ext_serial_win32_comm_no_acks', 'Level1');
cm.ExtModeTransports.add('MCHP_ARM4F_stf.tlc', 'serialNoAck', 'ext_serial_win32_comm_no_acks', 'Level1');
cm.ExtModeTransports.add('MCHP_ARM0p_stf.tlc', 'serialNoAck', 'ext_serial_win32_comm_no_acks', 'Level1');

cm.ExtModeTransports.add('MCHP_dsPIC_stf.tlc', 'XCP on Serial', 'ext_xcp', 'Level2 - Open');
cm.ExtModeTransports.add('MCHP_pic32_stf.tlc', 'XCP on Serial', 'ext_xcp', 'Level2 - Open');
cm.ExtModeTransports.add('MCHP_ARM7_stf.tlc', 'XCP on Serial', 'ext_xcp', 'Level2 - Open');
cm.ExtModeTransports.add('MCHP_ARM4F_stf.tlc', 'XCP on Serial', 'ext_xcp', 'Level2 - Open');
cm.ExtModeTransports.add('MCHP_ARM0p_stf.tlc', 'XCP on Serial', 'ext_xcp', 'Level2 - Open');


% Added menues to existing Simulink menue:
cm.addCustomMenuFcn('Simulink:MenuBar', @MCHP_sl_customization_Menu);
%cm.addCustomMenuFcn('Simulink:ToolsMenu', @getMyMenuItems);
%cm.addCustomMenuFcn('Simulink:PreContextMenu', @getMyMenuItems);
%cm.addCustomMenuFcn('Simulink:ContextMenu', @getMyMenuItems);
%cm.addCustomMenuFcn('Simulink:PreContextMenu', @getMyMenuItems2);
%Added new menue

function config = loc_createConfig

config = rtw.connectivity.ConfigRegistry;
config.ConfigName = 'Microchip connectivity using UART';
config.ConfigClass = 'mchppil.ConnectivityConfig';
% matching system target file
config.SystemTargetFile = {'MCHP_dsPIC_stf.tlc'};
% matching template makefile
config.TemplateMakefile = {'MCHP_dsPIC.tmf'};
config.TargetHWDeviceType = {'Generic->Custom'};


function config = loc_createConfig_PIC32

config = rtw.connectivity.ConfigRegistry;
config.ConfigName = 'Microchip PIC32 connectivity using UART';
config.ConfigClass = 'mchppil_PIC32.ConnectivityConfig';
% matching system target file
config.SystemTargetFile = {'MCHP_pic32_stf.tlc'};
% matching template makefile
config.TemplateMakefile = {'MCHP_pic32.tmf'};
config.TargetHWDeviceType = {'Generic->Custom'};


function config = loc_createConfig_SAM7

config = rtw.connectivity.ConfigRegistry;
config.ConfigName = 'Microchip SAMx connectivity using UART';
config.ConfigClass = 'mchppil_SAMx.ConnectivityConfig';
% matching system target file
config.SystemTargetFile = {'MCHP_ARM7_stf.tlc'};
% matching template makefile
%config.TemplateMakefile = {'MCHP_SAM7.tmf'};
config.TemplateMakefile = {};
% matching ToolChain
%config.Toolchain = rtw.connectivity.Utils.getHostToolchainNames();  
%config.Toolchain = {'XC32 C Compiler'};%{'XC32 C Compiler'}; %reg_xc32_SAMx70_tc
%config.TargetHWDeviceType = {'Generic->Custom'};

% [isOk XCxxPath_out cmplrversions compilerexe] = MCHP_findCompilerPath('c32','');
% for iC = 1:numel(XCxxPath_out)    
%     ToolchainNames{iC} = ['xc32 - v' cmplrversions{iC}{1} ' for Mirochip SAMx7 device'];
% end
ToolchainNames = {};
config.Toolchain = ToolchainNames;


%% Optimisation 

% function thisTfl = TfldsPIC_OperationEntry_ASM
% %%Instantiate a TFL registry entry
% thisTfl = RTW.TflRegistry;
% %%Define the TFL properties
% thisTfl.Name = 'Microchip Optimisation for dsPIC - ASM lib';
% thisTfl.Description = 'Microchip Optimisation for dsPIC - ASM lib';
% thisTfl.TableList = {...
% 					 'MCHP_Optim_TFL_dsPIC_ASM_MMUL',...
% 					 'MCHP_Optim_TFL_dsPIC_ASM_Atan2',...
% 					 'MCHP_Optim_TFL_dsPIC_ASM_SQRT',...
%                      'MCHP_Optim_TFL_dsPIC_ASM_SIN', ...
%                      'MCHP_Optim_TFL_dsPIC_ASM_COS', ...
%                      'MCHP_Optim_TFL_dsPIC_ASM_SINCOS', ...
%                      'MCHP_Optim_TFL_dsPIC_MACRO_ABS',...
%                      'MCHP_Optim_TFL_dsPIC_MACRO_AddSubSat', ...
%  					}; 
% thisTfl.BaseTfl = 'C89/C90 (ANSI)';
% thisTfl.TargetHWDeviceType = {'*'};  %% might use 'Generic->Custom'


function thisTfl = TfldsPIC_OperationEntry_MACRO
%%Instantiate a TFL registry entry
thisTfl = RTW.TflRegistry;
%%Define the TFL properties
thisTfl.Name = 'Microchip Code Replacement Library for dsPIC';
thisTfl.Description = 'Code Replacement Library used to generate optimized code for dsPIC devices.';
thisTfl.TableList = {...
					 'MCHP_Optim_TFL_dsPIC_ASM_MMUL',...
					 'MCHP_Optim_TFL_dsPIC_MACRO_Atan2',...
					 'MCHP_Optim_TFL_dsPIC_MACRO_SQRT',...
                     'MCHP_Optim_TFL_dsPIC_MACRO_SQRT32',...
                     'MCHP_Optim_TFL_dsPIC_MACRO_SIN', ...
                     'MCHP_Optim_TFL_dsPIC_MACRO_COS', ...
                     'MCHP_Optim_TFL_dsPIC_MACRO_SINCOS', ...
                     'MCHP_Optim_TFL_dsPIC_MACRO_ABS',...
                     'MCHP_Optim_TFL_dsPIC_MACRO_AddSubSat', ...
                     'MCHP_Optim_TFL_dsPIC_MACRO_Saturate',...
 					}; 
%thisTfl.BaseTfl = 'C89/C90 (ANSI)';
thisTfl.TargetHWDeviceType = {'*'};


% function thisTfl = TfldsPIC_OperationEntry_C_DSP
% %%Instantiate a TFL registry entry
% thisTfl = RTW.TflRegistry;
% %%Define the TFL properties
% thisTfl.Name = 'Microchip Optimisation for dsPIC - C/C-DSP';
% thisTfl.Description = 'Microchip Optimisation for dsPIC - C/C-DSP';
% thisTfl.TableList = {...
%                      'MCHP_Optim_TFL_dsPIC_C_Atan2', ...
%                      'MCHP_Optim_TFL_dsPIC_C_SIN' , ...
%                      'MCHP_Optim_TFL_dsPIC_C_COS', ...
%                      'MCHP_Optim_TFL_dsPIC_C_SINCOS', ...
%                      'MCHP_Optim_TFL_dsPIC_C_SQRT',...
%                      'MCHP_Optim_TFL_dsPIC_C_SQRT32',...
% 					 'MCHP_Optim_TFL_dsPIC_ASM_MMUL',...      
%                      'MCHP_Optim_TFL_dsPIC_MACRO_ABS',...
%                      'MCHP_Optim_TFL_dsPIC_MACRO_AddSubSat', ...
%  					}; 
% thisTfl.BaseTfl = 'C89/C90 (ANSI)';
% thisTfl.TargetHWDeviceType = {'*'};


function thisTfl = TfldsPIC_OperationEntry_PIC32_C_src
%%Instantiate a TFL registry entry
thisTfl = RTW.TflRegistry;
%%Define the TFL properties
thisTfl.Name = 'Microchip Optimisation for PIC32 - C (experimental)';
thisTfl.Description = 'Microchip Optimisation for PIC32 - C (experimental)';
thisTfl.TableList = {...
					 'MCHP_Optim_TFL_PIC32_C_SINCOS',...
 					}; 
%thisTfl.BaseTfl = 'C89/C90 (ANSI)';
thisTfl.TargetHWDeviceType = {'*'};  % might use 'Generic->Custom'
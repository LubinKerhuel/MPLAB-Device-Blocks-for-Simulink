function blkStruct = slblocks
%SLBLOCKS Defines the block library for a specific Toolbox or Blockset.


% Information for "Blocksets and Toolboxes" subsystem
blkStruct.Name = sprintf('Microchip Target');
blkStruct.OpenFcn = 'MCHP_Blockset';
blkStruct.MaskInitialization = '';
blkStruct.Mask.Display = 'MCHP_DrawLogo;disp(''Microchip Target'')';

% Define the library list for the Simulink Library browser.
% Return the name of the library model and the name for it

Browser(1).Library = 'MCHP_Blockset';
Browser(1).Name    = 'MPLAB Device Blocks for Simulink';
Browser(1).IsFlat  = 0;% Is this library "flat" (i.e. no subsystems)?

blkStruct.Browser = Browser;

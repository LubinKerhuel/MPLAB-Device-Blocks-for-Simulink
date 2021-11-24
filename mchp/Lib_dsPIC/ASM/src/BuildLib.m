clc
clear all

mkdir('obj') 

[A B C D] = MCHP_findCompilerPath('c30','');

GCCPATH = strrep([B{1} filesep 'bin'],'\','/');
%GCCPATH = strrep([RTW.transformPaths(B{1}) filesep 'bin'],'\','/');

cmd = ['!gmake -f MakeLib.mk OUTPUT="MCHP_CRL_ASM" OMF="elf" GCCPATH="' GCCPATH '" -j 9 -l 9'];
eval(cmd);
%!"C:\Program Files (x86)\Microchip\xc16\v1.26\bin\xc16-ar" -t MCHP_CRL_ASM_elf.a

cmd = ['!gmake -f MakeLib.mk OUTPUT="MCHP_CRL_ASM" OMF="coff" GCCPATH="' GCCPATH '" -j 9 -l 9'];
eval(cmd);

delete('*.lst');
rmdir('obj','s');

display('******** Done! ********');
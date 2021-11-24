[A B C D] = MCHP_findCompilerPath('c30','');

GCCPATH = strrep([B{1} filesep 'bin'],'\','/');
%GCCPATH = strrep([RTW.transformPaths(B{1}) filesep 'bin'],'\','/');

cmd = ['!gmake -f MakeLib.mk OUTPUT="MCHP_CRL_CDSP" OMF="elf" GCCPATH="' GCCPATH '" ']; %-j 9 -l 9
eval(cmd);

cmd = ['!gmake -f MakeLib.mk OUTPUT="MCHP_CRL_CDSP" OMF="coff" GCCPATH="' GCCPATH '" ']; % -j 9 -l 9
eval(cmd);

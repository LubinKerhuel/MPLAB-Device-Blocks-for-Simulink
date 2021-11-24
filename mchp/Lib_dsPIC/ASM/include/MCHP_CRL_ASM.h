/* Auto Generated file. Author: Lubin Kerhuel, Microchip Technology Inc */
#ifndef _MCHP_CRL_ASM_
#define _MCHP_CRL_ASM_

	extern int MCHP_ASM_sqrt_Q15( int u1);
	extern int MCHP_ASM_sqrt_Qxx( int u1, int u2);
	extern signed char MCHP_ASM_sqrt_Qxx_8( int u1, int u2);
	extern int MCHP_ASM_sqrt_Qxx_Sat( int u1, int u2, int u3);
	extern signed char MCHP_ASM_sqrt_Qxx_8_Sat( int u1, int u2,int u3);

    extern unsigned int MCHP_ASM_Atan2( unsigned int u2, unsigned int u1);
   	
    extern unsigned int MCHP_ASM_Sin( int u1, unsigned int u2, unsigned int u3);
    extern unsigned int MCHP_ASM_SinU( unsigned int u1, unsigned int u2, unsigned int u3);
 
    extern unsigned int MCHP_ASM_Cos( int u1, unsigned int u2, unsigned int u3);
    extern unsigned int MCHP_ASM_CosU( unsigned int u1, unsigned int u2, unsigned int u3);
    

    extern void MCHP_ASM_SinCos(int * y1, int * y2, int u1, unsigned int u2, unsigned int u3);
    extern void MCHP_ASM_SinCosU(unsigned int * y1, unsigned int * y2, int u1, unsigned int u2, unsigned int u3);
    
 
#endif

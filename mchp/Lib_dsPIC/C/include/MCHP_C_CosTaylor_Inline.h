#ifndef C_COS_TAYLOR_H
#define C_COS_TAYLOR_H

#include <xc.h>
	
static inline  __attribute__((always_inline)) unsigned int MCHP_C_CosTaylor_Inline(signed int u1,unsigned int u2,unsigned int u3) 
{
    union a{
        unsigned int  u;
        signed int    s;
    } y,in1,in2,in3,REG_W0,REG_W4,REG_W5,REG_W6,REG_W7, temp;
    
	in1.s = u1;
	in2.u = u2;
	in3.u = u3;
	
    signed long int ACCUM;

	REG_W5.s = ((signed long int) in1.s * in3.u) >> 16u;
    REG_W6.s = (signed long int) in1.s * in2.u;
    REG_W4.u = REG_W5.u + REG_W6.u;
    REG_W0.u = REG_W4.u;
    
    if (REG_W0.u & 0x4000){
        REG_W4.u = ~ REG_W4.u;
    }
      
    ACCUM = -32768L << 16u; 	// mimic Accumulator loading
	REG_W4.u = REG_W4.u << 2u;
	
    REG_W7.u = ((unsigned long int) REG_W4.u * REG_W4.u) >> 16u;
    temp.u = 40420u;
    ACCUM += (signed long int)temp.u * REG_W7.u;
	
    REG_W5.u = ((unsigned long int) REG_W7.u * REG_W7.u) >> 16u;
    temp.u = 8276u;
    ACCUM -= (signed long int)temp.u * REG_W5.u;
	
    REG_W5.u = ((unsigned long int) REG_W5.u * REG_W7.u) >> 16u;
    temp.u = 625u;
    ACCUM += (signed long int)temp.u * REG_W5.u;
    
	temp.u = 16384u;
    REG_W0.u = REG_W0.u + temp.u;
    
	if(REG_W0.s > 0){
		ACCUM = -ACCUM;
		y.s = ACCUM >> 16;	//don't shift directly by 17. ACCUM will be sign-extended due to the arithmetic shift performed.
		y.u = y.u >> 1;
    }
    else{
		y.s = ACCUM >> 16;	//don't shift directly by 17. ACCUM will be sign-extended due to the arithmetic shift performed.
		y.s = y.s >> 1;
    }
    
    // ACCUM = ACCUM >> 16u;   
    // if(REG_W0.s > 0){
    // ACCUM = -ACCUM;
    // };	
    // y.u = ACCUM >> 1;
    
	return y.u;
}

#endif
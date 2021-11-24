#ifndef C_COSU_TAYLOR_H
#define C_COSU_TAYLOR_H

#include <xc.h>

static inline __attribute__((always_inline)) unsigned int MCHP_C_CosUTaylor_Inline(unsigned int u1,unsigned int u2,unsigned int u3) 
{
    union a{
        unsigned int  u;
        signed int    s;
    } y,REG_W0,REG_W4,REG_W5,REG_W6,REG_W7, temp;

	signed long int ACCUM;
	ACCUM = -32768L << 16u; 	// mimic Accumulator loading
    
	REG_W5.u = ((unsigned long int) u1 * u3) >> 16u;
    REG_W6.u = (unsigned int)u1 * u2;
    REG_W4.u = REG_W5.u + REG_W6.u;
    REG_W0.u = REG_W4.u + 16384u;
	
    if (REG_W4.u & 0x4000){
        REG_W4.u = ~ REG_W4.u;
    }
      
	REG_W4.u = REG_W4.u << 2u;
	
    REG_W5.u = ((unsigned long int) REG_W4.u * REG_W4.u) >> 16u;
    REG_W6.u = ((unsigned long int) REG_W5.u * REG_W5.u) >> 16u;
    REG_W7.u = ((unsigned long int) REG_W5.u * REG_W6.u) >> 16u;
    
    temp.u = 40420u;
    ACCUM += (signed long int)temp.u * REG_W5.u;

    temp.u = 8276u;
    ACCUM -= (signed long int)temp.u * REG_W6.u;

    temp.u = 625u;
    ACCUM += (signed long int)temp.u * REG_W7.u;

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
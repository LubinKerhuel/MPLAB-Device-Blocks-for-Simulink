#ifndef C_SINCOSU_TAYLOR_H
#define C_SINCOSU_TAYLOR_H

#include <xc.h>


static inline __attribute__((always_inline)) void MCHP_C_SinCosUTaylor_Inline(unsigned int * sin_out, unsigned int * cos_out, unsigned int u1,unsigned int u2,unsigned int u3) 
{
    union a{
        unsigned int  u;
        signed int    s;
    } y,REG_W0,REG_W4,REG_W5,REG_W6,REG_W7, temp,
       REG_W5_SAVED, REG_W6_SAVED, REG_W4_SAVED, REG_W0_SAVED;

	signed long int ACCUM;
    
	ACCUM = -32768L << 16u; 	// mimic Accumulator loading
    
	REG_W5.u = ((unsigned long int) u1 * u3) >> 16u;
    REG_W6.u = (unsigned int)u1 * u2;
    REG_W4.u = REG_W5.u + REG_W6.u;
    REG_W0.u = REG_W4.u ; //+ 16384u;
	
	//Save intermediate scaling information to compute Cosine later
    REG_W5_SAVED.s = REG_W5.s;
    REG_W6_SAVED.s = REG_W6.s;
    REG_W4_SAVED.u = REG_W4.u;
    REG_W0_SAVED.u = REG_W0.u;
    
	// Compute Sine	
    if (!(REG_W4.u & 0x4000)){
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

    if(!(REG_W0.u & 0x8000)){
        ACCUM = -ACCUM;
        y.s = ACCUM >> 16;	//don't shift directly by 17. ACCUM will be sign-extended due to the arithmetic shift performed.
        y.u = y.u >> 1;
    }
    else{
        y.s = ACCUM >> 16;	//don't shift directly by 17. ACCUM will be sign-extended due to the arithmetic shift performed.
        y.s = y.s >> 1;
    }
    
	*sin_out = y.u;    
    
    //Restore intermediate results
	REG_W5.u = ((unsigned long int) u1 * u3) >> 16u;
    REG_W6.u = (unsigned int)u1 * u2;
    REG_W4.u = REG_W5.u + REG_W6.u;
    REG_W0.u = REG_W4.u + 16384u;
	    
    //Compute Cos
	ACCUM = -32768L << 16u; 	// mimic Accumulator loading
	
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
    
	*cos_out = y.u; 
}





/*
static inline __attribute__((always_inline)) void MCHP_C_SinCosUTaylor_Inline(unsigned int * sin_out, unsigned int * cos_out, unsigned int u1,unsigned int u2,unsigned int u3) 
{
    union a{
        unsigned int  u;
        signed int    s;
    } in1,in2,in3,REG_W0,REG_W1,REG_W2,REG_W4,REG_W5,REG_W6,REG_W7, temp;
    
	in1.u = u1;
	in2.u = u2;
	in3.u = u3;

    signed long int ACCUM;

    //Compute Sin
	REG_W5.u = ((unsigned long int) in1.u * in3.u) >> 16u;
    REG_W6.u = (unsigned long int) in1.u * in2.u;
    REG_W4.u = REG_W5.u + REG_W6.u;
    REG_W0.u = REG_W4.u;
    REG_W1.u = REG_W4.u << 2u;
    
    REG_W5.s =  -32768;
    ACCUM = (signed long int)REG_W5.s << 16u; 	
    
    REG_W7.u = ((unsigned long int) REG_W1.u * REG_W1.u) >> 16u;
    REG_W6.u = 40420u;
    ACCUM += (signed long int)REG_W6.u * REG_W7.u;

    REG_W5.u = ((unsigned long int) REG_W7.u * REG_W7.u) >> 16u;
    REG_W4.u = 8276u;
    ACCUM -= (signed long int)REG_W4.u * REG_W5.u;
	
    REG_W5.u = ((unsigned long int) REG_W5.u * REG_W7.u) >> 16u;
    REG_W4.u = 625u;
    ACCUM += (signed long int)REG_W4.u * REG_W5.u;
   
	if(REG_W0.s > 0){
		ACCUM = -ACCUM;
		REG_W2.s = ACCUM >> 16;	//don't shift directly by 17. ACCUM will be sign-extended due to the arithmetic shift performed.
		REG_W2.u = REG_W2.u >> 1;
    }
    else{
		REG_W2.s = ACCUM >> 16;	//don't shift directly by 17. ACCUM will be sign-extended due to the arithmetic shift performed.
		REG_W2.s = REG_W2.s>> 1;
    }
    
    //Compute Cos
    REG_W1.u = ~REG_W1.u;
    
    REG_W5.s =  -32768;
    ACCUM = (signed long int)REG_W5.s << 16u;
    
    REG_W7.u = ((unsigned long int) REG_W1.u * REG_W1.u) >> 16u;
    REG_W6.u = 40420u;
    ACCUM += (signed long int)REG_W6.u * REG_W7.u;
    
    REG_W5.u = ((unsigned long int) REG_W7.u * REG_W7.u) >> 16u;
    REG_W4.u = 8276u;
    ACCUM -= (signed long int)REG_W4.u * REG_W5.u;
    
    REG_W5.u = ((unsigned long int) REG_W5.u * REG_W7.u) >> 16u;
    REG_W4.u = 625u;
    ACCUM += (signed long int)REG_W4.u * REG_W5.u;
    
	if(REG_W0.s < 0){
		ACCUM = -ACCUM;
    }
    
    if (!(REG_W0.u & 0x4000)){
        ACCUM = - ACCUM;
    }

    REG_W1.s = ACCUM >> 16u;	//don't shift directly by 17. ACCUM will be sign-extended due to the arithmetic shift performed.
    REG_W1.s = REG_W1.s >> 1u;
    
    if (!(REG_W0.s & 0x4000)){
        temp.s = REG_W1.s;
        REG_W1.s = REG_W2.s;
        REG_W2.s = temp.s;
    }
    
    *sin_out = REG_W2.u;
    *cos_out = REG_W1.u;
}
*/

#endif
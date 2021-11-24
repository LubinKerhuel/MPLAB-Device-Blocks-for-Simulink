#ifndef C_SINCOS_TAYLOR_H
#define C_SINCOS_TAYLOR_H

#include <xc.h>


static inline __attribute__((always_inline)) void MCHP_C_SinCosTaylor_Inline(signed int * sin_out, signed int * cos_out, signed int u1,unsigned int u2,unsigned int u3) 
{
    union a{
        unsigned int  u;
        signed int    s;
    } y,in1,in2,in3,REG_W0,REG_W4,REG_W5,REG_W6,REG_W7, temp,
      REG_W5_SAVED, REG_W6_SAVED, REG_W4_SAVED, REG_W0_SAVED;
    
	in1.s = u1;
	in2.u = u2;
	in3.u = u3;
	
    signed long int ACCUM;

	REG_W5.s = ((signed long int) in1.s * in3.u) >> 16u;
    REG_W6.s = (signed long int) in1.s * in2.u;
    REG_W4.u = REG_W5.u + REG_W6.u;
    REG_W0.u = REG_W4.u;
    
	//Save intermediate scaling information to compute Cosine later
    REG_W5_SAVED.s = REG_W5.s;
    REG_W6_SAVED.s = REG_W6.s;
    REG_W4_SAVED.u = REG_W4.u;
    REG_W0_SAVED.u = REG_W0.u;
    
	// Compute Sine
    if (!(REG_W0.u & 0x4000)){
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
    
	if(!(REG_W0.u & 0x8000)){
		ACCUM = -ACCUM;
		y.s = ACCUM >> 16;	//don't shift directly by 17. ACCUM will be sign-extended due to the arithmetic shift performed.
		y.u = y.u >> 1;
    }
    else{
		y.s = ACCUM >> 16;	//don't shift directly by 17. ACCUM will be sign-extended due to the arithmetic shift performed.
		y.s = y.s >> 1;
    }
    
    *sin_out = y.s;
    
	//Restore intermediate results
    REG_W5.s = REG_W5_SAVED.s;
    REG_W6.s = REG_W6_SAVED.s;
    REG_W4.u = REG_W4_SAVED.u;
    REG_W0.u = REG_W0_SAVED.u;
    
	//Compute Cosine
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
    
    *cos_out = y.s;  
}




/*
static inline __attribute__((always_inline)) void MCHP_C_SinCosTaylor_Inline(signed int * sin_out, signed int * cos_out, signed int u1,unsigned int u2,unsigned int u3) 
{
    union a{
        unsigned int  u;
        signed int    s;
    } in1,in2,in3,REG_W0,REG_W1,REG_W2,REG_W4,REG_W5,REG_W6,REG_W7, temp;
    
	in1.s = u1;
	in2.u = u2;
	in3.u = u3;
    
    int guardBitsEnabled = 0;
    
    signed long int ACCUM;

    //Compute Sin
	REG_W5.s = ((signed long int) in1.s * in3.u) >> 16u;
    REG_W6.s = (signed long int) in1.s * in2.u;
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
        guardBitsEnabled = 1;
    }
    
    if (!(REG_W0.u & 0x4000)){
        ACCUM = - ACCUM;
        if( (ACCUM < 0) && (guardBitsEnabled == 0) ){
            REG_W1.s = ACCUM >> 16u;	
            REG_W1.u = REG_W1.u >> 1u;
        }
        else{
        	REG_W1.s = ACCUM >> 16u;	
            REG_W1.s = REG_W1.s >> 1u;    
        }
    }
	else{
		REG_W1.s = ACCUM >> 16u;	
		REG_W1.s = REG_W1.s >> 1u;
	}

 
    if (!(REG_W0.s & 0x4000)){
        temp.s = REG_W1.s;
        REG_W1.s = REG_W2.s;
        REG_W2.s = temp.s;
    }
    
    *sin_out = REG_W2.s;
    *cos_out = REG_W1.s;
}
*/

#endif
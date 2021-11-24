#ifndef C_SQRT_INLINE_H
#define C_SQRT_INLINE_H

#include <xc.h>

static inline __attribute__((always_inline)) int MCHP_C_Sqrt_Inline_Q15(signed int u1) 
{
    union a{
        unsigned int  u;
        signed int    s;
    } REG_W4, REG_W5, REG_W6, REG_W7, result, temp;
    
    signed long int ACCUM;
    unsigned int shiftValue;
	unsigned long int temp32u;
    result.s = 0;
    
    if (u1 > 0){
        
         asm volatile (
        "FF1L %[u1], %[shiftValue]     \n"
        : [shiftValue]"=r"(shiftValue)
        : [u1]"r"(u1)
        : "cc"
        );
         
        REG_W6.u = u1 <<  shiftValue;
        
        ACCUM = 0xB5050000;
        REG_W4.u = 23164u;
        ACCUM += (unsigned long int) REG_W4.u * REG_W6.u;
        
        REG_W5.u = ((unsigned long) REG_W6.u * REG_W6.u) >> 16u;
        REG_W4.u = 5721u;
        ACCUM -= (unsigned long int) REG_W4.u * REG_W5.u;
        
        REG_W5.u = ((unsigned long) REG_W6.u * REG_W5.u) >> 16u;
        REG_W4.u = 2570u;
        ACCUM += (unsigned long int) REG_W4.u * REG_W5.u;
        
        REG_W5.u = ((unsigned long) REG_W6.u * REG_W5.u) >> 16u;
        REG_W4.u = 1046u;
        ACCUM -= (unsigned long int) REG_W4.u * REG_W5.u;
        
        REG_W5.u = ((unsigned long) REG_W6.u * REG_W5.u) >> 16u;
        REG_W4.u = 228u;
        ACCUM += (unsigned long int) REG_W4.u * REG_W5.u;
        
        REG_W5.u = shiftValue >> 1;
        temp.u = (unsigned long int)ACCUM >> 16;
        REG_W7.u =  temp.u >> REG_W5.u;
		REG_W6.s = u1;
        REG_W4.u = 46341u;
        
        if (shiftValue & 0x0001){
			temp32u = (unsigned long) REG_W7.s * REG_W4.u;
            REG_W7.u = temp32u >> 16u;
            REG_W6.u = (unsigned int)temp32u;
        }
        
        REG_W6.u = REG_W6.u >> 15u;
        result.u = REG_W6.u + REG_W7.u;
    }
    
    return result.s;
}




static inline __attribute__((always_inline)) int MCHP_C_Sqrt_Inline_Qxx(int u1, int u2) 
{
    union a{
        unsigned int  u;
        signed int    s;
    } REG_W4, REG_W5, REG_W6, REG_W7, result, temp;
    
    signed long int ACCUM;
    unsigned int shiftValue;
	unsigned long int temp32u;
    result.s = 0;
    
    if (u1 > 0){
        
         asm volatile (
        "FF1L %[u1], %[shiftValue]     \n"
        : [shiftValue]"=r"(shiftValue)
        : [u1]"r"(u1)
        : "cc"
        );
         
        REG_W6.u = u1 <<  shiftValue;
        
        ACCUM = 0xB5050000;
        REG_W4.u = 23164u;
        ACCUM += (unsigned long int) REG_W4.u * REG_W6.u;
        
        REG_W5.u = ((unsigned long) REG_W6.u * REG_W6.u) >> 16u;
        REG_W4.u = 5721u;
        ACCUM -= (unsigned long int) REG_W4.u * REG_W5.u;
        
        REG_W5.u = ((unsigned long) REG_W6.u * REG_W5.u) >> 16u;
        REG_W4.u = 2570u;
        ACCUM += (unsigned long int) REG_W4.u * REG_W5.u;
        
        REG_W5.u = ((unsigned long) REG_W6.u * REG_W5.u) >> 16u;
        REG_W4.u = 1046u;
        ACCUM -= (unsigned long int) REG_W4.u * REG_W5.u;
        
        REG_W5.u = ((unsigned long) REG_W6.u * REG_W5.u) >> 16u;
        REG_W4.u = 228u;
        ACCUM += (unsigned long int) REG_W4.u * REG_W5.u;
        
        shiftValue = shiftValue + u2;
        REG_W5.u = shiftValue >> 1;
        temp.u = (unsigned long int)ACCUM >> 16;
        REG_W7.u =  temp.u >> REG_W5.u;
		REG_W6.s = u1;
        REG_W4.u = 46341u;
        
		// !!! Important to find out: why the following two multiplications are not the same? (obtained 2ULP with first approach and 1ULP with the second one)
        // !!! besides ULP, to be found which approach is the fastest
		// if (shiftValue & 0x0001){
            // REG_W7.u = ((unsigned long) REG_W7.s * REG_W4.u) >> 16u;
            // REG_W6.u =  (unsigned long) REG_W7.s * REG_W4.u;
        // }
		
		if (shiftValue & 0x0001){
			temp32u = (unsigned long) REG_W7.s * REG_W4.u;
            REG_W7.u = temp32u >> 16u;
            REG_W6.u = (unsigned int)temp32u;
        }
        
        REG_W6.u = REG_W6.u >> 15u;
        result.u = REG_W6.u + REG_W7.u;
    }
    
    return result.s;
}



static inline __attribute__((always_inline)) int MCHP_C_Sqrt_Inline_Qxx_Sat(int u1, int u2, int u3) 
{
    union a{
        unsigned int  u;
        signed int    s;
    } REG_W4, REG_W5, REG_W6, REG_W7, result, temp;
    
    signed long int ACCUM;
    unsigned int shiftValue;
	unsigned long int temp32u;
    result.s = 0;
    
    if (u1 > u3){
    result.u = 0x7FFF;
    }                                
    else if (u1 > 0){                 
        
         asm volatile (
        "FF1L %[u1], %[shiftValue]     \n"
        : [shiftValue]"=r"(shiftValue)
        : [u1]"r"(u1)
        : "cc"
        );
         
        REG_W6.u = u1 <<  shiftValue;
        
        ACCUM = 0xB5050000;
        REG_W4.u = 23164u;
        ACCUM += (unsigned long int) REG_W4.u * REG_W6.u;
        
        REG_W5.u = ((unsigned long) REG_W6.u * REG_W6.u) >> 16u;
        REG_W4.u = 5721u;
        ACCUM -= (unsigned long int) REG_W4.u * REG_W5.u;
        
        REG_W5.u = ((unsigned long) REG_W6.u * REG_W5.u) >> 16u;
        REG_W4.u = 2570u;
        ACCUM += (unsigned long int) REG_W4.u * REG_W5.u;
        
        REG_W5.u = ((unsigned long) REG_W6.u * REG_W5.u) >> 16u;
        REG_W4.u = 1046u;
        ACCUM -= (unsigned long int) REG_W4.u * REG_W5.u;
        
        REG_W5.u = ((unsigned long) REG_W6.u * REG_W5.u) >> 16u;
        REG_W4.u = 228u;
        ACCUM += (unsigned long int) REG_W4.u * REG_W5.u;
        
        shiftValue = shiftValue + u2;
        REG_W5.u = shiftValue >> 1;
        temp.u = (unsigned long int)ACCUM >> 16;
        REG_W7.u =  temp.u >> REG_W5.u;
		REG_W6.s = u1;
        REG_W4.u = 46341u;
        
        if (shiftValue & 0x0001){
			temp32u = (unsigned long) REG_W7.s * REG_W4.u;
            REG_W7.u = temp32u >> 16u;
            REG_W6.u = (unsigned int)temp32u;
        }
        
        REG_W6.u = REG_W6.u >> 15u;
        result.u = REG_W6.u + REG_W7.u;
    }
    
    return result.s;
}



static inline __attribute__((always_inline)) int MCHP_C_Sqrt_Inline_Qxx_8(int u1, int u2) 
{
    union a{
        unsigned int  u;
        signed int    s;
    } REG_W4, REG_W5, REG_W6, REG_W7, result, temp;
    
    signed long int ACCUM;
    unsigned int shiftValue;
	unsigned long int temp32u;
    result.s = 0;
    
    if (u1 > 0){
        
         asm volatile (
        "FF1L %[u1], %[shiftValue]     \n"
        : [shiftValue]"=r"(shiftValue)
        : [u1]"r"(u1)
        : "cc"
        );
         
        REG_W6.u = u1 <<  shiftValue;
        
        ACCUM = 0xB5050000;
        REG_W4.u = 23164u;
        ACCUM += (unsigned long int) REG_W4.u * REG_W6.u;
        
        REG_W5.u = ((unsigned long) REG_W6.u * REG_W6.u) >> 16u;
        REG_W4.u = 5721u;
        ACCUM -= (unsigned long int) REG_W4.u * REG_W5.u;
        
        REG_W5.u = ((unsigned long) REG_W6.u * REG_W5.u) >> 16u;
        REG_W4.u = 2570u;
        ACCUM += (unsigned long int) REG_W4.u * REG_W5.u;
        
        REG_W5.u = ((unsigned long) REG_W6.u * REG_W5.u) >> 16u;
        REG_W4.u = 1046u;
        ACCUM -= (unsigned long int) REG_W4.u * REG_W5.u;
        
        REG_W5.u = ((unsigned long) REG_W6.u * REG_W5.u) >> 16u;
        REG_W4.u = 228u;
        ACCUM += (unsigned long int) REG_W4.u * REG_W5.u;
        
        shiftValue = shiftValue + u2;
        REG_W5.u = shiftValue >> 1;
        temp.u = (unsigned long int)ACCUM >> 16;
        REG_W7.u =  temp.u >> REG_W5.u;
		
		// Attention! There is one corenr case for SQRT Qxx_8 only: we have mimic the rounding for a SAC.R instruction. 
		// For example, if ACCA has the value 0x00 007F BFEA, the SAC.R,W7 instruction will place the value 0x0080 in W7.
		// The convergent rounding mode from C is computational expensive, but we can take into account the input range for which the 
		// artifact is necessary (this only a temporary fix! the problem may reappear when the scaling is changed). 
		if( u1 >= 16257 && u1 <= 16384 ){
			 REG_W7.u++;
		}
		
        REG_W6.s = u1;
        REG_W4.u = 46341u;
        
        if (shiftValue & 0x0001){
			temp32u = (unsigned long) REG_W7.s * REG_W4.u;
            REG_W7.u = temp32u >> 16u;
            REG_W6.u = (unsigned int)temp32u;
        }
        
        REG_W6.u = REG_W6.u >> 15u;
        REG_W6.u = REG_W6.u + REG_W7.u;
        REG_W7.u = REG_W6.u << 8u;

        result.s = REG_W7.s >> 8u;
    }
    
    return result.s;
}



static inline __attribute__((always_inline)) int MCHP_C_Sqrt_Inline_Qxx_8_Sat(int u1, int u2, int u3) 
{
    union a{
        unsigned int  u;
        signed int    s;
    } REG_W4, REG_W5, REG_W6, REG_W7, result, temp;
    
    signed long int ACCUM;
    unsigned int shiftValue;
	unsigned long int temp32u;
    result.s = 0;
    
    if (u1 > u3){
        result.s = 0x007F;
    }                                   
    else if (u1 > 0){                    
        
         asm volatile (
        "FF1L %[u1], %[shiftValue]     \n"
        : [shiftValue]"=r"(shiftValue)
        : [u1]"r"(u1)
        : "cc"
        );
         
        REG_W6.u = u1 <<  shiftValue;
        
        ACCUM = 0xB5050000;
        REG_W4.u = 23164u;
        ACCUM += (unsigned long int) REG_W4.u * REG_W6.u;
        
        REG_W5.u = ((unsigned long) REG_W6.u * REG_W6.u) >> 16u;
        REG_W4.u = 5721u;
        ACCUM -= (unsigned long int) REG_W4.u * REG_W5.u;
        
        REG_W5.u = ((unsigned long) REG_W6.u * REG_W5.u) >> 16u;
        REG_W4.u = 2570u;
        ACCUM += (unsigned long int) REG_W4.u * REG_W5.u;
        
        REG_W5.u = ((unsigned long) REG_W6.u * REG_W5.u) >> 16u;
        REG_W4.u = 1046u;
        ACCUM -= (unsigned long int) REG_W4.u * REG_W5.u;
        
        REG_W5.u = ((unsigned long) REG_W6.u * REG_W5.u) >> 16u;
        REG_W4.u = 228u;
        ACCUM += (unsigned long int) REG_W4.u * REG_W5.u;
        
        shiftValue = shiftValue + u2;
        REG_W5.u = shiftValue >> 1;
        temp.u = (unsigned long int)ACCUM >> 16;
        REG_W7.u =  temp.u >> REG_W5.u;
        REG_W6.s = u1;
        REG_W4.u = 46341u;
        
        if (shiftValue & 0x0001){
			temp32u = (unsigned long) REG_W7.s * REG_W4.u;
            REG_W7.u = temp32u >> 16u;
            REG_W6.u = (unsigned int)temp32u;
        }
        
        REG_W6.u = REG_W6.u >> 15u;
        REG_W6.u = REG_W6.u + REG_W7.u;
        REG_W7.u = REG_W6.u << 8u;

        result.s = REG_W7.s >> 8u;
    }
    
    return result.s;
}

#endif
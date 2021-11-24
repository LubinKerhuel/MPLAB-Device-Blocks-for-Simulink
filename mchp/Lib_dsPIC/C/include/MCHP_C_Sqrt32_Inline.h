#ifndef C_SQRT32_INLINE_H
#define C_SQRT32_INLINE_H

#include <xc.h>

static inline __attribute__((always_inline)) int MCHP_C_Sqrt32_Inline_Q15(long int In1) 
{
    union a{
        unsigned int  u;
        signed int    s;
    } REG_W4, REG_W5, REG_W6, REG_W7, result, temp;
    
    signed long int ACCUM;
    unsigned int shiftValue;
	unsigned long int temp32u;
	int u1;
    int extraShift = 0;
    result.s = 0;
	
    if (In1 > 0){
		
        u1 = (unsigned long int)In1 >> 16u;
		
        asm volatile (
        "FF1L %[u1], %[shiftValue]     \n"
        : [shiftValue]"=r"(shiftValue)
        : [u1]"r"(u1)
        : "cc"
        );     
		
		if(shiftValue == 0){
			u1 = (int)In1;
			
			asm volatile (
			"FF1L %[u1], %[shiftValue]     \n"
			: [shiftValue]"=r"(shiftValue)
			: [u1]"r"(u1)
			: "cc"
			);
			
			//shiftValue = 16 + shiftValue;
            In1 = (unsigned long int)In1 << 16u;
            extraShift = 16;
		}
		
        temp32u  = (long int)In1 << shiftValue;
		REG_W6.u = (long int)temp32u >> 16u;
        
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
        
		shiftValue = shiftValue + extraShift;
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




static inline __attribute__((always_inline)) int MCHP_C_Sqrt32_Inline_Qxx(long int In1, int u2) 
{
    union a{
        unsigned int  u;
        signed int    s;
    } REG_W4, REG_W5, REG_W6, REG_W7, result, temp;
    
    signed long int ACCUM;
    unsigned int shiftValue;
	unsigned long int temp32u;
	int u1;
    int extraShift = 0;
    result.s = 0;
	
    if (In1 > 0){
		
        u1 = (unsigned long int)In1 >> 16u;
		
        asm volatile (
        "FF1L %[u1], %[shiftValue]     \n"
        : [shiftValue]"=r"(shiftValue)
        : [u1]"r"(u1)
        : "cc"
        );     
		
		if(shiftValue == 0){
			u1 = (int)In1;
			
			asm volatile (
			"FF1L %[u1], %[shiftValue]     \n"
			: [shiftValue]"=r"(shiftValue)
			: [u1]"r"(u1)
			: "cc"
			);
			
			//shiftValue = 16 + shiftValue;
            In1 = (unsigned long int)In1 << 16u;
            extraShift = 16;
		}
		
        temp32u  = (long int)In1 << shiftValue;
		REG_W6.u = (long int)temp32u >> 16u;
        
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
        
        shiftValue = shiftValue + u2 + extraShift;
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





static inline __attribute__((always_inline)) int MCHP_C_Sqrt32_Inline_Qxx_Sat(long int In1, int u2, long int u3) 
{
    union a{
        unsigned int  u;
        signed int    s;
    } REG_W4, REG_W5, REG_W6, REG_W7, result, temp;
    
    signed long int ACCUM;
    int shiftValue;
	unsigned long int temp32u;
	int u1;
    int extraShift = 0;
    result.s = 0;

    if (In1 > u3){
		result.u = 0x7FFF;
    }                                
    else if (In1 > 0){
			
		u1 = (long int)In1 >> 16u;
	
		asm volatile (
		"FF1L %[u1], %[shiftValue]     \n"
		: [shiftValue]"=r"(shiftValue)
		: [u1]"r"(u1)
		: "cc"
		);     
		
		if(shiftValue == 0){
			u1 = (int)In1;
			
			asm volatile (
			"FF1L %[u1], %[shiftValue]     \n"
			: [shiftValue]"=r"(shiftValue)
			: [u1]"r"(u1)
			: "cc"
			);
			
			//shiftValue = 16 + shiftValue;
			In1 = (long int)In1 << 16u;
			extraShift = 16;
		}

        temp32u  = (long int)In1 << shiftValue;
		REG_W6.u = (long int)temp32u >> 16u;
        
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
        
        shiftValue = shiftValue + extraShift + u2;
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

#endif
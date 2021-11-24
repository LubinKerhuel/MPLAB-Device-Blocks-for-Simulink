#ifndef CDSP_COSU_HORNER_H
#define CDSP_COSU_HORNER_H

#include <xc.h>

//LK: Coeficient if input range [-1 1[ represent [-pi/2 pi/2[ angle in radian:	
//[0.0192084639 -0.25272628 1.2335301 -0.9999952]
//-1.2335301r is smaller than -1 so the 3rd coefficient will be:  (-1.2335301 / 2) * 2^15(Q15)  =   -0.61676505  
     
//static signed _Fract CoeffValue[] = { 0.0191955566r, -0.25271606r, 0.6167602539r, -1r };        /* Rounded at 2^-15 */
static volatile _Fract CoeffValueU0 =  0.0191955566r;
static volatile _Fract CoeffValueU1 = -0.25271606r; 
static volatile _Fract CoeffValueU2 =  0.6167602539r; 
static volatile _Fract CoeffValueU3 = -1r;
static volatile _Fract ConstValueU  =  0.5r;

static inline unsigned int MCHP_CDSP_CosUHorner_Inline(unsigned int u1, unsigned int u2, unsigned int u3) 
{
    /* u1, input, signed might be any slope where LSB is represent a (negative) power of 2 value */
    /* u2 and u3 are precomputed scaling factor. Their values depends on input slope (LSB representation) */
    /* u2 and u3 are scaling factor such that x = u1 * u2 + u1 * u3 >> 16 */
    /* x is the normalized angle within [-1 1] coding for [-pi pi] */
    /* because cos is periodic, x could also be considered as normalized angle within [0 1] coding for [0 2*pi] */

	/* for a signed 16 bit input with LSB = 2^-13 (representable range is [-4 4[ ), we would have: */
	/* u2 = 1 and u3 = 17907 which allows multiplying input u1 with a factor 4/pi = 1.273239 */
	/* converting [-4 4[ range (LSB=2^-13 here) into [-1 1[ in Q1.15 */
	/* u2 beeing the integer part, u3 beeing the decimal part : (1.273239 - u2)*65536 */
		
    union a{
        signed _Fract f;
        unsigned int  u;
        signed int    s;
    }x,y,normalizedAngleInput, temp, input1,input2, input3;
               
	input1.u = u1;
	input2.u = u2;
	input3.u = u3;
	 
    //--------------------- Scale & modulo ----------------
    x.u = input1.u * input2.u +  (unsigned int) ((unsigned long int) input3.u * input1.u >> 16);    /* x.s is a normalized angle ([-1 1]) ; any wrap act as a modulo which is good */
    normalizedAngleInput.u = x.u;	/* Save value for post processing solution reconstruction */

	/* Let's consider only the first quadrant */       
	/* we consider x as unsigned here i.e. range [0 1] represent Real angle value [0 2*pi] */ 
    if (x.u & 0x4000){                                  //Check if negative (quadrant 2 and 3 are negative cos output, but after mult by 4, quadrant 3 and 4 have reverse signed, need to negate quadrant 2 and 4)
        x.u = ~x.u;                                     //Negate sum. Use Complement to avoid problem with -1 (=-1 with NEG instruction) adding a tiny shift on result
    }
	
    x.u = x.u << 2;                                     //Shift W1 two times to the left. remain only first quadrant out of four
                                                        
    x.u = (unsigned long) x.u*x.u >> 16;				/* [0 1] represent [0 pi/2] real angle value, unsigned value, elevta to its squared value */
	x.u = x.u >> 1;                                     // our polynomial use signed input. convert unsigned to signed, insert one sign bit. 
    //--------------------- Compute ---------------------
    //x.f = (short _Accum) x.f*x.f;		/* square, did it already within unsigned representation (easier) */
    y.f = CoeffValueU0;
    
    y.f = (short _Accum)x.f*y.f + CoeffValueU1;
    y.f = (short _Accum)x.f*y.f + CoeffValueU2; /* Coef 2 is bigger than 1. We divided it by 2, and perform the operation twice */
	temp.f = (short _Accum)x.f*CoeffValueU2 + CoeffValueU3;
    y.f = (short _Accum)x.f*y.f + temp.f;   /* see coef 2 repeated here */
    
    //--------------------- Reconstruct --------------------------
   //y.f = (short _Accum)y.f * 0.5r; //to comply with Simulink output (fixdt(1,16,14)); LSB is 2^-14
	y.f = (short _Accum)y.f * ConstValueU;
	
    if ( (normalizedAngleInput.u +  16384u) < 0x8000){ /* Benefit from wrap effect to detect quadrant requiring to negate the output*/
        y.s = -y.s; /* negate output if quadrant required it */
    }  
    //-------------------- Return result ----------------------
   
   return y.u;
}

#endif


/************************************************************************************************
; MPLAB 16-Bit Device Blocks for Simulink              		       								*
;																	   							*
; Product Page:  http://www.microchip.com/Developmenttools/ProductDetails.aspx?PartNO=SW007023  *
;         Forum: http://www.microchip.com/forums/f192.aspx         								*
;         Wiki:  http://microchipdeveloper.com/simulink:start      								*
; 																	   							*
; Author:        Lubin Kerhuel, Constantin Velea                  								*
; Company:       Microchip Technology Inc                         								* 
; Filename:      MCHP_DSP_atan2Taylor.c                                    						*
; Date:          04/05/2017                                       								*
; File Version:  1.1                                             								*
;************************************************************************************************
;
; SOFTWARE LICENSE AGREEMENT:
; Microchip Technology Incorporated ("Microchip") retains all ownership and 
; intellectual property rights in the code accompanying this message and in all 
; derivatives hereto.  You may use this code, and any derivatives created by 
; any person or entity by or on your behalf, exclusively with Microchip's
; proprietary products.  Your acceptance and/or use of this code constitutes 
; agreement to the terms and conditions of this notice.
;
; CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO 
; WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
; TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A 
; PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S 
; PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
;
; YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE, WHETHER 
; IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), 
; STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, 
; PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF 
; ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN 
; ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT 
; ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO 
; THIS CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO 
; HAVE THIS CODE DEVELOPED.
;
; You agree that you are solely responsible for testing the code and 
; determining its suitability.  Microchip has no obligation to modify, test, 
; certify, or support the code.
;
;************************************************************************************************/

//===============================================================================================
// This is an improved version of the first C-DSP implementation for C-DSP ATAN2-Taylor.
// Is has two versions (Version 2 and Version 3). Best Execution Time: v2
	
	
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

unsigned int MCHP_DSP_atan2Taylor(unsigned int In1, unsigned int In2)
{   
    union a{
        _Fract f;
        unsigned int u;
        signed int   s;
    }x,y, xa,ya, u, temp_2, temp, xMULy, round, debug;
               
    short _Accum  Accum_Result;
    
    //p = [0   1.271118164062500   0.030532836914063  -0.576385498046875   0.338684082031250  -0.063873291015625]; % floored (fix)
	static volatile _Fract p2 =  0.635559082031250r;  //  1.271118164062500 divided by 2 
    static volatile _Fract p3 =  0.030532836914063r;
    static volatile _Fract p4 = -0.576385498046875r;
    static volatile _Fract p5 =  0.338684082031250r;
    static volatile _Fract p6 = -0.063873291015625r;

   
    x.s = In1; //0x1F40
    y.s = In2; //0x4E20

	//-------------------------- handle corner case ---------------------------
	//  atan2(0,y) => 0 for y > 0,
	//			   => pi for y < 0 (we would return -pi with our algorithm)
	//			   => undefined for y = 0 (will be set to 0)
	if (x.s == 0){
		if (y.s > 0){
			return 0;
		}
		if (y.s < 0){
			return 25736;			
		}
		if (y.s == 0){
			return 0;
		}
	}
	// ------------------------ Retrieve the absolute value --------------------
    xa.u = abs(x.s); //0x1F40
    
    //Saturate if required
	if (xa.s < 0){
		xa.s = ~ xa.s;
	}
	
	//Saturate if required
    ya.u = abs(y.s); //ox4E20
	if (ya.s < 0){
		ya.s = ~ ya.s;
	}
    //--------------------------------------------------------------------------
    
    if (xa.u < ya.u){
		//u(xa<ya) = xa(xa<ya) ./ ya(xa<ya)
        u.u = __builtin_divud( (unsigned long)xa.u * 65535u, (unsigned int)ya.u);  //0x6666
        //u.u = u.u / 2u;       
    }
    else{
		//u(ya<=xa) = ya(ya<=xa)./ xa(ya<=xa)
        u.u = __builtin_divud( (unsigned long)ya.u * 65535u, (unsigned int)xa.u);   //0x6666
        //u.u = u.u / 2u;       
    }
    
	Accum_Result = 0k;
	
    // --------------------------------- Compute -------------------------------- 
	//o = p(6)*qFx(16,u-x0,5) + p(5)*qFx(16,u-x0,4) + p(4)*qFx(16,u-x0,3) + p(3)*qFx(16,u-x0,2) + p(2)*qFx(16,u)  + p(1); 

    temp.u = u.u >> 1;
    Accum_Result  = (short _Accum) p2 * temp.f;
    Accum_Result += (short _Accum) p2 * temp.f; 
    //debug.f = Accum_Result; //0x8229
    
// Version 2    
    temp.u =  ( (unsigned long) u.u * u.u) >> 17;  //0x28F5                    		   
    //temp.u =  temp.u >> 1;   //0x147A                    		 // x^2    
    Accum_Result += (short _Accum) p3 * temp.f;    
    //debug.f = Accum_Result;    //0x82C9
    
    temp.u =  ( (unsigned long) temp.u * u.u) >> 16;  //0x1061                  		     
   // temp_2.u =  temp.u >> 1;       //0x0830               		 // x^3    
    Accum_Result += (short _Accum) p4 * temp.f;
    //debug.f = Accum_Result;  //0x7E11
    
    temp.u =  ( (unsigned long) temp.u * u.u) >> 16;  //0x068D                  		    
   //// temp_2.u =  temp.u >> 1;    //0x0346                  		 // x^4    
    Accum_Result += (short _Accum) p5 * temp.f;
    //debug.f = Accum_Result; //0x7F2D
    
    temp.u =  ( (unsigned long) temp.u * u.u) >> 16;  //0x029E                  		 
   // temp_2.u =  temp.u >> 1;       //0x014F               		 // x^5    
    Accum_Result += (short _Accum) p6 * temp.f;    
// End of version 2   
    
    
//// Version 3    
//    temp.u =  ( (unsigned long) u.u * u.u) >> 16;  //0x28F5                    		   
//    temp_2.u =  temp.u >> 1;   //0x147A                    		 // x^2    
//    Accum_Result += (short _Accum) p3 * temp_2.f;    
//    //debug.f = Accum_Result;    //0x82C9
//    
//    temp.u =  ( (unsigned long) temp.u * u.u) >> 16;  //0x1061                  		     
//    temp_2.u =  temp.u >> 1;       //0x0830               		 // x^3    
//    Accum_Result += (short _Accum) p4 * temp_2.f;
//    //debug.f = Accum_Result;  //0x7E11
//    
//    temp.u =  ( (unsigned long) temp.u * u.u) >> 16;  //0x068D                  		    
//    temp_2.u =  temp.u >> 1;    //0x0346                  		 // x^4    
//    Accum_Result += (short _Accum) p5 * temp_2.f;
//    //debug.f = Accum_Result; //0x7F2D
//    
//    temp.u =  ( (unsigned long) temp.u * u.u) >> 16;  //0x029E                  		 
//    temp_2.u =  temp.u >> 1;       //0x014F               		 // x^5    
//    Accum_Result += (short _Accum) p6 * temp_2.f;
////End of version 3

    
    temp.f = Accum_Result; 	//0x7F17
    temp.u = temp.u >> 2 ;  //0x1FC5                        //o = o./4;
    
    //--------------------- Reconstruct the result: ----------------------------
    
	//o(ya<=xa) = 0.5 - o(ya<=xa)
    if (ya.u <= xa.u )
		temp.s = 0x4000 - temp.s;	
	
    //o((x.*y)<0) = -o((x.*y)<0)
    xMULy.s = x.s ^ y.s; //0x5160
    
    if ( xMULy.s < 0 ){                 //if x*y < 0
        temp.s = -temp.s;
        if( temp.s == 0){
          temp.s = temp.s - 1;         //if result is 0, decrement temp.s to get 0xFFFF
        }
	}
  
    //o(y<0) = o(y<0)+1;
	if (y.s < 0){
        temp.s =  temp.s - 0x8000;        
    }
    
    //Extract the result with rounding: temp.f =  temp.f  * 0.7853r; 
    round.s = ((signed long int) temp.s * 51472) & 0x0000FFFF ;  // 0xA950              // PI/4 in Q0.16 is 51472 
	round.u = round.u >> 15;       //0x0001                                    // Extract the rounding
	temp.s = (((signed long int) temp.s * 51472) >> 16) + round.s;  //0x18F4   // Add the rounding and store the result
    
return temp.s;
}

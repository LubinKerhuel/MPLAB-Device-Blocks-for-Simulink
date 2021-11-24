/************************************************************************************************
; MPLAB 16-Bit Device Blocks for Simulink              		       								*
;																	   							*
; Product Page:  http://www.microchip.com/Developmenttools/ProductDetails.aspx?PartNO=SW007023  *
;         Forum: http://www.microchip.com/forums/f192.aspx         								*
;         Wiki:  http://microchipdeveloper.com/simulink:start      								*
; 																	   							*
; Author:        Lubin Kerhuel, Constantin Velea                  								*
; Company:       Microchip Technology Inc                         								* 
; Filename:      MCHP_DSP_atan2Horner.c                                    						*
; Date:          04/05/2017                                       								*
; File Version:  1.0                                           									*
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

// Compute atan vis C99 fixed point extension 
#include <math.h>
#include <stdint.h>
#include <stdio.h>

// Fractional and accumulator types 
typedef signed _Fract Q15;
typedef signed short _Accum ACC;

int MCHP_DSP_atan2Horner(int In1, int In2)
{
	union a{
        _Fract f;
        unsigned int u;
        signed int   s;
    }x,y,xa, ya, u, temp, xMULy, aux, pi, piDiv2;
	
	static volatile Q15 Coeff[] = { 0.00786337627r, -0.03701299998r,  0.08387118962r,  
    -0.1348719133r,  0.19881482434r,  -0.33326514917r,  0.99999934782r };
	
    x.s = In1; 
    y.s = In2; 
	
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
    xa.u = abs(x.s); 
    
    //Saturate if required
	if (xa.s < 0){
		xa.s = ~ xa.s;
	}
	
    ya.u = abs(y.s);
	//Saturate if required
	if (ya.s < 0){
		ya.s = ~ ya.s;
	}
    //--------------------------------------------------------------------------
	
    if (xa.u < ya.u){
		//u(xa<ya) = xa(xa<ya) ./ ya(xa<ya)
        u.u = __builtin_divud( (unsigned long)xa.u * 65535u, (unsigned int)ya.u);  
        //u.u = u.u / 2u;       
    }
    else{
		//u(ya<=xa) = ya(ya<=xa)./ xa(ya<=xa)
        u.u = __builtin_divud( (unsigned long)ya.u * 65535u, (unsigned int)xa.u);  
        //u.u = u.u / 2u;       
    }

	u.u = u.u >> 1 ;
    temp.f = (ACC)u.f * u.f  ;
		
    //---------------------------  Horner expansion  -----------------------
    int i = 0;
    
	aux.f = Coeff[0];
	i++;
	
    while (i <= 6) {
        aux.f = (ACC)temp.f * aux.f + Coeff[i];
        i++;
    }
    //-------------------------  Horner expansion ends -----------------------
	
	temp.f = (ACC) aux.f * u.f;
	temp.u = temp.u >> 2; // temp.f from Q15 to Q13 representation
	
    //------------------------- Reconstruct the result: ----------------------
	//o(ya<=xa) = pi/2 - hornerOut(ya<=xa)
	// represent pi in Q13: (pi*2^13-1)= 25735
	
	pi.u = 25736;
	piDiv2.u = pi.u >> 1;
	
    if (ya.u <= xa.u ){
		temp.s = piDiv2.s - temp.s;	
	}
	
    //o((x.*y)<0) = -o((x.*y)<0)
    xMULy.s = x.s ^ y.s; 
    if ( xMULy.s < 0 ){                //if x*y < 0
        temp.s = -temp.s;
        if( temp.s == 0){
          temp.s = temp.s - 1;         //if result is 0, decrement temp.s to get 0xFFFF
        }
	}
	
    //o(y<0) = o(y<0)+ pi;
	if (y.s < 0){
        if(temp.s + pi.s > 0){
            temp.s = temp.s + pi.s;
        }
        else{
            temp.s =  pi.s -  temp.s;
            temp.s = -temp.s;
        }
            
    }
   
   //hornerOut(hornerOut>pi) = hornerOut(hornerOut>pi) - 2*pi;
   if (temp.s > pi.s){
        temp.s = temp.s - 2*pi.s;
    }
	 
	return temp.s;

}


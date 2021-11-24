/************************************************************************************************
; MPLAB 16-Bit Device Blocks for Simulink              		       								*
;																	   							*
; Product Page:  http://www.microchip.com/Developmenttools/ProductDetails.aspx?PartNO=SW007023  *
;         Forum: http://www.microchip.com/forums/f192.aspx         								*
;         Wiki:  http://microchipdeveloper.com/simulink:start      								*
; 																	   							*
; Author:        Lubin Kerhuel, Constantin Velea                  								*
; Company:       Microchip Technology Inc                         								* 
; Filename:      MCHP_DSP_atan2CORDIC.c                                    						*
; Date:          16/02/2017                                       								*
; File Version:  1.00                                             								*
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

#ifndef __XC16_VERSION__ 
    #error Code Replacement source code use fixed point arithmetic available with XC16 Compiler. Use XC16. 
#elif (__XC16_VERSION__ < 1025)
    #error Code Replacement source code use fixed point arithmetic available from XC16 v1.25. Update XC16 or use a precompile fixed point library.
#endif

#include <xc.h>
#include <stdint.h>

_Fract MCHP_DSP_atan2Cordic(_Fract Q,  _Fract I)  __attribute__((convergent));

_Fract MCHP_DSP_atan2Cordic(_Fract Q,  _Fract I) 
{
    static volatile _Fract CORDIC_DATA[16] = {
         0.250000000000000r
        ,0.147583007812500r
        ,0.0779724121093750r
        ,0.0395812988281250r
        ,0.0198669433593750r
        ,0.00994873046875000r
        ,0.00497436523437500r
        ,0.00247192382812500r
        ,0.00125122070312500r
        ,0.000610351562500000r
        ,0.000305175781250000r
        ,0.000152587890625000r
        ,9.15527343750000e-05r
        ,3.05175781250000e-05r
        ,3.05175781250000e-05r
        ,0r };
        
    _Accum A_ACCUMULATOR;
    _Accum B_ACCUMULATOR;
    
    static volatile _Fract NEG_PI_BY_2   = -0.5000r; //0xC000
    static volatile _Fract PI_BY_2       =  0.5000r; //0x3FFF
    static volatile _Fract PI            =  1.0000r; //0x7FFF
    static volatile _Fract NEG_PI        = -1.0000r; //0x8000
    static volatile _Fract OPTIM_SCALING_VALUE = 0.785398163397448r;   // PI/4 or 0x0C91 << 3
    
    _Fract ACCUM_PHASE = 0;
    //_Fract Result;
    unsigned int iter;
    _Fract W5 = PI;
    _Fract Temp;
        
    
    if ( Q != NEG_PI){
        if(I != NEG_PI){
            
            if(I <= 0r){
                Temp = I;
                
                if (Q > 0r){
                    I = Q;
                    Q = -Temp ; 
                    ACCUM_PHASE = NEG_PI_BY_2;
                } else {
                    I = -Q ; 
                    Q = Temp;
                    ACCUM_PHASE = PI_BY_2;
                }
            }
            
            B_ACCUMULATOR = (Q >> 1);
            A_ACCUMULATOR = (I >> 1);
            
            for(iter = 0; iter <= 14; iter++){  /* Set CORDIC Iteration value */
                Temp = A_ACCUMULATOR ;
                
                if ( (_Fract) B_ACCUMULATOR < 0r){
                    /* rotate pos */ 
                    A_ACCUMULATOR -= (short _Accum) W5* (_Fract) B_ACCUMULATOR;
                    B_ACCUMULATOR += (short _Accum) W5* Temp;
                    ACCUM_PHASE += CORDIC_DATA[iter];
                }
                else
                {
                    /* rotate neg */
                    A_ACCUMULATOR += (short _Accum) W5*(_Fract) B_ACCUMULATOR;
                    B_ACCUMULATOR -= (short _Accum) W5* Temp;
                    ACCUM_PHASE -= CORDIC_DATA[iter];
                }                
                W5 = W5 >> 1;
            }            
            return ((-OPTIM_SCALING_VALUE) * ACCUM_PHASE);            
        }
        else /* (I == NEG_PI) */
            return (OPTIM_SCALING_VALUE * PI);
    }
    else /* ( Q == NEG_PI) */
        return (OPTIM_SCALING_VALUE * NEG_PI_BY_2);
}

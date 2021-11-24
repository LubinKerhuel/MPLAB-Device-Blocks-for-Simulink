;************************************************************************************************
; MPLAB 16-Bit Device Blocks for Simulink              		       								*
;																	   							*
; Product Page:  http://www.microchip.com/Developmenttools/ProductDetails.aspx?PartNO=SW007023  *
;         Forum: http://www.microchip.com/forums/f192.aspx         								*
;         Wiki:  http://microchipdeveloper.com/simulink:start      								*
; 																	   							*
; Author:        Lubin Kerhuel, Constantin Velea                  								*
; Company:       Microchip Technology Inc                         								* 
; Filename:      MCHP_INLINE_Atan2.h                                    						*
; Date:          28th Sept 2016                                    								*
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
;
;   Function Name             : MCHP_Cos 
;   Function Description      : Finds the Cosine
;   Input Format              : Q1.xx ; represent radian, might be any signed fractional with power of 2 scaling. 
;   Output Format             : 1.14 (-16384 to 16384) Q1.14
;   Registers Used            : w0,1,2,3,4,5,6,7   
;   Global variables          : NA
;   
;   Input                     : w0 angle in radian,  Q1.xx
;                             : w1 scaling factor precomputed (integer part)
;                             : w2 scaling factor precomputed (fractional part)
;                             ==> w0 * (w1 + w2*1/65535) = angle in Q1.15 format within [-1 1[ (normalized; not radian anymore)
;   Cycles in assembly        Total:    32 instruction cycles 
;                             Routine:  26 instruction cycles (Total - Push & Pop)
;                             Push/Pop:+ 6 (Accumlator A)  
;                             Including Call & return overhead: 45 instruction cycles 
;
;   Function Name            Date                History                Remarks
;   -------------       ---------------       -------------             -------
;     MCHP_Cos          28th Sept 2016           Created                   -
;                            
;   Author: Lubin Kerhuel, Constantin Velea 
;
;*************************************************************************************************
    
    
.section .libq,code
.global  _MCHP_ASM_Cos

_MCHP_ASM_Cos:          
    
    push    ACCAL
    push    ACCAH
    push    ACCAU
    push    CORCON
 
    MOV     #0x10F1, w4     
    MOV     w4,CORCON       ; Set DSP for Integer mode enabled for DSP multiply
                            ; Unbiased (convergent) rounding enabled
                            ; 9.31 saturation (super saturation)
                            ; Accumulator A saturation enabled
                            ; DSP engine multiplies are unsigned

                
    ;Map [-pi pi] (radian) in Q1.xx to [-1 1] in Q1.15 (normalized)
    ;MOV    #17907,W2       ; for Q1.13, we woule have W2 = 17907, and W1 = 1 => multiply input with 4/pi*65535 (1.273239544735163) Value 4 code for the integer part.
    MUL.SU  W0,W2,W2        ; multiply fractional part,result in [W3:W2], fractional value in W3
    MUL.SU  W0,W1,W0        ; multiply integer part, returl in [W1:W0], integer value in W0
    ADD     W0,W3,W0        ; summ fractional part with integer part (might overflow, it is ok)
    
    MOV     w0,w1           ; Copy the Input value in W1 ( w0 = w1 = x) 

                            ; Map:
                            ; [-1 0[ => [0 1[
    BTSC    w1,#14          ; Check if w1 negative. Skip next intruction if w1 is positive
    COM     w1,w1           ; Neg W1. Use Complement to avoid problem with -1 (=-1 with NEG instruction) adding a tiny shift on result

                            ; Multiplying by 2 will map quadrants (considering signed input):
                            ; [-1    -0.5 [ => [0 1[
                            ; [-0.5   0.5 [ => [-1 1[
                            ; [ 0.5   1   [ => [-1 0[                            
    SL      w1,#2,w1        ; Shift W1 two times to the right. (multiply by 2, + convert Q1.15 toward Q0.16) Store result in w1
                            ; w1 is now a 16 bit unsigned value within [0 1[
    
                            ; Polynomial approximation
                            ; 2 * (p2*x^2 + p4*x^4 + p6*x^6) - 1;                            
                            ; p = [0.616761227115185  -0.126287505901490   0.009543460036834]; % unsigned 16 bit coef to maximise precision and speed
                            ; set DSP for unsigned multiplication, might use either integer mode or fractional mode (prefered way is integer)

    ;Polynomial, with x input [0 1[ representing angle range [0 pi/2[
    MOV     #-32768,w7      ; Load -0.5 into Accumulator
    LAC     w7,A        

    ;Start polynomial approximation 
    MUL.UU  w1,w1,w4        ; x^2
    MOV     #40420,w7       ; Load coefficient : 0.616761227115185
    MAC     w5*w7, A

    MUL.UU  w5,w5,w6        ; x^4
    MOV     #8276,w4        ; Load coefficient : -0.126287505901490
    MSC     w4*w7, A

    MUL.UU  w5,w7,w4        ; x^6
    MOV     #625, w7        ; Load coefficient : 0.009543460036834
    MAC     w5*w7, A

    ;Reconstruct solution
    ;Negate the result if (x + 0.5) > 0
    MOV     #16384,w7       ; Load 0.5 * 2^15 
    ADD     w0,w7,w0        ; Add w0 with 0.5, store in w0. 
    BTSS    SR,#3           ; Check if w0 + (0.5 * 2^15) > w7 by testing SR <3> N: MCU ALU Negative bit 
    NEG     A               ; Negate Accumulator A

    ;Extract the result from Accumulator (round signed)
    SAC.R   A,#1,w0         ; Shift 1 place to the right - to comply with Simulink output which is fixdt(1,16,14) (Q1.14)by default 

    pop     CORCON
    pop     ACCAU   
    pop     ACCAH   
    pop     ACCAL
    
    RETURN
	
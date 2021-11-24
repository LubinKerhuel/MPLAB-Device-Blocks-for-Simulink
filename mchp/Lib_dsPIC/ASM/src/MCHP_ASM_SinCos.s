;************************************************************************************************
; MPLAB 16-Bit Device Blocks for Simulink              		       								*
;																	   							*
; Product Page:  http://www.microchip.com/Developmenttools/ProductDetails.aspx?PartNO=SW007023  *
;         Forum: http://www.microchip.com/forums/f192.aspx         								*
;         Wiki:  http://microchipdeveloper.com/simulink:start      								*
; 																	   							*
; Author:        Lubin Kerhuel, Constantin Velea                  								*
; Company:       Microchip Technology Inc                         								* 
; Filename:      MCHP_ASM_SinCos.s	                                    						*
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

;-------------------------------------------
;Registers to function parameters mapping:
;-------------------------------------------
;W0 - Pointer for Sin result storage
;W1 - Pointer for Cos result storage
;W2 - Input value
;W3 - Scale factor - Integer part
;W4 - Scale factor - Fractional part
;-------------------------------------------

.section .libq,code
.global  _MCHP_ASM_SinCos

_MCHP_ASM_SinCos:          
    
    push    ACCAL
    push    ACCAH
    push    ACCAU
    push    CORCON
	
    MOV     #0x10F1, w7     
    MOV     w7,CORCON       ; Set DSP for Integer mode enabled for DSP multiply
                            ; Unbiased (convergent) rounding enabled
                            ; 9.31 saturation (super saturation)
                            ; Accumulator A saturation enabled
                            ; DSP engine multiplies are unsigned
PUSH W1
PUSH W0 

;MOV W2,W0
;MOV W3,W1
;MOV W4,W2	
				
;--- Compute Sin ----------------------------------------------------------------------------------------
			            
    ;MUL.SU  W0,W2,W2        ; multiply fractional part,result in [W3:W2], fractional value in W3
    ;MUL.SU  W0,W1,W0        ; multiply integer part, returl in [W1:W0], integer value in W0
    ;ADD     W0,W3,W0        ; summ fractional part with integer part (might overflow, it is ok)

	MUL.SU  W2,W4,W4        ; multiply fractional part,result in [W3:W2], fractional value in W3
    MUL.SU  W2,W3,W0        ; multiply integer part, returl in [W1:W0], integer value in W0
    ADD     W0,W5,W0        ; summ fractional part with integer part (might overflow, it is ok)

PUSH W0

	MOV #0x4000,w7  		; The implementation is the same as Cos, except this input offset 
	SUB w7,w0,w0  
	
    MOV     w0,w1           ; Copy the Input value in W1 ( w0 = w1 = x) 

    BTSC    w1,#14          ; Check if w1 negative. Skip next intruction if w1 is positive
    COM     w1,w1           ; Neg W1. Use Complement to avoid problem with -1 (=-1 with NEG instruction) adding a tiny shift on result
    SL      w1,#2,w1        ; Shift W1 two times to the right. Store result in w1
                            ; w1 is now a 16 bit unsigned value
    
    ;Polynomial, with x input [0 1[ representing angle range [0 pi/2[
    MOV     #-32768,w7      ; Load -0.5 into Accumulator
    LAC     w7,A        

    ;Start polynomial approximation 
    MUL.UU  w1,w1,w4
    MOV     #40420,w7       ; Load coefficient : 0.616761227115185
    MAC     w5*w7, A

    MUL.UU  w5,w5,w6   
    MOV     #8276,w4        ; Load coefficient : -0.126287505901490
    MSC     w4*w7, A

    MUL.UU  w5,w7,w4   
    MOV     #625, w7        ; Load coefficient : 0.009543460036834
    MAC     w5*w7, A

    ;Reconstruct solution
    ;Negate the result if (x + 0.5) > 0
    MOV     #16384,w7       ; Load 0.5 * 2^15 
    ADD     w0,w7,w0        ; Add w0 with 0.5, store in w0. 
    BTSS    SR,#3           ; Check if w0 + (0.5 * 2^15) > w7 by testing SR <3> N: MCU ALU Negative bit 
    NEG     A               ; Negate Accumulator A

    ;Extract the result from Accumulator (round signed)
POP W0
POP W7
	SAC.R   A,#1,[W7]       ; Shift 1 place to the right - to comply with Simulink output which is fixdt(1,16,14) by default 
	
;--- Compute Cos ----------------------------------------------------------------------------------------

	;;Map [-pi pi] in Q1.13 to [-1 1] in Q1.15
    ;;MOV    #17907,W1       ; for Q1.13, we woule have W1 = 17907, and W2 = 1 => multiply input with 4/pi*65535 (1.273239544735163) Value 4 code for the integer part.
    ;MUL.SU  W0,W2,W2        ; multiply fractional part,result in [W3:W2], fractional value in W3
    ;MUL.SU  W0,W1,W0        ; multiply integer part, returl in [W1:W0], integer value in W0
    ;ADD     W0,W3,W0        ; summ fractional part with integer part (might overflow, it is ok)
    
    MOV     w0,w1           ; Copy the Input value in W1 ( w0 = w1 = x) 

    BTSC    w1,#14          ; Check if w1 negative. Skip next intruction if w1 is positive
    COM     w1,w1           ; Neg W1. Use Complement to avoid problem with -1 (=-1 with NEG instruction) adding a tiny shift on result
    SL      w1,#2,w1        ; Shift W1 two times to the right. Store result in w1
                            ; w1 is now a 16 bit unsigned value
    
    ;Polynomial, with x input [0 1[ representing angle range [0 pi/2[
    MOV     #-32768,w7      ; Load -0.5 into Accumulator
    LAC     w7,A        

    ;Start polynomial approximation 
    MUL.UU  w1,w1,w4
    MOV     #40420,w7       ; Load coefficient : 0.616761227115185
    MAC     w5*w7, A

    MUL.UU  w5,w5,w6   
    MOV     #8276,w4        ; Load coefficient : -0.126287505901490
    MSC     w4*w7, A

    MUL.UU  w5,w7,w4   
    MOV     #625, w7        ; Load coefficient : 0.009543460036834
    MAC     w5*w7, A

    ;Reconstruct solution
    ;Negate the result if (x + 0.5) > 0
    MOV     #16384,w7       ; Load 0.5 * 2^15 
    ADD     w0,w7,w0        ; Add w0 with 0.5, store in w0. 
    BTSS    SR,#3           ; Check if w0 + (0.5 * 2^15) > w7 by testing SR <3> N: MCU ALU Negative bit 
    NEG     A               ; Negate Accumulator A

    ;Extract the result from Accumulator (round signed)
POP W7
    SAC.R   A,#1,[W7]         ; Shift 1 place to the right - to comply with Simulink output which is fixdt(1,16,14) by default 


    pop     CORCON
    pop     ACCAU   
    pop     ACCAH   
    pop     ACCAL
    
    RETURN
	
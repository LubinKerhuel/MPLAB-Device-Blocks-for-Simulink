;************************************************************************************************
; MPLAB 16-Bit Device Blocks for Simulink              		       								*
;																	   							*
; Product Page:  http://www.microchip.com/Developmenttools/ProductDetails.aspx?PartNO=SW007023  *
;         Forum: http://www.microchip.com/forums/f192.aspx         								*
;         Wiki:  http://microchipdeveloper.com/simulink:start      								*
; 																	   							*
; Author:        Lubin Kerhuel, Constantin Velea                  								*
; Company:       Microchip Technology Inc                         								* 
; Filename:      MCHP_ASM_sqrt_Qxx_8_Sat.s                                    					*
; Date:          23 Sept 2016                                       							*
; File Version:  1.2                                             								*
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
;        Function Name             : _sqrt_Qxx_8_Sat
;        Function Description      : Finds the Square root
;        Input Format              : 1.15 ( 1 to 32767)
;        Output Format             : 1.15 ( 1 to 32767)
;        Registers Used            : w0,1,3,4,5,6,7   
;        Global variables (If any) : NA
;        Cycles in assembly        : Max - xx Min - 10
;
;    Function Name                         Date                          History
;   ---------------                   ---------------                 -------------
;     _sqrt_Qxx_8_Sat                     16th July 2007                    Created (Original libq library)
;
;                                     8th Sept 2016                   - Replaced 8th order polynomial (Taylor Serie for sqrt @ 1)
;                                     C.Velea - L.Kerhuel               with an optimized 5th order polynomial providing 1LSB accuracy
;                                                                     - Speed-up implementation removing B accumulator
;                                                                     - Added Accumulator save/restore to comply with compiler requirement (monotask and multitask project)
;                                     23 Sept 2016                    - Added Scalling input / Output is 8 bit
;
;************************************************************************************************/



.section .libq,code
.global _MCHP_ASM_sqrt_Qxx_8_Sat
_MCHP_ASM_sqrt_Qxx_8_Sat:
    MOV    w0,w7    ; W7 = x

    MOV    #0x7F,W0 ; Saturation value for unsigned 8 bit output
    CPSLT  W7,W2     ; Compare input with saturation input. if W7<W2 then skip next instruction
    return           ; return saturation value

    CLR    W0         
    CPSGT  w7,W0    ; if w <= 0 Input is considered as signed
    return          ; return 0
    
    push    ACCAL
    push    ACCAH
    push    ACCAU
    push    CORCON

    MOV    #0x10F1, w4  ; Set DSP for integer, Unsigned DSP multiplication, Super Saturation, rounding convergent
    MOV    w4,CORCON    ; Write to CORCON; Previously used IOR, not clear why IOR is better than mov.

;Input Scaling
    FF1l   w7,w3        ; find first bit from the left; Scaling information w3 is to reconstruct solution; Output within [0 16] ; 0 for not found. 1 is MSB, 16 is LSB
    SL     w7,w3,w6     ; Scale input from signed [0.5 1] toward [0 1] unsigned (increased accuracy) the first bit set at 1 is out (position 16)
                        ; this instruction always overflow, on purpose
                        ; w6 = xs (x scaled)

; Polynome approximate sqrt(x) for input function range [0.5 1] (correspond to [0 1] input range after scaling)
; sqrt(x) = p5*x^5 + p4*x^4 + p3*x^3 + p2*x^2 + p1*x + p0 
; with p = [0.707107543945313   0.353454589843750  -0.087295532226563   0.039215087890625  -0.015960693359375 0.003479003906250];
    
;p(0)   
;   Cannot assign P(0) directly to A. p(0) => 46341 = 32767 + 13574 ; even better: 15447 + 15447*2  
    MOV     #15447, w4  ; w4 = p(1) LAC take signed input, even if Accum is set for unsigned multiplication
    LAC     w4,A        ; A = 15447 LAC operand is always considered as signed. Cannot load Accumulator with unsigned value > 0.5
    ADD     w4,#-1,A    ; Add with left shift: A = A + 15447*2 (A = P(0) = 46341)       

;p(1)*x 
    MOV     #23164, w4  ; w4 = p(1)
    MAC     w4*w6, A    ; f = f + xs * p(1)     
    
;p(2)*x^2       
    MUL.UU  w6,w6,w4    ; w5 = xs^2
    MOV     #5721, w4   ; w4 = p(2)
    MSC     w4*w5,A     ; f = f + xs^2 * p(2);       
    
;p(3)*x^3       
    MUL.UU  w6,w5,w4    ; w5 = xs^3
    MOV     #2570, w4   ; w4 = p(3)
    MAC     w4*w5,A     ; f = f + xs^3 * p(3);

;p(4)*x^4   
    MUL.UU  w6,w5,w4    ; w5 = xs^4
    MOV     #1046, w4   ; w4 = p(4)
    MSC     w4*w5,A     ; f = f + xs^4 * p(4);

;p(5)*x^5
    MUL.UU  w6,w5,w4    ; w5 = xs^5
    MOV     #228, w4    ; w4 = p(5)
    MAC     w4*w5,A     ; f = f + xs^5 * p(5);
    
;--------------------------------------------------- 
    ADD     w3,w1,w3    ; Add extra shift related to input Scaling, as well as input/output relative scaling.
    
    LSR     w3,#1,w5
    SFTAC   A,w5        ; Right shift accumulator, Re-construct answer from Scaling information
    SAC.R   A,#0, w1    ; Accumulator extraction with rounding. Output represent a signed q1.15. Value is always positive.
    
    MOV     #46341, w4  ; (round(sqrt(2)/2*2^16)*2^-16) = 0.7071 * 2^16-1
    BTSC    w3,#0       ; if required Re-Scaling    
    MUL.SU  w1,w4,w0    ; then multiply by sqrt(2)
                        ; result is in w1, with w0 = 0 if no rescaling, or w0 is the 16 LSBs from the multiplication    
    LSR     w0,#15,w0
    ADD     w0,w1,w0    ; round  and return result in w0

    SL 	    w0,#8,w1    
    ASR     w1,#8,w0    ; Clear 8 MSB to return 8 bit output value

    pop     CORCON
    pop     ACCAU   
    pop     ACCAH   
    pop     ACCAL
    
	RETURN
	
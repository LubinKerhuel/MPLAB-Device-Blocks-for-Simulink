;************************************************************************************************
; MPLAB 16-Bit Device Blocks for Simulink              		       								*
;																	   							*
; Product Page:  http://www.microchip.com/Developmenttools/ProductDetails.aspx?PartNO=SW007023  *
;         Forum: http://www.microchip.com/forums/f192.aspx         								*
;         Wiki:  http://microchipdeveloper.com/simulink:start      								*
; 																	   							*
; Author:        Lubin Kerhuel, Constantin Velea                  								*
; Company:       Microchip Technology Inc                         								* 
; Filename:      MCHP_ASM_Atan2.s	                                    						*
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



.section .libq,code
.global  _MCHP_ASM_Atan2
    
_MCHP_ASM_Atan2:       

; handle corner case: 
; atan2(0,y) = 0 for y > 0,
;			 = pi for y < 0 (we would return -pi with our algorithm)
;			 undefined for y = 0 (will be set to 0)
MOV 	W0,W2
MOV 	#25736,W0 	; W0 = pi (Q1.13)
BTSS  	W1,#15 		; if Y >= 0,
MOV 	#0,W0 		;  set W0 = 0
CP0 	W2
BTSC 	SR,#1 	; Test Z bit Status if X == 0, thenr eturn either pi or 0 depending on Y sign
RETURN
; From here, at least X is different from 0

MOV W2,W0       ; W2 = W0 = X

push    ACCAL
push    ACCAH
push    ACCAU
push    CORCON

push.D  W0      ; preserve W0 and W1    
    
MOV #0x10F1, w4 
MOV w4,CORCON   ; Integer mode enabled for DSP multiply
            ; Unbiased (convergent) rounding enabled
            ; 9.31 saturation (super saturation)
            ; Accumulator A saturation enabled
            ; DSP engine multiplies are unsigned


            
;BTSC    W1,#15      ; is signed         
;NEG     W1,W1       ; W0 = abs(Y)
;BTSC    SR,#3       ; on oveflow
;MOV     #0x7FFF,W1  ; set +1 almost
;BTSC    W2,#15      ; is signed         
;NEG     W2,W2       ; W2 = abs(X)
;BTSC    SR,#3       ; on oveflow
;MOV     #0x7FFF,W2  ; set +1 almost
            

; W1 = |W1| with saturation           
BTSC   w1,#15
NEG    w1,w1
BTSC   w1,#15 	; will saturate if required
COM    w1,w1 

; W2 = |W2| with saturation           
BTSC   w2,#15
NEG    w2,w2
BTSC   w2,#15 	; will saturate if required
COM    w2,w2 


MOV W2,W3       ; W3: Preserve abs(X) to be used later          
            
CPSGT   W2,W1       ; Skip if |Y| > |X| 
EXCH    W1,W2       ; Exchange X and Y      
                    ; W2: Preserve max(abs(X),abs(Y)) to be used later
; assume W1 < W2

; protect against divide by 0 (not required as we handle corner case early)
; as one value is different form 0, we assume the biggest abs value is not 0
;MOV     #0xFFFF,W0  ; result of division 0/0
;CP      W1,W2          ; only possible  if both register are equal to 0
;BRA     Z,MCHP_ASM_atan2TAYLOR_Divide0

; Divide Lower value by Biggest value 
CLR W0          
PUSH   RCOUNT        ; Save RCOUNT register     
REPEAT #17           ; Execute DIV.UD 18 times
DIV.UD   W0, W2      ; Divide W1:W0 by W2. Place the result in W0 (unsigned)            
POP    RCOUNT        ; Restore RCOUNT register          

MCHP_ASM_atan2TAYLOR_Divide0:

MOV W0,W4       ; Save unsigned quotient for reconstruction later

            
; Compute polynomial :
; o = p(6)*qFx(16,u-x0,5) + p(5)*qFx(16,u-x0,4) + p(4)*qFx(16,u-x0,3) + p(3)*qFx(16,u-x0,2) + p(2)*qFx(16,u)  + p(1);           
; with p = [0   1.271118164062500   0.030532836914063  -0.576385498046875   0.338684082031250  -0.063873291015625]; % floored (fix)
          
; + p(1)
; CLR A         
            
; + P(2) * u ; use P(2)/2 as P(2) > 1           
MOV #41652,W6   ; 83304/2 = 41652
MPY     W4*W6, A    ; CLR A         
MAC     W4*W6, A                        
        
; + p(3)*qFx(16,u-x0,2)         
mul.uu  W0,W0,W4    ; x^2
MOV #2001,W6    
MAC     W5*W6, A    
;inc		W4,W4 		; increased accuracy        

; + p(4)*qFx(16,u-x0,3)
mul.uu  W0,W5,W4    ; x^3
MOV #37775,W6   
MSC     W5*W6, A            
;inc		W4,W4 		; increased accuracy

; + p(4)*qFx(16,u-x0,3)
mul.uu  W0,W5,W4    ; x^4           
MOV #22196,W6   
MAC     W5*W6, A
            
; + p(4)*qFx(16,u-x0,3)
mul.uu  W0,W5,W4    ; x^5                       
MOV #4187,W6    
MSC     W5*W6, A
        
SAC.R   A,#3, W0    ; Extract-round Accum in W1 Convert to Q1.15 also (Shift one more to the right ; sign bit will always be 0 here)
            
        
; Reconstruct the result:
; o(ya<=xa) = 0.5 - o(ya<=xa); % % tan(x) = pi/2 - tan(1/x)         
MOV #0x4000,W1    ; W0 = 0.5
CPSNE   W2,W3
SUB W1,W0,W0    

POP.D   W2  ; W2 = X; W3 = Y
; o((x.*y)<0) = -o((x.*y)<0); % tan(-x) = - tan(x);
;LAC     W0,A
XOR  	W2,W3,W5 	; do not use mult: 0 * negative should be negative.
BTSC    W5,#15  ; if x*y < 0
NEG     W0,W0
BTSC 	SR,#1 		; if result is 0, then decrement W0 to get 0xFFFF. There will also be one DEC when x*y = 0
DEC 	W0,W0
;COM 	W0,W0 		; corner case: NEG 0 should provide 0xFFFF. Provide a 2 sometime 3 ULP precision.
;NEG     A
;SAC.R   A,W0    ; will saturate is required
            
; o(y<0) = o(y<0)+1;        
MOV #0x8000,W1    ; W0 = -1         
BTSC    W3,#15
SUB W0,W1,W0

; Scale Result from [-1 1] in Q1.15 to [-pi pi] in Q1.13 ==> Multiply the result with PI/4 (Q1.15)
MOV #51472,W4   ; Load PI/4 in Q0.16        
MUL.SU  w0,w4,w0    ; Signed multiplication with PI/$
LSR 	W0,#15,W0 		; for proper rounding
ADD     W0,W1,W0 	; add rounding 
            
pop     CORCON
pop     ACCAU   
pop     ACCAH   
pop     ACCAL

RETURN

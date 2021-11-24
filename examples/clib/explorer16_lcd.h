// ------------ LCD Functions Header -------------------
#ifndef LCD_H
#define	LCD_H

#include <MCHP_modelInclude.h>

/* Define Interface Pins */
/* Theses PORT must be defined as output */
/* Within the Simulink C Function Call block */

//Define Interface Pins
#define RS LATBbits.LATB15
#define EN LATDbits.LATD4

#define D4 LATEbits.LATE4
#define D5 LATEbits.LATE5
#define D6 LATEbits.LATE6
#define D7 LATEbits.LATE7

/*  #define LCD_COMMAND_CLEAR_SCREEN        0x01 */
/*  #define LCD_COMMAND_RETURN_HOME         0x02 */
/*  #define LCD_COMMAND_ENTER_DATA_MODE     0x06 */
/*  #define LCD_COMMAND_CURSOR_OFF          0x0C */
/*  #define LCD_COMMAND_CURSOR_ON           0x0F */
/*  #define LCD_COMMAND_MOVE_CURSOR_LEFT    0x10 */
/*  #define LCD_COMMAND_MOVE_CURSOR_RIGHT   0x14 */
/*  #define LCD_COMMAND_SET_MODE_8_BIT      0x38 */
/*  #define LCD_COMMAND_ROW_0_HOME          0x80 */
/*  #define LCD_COMMAND_ROW_1_HOME          0xC0 */

void Lcd_Cmd(char a)
{
    EN = 0;
	RS = 0;            
	D4 = (a & 1)>>0;
    D5 = (a & 2)>>1;
    D6 = (a & 4)>>2;
    D7 = (a & 8)>>3;
	EN  = 1;             
    __delay_us(1);
    EN  = 0;
    __delay_us(1);
}

//Initialization routine for the LCD
void Lcd_Init(){
    EN = 0;
    RS = 0; 
    D4 = 0;
    D5 = 0;
    D6 = 0;
    D7 = 0;
    
//    // Initialization for Truly LCD with Novatek chip (Explorer 16/32 development board)
//    // Use twice the FunctionSet instruction "Lcd_Cmd(0x02), Lcd_Cmd(0x08)" to overcome the issue with Explorer 16/32 reset
//    __delay_ms(50);
//    Lcd_Cmd(0x02);  // Function Set
//    Lcd_Cmd(0x08);  // Function Set (N=1:2 or 4 lines Mode, F=0:5x7 dots)   
//    __delay_ms(10);
//    Lcd_Cmd(0x02);  // Function Set
//    Lcd_Cmd(0x08);  // Function Set (N=1:2 or 4 lines Mode, F=0:5x7 dots)
//    __delay_us(50);
//    Lcd_Cmd(0x00);  // Display ON/OFF Control
//    Lcd_Cmd(0x0C);  // Display control (D=1:Display On, C=0:Cursor OFF, B=0:Blink OFF)
//    __delay_us(50);  
//    Lcd_Cmd(0x00);   // Clear Display
//    Lcd_Cmd(0x01);   // Clear Display
//    __delay_ms(3);
//    Lcd_Cmd(0x00);   // Entry Mode Set
//    Lcd_Cmd(0x02);   // Entry Mode Set (I/D=1:increment mode, S=0:entire shift OFF)
    
    
    //New Raystar LCD init routine   
    //Step 1
    __delay_ms(100);
    //Step 2
    Lcd_Cmd(0b0011);        
    __delay_ms(5);
    //Step 3    
    Lcd_Cmd(0b0011);        
    __delay_us(100);
    //Step 4    
    Lcd_Cmd(0b0011);        
    __delay_us(100);
    //Step 5    
    Lcd_Cmd(0b0010);        
    __delay_us(100);
    //Step 6    
    Lcd_Cmd(0b0010);  
    Lcd_Cmd(0b1000);
    __delay_us(60);
    //Step 7    
    Lcd_Cmd(0b0000);  
    Lcd_Cmd(0b1000);
    __delay_us(60);
    //Step 8    
    Lcd_Cmd(0b0000);  
    Lcd_Cmd(0b0001);
    __delay_ms(4);    
    //Step 9    
    Lcd_Cmd(0b0000);  
    Lcd_Cmd(0b0110);
    __delay_us(60);
    // Step 10: Initialization ends, but the display is OFF, so turn it ON with next cmd  
    //Step 11    
    Lcd_Cmd(0b0000);  
    Lcd_Cmd(0b1100);
    __delay_us(60);      
    
}

//Clear entire display and sets DDRAM address 0 in address counter.
void Lcd_Clear(){
	Lcd_Cmd(0x0);
	Lcd_Cmd(0x1);
    __delay_us(2000);
}

//Sets DDRAM address 0 in address counter. Also returns display from being
//shifted to original position. DDRAM contents remain unchanged.
void LCD_Return_Home(){
    Lcd_Cmd(0x0);
	Lcd_Cmd(0x2);
    __delay_us(2000);
}

void Lcd_Shift_Right(){
	Lcd_Cmd(0x01);
	Lcd_Cmd(0x0C);
    __delay_us(60);
}

void Lcd_Shift_Left(){
	Lcd_Cmd(0x01);
	Lcd_Cmd(0x08);
    __delay_us(60);
}

//Set DDRAM address
void Lcd_Set_Cursor(char a, char b)
{
	char temp,z,y;
	if(a == 1)
    {
	  temp = 0x80 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
        __delay_us(60);
	}
	else if(a == 2)
    {
		temp = 0xC0 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
        __delay_us(60);
	}
    else if(a == 3)
    {
		temp = 0x94 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
        __delay_us(60);
	}
    else if(a == 4)
    {
		temp = 0xD4 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
        __delay_us(60);
	}
}

void Lcd_Write_Char(char a)
{
   char temp;
   char y;
   
   temp = a&0x0F;
   y = a&0xF0;
   RS = 1;
   EN = 0;   
        D4 = ((y>>4) & 1)>>0;
        D5 = ((y>>4) & 2)>>1;
        D6 = ((y>>4) & 4)>>2;
        D7 = ((y>>4) & 8)>>3;
   EN = 1;
   __delay_us(50);
   EN = 0;
   __delay_us(50);
        D4 = (temp & 1) >> 0;
        D5 = (temp & 2) >> 1;
        D6 = (temp & 4) >> 2;
        D7 = (temp & 8) >> 3;
   EN = 1;
   __delay_us(50);
   EN = 0;
   __delay_us(50);
}

void Lcd_Write_String(char *a)
{
	int i;
	for(i=0; a[i]!='\0'; i++)
        Lcd_Write_Char(a[i]);
}

#endif
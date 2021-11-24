#include "explorer16_lcd.h"

/* Ports must be declared as output:  */	
/*  	#define RS LATBbits.LATB15    */
/*  	#define EN LATDbits.LATD4     */
/*   							      */
/*  	#define D4 LATEbits.LATE4     */
/*  	#define D5 LATEbits.LATE5     */
/*  	#define D6 LATEbits.LATE6     */
/*  	#define D7 LATEbits.LATE7     */

void LCD_Initilisation() {
    
 	__delay_ms(200);
	Lcd_Init();			  
}


void LCD_Write_xy( char* buffer , unsigned int x, unsigned int y){
    Lcd_Set_Cursor(y,x);
	Lcd_Write_String((char*) buffer);
}

void LCD_Write( char* buffer ){
	Lcd_Write_String((char*) buffer);
}

#include "explorer16_lcd.h"


void LCD_StartUp(){
	
	unsigned int a;
	
	// TRISB &= ~(1 << 15);	// RS
	// TRISD &= ~(1 <<  4);	// EN
	
	// TRISE &= ~(1 <<  4);	// D4
	// TRISE &= ~(1 <<  5);	// D5
	// TRISE &= ~(1 <<  6);	// D6
	// TRISE &= ~(1 <<  7);	// D7
	
	Lcd_Init();				  
	Lcd_Clear();
	
	Lcd_Set_Cursor(1,1);
	Lcd_Write_String("    LCD DEMO");
	Lcd_Set_Cursor(2,1);
	Lcd_Write_String(" Explorer 16/32");
	__delay_ms(3000);
	Lcd_Clear();
    Lcd_Set_Cursor(1,1);
	Lcd_Write_String("Press S4 button");
	Lcd_Set_Cursor(2,1);
	Lcd_Write_String("to switch mode");
	__delay_ms(5000);
    Lcd_Clear();
    Lcd_Set_Cursor(1,1);
	Lcd_Write_String("Move POT slider");
	Lcd_Set_Cursor(2,1);
	Lcd_Write_String("to inc. voltage");
	__delay_ms(5000);
}

void PrintMinutes(unsigned int minutes){
    
    char buffer[17];
    sprintf(buffer, "Up time:%02u:", minutes);	
	Lcd_Set_Cursor(1,1);
	Lcd_Write_String(buffer);
}

void PrintSeconds(unsigned int seconds){
    
    char buffer[17];
    sprintf(buffer, "%02u   ", seconds);	
	Lcd_Set_Cursor(1,12);
	Lcd_Write_String(buffer);
}

void PrintVoltage(double v){
    
	char buffer[17];
    sprintf(buffer, "Voltage:%.2f    ", v);
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String(buffer);
}





void PrintTemperature(double temp){
    
	char buffer[17];
    sprintf(buffer, "Temp:%.2f\xDF%C   ", temp);
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String(buffer);
}


void PrintCPU(double CPU_Load){
    
	char buffer[17];
    sprintf(buffer, "%CPU load: %.2f%% ", CPU_Load);
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String(buffer);
}




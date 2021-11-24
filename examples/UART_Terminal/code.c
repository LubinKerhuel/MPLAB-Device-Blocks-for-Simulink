#ifndef UART_TERMINAL_TEST_H
#define UART_TERMINAL_TEST_H

#ifndef FCY
#define FCY 70000000
#endif

#define BUFFER_SIZE 100

#include <xc.h>
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <libpic30.h> 
#include <time.h>
#include <string.h>

/*----------------- Useful links (VT-100 Terminal Commands) -----------------
http://ascii-table.com/ansi-escape-sequences.php
http://www.termsys.demon.co.uk/vtansi.htm
https://vt100.net/
----------------------------------------------------------------------------*/
//*Note: be aware of the buffer size (the standard terminal line is 80 characters long)

// Print help menu and provide basic information about the commands
void printHelp(unsigned char str[1500]){
	sprintf(str, "\r\033[37;44m");
	strcat(str,  "------------------------ dsPIC UART Terminal -----------------------\n\r");
	strcat(str,  "Note: activate VT100 terminal protocol if you do not see the colors \n\r");
	strcat(str,  "                                                                    \n\r");
	strcat(str,  "Available commands:                                                 \n\r");
	strcat(str,  "-------------------                                                 \n\r");
	strcat(str,  "\"clear\" => clear promt                                              \n\r");
	strcat(str,  "\"help\"  => display available commands and controls                  \n\r");
	strcat(str,  "\"info\"  => various info (CPU/Compiler)                              \n\r");
	strcat(str,  "\"cpu\"   => display CPU usage. POT add synthetic load.               \n\r");
	strcat(str,  "\"temp\"  => display sensor temperature readings (*C)                 \n\r");
	strcat(str,  "\"time\"  => display running time                                     \n\r");
	strcat(str,  "\033[30;47m");
	strcat(str,  "<ESC> Clear | <ENTER> Select | <BACKSPACE> Delete char              \n\r\n\r");
	strcat(str,  "\033[32;40m");
}

void printInfo(unsigned char str[1500]){
	unsigned char tempBuffer[150];
		
	unsigned int hasDSP;
	unsigned int hasEEDATA;
	unsigned int hasDMA;
	unsigned int hasDMAV2;
	unsigned int hasCODEGUARD;
	unsigned int hasPMP;
	unsigned int hasPMPV2;
	unsigned int hasPMP_ENHANCED;
	unsigned int hasEDS;
	unsigned int has5VOLTS;
	
	#ifdef __HAS_DSP__
		hasDSP = 1;
	#else
		hasDSP = 0;
	#endif
	
	#ifdef ___HAS_EEDATA__
		hasEEDATA = 1;
	#else
		hasEEDATA = 0;
	#endif
	
	#ifdef __HAS_DMA__
		hasDMA = 1;
	#else
		hasDMA = 0;
	#endif
	
	#ifdef __HAS_DMAV2__
		hasDMAV2 = 1;
	#else
		hasDMAV2 = 0;
	#endif
	
	#ifdef __HAS_CODEGUARD__
		hasCODEGUARD = 1;
	#else
		hasCODEGUARD = 0;
	#endif
	
	#ifdef __HAS_PMP__
		hasPMP = 1;
	#else
		hasPMP = 0;
	#endif
	
	#ifdef _HAS_PMPV2_
		hasPMPV2 = 1;
	#else
		hasPMPV2 = 0;
	#endif

	#ifdef __HAS_PMP_ENHANCED__
		hasPMP_ENHANCED = 1;
	#else
		hasPMP_ENHANCED = 0;
	#endif
	
	#ifdef __HAS_EDS__
		hasEDS = 1;
	#else
		hasEDS = 0;
	#endif
	
	#ifdef __HAS_5VOLTS__
		has5VOLTS = 1;
	#else
		has5VOLTS = 0;
	#endif

	str[0] = 0;
	tempBuffer[0] = 0;
	
	strcat(str, "\r\033[30;42m");
    sprintf(tempBuffer, "Device and Compiler information:                                   \n\r");
	strcat(str, tempBuffer);
	strcat(str, "\033[32;40m");
	sprintf(tempBuffer, "Device has DSP unit:          %u                                    \n\r", hasDSP);
	strcat(str, tempBuffer);
	sprintf(tempBuffer, "Device has EEPROM memory:     %u                                    \n\r", hasEEDATA);
	strcat(str, tempBuffer);
	sprintf(tempBuffer, "Device has DMA unit:          %u                                    \n\r", hasDMA);
	strcat(str, tempBuffer);
	sprintf(tempBuffer, "Device has DMAV2 unit:        %u                                    \n\r", hasDMAV2);
	strcat(str, tempBuffer);
	sprintf(tempBuffer, "Device has CODEGUARD unit:    %u                                    \n\r", hasCODEGUARD);
	strcat(str, tempBuffer);
	sprintf(tempBuffer, "Device has PMP unit:          %u                                    \n\r", hasPMP);
	strcat(str, tempBuffer);
	sprintf(tempBuffer, "Device has PMPV2 unit:        %u                                    \n\r", hasPMPV2);
	strcat(str, tempBuffer);
	sprintf(tempBuffer, "Device has ENHANCED PMP unit: %u                                    \n\r", hasPMP_ENHANCED);
	strcat(str, tempBuffer);
	sprintf(tempBuffer, "Device has EDS unit:          %u                                    \n\r", hasEDS);
	strcat(str, tempBuffer);
	strcat(str, "\033[30;42m");
	sprintf(tempBuffer, "\033[30;42mCompiler Version: %u | Date Compiled: %s                \n\r\n\r", __XC16_VERSION__,__DATE__);
	strcat(str, tempBuffer);
	strcat(str, "\033[32;40m");
}


/* Function to add Rx bytes to buffer, manage the buffer and extract the commands
----------------------------  Return values list: ------------------------------------
return 0; 	// no bytes received, nothing to send (normally not used)
return 1; 	// invalid command received
return 2;   // clear the current line (ESC key pressed)
return 10;  // command "cpu" hit
return 11;  // command "clear" hit
return 12; 	// command "help" hit
return 13; 	// command "temp" hit
return 14; 	// command "time" hit
return 15; 	// command "info" hit
-------------------------------------------------------------------------------------*/
unsigned char addToBuffer( unsigned char outputBuffer[BUFFER_SIZE], unsigned char *rxBuffer, unsigned int rxCnt){

	unsigned int idx;
	static unsigned int noOfCharsInBuffer = 0;
	static unsigned int flushTheBufferIsRequired = 0;
	
	if (flushTheBufferIsRequired == 1){
		outputBuffer[0] = 0; 			// no need to re-initialize all elements from the buffter to zero (NUL). 
		noOfCharsInBuffer = 0;
		flushTheBufferIsRequired = 0;
	}

	if (rxCnt != 0){
		// check if the last character is "new line: DEC 13" or "escape: DEC 27" or "backspace: DEC 127 ?"
		if(rxBuffer[rxCnt-1] != 13 && rxBuffer[rxCnt-1] != 27 && rxBuffer[rxCnt-1] != '\b'){
			strcat(outputBuffer, rxBuffer);
			noOfCharsInBuffer = noOfCharsInBuffer + rxCnt;
			return 2; //clear the current line and reprint the output buffer
		}
		else{
			// check if we can safely delete one char when "backspace: DEC 127" is pressed
			if(rxBuffer[rxCnt-1] == '\b' && noOfCharsInBuffer > 0){
				outputBuffer[noOfCharsInBuffer-1] = '\0';
				noOfCharsInBuffer = noOfCharsInBuffer - 1;		
				return 2; //clear the current line and reprint the output buffer
			}
			// check if "new line (ENTER): (DEC) 13" was pressed and match a possible command. If no command matched, print a warning message. Rise the "flush" flag anyway
			else if (rxBuffer[rxCnt-1] == 13 && noOfCharsInBuffer > 0){
				if (!strcmp(outputBuffer, "cpu")){
					strcpy(outputBuffer,"\n\r");
					flushTheBufferIsRequired = 1;
					return 10; // command "cpu" hit
				}
				if (!strcmp(outputBuffer, "temp")){
					strcpy(outputBuffer,"\n\r");
					flushTheBufferIsRequired = 1;
					return 13; // command "temp" hit
				}		
				if (!strcmp(outputBuffer, "time")){
					strcpy(outputBuffer,"\n\r");
					flushTheBufferIsRequired = 1;
					return 14; // command "time" hit
				}
				if (!strcmp(outputBuffer, "info")){
					strcpy(outputBuffer,"\n\r");
					flushTheBufferIsRequired = 1;
					return 15; // command "info" hit
				}				
				else if (!strcmp(outputBuffer, "clear")){
					strcpy(outputBuffer,"\n\r");
					flushTheBufferIsRequired = 1;
					return 11; // command "clear" hit
				}
				else if (!strcmp(outputBuffer, "help")){
					strcpy(outputBuffer,"\n\r");
					flushTheBufferIsRequired = 1;
					return 12; // command "help" hit
				}
				else{
					// Print error message if no command match
					strcpy(outputBuffer,"\n\r\033[31;7m error: invalid command. Type \"help\" to display available commands  \033[32;27m\n\r");
					flushTheBufferIsRequired = 1;
					return 1; // invalid command received
				}
			}
			//this means that "ESC: (DEC) 27" was pressed. Need to Flush the buffer and return "2".			
			else if (rxBuffer[rxCnt-1] == 27){
				outputBuffer[0] = 0;
		 		noOfCharsInBuffer = 0;
				return 2; //clear the current line and re-print the output buffer
			}
		}
	}	

return 0; // no bytes received, nothing to send
}

#endif
/* Define function output structure */
/* Must define a Simulink BUS to fit with that structure */
/* MATLAB Function Simulink.importExternalCTypes('SAMx7_CAN_driver.h') to extract */
/* structure and create simulink BUS variable with same name. */

typedef struct {
	unsigned int ID;	
	unsigned char buffer[8];
	short int DLC;
} MCHP_MCAN_Frame_t;
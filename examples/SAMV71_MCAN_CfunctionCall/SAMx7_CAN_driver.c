//#include "./Xplained_SAMV71_CAN.X/rtmodel.h"
#include "xc.h"
#include "string.h" /* memcpy */
//#include "plib_mcan_common.h"




typedef enum
{
    MCAN_MSG_ATTR_TX_FIFO_DATA_FRAME = 0,
    MCAN_MSG_ATTR_TX_FIFO_RTR_FRAME,
    MCAN_MSG_ATTR_TX_BUFFER_DATA_FRAME,
    MCAN_MSG_ATTR_TX_BUFFER_RTR_FRAME
} MCAN_MSG_TX_ATTRIBUTE;


// *****************************************************************************
// *****************************************************************************
// Global Data
// *****************************************************************************
// *****************************************************************************
#define MCAN_STD_ID_Msk        0x7FF

typedef union 
    {
	uint8_t c[8] ; /* access byte level */                    
	uint16_t ui16[4];  
	uint32_t ui32[2];  
	float f32[2];
	double f64;
    } MCHP_union;
	

typedef struct __attribute__((packed,aligned(4)))  
{ 	
	unsigned int ID: 29;
	unsigned int RTR : 1;	
	unsigned int XTD : 1;
	unsigned int ESI : 1;
	
	unsigned int RXTS:16;
	unsigned int DLC:4;
	unsigned int BRS:1;
	unsigned int FDF:1;
	unsigned int spacer0:1;
	unsigned int FIDX:7;
	unsigned int ANMF:1;
	
	MCHP_union buffer;
} MCHP_MCAN_RxBuf_t;

typedef struct __attribute__((packed,aligned(4)))  
{ 
	unsigned int ID: 29;
	unsigned int RTR : 1;	
	unsigned int XTD : 1;
	unsigned int ESI : 1;
	
	unsigned int spacer1:16;
	unsigned int DLC:4;
	unsigned int BRS:1;
	unsigned int FDF:1;
	unsigned int spacer0:1;
	unsigned int EFC:1;
	unsigned int MM:8;
	
	MCHP_union buffer;
} MCHP_MCAN_TxBuf_t;

typedef struct __attribute__((packed,aligned(4)))  
{ 
	unsigned int ID: 29;
	unsigned int RTR : 1;	
	unsigned int XTD : 1;
	unsigned int ESI : 1;
	
	unsigned int TXTS:16;
	unsigned int DLC:4;
	unsigned int BRS:1;
	unsigned int FDF:1;
	unsigned int ET:2;	
	unsigned int MM:8;
	
	MCHP_union buffer;
} MCHP_MCAN_TxEvent_t;

/* Defined in .h file so as to get parsed easily with Matlab function Simulink.importExternalCTypes('SAMx7_CAN_driver.h') */
#include "SAMx7_CAN_driver.h"
/*
typedef struct {
	unsigned int ID;	
	unsigned char buffer[8];
	short int DLC;
} MCHP_MCAN_Frame_t;
*/


/* Filters */
/* Each filter can target any buffer. Several filters might target the same buffer */
#define N_StdFilter 2
#define N_ExtFilter 2

/* Rx Buffers - 0 value might provide warning (non standard c) */ 
#define CAN1_RX_DEDICATED_SIZE 2
#define CAN1_RX_FIFO0_SIZE 4
#define CAN1_RX_FIFO1_SIZE 0    

/* Tx buffers - 0 values might provide warning*/
#define CAN1_TX_FIFO_SIZE 4
#define CAN1_TX_EVENTFIFO_SIZE 0


/* CAN rely on "DMA underneath".
 * Either place data in TCM avoiding any cache inconsistancy.
 * Or use instructions 
 * update from cache to RAM: SCB_CleanDCache_by_Addr( (uint32_t*) addr,SIZE ) 
 * update from RAM to cache: SCB_InvalidateDCache_by_Addr((uint32_t*) addr,SIZE);
 * Both instruction apply on 32 bytes length, make sure to align data to be read-write on 32 address. 
 * Do not update region where it is not required du to risk of inconsistancy 
 */
struct __attribute__((packed,aligned(32)))  MCHP_CAN0_t {
	/* From RAM to peripheral */
	mcan_sidfe_registers_t StdFilter[N_StdFilter] ; /* Standard Message ID Filter  */
	mcan_xidfe_registers_t ExtFilter[N_ExtFilter] ; /* Extended Standard Message ID Filter  */
	MCHP_MCAN_TxBuf_t TX_FIFO_buf[CAN1_TX_FIFO_SIZE] ;
	//char empty0[8]; /* 32 alignement for DCACHE management */
	/* From peripheral to RAM */
	/* alignement required for clean split between SCB_CleanDCache_by_Addr or SCB_InvalidateDCache_by_Addr regions */
	__attribute__((aligned(32))) 
	MCHP_MCAN_RxBuf_t RX_buf[CAN1_RX_DEDICATED_SIZE];	
	MCHP_MCAN_RxBuf_t RX_FIFO0_buf[CAN1_RX_FIFO0_SIZE] ;	
	MCHP_MCAN_RxBuf_t RX_FIFO1_buf[CAN1_RX_FIFO1_SIZE] ;	
	MCHP_MCAN_TxEvent_t TX_EVENTFIFO_buf[CAN1_TX_EVENTFIFO_SIZE];
	
} MCHP_CAN1 __attribute__((space(data)))  = {	//   __attribute__((tcm)) 
	/* Standard Message ID Filter  */
	/* Standard Filter Type: MCAN_SIDFE_0_SFT_RANGE ; MCAN_SIDFE_0_SFT_DUAL ; MCAN_SIDFE_0_SFT_CLASSIC */
	/* Standard Filter Buffer:  MCAN_SIDFE_0_SFEC_STF0M ; MCAN_SIDFE_0_SFEC_STF1M ; MCAN_SIDFE_0_SFEC_STRXBUF */
	/* Filter value 1:  CAN_SIDFE_0_SFID1 */
	/* Filter value 2:  CAN_SIDFE_0_SFID2 */
	.StdFilter = {		
		{ MCAN_SIDFE_0_SFT_RANGE |		/* MCAN_SIDFE_0_SFT_RANGE - MCAN_SIDFE_0_SFT_DUAL - MCAN_SIDFE_0_SFT_CLASSIC */
		  MCAN_SIDFE_0_SFEC_STF0M |	/* Store in: MCAN_SIDFE_0_SFEC_STF0M -  MCAN_SIDFE_0_SFEC_STF1M  - MCAN_SIDFE_0_SFEC_STRXBUF */
		  MCAN_SIDFE_0_SFID1(0x460) | MCAN_SIDFE_0_SFID2(0x46F)                   			
		},
		{ MCAN_SIDFE_0_SFT_RANGE |
		  MCAN_SIDFE_0_SFEC_STRXBUF |	/* Dedicated Rx Buffer */
		  MCAN_SIDFE_0_SFID1(0x45a) |  MCAN_SIDFE_0_SFID2(0x0) /* Id (SFID1) and Buffer Index (SFID2) */
		},
	},	
	
	.ExtFilter = {		
		{  MCAN_XIDFE_0_EFEC_STRXBUF | 		    
		   MCAN_XIDFE_0_EFID1(0x100000a5),
		   MCAN_XIDFE_1_EFT_RANGE |
		   MCAN_XIDFE_1_EFID2(0x1)  
		},
		{   MCAN_XIDFE_0_EFEC_STF1M |
		    MCAN_XIDFE_0_EFID1(0x10000096),		
		    MCAN_XIDFE_1_EFT_RANGE |
		   MCAN_XIDFE_1_EFID2(0x10000096)  
		},		
	},
};


/* DECLARE CAN1 MEMPRY for both Input/Output frame */

MCHP_MCAN_RxBuf_t CAN1_RX_buffer;


/******************************************************************************
Local Functions
******************************************************************************/


void CAN_Init(void)
{		  
    
	
    /* Disable selected programmable clock  */
    PMC_REGS->PMC_SCDR = PMC_SCDR_PCK5_Msk;
    
    /* Supposing MCK is set to 150Mhz */
    /* Configure selected programmable clock    */    
    PMC_REGS->PMC_PCK[5]= PMC_PCK_CSS_MCK | PMC_PCK_PRES(2); /* (Divide by PRES + 1)  here from MCK=150MHz to PCK5 = 50Mhz */

    /* Enable selected programmable clock   */
    PMC_REGS->PMC_SCER =    PMC_SCER_PCK5_Msk;
   
    /* Wait for clock to be ready   */
    while( (PMC_REGS->PMC_SR & (PMC_SR_PCKRDY5_Msk) ) != (PMC_SR_PCKRDY5_Msk));

    // CAN1 ;  PID=37 & 38
    PMC_REGS->PMC_PCER1 |= 0x0020;	/* Activate clock for PID37 peripehral */
    //PMC_REGS->PMC_PCER0=0x31c00;



    /* SAMV71_ULTRA */
    PIOC_REGS->PIO_ABCDSR[0]= 0x0;
    PIOC_REGS->PIO_ABCDSR[1]= 0x5000;
    /* PORTC PIO Disable and Peripheral Enable*/
    PIOC_REGS->PIO_PDR = 0x5000;
    PIOC_REGS->PIO_PER = ~0x5000;    
    
    /* SAME70 ? To be checked */
//     PIOC_REGS->PIO_ABCDSR[0]= 0x0;    
//     PIOC_REGS->PIO_ABCDSR[1]= 0x1000;
//     /* PORTC PIO Disable and Peripheral Enable*/
//     PIOC_REGS->PIO_PDR = 0x1000;
//     PIOC_REGS->PIO_PER = ~0x1000;    
    
//    PIOD_REGS->PIO_ABCDSR[0]= 0x1000;
//    PIOD_REGS->PIO_ABCDSR[1]= 0x0;
//    /* PORTD PIO Disable and Peripheral Enable*/
//    PIOD_REGS->PIO_PDR = 0x1000;
//    PIOD_REGS->PIO_PER = ~0x1000;   
    
    
    

    /* Start MCAN initialization */
    MCAN1_REGS->MCAN_CCCR = MCAN_CCCR_INIT_ENABLED;
    while ((MCAN1_REGS->MCAN_CCCR & MCAN_CCCR_INIT_Msk) != MCAN_CCCR_INIT_Msk);

    /* Set CCE to unlock the configuration registers */
    MCAN1_REGS->MCAN_CCCR |= MCAN_CCCR_CCE_Msk;
            
    
    
    MCAN1_REGS->MCAN_CCCR |= MCAN_CCCR_TEST_ENABLED;     /* Activate TEST mode. Comment for normal use */
//    MCAN1_REGS->MCAN_TEST = MCAN_TEST_TX_DOMINANT;
//    MCAN1_REGS->MCAN_TEST = MCAN_TEST_TX_RECESSIVE;
//    MCAN1_REGS->MCAN_TEST = MCAN_TEST_TX_RESET;       
    
    MCAN1_REGS->MCAN_TEST |= MCAN_TEST_LBCK_ENABLED;    /* Activate Loop back. Comment for normal use */
   

    /* Set Nominal Bit timing and Prescaler Register */
    {
	uint32_t tmp_reg = 0; /* Bit time = tq * (NTSEG1 + NTSEG2 - 3) , 10MHz to 500Khz */
	tmp_reg |= MCAN_NBTP_NBRP(4) ;	/* Tq = Clk * (NBRP+1)  - Fq = 50/5 => 10Mhz */
	tmp_reg |= MCAN_NBTP_NTSEG1(13); /* Prop_seg + Phase Seg1 */
	tmp_reg |= MCAN_NBTP_NTSEG2(4); /* Phase Seg2 */
	tmp_reg |= MCAN_NBTP_NSJW(4); /* Synchronisation Jump: Tq*(NSJW+1) */		
	    
    MCAN1_REGS->MCAN_NBTP  = tmp_reg;
    }


    /* Set MCAN CCCR Init for Message RAM Configuration */
    MCAN1_REGS->MCAN_CCCR |= MCAN_CCCR_INIT_ENABLED;
    while ((MCAN1_REGS->MCAN_CCCR & MCAN_CCCR_INIT_Msk) != MCAN_CCCR_INIT_Msk);
    
    MCAN1_REGS->MCAN_NDAT1 = MCAN_NDAT1_Msk;
    MCAN1_REGS->MCAN_NDAT2 = MCAN_NDAT2_Msk;    

    /* Set CCE to unlock the configuration registers */
    MCAN1_REGS->MCAN_CCCR |= MCAN_CCCR_CCE_Msk;

    /* Global Filter Configuration Register */
    MCAN1_REGS->MCAN_GFC = MCAN_GFC_ANFS(2) | MCAN_GFC_ANFE(2); /* Reject non matching frame (Standard & Extended) */

    /* Extended ID AND Mask Register */
    MCAN1_REGS->MCAN_XIDAM = MCAN_XIDAM_Msk;
    
    
    /* Receive FIFO 0 Configuration Register */
    MCAN1_REGS->MCAN_RXF0C = MCAN_RXF0C_F0S(CAN1_RX_FIFO0_SIZE) | MCAN_RXF0C_F0WM(0) | // MCAN_RXF0C_F0OM_Msk
            MCAN_RXF0C_F0SA(((uint32_t) &MCHP_CAN1.RX_FIFO0_buf >> 2));

    /* Receive FIFO 1 Configuration Register */
    MCAN1_REGS->MCAN_RXF1C = MCAN_RXF1C_F1S(1) | MCAN_RXF1C_F1WM(0) | MCAN_RXF1C_F1OM_Msk |
            MCAN_RXF1C_F1SA(((uint32_t)&MCHP_CAN1.RX_FIFO1_buf >> 2));
    
    /* Receive Rx Buffer Configuration */
    MCAN1_REGS->MCAN_RXBC = MCAN_RXBC_RBSA(( (uint32_t) &MCHP_CAN1.RX_buf >> 2));
        
    /* Transmit Buffer/FIFO Configuration Register */
    MCAN1_REGS->MCAN_TXBC = MCAN_TXBC_TFQS(CAN1_TX_FIFO_SIZE) |
            MCAN_TXBC_TBSA(((uint32_t) &MCHP_CAN1.TX_FIFO_buf >> 2));

    /* Transmit Event FIFO Configuration Register */
    MCAN1_REGS->MCAN_TXEFC = MCAN_TXEFC_EFWM(0) | MCAN_TXEFC_EFS(CAN1_TX_EVENTFIFO_SIZE) |
            MCAN_TXEFC_EFSA(((uint32_t) &MCHP_CAN1.TX_EVENTFIFO_buf >> 2));

    /* Standard ID Filter Configuration Register */
    MCAN1_REGS->MCAN_SIDFC = MCAN_SIDFC_LSS(2) | MCAN_SIDFC_FLSSA(((uint32_t) MCHP_CAN1.StdFilter >> 2));
        
    /* Extended ID Filter Configuration Register */
    MCAN1_REGS->MCAN_XIDFC = MCAN_XIDFC_LSE(2) | MCAN_XIDFC_FLESA(((uint32_t) MCHP_CAN1.ExtFilter  >> 2));
    
    /* Set 16-bit MSB of mcan1 base address */
    MATRIX_REGS->CCFG_SYSIO = (MATRIX_REGS->CCFG_SYSIO & ~CCFG_SYSIO_CAN1DMABA_Msk)
                            | CCFG_SYSIO_CAN1DMABA(((uint32_t) &MCHP_CAN1 >> 16));    
        
    /* Set the operation mode */
    //MCAN1_REGS->MCAN_CCCR = MCAN_CCCR_INIT_DISABLED;
    //while ((MCAN1_REGS->MCAN_CCCR & MCAN_CCCR_INIT_Msk) == MCAN_CCCR_INIT_Msk);
    MCAN1_REGS->MCAN_CCCR &= ~MCAN_CCCR_INIT_ENABLED;
    while ((MCAN1_REGS->MCAN_CCCR & MCAN_CCCR_INIT_Msk) == MCAN_CCCR_INIT_Msk);     

    
    
    /* Enable the interrupt sources and configure the priorities as configured
     * from within the "Interrupt Manager" of MHC. */
//    NVIC_SetPriority(MCAN1_INT0_IRQn, 0);
//    NVIC_EnableIRQ(MCAN1_INT0_IRQn);    
    
    /* Select interrupt line */
//    MCAN1_REGS->MCAN_ILS = 0x0;

    /* Enable interrupt line */
//    MCAN1_REGS->MCAN_ILE = MCAN_ILE_EINT0_Msk;

    /* Enable MCAN interrupts */
//    MCAN1_REGS->MCAN_IE = MCAN_IE_BOE_Msk;

         
}



// *****************************************************************************
/* Function:
    bool MCAN1_MessageTransmit(uint32_t id, uint8_t length, uint8_t* data, MCAN_MODE mode, MCAN_MSG_TX_ATTRIBUTE msgAttr)

   Summary:
    Transmits a message into CAN bus.

   Precondition:
    MCAN1_Initialize must have been called for the associated MCAN instance.

   Parameters:
    id      - 11-bit / 29-bit identifier (ID).
    length  - length of data buffer in number of bytes.
    data    - pointer to source data buffer
    mode    - MCAN mode Classic CAN or CAN FD without BRS or CAN FD with BRS
    msgAttr - Data Frame or Remote frame using Tx FIFO or Tx Buffer

   Returns:
    Request status.
    true  - Request was successful.
    false - Request has failed.
*/
unsigned int MCAN1_MessageTransmit(uint32_t id, int16_t length, uint8_t* data)
{	
    const static MCAN_MSG_TX_ATTRIBUTE msgAttr = MCAN_MSG_ATTR_TX_FIFO_DATA_FRAME;
    uint8_t tfqpi = 0;
    static uint8_t messageMarker = 0;
    
    if (length < 0)
	    return 1; /* Do not send anything. Ok */

    switch (msgAttr)
    {
        case MCAN_MSG_ATTR_TX_FIFO_DATA_FRAME:
        case MCAN_MSG_ATTR_TX_FIFO_RTR_FRAME:
            if (MCAN1_REGS->MCAN_TXFQS & MCAN_TXFQS_TFQF_Msk)
            {
                /* The FIFO is full */
                return 0;
            }
            tfqpi = (uint8_t)((MCAN1_REGS->MCAN_TXFQS & MCAN_TXFQS_TFQPI_Msk) >> MCAN_TXFQS_TFQPI_Pos);
            break;
        default:
            /* Invalid Message Attribute */
            return 0;
    }

    /* If the id is longer than 11 bits, it is considered as extended identifier */
    MCHP_CAN1.TX_FIFO_buf[tfqpi].ESI = 0;
    MCHP_CAN1.TX_FIFO_buf[tfqpi].RTR = 0;
    if (id > MCAN_STD_ID_Msk)
    {
        /* An extended identifier is stored into ID */
	    MCHP_CAN1.TX_FIFO_buf[tfqpi].ID = id;
	    MCHP_CAN1.TX_FIFO_buf[tfqpi].XTD = 1; /* Extended ID */         
    }
    else
    {
        /* A standard identifier is stored into ID[28:18] */
	MCHP_CAN1.TX_FIFO_buf[tfqpi].ID = id << 18;
	MCHP_CAN1.TX_FIFO_buf[tfqpi].XTD = 0;        
    }

    /* Limit length */
    if (length > 8)
        length = 8;
    MCHP_CAN1.TX_FIFO_buf[tfqpi].DLC = length;

    if (msgAttr == MCAN_MSG_ATTR_TX_BUFFER_DATA_FRAME || msgAttr == MCAN_MSG_ATTR_TX_FIFO_DATA_FRAME)
    {
        /* copy the data into the payload */
	memcpy((uint8_t *) MCHP_CAN1.TX_FIFO_buf[tfqpi].buffer.c, data, length);
	MCHP_CAN1.TX_FIFO_buf[tfqpi].RTR = 0;
	
    }
    else if (msgAttr == MCAN_MSG_ATTR_TX_BUFFER_RTR_FRAME || msgAttr == MCAN_MSG_ATTR_TX_FIFO_RTR_FRAME)
    {        
	MCHP_CAN1.TX_FIFO_buf[tfqpi].RTR = 1;
    }

    MCHP_CAN1.TX_FIFO_buf[tfqpi].MM = ++messageMarker;
    MCHP_CAN1.TX_FIFO_buf[tfqpi].EFC = 0;



    SCB_CleanDCache_by_Addr( (uint32_t*) ((uint32_t) &MCHP_CAN1.TX_FIFO_buf[tfqpi] & 0xFFFFFFE0)  ,sizeof(MCHP_MCAN_TxBuf_t) + ((uint32_t)&MCHP_CAN1.TX_FIFO_buf[tfqpi] & 31));

    /* if wants interrupts: */
//    MCAN1_REGS->MCAN_TXBTIE = 1U << tfqpi;
//    MCAN1_REGS->MCAN_IE |= MCAN_IE_TCE_Msk;     
    //MCAN1_REGS->MCAN_IE |= MCAN_IE_DRXE_Msk;
    
    /* request the transmit */
    MCAN1_REGS->MCAN_TXBAR = 1U << tfqpi;

    return 1;
}




MCHP_MCAN_Frame_t  MCAN1_Read_FIFO0(void) {
	MCHP_MCAN_Frame_t D;
	uint8_t rxgi;
	
	D.ID = -1;
	D.DLC = -1;
	
        if (MCAN1_REGS->MCAN_RXF0S & MCAN_RXF0S_F0FL_Msk)
        {		
            /* Read data from the Rx FIFO0 */
           rxgi = (uint8_t)((MCAN1_REGS->MCAN_RXF0S & MCAN_RXF0S_F0GI_Msk) >> MCAN_RXF0S_F0GI_Pos);

	   SCB_InvalidateDCache_by_Addr ((uint32_t*) ((uint32_t) &MCHP_CAN1.RX_FIFO0_buf[rxgi] & 0xFFFFFFE0)  , sizeof(MCHP_MCAN_RxBuf_t)  +  ((uint32_t) &MCHP_CAN1.RX_FIFO0_buf[rxgi] & 31)); 
	   
            /* Get received identifier */
            if (MCHP_CAN1.RX_FIFO0_buf[rxgi].XTD)            
		D.ID = MCHP_CAN1.RX_FIFO0_buf[rxgi].ID;
	    else
                D.ID = MCHP_CAN1.RX_FIFO0_buf[rxgi].ID >> 18;            

            /* Get received data length */
            D.DLC = MCHP_CAN1.RX_FIFO0_buf[rxgi].DLC;

            /* Copy data to user buffer */
            memcpy( D.buffer ,MCHP_CAN1.RX_FIFO0_buf[rxgi].buffer.c, D.DLC);

            /* Ack the fifo position */
           MCAN1_REGS->MCAN_RXF0A = MCAN_RXF0A_F0AI(rxgi);
	}
	return D;		
}

MCHP_MCAN_Frame_t  MCAN1_Read_FIFO1(void) {
	/* Read data from the Rx FIFO1 */
	
	MCHP_MCAN_Frame_t D;
	uint8_t rxgi;
	
	D.ID = -1;
	D.DLC = -1;
	
        if (MCAN1_REGS->MCAN_RXF1S & MCAN_RXF1S_F1FL_Msk) /* New data available ? */
        {		            
           rxgi = (uint8_t)((MCAN1_REGS->MCAN_RXF1S & MCAN_RXF1S_F1GI_Msk) >> MCAN_RXF1S_F1GI_Pos);

	   SCB_InvalidateDCache_by_Addr ((uint32_t*) ((uint32_t) &MCHP_CAN1.RX_FIFO1_buf[rxgi] & 0xFFFFFFE0)  , sizeof(MCHP_MCAN_RxBuf_t)  +  ((uint32_t) &MCHP_CAN1.RX_FIFO1_buf[rxgi] & 31)); 
	   
            /* Get received identifier */
            if (MCHP_CAN1.RX_FIFO1_buf[rxgi].XTD)            
		D.ID = MCHP_CAN1.RX_FIFO1_buf[rxgi].ID;
	    else
                D.ID = MCHP_CAN1.RX_FIFO1_buf[rxgi].ID >> 18;            

            /* Get received data length */
            D.DLC = MCHP_CAN1.RX_FIFO1_buf[rxgi].DLC;

            /* Copy data to user buffer */
            memcpy( D.buffer ,MCHP_CAN1.RX_FIFO1_buf[rxgi].buffer.c, D.DLC);

            /* Ack the fifo position */
           MCAN1_REGS->MCAN_RXF1A = MCAN_RXF1A_F1AI(rxgi);
	}
	return D;		
}


MCHP_MCAN_Frame_t  MCAN1_Read_DEDICATED(uint8_t rxgi) {
    /* New Message in Dedicated Rx Buffer */
	MCHP_MCAN_Frame_t D;
	
	D.ID = -1;
	D.DLC = -1;	
				
	
	if (rxgi <= 31) {	
	if ((MCAN1_REGS->MCAN_NDAT1 & (1 << rxgi)) == 0)
		return D;	
	} else {
		if ((MCAN1_REGS->MCAN_NDAT2 & (1 << (rxgi - 32)) ) == 0)
		return D;
	}
		
	/* Data is available. Read it */
	SCB_InvalidateDCache_by_Addr ((uint32_t*) ((uint32_t) &MCHP_CAN1.RX_buf[rxgi] & 0xFFFFFFE0)  , sizeof(MCHP_MCAN_RxBuf_t)  +  ((uint32_t) &MCHP_CAN1.RX_buf[rxgi] & 31)); 

	 /* Get received identifier */
	 if (MCHP_CAN1.RX_buf[rxgi].XTD)            
	     D.ID = MCHP_CAN1.RX_buf[rxgi].ID;
	 else
	     D.ID = MCHP_CAN1.RX_buf[rxgi].ID >> 18;            

	 /* Get received data length */
	 D.DLC = MCHP_CAN1.RX_buf[rxgi].DLC;

	 /* Copy data to user buffer */
	 memcpy( D.buffer ,MCHP_CAN1.RX_buf[rxgi].buffer.c, D.DLC);	
	

        /* Clear new data flag */
        if (rxgi < 32)        
            MCAN1_REGS->MCAN_NDAT1 = (1 << rxgi);        
        else        
            MCAN1_REGS->MCAN_NDAT2 = (1 << (rxgi - 32));
}




// *****************************************************************************
/* Function:
    void MCAN1_INT0_InterruptHandler(void)

   Summary:
    MCAN1 Peripheral Interrupt Handler.

   Description:
    This function is MCAN1 Peripheral Interrupt Handler and will
    called on every MCAN1 interrupt.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None.

   Remarks:
    The function is called as peripheral instance's interrupt handler if the
    instance interrupt is enabled. If peripheral instance's interrupt is not
    enabled user need to call it from the main while loop of the application.
*/
//void MCAN1_INT0_InterruptHandler(void)
//void MCAN1_INT0_Handler(void)
//{
//	
//    const uint8_t msgLength[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64};	
//    uint8_t length = 0;
//    uint8_t rxgi = 0;
//    uint8_t bufferIndex = 0;
//    mcan_rxbe_registers_t *rxbeFifo = NULL;
//    mcan_rxf0e_registers_t *rxf0eFifo = NULL;
//    mcan_rxf1e_registers_t *rxf1eFifo = NULL;
//    uint32_t ir = MCAN1_REGS->MCAN_IR;
//
//    // SCB_InvalidateDCache_by_Addr (uint32_t *addr, int32_t dsize)
//
//    
//    
//    /* Check if error occurred */
//    if (ir & MCAN_IR_BO_Msk)
//    {
//        MCAN1_REGS->MCAN_IR = MCAN_IR_BO_Msk;
//    }
//    /* New Message in Rx FIFO 0 */
//    if (ir & MCAN_IR_RF0N_Msk)
//    {
//        MCAN1_REGS->MCAN_IR = MCAN_IR_RF0N_Msk;
//        MCAN1_REGS->MCAN_IE &= (~MCAN_IE_RF0NE_Msk);
//
//        if (MCAN1_REGS->MCAN_RXF0S & MCAN_RXF0S_F0FL_Msk)
//        {
//            /* Read data from the Rx FIFO0 */
//            rxgi = (uint8_t)((MCAN1_REGS->MCAN_RXF0S & MCAN_RXF0S_F0GI_Msk) >> MCAN_RXF0S_F0GI_Pos);
//            rxf0eFifo = (mcan_rxf0e_registers_t *) ((uint8_t *)mcan1Obj.msgRAMConfig.rxFIFO0Address + rxgi * MCAN1_RX_FIFO0_ELEMENT_SIZE);
//
//            /* Get received identifier */
//            if (rxf0eFifo->MCAN_RXF0E_0 & MCAN_RXF0E_0_XTD_Msk)
//            {
//                *mcan1RxMsg[MCAN_MSG_ATTR_RX_FIFO0][rxgi].rxId = rxf0eFifo->MCAN_RXF0E_0 & MCAN_RXF0E_0_ID_Msk;
//            }
//            else
//            {
//                *mcan1RxMsg[MCAN_MSG_ATTR_RX_FIFO0][rxgi].rxId = (rxf0eFifo->MCAN_RXF0E_0 >> 18) & MCAN_STD_ID_Msk;
//            }
//
//            /* Check RTR and FDF bits for Remote/Data Frame */
//            if ((rxf0eFifo->MCAN_RXF0E_0 & MCAN_RXF0E_0_RTR_Msk) && ((rxf0eFifo->MCAN_RXF0E_1 & MCAN_RXF0E_1_FDF_Msk) == 0))
//            {
//                *mcan1RxMsg[MCAN_MSG_ATTR_RX_FIFO0][rxgi].msgFrameAttr = MCAN_MSG_RX_REMOTE_FRAME;
//            }
//            else
//            {
//                *mcan1RxMsg[MCAN_MSG_ATTR_RX_FIFO0][rxgi].msgFrameAttr = MCAN_MSG_RX_DATA_FRAME;
//            }
//
//            /* Get received data length */
//            length = msgLength[((rxf0eFifo->MCAN_RXF0E_1 & MCAN_RXF0E_1_DLC_Msk) >> MCAN_RXF0E_1_DLC_Pos)];
//
//            /* Copy data to user buffer */
//            memcpy(mcan1RxMsg[MCAN_MSG_ATTR_RX_FIFO0][rxgi].rxBuffer, (uint8_t *)&rxf0eFifo->MCAN_RXF0E_DATA, length);
//            *mcan1RxMsg[MCAN_MSG_ATTR_RX_FIFO0][rxgi].rxsize = length;
//
//            /* Ack the fifo position */
//           MCAN1_REGS->MCAN_RXF0A = MCAN_RXF0A_F0AI(rxgi);
//
//
//        }
//    }
//    /* New Message in Rx FIFO 1 */
//    if (ir & MCAN_IR_RF1N_Msk)
//    {
//        MCAN1_REGS->MCAN_IR = MCAN_IR_RF1N_Msk;
//        MCAN1_REGS->MCAN_IE &= (~MCAN_IE_RF1NE_Msk);
//
//        if (MCAN1_REGS->MCAN_RXF1S & MCAN_RXF1S_F1FL_Msk)
//        {
//            /* Read data from the Rx FIFO1 */
//            rxgi = (uint8_t)((MCAN1_REGS->MCAN_RXF1S & MCAN_RXF1S_F1GI_Msk) >> MCAN_RXF1S_F1GI_Pos);
//            rxf1eFifo = (mcan_rxf1e_registers_t *) ((uint8_t *)mcan1Obj.msgRAMConfig.rxFIFO1Address + rxgi * MCAN1_RX_FIFO1_ELEMENT_SIZE);
//
//            /* Get received identifier */
//            if (rxf1eFifo->MCAN_RXF1E_0 & MCAN_RXF1E_0_XTD_Msk)
//            {
//                *mcan1RxMsg[MCAN_MSG_ATTR_RX_FIFO1][rxgi].rxId = rxf1eFifo->MCAN_RXF1E_0 & MCAN_RXF1E_0_ID_Msk;
//            }
//            else
//            {
//                *mcan1RxMsg[MCAN_MSG_ATTR_RX_FIFO1][rxgi].rxId = (rxf1eFifo->MCAN_RXF1E_0 >> 18) & MCAN_STD_ID_Msk;
//            }
//
//            /* Check RTR and FDF bits for Remote/Data Frame */
//            if ((rxf1eFifo->MCAN_RXF1E_0 & MCAN_RXF1E_0_RTR_Msk) && ((rxf1eFifo->MCAN_RXF1E_1 & MCAN_RXF1E_1_FDF_Msk) == 0))
//            {
//                *mcan1RxMsg[MCAN_MSG_ATTR_RX_FIFO1][rxgi].msgFrameAttr = MCAN_MSG_RX_REMOTE_FRAME;
//            }
//            else
//            {
//                *mcan1RxMsg[MCAN_MSG_ATTR_RX_FIFO1][rxgi].msgFrameAttr = MCAN_MSG_RX_DATA_FRAME;
//            }
//
//            /* Get received data length */
//            length = msgLength[((rxf1eFifo->MCAN_RXF1E_1 & MCAN_RXF1E_1_DLC_Msk) >> MCAN_RXF1E_1_DLC_Pos)];
//
//            /* Copy data to user buffer */
//            memcpy(mcan1RxMsg[MCAN_MSG_ATTR_RX_FIFO1][rxgi].rxBuffer, (uint8_t *)&rxf1eFifo->MCAN_RXF1E_DATA, length);
//            *mcan1RxMsg[MCAN_MSG_ATTR_RX_FIFO1][rxgi].rxsize = length;
//
//            /* Ack the fifo position */
//            MCAN1_REGS->MCAN_RXF1A = MCAN_RXF1A_F1AI(rxgi);
//
//        }
//    }
//    /* New Message in Dedicated Rx Buffer */
//    if (ir & MCAN_IR_DRX_Msk)
//    {
//	    SCB_InvalidateDCache_by_Addr((uint32_t*)&CAN1_RX_buffer,(sizeof(CAN1_RX_buffer)/32+1)*32); 
//	    
//        MCAN1_REGS->MCAN_IR = MCAN_IR_DRX_Msk;
//        MCAN1_REGS->MCAN_IE &= (~MCAN_IE_DRXE_Msk);
//
//        /* Read data from the Rx Buffer */
//        if (MCAN1_REGS->MCAN_NDAT1 != 0)
//        {
//            for (rxgi = 0; rxgi <= 0x1F; rxgi++)
//            {
//                if ((MCAN1_REGS->MCAN_NDAT1 & (1 << rxgi)) == (1 << rxgi))
//                    break;
//            }
//        }
//        else
//        {
//            for (rxgi = 0; rxgi <= 0x1F; rxgi++)
//            {
//                if ((MCAN1_REGS->MCAN_NDAT2 & (1 << rxgi)) == (1 << rxgi))
//                {
//                    rxgi = rxgi + 32;
//                    break;
//                }
//            }
//        }
//        rxbeFifo = (mcan_rxbe_registers_t *) ((uint8_t *)mcan1Obj.msgRAMConfig.rxBuffersAddress + rxgi * MCAN1_RX_BUFFER_ELEMENT_SIZE);
//
//        for (bufferIndex = 0; bufferIndex < 1; bufferIndex++)
//        {
//            if (bufferIndex < 32)
//            {
//                if ((mcan1Obj.rxBufferIndex1 & (1 << (bufferIndex & 0x1F))) == (1 << (bufferIndex & 0x1F)))
//                {
//                    mcan1Obj.rxBufferIndex1 &= ~(1 << (bufferIndex & 0x1F));
//                    break;
//                }
//            }
//            else if ((mcan1Obj.rxBufferIndex2 & (1 << ((bufferIndex - 32) & 0x1F))) == (1 << ((bufferIndex - 32) & 0x1F)))
//            {
//                mcan1Obj.rxBufferIndex2 &= ~(1 << ((bufferIndex - 32) & 0x1F));
//                break;
//            }
//        }
//
//        /* Get received identifier */
//        if (rxbeFifo->MCAN_RXBE_0 & MCAN_RXBE_0_XTD_Msk)
//        {
//            *mcan1RxMsg[MCAN_MSG_ATTR_RX_BUFFER][bufferIndex].rxId = rxbeFifo->MCAN_RXBE_0 & MCAN_RXBE_0_ID_Msk;
//        }
//        else
//        {
//            *mcan1RxMsg[MCAN_MSG_ATTR_RX_BUFFER][bufferIndex].rxId = (rxbeFifo->MCAN_RXBE_0 >> 18) & MCAN_STD_ID_Msk;
//        }
//
//        /* Check RTR and FDF bits for Remote/Data Frame */
//        if ((rxbeFifo->MCAN_RXBE_0 & MCAN_RXBE_0_RTR_Msk) && ((rxbeFifo->MCAN_RXBE_1 & MCAN_RXBE_1_FDF_Msk) == 0))
//        {
//            *mcan1RxMsg[MCAN_MSG_ATTR_RX_BUFFER][bufferIndex].msgFrameAttr = MCAN_MSG_RX_REMOTE_FRAME;
//        }
//        else
//        {
//            *mcan1RxMsg[MCAN_MSG_ATTR_RX_BUFFER][bufferIndex].msgFrameAttr = MCAN_MSG_RX_DATA_FRAME;
//        }
//
//        /* Get received data length */
//        length = msgLength[((rxbeFifo->MCAN_RXBE_1 & MCAN_RXBE_1_DLC_Msk) >> MCAN_RXBE_1_DLC_Pos)];
//
//        /* Copy data to user buffer */
//        memcpy(mcan1RxMsg[MCAN_MSG_ATTR_RX_BUFFER][bufferIndex].rxBuffer, (uint8_t *)&rxbeFifo->MCAN_RXBE_DATA, length);
//        *mcan1RxMsg[MCAN_MSG_ATTR_RX_BUFFER][bufferIndex].rxsize = length;
//
//        /* Clear new data flag */
//        if (rxgi < 32)
//        {
//            MCAN1_REGS->MCAN_NDAT1 = (1 << rxgi);
//        }
//        else
//        {
//            MCAN1_REGS->MCAN_NDAT2 = (1 << (rxgi - 32));
//        }
//
//    }            if (bufferIndex < 32)
//            {
//                if ((mcan1Obj.rxBufferIndex1 & (1 << (bufferIndex & 0x1F))) == (1 << (bufferIndex & 0x1F)))
//                {
//                    mcan1Obj.rxBufferIndex1 &= ~(1 << (bufferIndex & 0x1F));
//                    break;
//                }
//            }
//            else if ((mcan1Obj.rxBufferIndex2 & (1 << ((bufferIndex - 32) & 0x1F))) == (1 << ((bufferIndex - 32) & 0x1F)))
//            {
//                mcan1Obj.rxBufferIndex2 &= ~(1 << ((bufferIndex - 32) & 0x1F));
//                break;
//            }
//        }
//
//        /* Get received identifier */
//        if (rxbeFifo->MCAN_RXBE_0 & MCAN_RXBE_0_XTD_Msk)
//        {
//            *mcan1RxMsg[MCAN_MSG_ATTR_RX_BUFFER][bufferIndex].rxId = rxbeFifo->MCAN_RXBE_0 & MCAN_RXBE_0_ID_Msk;
//        }
//        else
//        {
//            *mcan1RxMsg[MCAN_MSG_ATTR_RX_BUFFER][bufferIndex].rxId = (rxbeFifo->MCAN_RXBE_0 >> 18) & MCAN_STD_ID_Msk;
//        }
//
//        /* Check RTR and FDF bits for Remote/Data Frame */
//        if ((rxbeFifo->MCAN_RXBE_0 & MCAN_RXBE_0_RTR_Msk) && ((rxbeFifo->MCAN_RXBE_1 & MCAN_RXBE_1_FDF_Msk) == 0))
//        {
//            *mcan1RxMsg[MCAN_MSG_ATTR_RX_BUFFER][bufferIndex].msgFrameAttr = MCAN_MSG_RX_REMOTE_FRAME;
//        }
//        else
//        {
//            *mcan1RxMsg[MCAN_MSG_ATTR_RX_BUFFER][bufferIndex].msgFrameAttr = MCAN_MSG_RX_DATA_FRAME;
//        }
//
//        /* Get received data length */
//        length = msgLength[((rxbeFifo->MCAN_RXBE_1 & MCAN_RXBE_1_DLC_Msk) >> MCAN_RXBE_1_DLC_Pos)];
//
//        /* Copy data to user buffer */
//        memcpy(mcan1RxMsg[MCAN_MSG_ATTR_RX_BUFFER][bufferIndex].rxBuffer, (uint8_t *)&rxbeFifo->MCAN_RXBE_DATA, length);
//        *mcan1RxMsg[MCAN_MSG_ATTR_RX_BUFFER][bufferIndex].rxsize = length;
//
//        /* Clear new data flag */
//        if (rxgi < 32)
//        {
//            MCAN1_REGS->MCAN_NDAT1 = (1 << rxgi);
//        }
//        else
//        {
//            MCAN1_REGS->MCAN_NDAT2 = (1 << (rxgi - 32));
//        }
//
//    }
//
//    /* TX Completed */
//    if (ir & MCAN_IR_TC_Msk)
//    {
//        MCAN1_REGS->MCAN_IR = MCAN_IR_TC_Msk;
//        MCAN1_REGS->MCAN_IE &= (~MCAN_IE_TCE_Msk);
//        for (bufferIndex = 0; bufferIndex < (MCAN1_TX_FIFO_BUFFER_SIZE/MCAN1_TX_FIFO_BUFFER_ELEMENT_SIZE); bufferIndex++)
//        {
//            if ((MCAN1_REGS->MCAN_TXBTO & (1 << (bufferIndex & 0x1F))) &&
//                (MCAN1_REGS->MCAN_TXBTIE & (1 << (bufferIndex & 0x1F))))
//            {
//                MCAN1_REGS->MCAN_TXBTIE &= ~(1 << (bufferIndex & 0x1F));
//            }
//        }
//    }
//
//    if(0) 
//    {
//    /* TX FIFO is empty */
//    if (ir & MCAN_IR_TFE_Msk)
//    {
//        MCAN1_REGS->MCAN_IR = MCAN_IR_TFE_Msk;
//        uint8_t getIndex = (uint8_t)((MCAN1_REGS->MCAN_TXFQS & MCAN_TXFQS_TFGI_Msk) >> MCAN_TXFQS_TFGI_Pos);
//        uint8_t putIndex = (uint8_t)((MCAN1_REGS->MCAN_TXFQS & MCAN_TXFQS_TFQPI_Msk) >> MCAN_TXFQS_TFQPI_Pos);
//        for (uint8_t fifoIndex = getIndex; ; fifoIndex++)
//        {
//            if (fifoIndex >= putIndex)
//            {
//                break;
//            }
//        }
//    }
//    }
//}


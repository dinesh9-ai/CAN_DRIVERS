/*
 * CAN_REGDEF.h
 *
 *  Created on: 04-Feb-2024
 *      Author: dinesh
 */

#ifndef MAIN_INCLUDE_CAN_REGDEF_H_
#define MAIN_INCLUDE_CAN_REGDEF_H_

#include "CAN.h"

/* CAN register start address*/
#define CAN_MODULE  ((volatile CAN_MODULE_t *) 0x3ff6b000)

/* Get Standard Message ID */
#define _CAN_GET_STD_ID (((uint32_t)CAN_MODULE->MCX_CTRL.FCTRL.TX_RX.STD.ID[0] << 3 ) | (CAN_MODULE->MBX_CTRL.FCTRL.TX_RX.STD.ID[5] >> 5))

/* Get Extended Message ID */
#define _CAN_GET_EXT_ID (((uint32_t)CAN_MODULE->MCX_CTRL.FCTRL.TX_RX.EXT.ID[0] << 21)| (CAN_MODULE->MCX_CTRL.FCTRL.TX_RX.EXT.ID[1] << 13) | (CAN_MODULE->MCX_CTRL.FCTRL.TX_RX.EXT.ID[2] << 5 ) | (CAN_MODULE->MCX_CTRL.FCTRL.TX_RX.EXT.ID[3] >> 3))

/* Set Standard Message ID */
#define _CAN_SET_STD_ID(x)  CAN_MODULE->MCX_CTRL.FCTRL.TX_RX.STD.ID[0] = ((x)>>3);\
							CAN_MODULE->MCX_CTRL.FCTRL.TX_RX.STD.ID[1] = ((x)<<5);

/* Set Extended Message Id */
#define _CAN_SET_EXT_ID(x)  CAN_MODULE->MCX_CTRL.FCTRL.TX_RX.EXT.ID[0] = ((x) >> 21 );\
							CAN_MODULE->MCX_CTRL.FCTRL.TX_RX.EXT.ID[1] = ((x) >> 13); \
							CAN_MODULE->MCX_CTRL.FCTRL.TX_RX.EXT.ID[2] = ((x) >> 5);\
							CAN_MODULE->MCX_CTRL.FCTRL.TX_RX.EXT.ID[3] = ((x) << 3);



/* Interrupt Register */
typedef enum{
		__CAN_IRQ_Rx 				= BIT(0),
		__CAN_IRQ_Tx 				= BIT(1),
		__CAN_IRQ_ERR 				= BIT(2),
		__CAN_IRQ_DATA_OVERRUN 		= BIT(3),
		__CAN_IRQ_WAKEUP 			= BIT(4),
		__CAN_IRQ_ERR_PASSIVE		= BIT(5),
		__CAN_IRQ_ARB_LOST 			= BIT(6),
		__CAN_IRQ_BUS_ERR 			= BIT(7)
} __CAN_IRQ_t;


/* OCMODE */
typedef enum{
	__CAN_OC_BOM = 0b00,
	__CAN_OC_TOM = 0b01,
	__CAN_OC_NOM = 0b10,
	__CAN_OC_COM = 0b11
} __CAN_OCMODE_t;


/*CAN CONTROLLER */
typedef struct{
	/* Mode Register */
	union{
		uint32_t U;
		struct{
			unsigned int RM				:1;  /* Reset Mode */
			unsigned int LOM			:1;  /* Listen Mode*/
			unsigned int STM			:1;	 /* Self Test Mode */
			unsigned int AFM			:1;  /* Acceptance Filter Mode */
			unsigned int SM				:1;  /* Sleep Mode */
			unsigned int RESERVED_27	:27; /* Reserved */
		}B;
	}Mode;

	/* Command Register */
	union{
		uint32_t U;
		struct{
			unsigned int TR				:1;		/* Transmission  Request */
			unsigned int AT				:1;		/* Abort Transmission */
			unsigned int RRB			:1;		/* Release Receive Buffer */
			unsigned int CDO			:1;		/* Clear Data Overrun */
			unsigned int GTS			:1;		/* Go to Sleep */
			unsigned int RESERVED_27	:27;	/* Reserved */
		}B;
	}CMR;

	/* Status Register */
	union{
		uint32_t U;
		struct{
			unsigned int RBS				:1;   /* Receive Buffer Status */
			unsigned int DOS				:1;	  /* Data Overrun Status */
			unsigned int TBS				:1;   /* Transmit Buffer Status */
			unsigned int TCS				:1;	  /* Transmit Complete Status */
			unsigned int RS					:1;   /* Receive Status */
			unsigned int TS					:1;   /* Transmit Status */
			unsigned int ES					:1;   /* Error Status */
			unsigned int BS					:1;   /* Bus Status */
			unsigned int RESERVED_27		:27;  /* Reserved */
		}B;
	}SR;

	/* Interrupt Register */
	union{
		uint32_t U;
		struct{
			unsigned int RI:1; 				/* Receive Interrupt */
			unsigned int TI:1; 				/* Transmit Interrupt */
			unsigned int EI:1; 				/* Error Interrupt Enable */
			unsigned int DOI:1; 			/* Data Overrun Interrupt */
			unsigned int WUI:1;				/* Wake-up Interrupt */
			unsigned int EPI:1;				/* Error Passive Interrupt */
			unsigned int ALI:1;				/* Arbitration Lost Interrupt */
			unsigned int BEI:1;				/* Bus Error Interrupt*/
			unsigned int RESERVED_27:27;	/* RESERVED */
		}B;
	}IR;


	/* Interrupt Enable Register */
	union{
		uint32_t U;
		struct{
			unsigned int RI:1; 				/* Receive Interrupt */
						unsigned int TIE			:1; 				/* Transmit Interrupt  Enable */
						unsigned int EIE			:1; 				/* Error Interrupt Enable  Enable */
						unsigned int DOIE			:1; 				/* Data Overrun Interrupt  Enable */
						unsigned int WUIE			:1;				/* Wake-up Interrupt  Enable */
						unsigned int EPIE			:1;				/* Error Passive Interrupt  Enable */
						unsigned int ALIE			:1;				/* Arbitration Lost Interrupt  Enable */
						unsigned int BEIE			:1;				/* Bus Error Interrupt Enable */
						unsigned int RESERVED_27	:27;		/* RESERVED */
			}B;
		}IER;

	uint32_t RESERVED_D0;

	/*Bus Timing Register 0 */
	union{
		uint32_t U;
		struct{
			unsigned int BRP			:6;		/* Baud rate pre-scaler [5:0] */
			unsigned int SJW			:2;		/* Synchronization Jump Width [7:6] */
			unsigned int RESERVED_24	:24;	/* Reserved */
		}B;
	}BTR0;

	/* Bus Timing Register 1 */
	union{
		uint32_t U;
		struct{
			unsigned int TSEG1			:4; /* Time Segment 1 */
			unsigned int TSEG2			:3;	/* Time Segment 2 */
			unsigned int SAM			:1; /* Sampling */
			unsigned int RESERVED_24	:24;/* Reserved */
		}B;
	}BTR1;

	/* */
	union{
		uint32_t U;
		struct{
			unsigned int OCMODE			:2; 	/* output control mode [1:0] */
			unsigned int OCPOL0			:1; 	/* output control polarity */
			unsigned int OCTN0			:1; 	/* output control transistor */
			unsigned int OCTP0			:1; 	/* output control transistor */
			unsigned int OCPOL1			:1;		/* output control polarity */
			unsigned int OCTN1			:1;		/* output control transistor */
			unsigned int OCTP1			:1;		/* output control transistor */
			unsigned int RESERVED_24	:24;	/* Reserved */
		}B;
	}OCR;

	uint32_t RESERVED_2[2];

	/* Arbitration Lost Capture Register */
	union{
		uint32_t U;
		struct{
			unsigned int ALC			:8;  /* Arbitration loss capture [7:0] */
			unsigned int RESERVED_24	:24; /* RESERVED */
		}B;
	}ALC;

	/* Error Code Capture Register */
	union{
		uint32_t U;
		struct{
			unsigned int ECC			:4;  /* ERROR CODE CAPTUE */
			unsigned int RESERVED_24	:24; /* RESERVED */
		}B;
	}ECC;

	/* Error Warning Limit Register */
	union{
		uint32_t U;
		struct{
			unsigned int EWLR			:8;   /* [7:0] Error Warning Limit */
			unsigned int RESERVED_24	:24;  /* RESERVED */
		}B;
	}EWLR;

	/* Receive Error Counter Register */
	union{
		uint32_t U;
		struct{
			unsigned int  RXERR			:8;  /* RX Error counter [7:0] */
			unsigned int RESERVED_24	:24; /* RESERVED */
		}B;
	}RXERR;

	/* Transmit Error Counter Register */
	union{
			uint32_t U;
			struct{
				unsigned int  TXERR			:8;  /* TX Error counter [7:0] */
				unsigned int RESERVED_24	:24; /* RESERVED */
			}B;
	}TXERR;

	/*  */
	union{
		struct{
			uint32_t CODE[4];			/* Acceptance Message ID */
			uint32_t MASK[4];			/* Acceptance Mask */
			uint32_t RESERVED_2[5];
		}ACC;							/* Acceptance filtering */

		struct{
			CAN_FRAME_INFO_t FIR;
			union{
				struct{
					uint32_t ID[2]; 		/* Standard message frame ID */
					uint32_t data[8]; 		/* Standard Frame payload */
					uint32_t RESERVED[2];
				}STD;						/* Standard Frame Format */

				struct{
					uint32_t ID[4];		/* Extended Frame ID */
					uint32_t data[8];	/* Extended Frame payload */
				}EXT;					/* Extended Frame Format */
			}TX_RX;						/* TX RX Interface */
		}FCTRL;							/* Frame Control */
	}MBX_CTRL;							/* Mailbox Control */

	union{
		uint32_t U;
		struct{
			unsigned int RMC:8; 			/* Rx Message Counter */
			unsigned int RESERVED_24:24; 	/* RESERVED */
		}B;
	}RMC;

	union{
		uint32_t U;
		struct{
			unsigned int RBSA:8; 			/* Rx Buffer Start Address */
			unsigned int RESERVED_24:24; 	/* RESERVED */
		}B;
	}RBSA;


	union{
		uint32_t U;
		struct{
			unsigned int COD			:3;  /* [2:0] CLKOUT frequency selector based of fOSC */
			unsigned int COFF			:1;  /* clock out */
			unsigned int RESERVED_1		:1;  /* RESERVED */
			unsigned int RXINTEN		:1;  /* This bit allows the TX1 output to be used as a dedicated receive interrupt output*/
			unsigned int CBP			:1;  /* allows to bypass the CAN input comparator and is only possible in reset mode */
			unsigned int CAN_M			:1;  /*  If CDR.7 is at logic 0 the CAN controller operates in BasicCAN mode. If set to logic 1 the CAN controller operates in PeliCAN mode. Write access is only possible in reset mode */
			unsigned int RESERVED_24	:24; /* RESERVED */
		}B;
	}CDR;

	 uint32_t IRAM[2];
} CAN_MODULE_t;






#endif /* MAIN_INCLUDE_CAN_REGDEF_H_ */

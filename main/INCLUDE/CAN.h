/*
 * CAN.h
 *
 *  Created on: 04-Feb-2024
 *      Author: dinesh
 */

#ifndef MAIN_INCLUDE_CAN_H_
#define MAIN_INCLUDE_CAN_H_


#include <stdint.h>
#include "CAN_CONFIG.h"

/* Defines CAN FRAME FORMAT */
typedef enum{
	CAN_FRAME_STD	=0,	/* STANDARD FRAME */
	CAN_FRAME_EXT	=1  /* EXTENDED FRAME */
}CAN_FRAME_FORMAT_t;

/* Defines whether RTR is used or not */
typedef enum{
	CAN_NO_RTR	=0,  /* NO RTR FRAME */
	CAN_RTR		=1	 /* RTR FRAME */
} CAN_RTR_t;

/*  Defines CAN Frame Information */
typedef union{
	uint32_t U;
	struct{
			uint8_t 				DLC:4;				/* Defines DLC length*/
			unsigned int 			unknown_2:2;		/* Reserved */
			CAN_RTR_t    			RTR:1;				/* Defines RT */
			CAN_FRAME_FORMAT_t		FF:1;				/* Define FRAME FORMAT */
			unsigned int 			RESERVED_24:24;		/* RSERVED */
	}B;
} CAN_FRAME_INFO_t;

/* CAN FRAME STRUCTURE */
typedef struct{
	CAN_FRAME_INFO_t 	FIR;     /* Define CAN FRAME INFO */
	uint32_t			MSG_ID;	 /* Defines MSG_ID */
	union{
		uint8_t 	u8[8];
		uint32_t 	u32[32];
	}data;						 /*Defines PAYLOAD byte*/
}CAN_FRAME_t;


/* Initialize the CAN Module */
int CAN_INIT(void);

void CAN_READ_FRAME(void);

/*Send a CAN Frame */
/*
 * params : p_frame pointer to the frame to be send
 *
 */
int CAN_WRITE_FRAME(const CAN_FRAME_t* p_frame);


/* Stops CAN Communication  */
int CAN_STOP(void);

#endif /* MAIN_INCLUDE_CAN_H_ */

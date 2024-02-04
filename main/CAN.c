/*
 * CAN.c
 *
 *  Created on: 04-Feb-2024
 *      Author: dinesh
 */

#include "CAN.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "esp_intr.h"
#include "soc/dport_reg.h"
#include <math.h>

#include "driver/gpio.h"

#include "can_regdef.h"
#include "CAN_config.h"


static void CAN_READ_FRAME();

static void CAN_ISR(void *arg_p);

static void CAN_ISR( void *arg_p){

	//Interrupt Flag Buffer
	__CAN_IRQ_t Interrupt;

	//Read CAN Interrupt
	Interrupt = CAN_MODULE->IR.U;

	//Handle Tx Interrupt
	if((Interrupt & __CAN_IRQ_Tx )!= 0){
		//Handler
	}

	//Handle Rx Interrupt
	if((Interrupt & __CAN_IRQ_Rx ) != 0){
		CAN_READ_FRAME();
	}

	// Handle Error Interrupts
	if((Interrupt & ( __CAN_IRQ_ERR | __CAN_IRQ_DATA_OVERRUN | __CAN_IRQ_WAKEUP | __CAN_IRQ_ERR_PASSIVE | __CAN_IRQ_ARB_LOST | __CAN_IRQ_BUS_ERR )) != 0){
		/* Handler */
	}
}


static void CAN_READ_FRAME(){

	uint8_t __byte_i; //Byte Iterator

	CAN_FRAME_t __frame; //Frame read Buffer

	if(CAN_NODE.Rx_Queue == NULL){

		// tells hardware that frame has been read
		CAN_MODULE->CMR.B.RRB=1;
		return;
	}

	// Get FIR
	__frame.FIR.U=CAN_MODULE->MBX_CTRL.FCTRL.FIR.U;

	// Check if its Standard Frame
	if(__frame.FIR.B.FF == CAN_FRAME_STD)
	{
		__frame.MSG_ID=_CAN_GET_STD_ID;

		// copy bytes
		for(__byte_i = 0; __byte_i < __frame.FIR.B.DLC; __byte_i++)
		{
			__frame.data.u8[__byte_i]=CAN_MODULE->MBX_CTRL.FCTRL.TX_RX.STD.data[__byte_i];
		}
	}
	//If it is Extended Frame
	else{
		__frame.MSG_ID=_CAN_GET_EXT_ID;
		// copy bytes
		for(__byte_i = 0; __byte_i < __frame.FIR.B.DLC; __byte_i++)
		{
			__frame.data.u8[__byte_i]=CAN_MODULE->MBX_CTRL.FCTRL.TX_RX.EXT.data[__byte_i];
		}
	}

	//Send frame to input queue
	xQueueSendFromISR(CAN_NODE.Rx_Queue,&__frame,0);

	// Tell Hardware that Frame has been Read
	CAN_MODULE->CMR.B.RRB=1;
}


int CAN_WRITE_FRAME(const CAN_FRAME_t* p_frame ){

	//byte Iterator
	uint8_t __byte_i;

	//Copy Frame Information record
	CAN_MODULE->MBX_CTRL.FCTRL.FIR.U = p_frame->FIR.U;

	//Standard Frame
	if(p_frame->FIR.B.FF == CAN_FRAME_STD){

		//Write Message ID
		_CAN_SET_STD_ID(p_frame->MSG_ID);

		for(__byte_i=0;__byte_i<p_frame->FIR.B.DLC;__byte_i++){
			CAN_MODULE->MBX_CTRL.FCTRL.TX_RX.STD.data[__byte_i] = p_frame->data.u8[__byte_i];
		}
	}
	//Extended Frame
	else{
		//Write Message ID
		_CAN_SET_EXT_ID(p_frame->MSG_ID);

		for(__byte_i=0;__byte_i<p_frame->FIR.B.DLC;__byte_i++){
			CAN_MODULE->MBX_CTRL.FCTRL.TX_RX.EXT.data[__byte_i] = p_frame->data.u8[__byte_i];
		}
	}

	//Transmit Frame
	CAN_MODULE->CMR.B.TR=1;

	return 0;
}


int CAN_INIT(){

	//Time Quantum
	double __tq;

	//enable Module
	DPORT_SET_PERI_REG_MASK(DPORT_PERIP_CLK_EN_REG, DPORT_CAN_CLK_EN);
	DPORT_CLEAR_PERI_REG_MASK(DPORT_PERIP_RST_EN_REG, DPORT_CAN_RST);

	//Configure TX Pin
	gpio_setup_level(CAN_NODE.Tx_PIN,1);
	gpio_set_direction(CAN_NODE.Tx_PIN,GPIO_MODE_OUTPUT);
	gpio_matrix_out(CAN_NODE.Tx_PIN,CAN_TX_IDX,0,0);
	gpio_pad_select_gpio(CAN_NODE.Tx_PIN);

	//Configure RX Pin
	gpio_set_direction(CAN_NODE.Rx_PIN,GPIO_MODE_INPUT);
	gpio_matrix_in(CAN_NODE.Rx_PIN,CAN_RX_IDX,0);
	gpio_pad_select_gpio(CAN_NODE.Rx_PIN);

	//Set to PELICAN Mode
	CAN_MODULE->CDR.B.CAN_M = 0x1;

	//Select Time Quanta and set TSEG1
	switch(CAN_NODE.speed){
	case SPEED_1000KBPS:
		CAN_MODULE->BTR1.B.TSEG1=0x4;
		__tq=0.125;
		break;

	case SPEED_800KBPS:
		CAN_MODULE->BTR1.B.TSEG1=0x6;
		__tq=0.125;
		break;

	default:
		CAN_MODULE->BTR1.B.TSEG1=0xc;
		__tq=((float)1000/CAN_NODE.speed) / 16;

	}

	//set Baud rate prescaler
	CAN_MODULE->BTR0.B.BRP = (uint8_t)round((((APB_CLK_FREQ * __tq) / 2) - 1)/1000000)-1;

	// Set Sampling
	CAN_MODULE->BTR0.B.SAM=0x1;

	//enable all interupts
	CAN_MODULE->IER.U=0xff;

	//no acceptance filtering
	CAN_MODULE->MBX_CTRL.ACC.CODE[0] = 0;
	CAN_MODULE->MBX_CTRL.ACC.CODE[1] = 0;
	CAN_MODULE->MBX_CTRL.ACC.CODE[2] = 0;
	CAN_MODULE->MBX_CTRL.ACC.CODE[3] = 0;
	CAN_MODULE->MBX_CTRL.ACC.MASK[0] = 0xff;
	CAN_MODULE->MBX_CTRL.ACC.MASK[1] = 0xff;
	CAN_MODULE->MBX_CTRL.ACC.MASK[2] = 0xff;
	CAN_MODULE->MBX_CTRL.ACC.MASK[3] = 0xff;

	//Set to Normal Mode
	CAN_MODULE->OCR.B.OCMODE = __CAN_OC_NOM;

	//clear all error counters
	CAN_MODULE->TXERR.U = 0;
	CAN_MODULE->RXERR.U = 0;
	(void) CAN_MODULE->ECC;

	//clear all Interrupt Flags
	(void)CAN_MODULE->IR.U;

	//install CAN ISR
	esp_intr_alloc(ETS_CAN_INTR_SOURCE,0,CAN_isr,NULL,NULL);

	//Release Reset Mode
	CAN_MODULE->Mode.B.RM = 0;

	return 0;

}


int CAN_STOP(){
	CAN_MODULE->Mode.B.RM = 1;

	return 0;
}

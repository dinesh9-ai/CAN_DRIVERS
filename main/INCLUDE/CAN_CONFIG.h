/*
 * CAN_CONFIG.h
 *
 *  Created on: 04-Feb-2024
 *      Author: dinesh
 */

#ifndef MAIN_INCLUDE_CAN_CONFIG_H_
#define MAIN_INCLUDE_CAN_CONFIG_H_


#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "driver/gpio.h"


//CAN Node speeds
typedef enum{
	SPEED_100KBPS = 100,  /* 100  KBPS SPEED */
	SPEED_125KBPS = 125,  /* 125  KBPS SPEED */
	SPEED_250KBPS = 250,  /* 250  KBPS SPEED */
	SPEED_500KBPS = 500,  /* 500  KBPS SPEED */
	SPEED_800KBPS = 800,  /* 800  KBPS SPEED */
	SPEED_1000KBPS = 1000 /* 1000 KBPS SPEED */
} CAN_SPEED_t;


//CAN NODE CONFIG
typedef struct{
	CAN_SPEED_t 		speed;    /* To Set the CAN Node Speed */
	gpio_num_t 			Tx_PIN;	  /* TO Select the Tx PIN for CAN */
	gpio_num_t			Rx_PIN;	  /* TO Select the Rx PIN for CAN */
	QueueHandle_t		Rx_Queue; /* To Select RTOS Rx Queue */
}CAN_CONFIG_t;


extern CAN_CONFIG_t CAN_NODE;

#endif /* MAIN_INCLUDE_CAN_CONFIG_H_ */

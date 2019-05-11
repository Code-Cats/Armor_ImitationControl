#ifndef __CAN_ANALYSIS_H
#define __CAN_ANALYSIS_H

#include "stm32f1xx_hal.h"

#define ZGYRO_FEEDBACK_MSG_ID    0x401

typedef struct
{
	uint8_t Data[8];
}CAN_RecvMsg;

HAL_StatusTypeDef CAN_Filter_Config(void);
HAL_StatusTypeDef CAN_TRANSMIT(void);
uint8_t CANx_SendNormalData(CAN_HandleTypeDef* hcan,uint16_t ID,uint8_t *pData);

#endif


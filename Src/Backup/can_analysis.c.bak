#include "can_analysis.h"

extern CAN_HandleTypeDef hcan;

HAL_StatusTypeDef CAN_Filter_Config(void)
{	
//	CAN_TxHeaderTypeDef hcan_tx;
//	CAN_RxHeaderTypeDef hcan_rx;

//	hcan.pTxMsg = &TxMessage;

//	hcan.pRxMsg = &RxMessage;



//	/*##-1- Configure CAN1 Transmission Massage #####################################*/

//	hcan.pTxMsg->StdId = 0x123;

//	hcan.pTxMsg->RTR = CAN_RTR_DATA;

//	hcan.pTxMsg->IDE = CAN_ID_STD;

//	hcan.pTxMsg->DLC = 8;


	 HAL_StatusTypeDef  HAL_Status;  
	/*##-2- Configure the CAN1 Filter ###########################################*/
	CAN_FilterTypeDef sFilterConfig;

	sFilterConfig.FilterIdHigh = 0;

	sFilterConfig.FilterIdLow = 0;

	sFilterConfig.FilterMaskIdHigh = 0;

	sFilterConfig.FilterMaskIdLow = 0;

	sFilterConfig.FilterBank = 0;

	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;

	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;

	sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;

	sFilterConfig.FilterActivation = ENABLE;

	sFilterConfig.SlaveStartFilterBank = 14;

	HAL_CAN_ConfigFilter(&hcan, &sFilterConfig);
	
	HAL_Status=HAL_CAN_Start(&hcan);  //开启CAN  
	if(HAL_Status!=HAL_OK)
	{  
		//printf("开启CAN失败\r\n");    
		return HAL_Status;
	}    
	HAL_Status=HAL_CAN_ActivateNotification(&hcan,   CAN_IT_RX_FIFO0_MSG_PENDING);  
	if(HAL_Status!=HAL_OK)
	{  
		//printf("开启挂起中段允许失败\r\n");  
		return HAL_Status;
	}  
	return HAL_OK;
}

HAL_StatusTypeDef CAN_TRANSMIT(void)
{
	CAN_TxHeaderTypeDef TxMessage1;
	TxMessage1.DLC=8;
	TxMessage1.StdId=ZGYRO_FEEDBACK_MSG_ID;
	TxMessage1.ExtId=0x00000401;
	TxMessage1.IDE=CAN_ID_STD;
	TxMessage1.RTR=CAN_RTR_DATA;
	uint8_t TR_BUF[8]="20180411";

	if(HAL_CAN_AddTxMessage(&hcan,&TxMessage1,TR_BUF,(uint32_t*)CAN_TX_MAILBOX0)!=HAL_OK)
	{
	 //printf("發送失敗");
		return HAL_ERROR;
	}
	return HAL_OK;
}

uint32_t can_test_count[10]={0};
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  if(hcan->Instance==CAN1)
  {
	 CAN_RxHeaderTypeDef RxMessage1;
	 CAN_RecvMsg can_recvmsg;
     HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&RxMessage1,can_recvmsg.Data);
     //printf("\r\n%s",can_recvmsg.Data);
	  switch(RxMessage1.StdId)
	  {
		 case 0x201:	//右
		{
			can_test_count[0]++;
			break;
		}
		case 0x202:	//左
		{
			can_test_count[1]++;
			break;
		}
		case 0x203:	//shoot 上
		{
			can_test_count[2]++;
			break;
		}
		case 0x204:	//空
		{
			can_test_count[3]++;
			break;
		}
		case 0x205:	//yaw
		{
			can_test_count[4]++;
			break;
		}
		case 0x206:	//pitch
		{
			can_test_count[5]++;
			break;
		}
		case 0x207:	//shoot
		{
			can_test_count[6]++;
			break;
		}
		case 0x404:	//外部发送ID
		{
			can_test_count[7]++;
			break;
		}
		default:
		break; 
	  }
  }
}

//void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
//{
//  if(hcan->Instance==CAN2)
//  {
//     HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO1,&RxMessage1,can_recvmsg.Data);
//     printf("\r\n%s",can_recvmsg.Data);
//  }
//}

////void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)  //接收回调函数

////{

////uint8_t  Data[8];

////HAL_StatusTypeDef	HAL_RetVal;

////if(hcan ==&hcan1){	

////  HAL_RetVal=HAL_CAN_GetRxMessage(&hcan1,  CAN1FIFO, &RxMeg,  Data);

////  if ( HAL_OK==HAL_RetVal)

////  {                              			

////      //在这里接收数据

////   }

////}

//发送数据函数
uint8_t tempdata[100];
uint8_t CANx_SendNormalData(CAN_HandleTypeDef* hcan,uint16_t ID,uint8_t *pData)
{
	CAN_TxHeaderTypeDef TxMeg;
	HAL_StatusTypeDef	HAL_RetVal;

	uint8_t  FreeTxNum=0;

	TxMeg.StdId=ID;

	TxMeg.DLC=8;
	//TxMessage1.StdId=ZGYRO_FEEDBACK_MSG_ID;
	//TxMessage1.ExtId=0x00000401;
	TxMeg.IDE=CAN_ID_STD;
	TxMeg.RTR=CAN_RTR_DATA;

	if(!hcan || ! pData )  
	{
		return 1;
	}

	FreeTxNum=HAL_CAN_GetTxMailboxesFreeLevel(hcan);

	TxMeg.DLC=8;

	uint32_t count_overtime=0;
	while(0==FreeTxNum)
	{
		FreeTxNum=HAL_CAN_GetTxMailboxesFreeLevel(hcan);
		count_overtime++;
		if(count_overtime>3000)
		{
			break;
		}
	}

	//HAL_Delay(1);   //没有延时很有可能会发送失败

	HAL_RetVal=HAL_CAN_AddTxMessage(hcan,&TxMeg,pData,(uint32_t*)tempdata); 

	if(HAL_RetVal!=HAL_OK)
	{
		return 2;
	}
	return 0;
}


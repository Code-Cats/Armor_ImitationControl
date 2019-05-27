#include "hit_recognition.h"

#define AIMORDATA_DEFAULT \
{\
	OFF_LINE,\
	0,\
	ArmorHit_CallBack,\
}

AimorDataTypeDef AimorData[5]={AIMORDATA_DEFAULT, AIMORDATA_DEFAULT, AIMORDATA_DEFAULT, AIMORDATA_DEFAULT, AIMORDATA_DEFAULT};
/****************************************************
name:CAN_GetTxMailboxesFreeLevel
function:��ѯ�Ƿ��п�������
@param:CANx:CAN1 or CAN2
@return:1:free 0:buzy
description:
****************************************************/
uint8_t CAN_GetTxMailboxesFreeLevel(CAN_TypeDef* CANx)
{
//	uint8_t transmit_mailbox = 0;
	/* Check the parameters */
	assert_param(IS_CAN_ALL_PERIPH(CANx));
	/* Select one empty transmit mailbox */
	if ((CANx->TSR&CAN_TSR_TME0) == CAN_TSR_TME0)
	{
		//transmit_mailbox = 0;
		return 1;
	}
	else if ((CANx->TSR&CAN_TSR_TME1) == CAN_TSR_TME1)
	{
		//transmit_mailbox = 1;
		return 1;
	}
	else if ((CANx->TSR&CAN_TSR_TME2) == CAN_TSR_TME2)
	{
		//transmit_mailbox = 2;
		return 1;
	}
	else
	{
		//transmit_mailbox = CAN_TxStatus_NoMailBox;	//CAN_NO_MB
		return 0;
	}
}

/****************************************************
name:CAN_SendNormalMsg
function:CAN����ͨ�ú���
@param:CANx:CAN1 or CAN2
@param:StdId:CAN StdID
@param:pdata:SendData Point
@param:length:SendData Length
@return:void
description:
****************************************************/
uint8_t tempCanTxdata2[100];
bool CAN_SendNormalMsg(CAN_HandleTypeDef* hcanx, uint32_t StdId, uint8_t* pdata, uint8_t length)
{
	CAN_TxHeaderTypeDef TxMessage;
	HAL_StatusTypeDef	HAL_RetVal;
	TxMessage.StdId = StdId;      //֡IDΪ���������CAN_ID
    TxMessage.IDE = CAN_ID_STD;    //��׼֡
    TxMessage.RTR = CAN_RTR_DATA;  //����֡
    TxMessage.DLC = 0x08;          //֡���ȳ�ʼ��Ϊ8
	
	for(uint8_t Txcount=0;Txcount<length;)
	{
		uint8_t remain=length-Txcount;
		if(remain>=8)
		{
			TxMessage.DLC = 0x08;
			
		}
		else
		{
			TxMessage.DLC = remain;
		}
		
		//for(int i=0;i<TxMessage.DLC;i++)
		//{
			//TxMessage.Data[i]=pdata[i+Txcount];
		//}
		uint32_t count_overtime=0;
		while(HAL_CAN_GetTxMailboxesFreeLevel(hcanx)==0)	//HAL_CAN_GetTxMailboxesFreeLevel(hcan);
		{
			count_overtime++;
			if(count_overtime>5000)
				break;
		}
		HAL_RetVal=HAL_CAN_AddTxMessage(hcanx,&TxMessage,&pdata[Txcount],(uint32_t*)tempCanTxdata2); 

		if(HAL_RetVal!=HAL_OK)
		{
			return false;
		}
		
		Txcount+=TxMessage.DLC;
		
	}
	return true;
}

/****************************************************
name:CAN_RefereeStart_SendMsg
function:���Ͳ�����������
@param:void
@return:void
description:�����ݴ���TxMessage�ṹ������CAN_Transmit����
****************************************************/
uint8_t RefereeStartData1[8]={0x5a, 0x00, 0x00, 0x33, 0x00, 0x01, 0x01, 0xff};
uint8_t RefereeStartData2[8]={0xff, 0x01, 0x00, 0x00, 0x15, 0x12, 0xfc, 0xea};
void CAN_RefereeStart_SendMsg(void)
{
//	CanTxMsg TxMessage;
//	TxMessage.StdId = 0x0fe;      //֡IDΪ���������CAN_ID
//    TxMessage.IDE = CAN_ID_STD;    //��׼֡
//    TxMessage.RTR = CAN_RTR_DATA;  //����֡
//    TxMessage.DLC = 0x08;          //֡����Ϊ8
//    
//	for(int i=0;i<TxMessage.DLC;i++)
//	{
//		TxMessage.Data[i]=RefereeStartData1[i];
//	}
//    CAN_Transmit(CAN1,&TxMessage);
	CAN_SendNormalMsg(&hcan,0xfe,RefereeStartData1,8);
	
	//delay_us(10);
	HAL_Delay(1);
	
//	for(int i=0;i<TxMessage.DLC;i++)
//	{
//		TxMessage.Data[i]=RefereeStartData2[i];
//	}
//    CAN_Transmit(CAN1,&TxMessage);
	
	CAN_SendNormalMsg(&hcan,0xfe,RefereeStartData2,8);
	
}


/****************************************************
name:CAN_HeartBeat_SendMsg
function:���Ͳ�����������
@param:void
@return:void
description:�����ݴ���TxMessage�ṹ������CAN_Transmit����
****************************************************/
uint8_t RefereeHeartBeatData1[16]={0x5a, 0x00, 0x00, 0x0c, 0x00, 0x01, 0x01, 0xff,\
							0xff, 0x01, 0x00, 0x00, 0x15, 0xb8, 0x2f, 0x6f };
uint8_t RefereeHeartBeatData2[16]={0x5a, 0x00, 0x00, 0x56, 0x00, 0x01, 0x01, 0xff,\
							0xff, 0x01, 0x00, 0x00, 0x15, 0x7e, 0xdd, 0x67 };
uint8_t RefereeHeartBeatData3[16]={0x5a, 0x00, 0x00, 0xac, 0x00, 0x01, 0x01, 0xff,\
							0xff, 0x01, 0x00, 0x00, 0x15, 0x72, 0xe8, 0x76 };
void CAN_HeartBeat_SendMsg(void)
{	
//	CanTxMsg TxMessage;
//	TxMessage.StdId = 0xfe;      //֡IDΪ���������CAN_ID
//    TxMessage.IDE = CAN_ID_STD;    //��׼֡
//    TxMessage.RTR = CAN_RTR_DATA;  //����֡
//    TxMessage.DLC = 0x08;          //֡����Ϊ8
    
	static uint8_t heatcount=0;
	
	switch(heatcount)
	{
		case 0:
		{
//			for(int i=0;i<TxMessage.DLC;i++)
//			{
//				TxMessage.Data[i]=RefereeStartData2[i];
//			}
			CAN_SendNormalMsg(&hcan,0xfe,RefereeHeartBeatData1,16);
			heatcount++;
			break;
		}
		case 1:
		{
			CAN_SendNormalMsg(&hcan,0xfe,RefereeHeartBeatData2,16);
			heatcount++;
			break;
		}
		case 2:
		{
			CAN_SendNormalMsg(&hcan,0xfe,RefereeHeartBeatData3,16);
			heatcount=0;
			break;
		}
		default:
			break;
	}
}


uint8_t RefereeSetTurnONTESTData[2]={0x01 };//0xe8
void CAN_TurnONTEST_SendMsg(void)	//����0x350��������
{
	CAN_SendNormalMsg(&hcan,0x350,RefereeSetTurnONTESTData,1);
}


/****************************************************
name:CAN_SetID0TurnON_SendMsg
function:���Ͳ�����������
@param:void
@return:void
description:�����ݴ���TxMessage�ṹ������CAN_Transmit����
****************************************************/
//u8 RefereeID0SetTurnONData[16]={0x5a, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02 ,\
//							0x00, 0x01, 0x00, 0x07, 0x15, 0x12, 0xb1, 0x4f };//0xe8	//ֻ�н��˴�װ�ײ���
uint8_t RefereeID0SetTurnONData[16]={0x5a,0x00,0x00,0x05,0x00,0x01,0x01,0x02 ,\
							0x00,0x01,0x00,0x07,0x15,0xcf,0x1d,0x49 };//0xe8
void CAN_SetID0TurnON_SendMsg(void)	//�������� �صƺ󲻿���
{
	CAN_SendNormalMsg(&hcan,0x140,RefereeID0SetTurnONData,16);
}

/****************************************************
name:CAN_SetID1TurnON_SendMsg
function:���Ͳ�����������
@param:void
@return:void
description:�����ݴ���TxMessage�ṹ������CAN_Transmit����
****************************************************/
uint8_t RefereeID1SetTurnONData[16]={0x5a, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02 ,\
							0x01, 0x01, 0x00, 0x07, 0x15, 0xdf, 0x73, 0x56 };//0xe8
void CAN_SetID1TurnON_SendMsg(void)	//�������� �صƺ󲻿���
{
	CAN_SendNormalMsg(&hcan,0x141,RefereeID1SetTurnONData,16);
}

/****************************************************
name:CAN_SetID2TurnON_SendMsg
function:���Ͳ�����������
@param:void
@return:void
description:�����ݴ���TxMessage�ṹ������CAN_Transmit����
****************************************************/
uint8_t RefereeID2SetTurnONData[16]={0x5a, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02 ,\
							0x02, 0x01, 0x00, 0x07, 0x15, 0x91, 0x74, 0xf1 };//0xe8
void CAN_SetID2TurnON_SendMsg(void)	//�������� �صƺ󲻿���
{
	CAN_SendNormalMsg(&hcan,0x142,RefereeID2SetTurnONData,16);
}

/****************************************************
name:RefereeID3SetTurnONData
function:���Ͳ�����������
@param:void
@return:void
description:�����ݴ���TxMessage�ṹ������CAN_Transmit����
****************************************************/
uint8_t RefereeID3SetTurnONData[16]={0x5a, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02 ,\
							0x03, 0x01, 0x00, 0x07, 0x15, 0x5c, 0xb6, 0xe8 };//0xe8
void CAN_SetID3TurnON_SendMsg(void)	//�������� �صƺ󲻿���
{
	CAN_SendNormalMsg(&hcan,0x143,RefereeID3SetTurnONData,16);
}

/****************************************************
name:CAN_SetID4TurnON_SendMsg
function:���Ͳ�����������
@param:void
@return:void
description:�����ݴ���TxMessage�ṹ������CAN_Transmit����
****************************************************/
uint8_t RefereeID4SetTurnONData[16]={0x5a, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02 ,\
							0x04, 0x01, 0x00, 0x07, 0x15, 0x0d, 0x6b, 0xb7 };//0xe8
void CAN_SetID4TurnON_SendMsg(void)	//�������� �صƺ󲻿���
{
	CAN_SendNormalMsg(&hcan,0x144,RefereeID3SetTurnONData,16);
}


void CAN_SetIDxTurnON_Task(AimorIDEnum id)
{
	switch(id)
	{
		case AIMORID_240:
		{
			CAN_SetID0TurnON_SendMsg();
			break;
		}
		case AIMORID_241:
		{
			CAN_SetID1TurnON_SendMsg();
			break;
		}
		case AIMORID_242:
		{
			CAN_SetID2TurnON_SendMsg();
			break;
		}
		case AIMORID_243:
		{
			CAN_SetID3TurnON_SendMsg();
			break;
		}
		case AIMORID_244:
		{
			CAN_SetID4TurnON_SendMsg();
			break;
		}
		default:
			break;
	}
}


/****************************************************
name:CAN_SetID0Red2Blue_SendMsg
function:���Ͳ���ת��������
@param:void
@return:void
description:�����ݴ���TxMessage�ṹ������CAN_Transmit����
****************************************************/
uint8_t RefereeID0SetRedToBlueData1[18]={0x5a,0x02,0x00,0x01,0x00,0x01,0x0b,0x02 ,\
							0x00,0x01,0x00,0x04,0x00,0x70,0x01,0x0B,\
							0x1f,0x9a};	//��OK
uint8_t RefereeID0SetRedToBlueData2[20]={0x5a,0x04,0x00,0x02,0x00,0x01,0x0b,0x02 ,\
							0x00,0x01,0x00,0x05,0x00,0x96,0x00,0x00,\
							0xff,0x00,0xa0,0xd0};	//��OK
void CAN_SetID0Red2Blue_SendMsg(void)
{
	CAN_SendNormalMsg(&hcan,0x140,RefereeID0SetRedToBlueData1,18);
	HAL_Delay(1);
	CAN_SendNormalMsg(&hcan,0x140,RefereeID0SetRedToBlueData2,20);
}


/****************************************************
name:CAN_SetID1Red2Blue_SendMsg //OK
function:���Ͳ���ת��������
@param:void
@return:void
description:�����ݴ���TxMessage�ṹ������CAN_Transmit����
****************************************************/
uint8_t RefereeID1SetRedToBlueData1[18]={0x5a,0x02,0x00,0x00,0x00,0x01,0x0b,0x02 ,\
							0x01,0x01,0x00,0x04,0x00,0x73,0x01,0x0b,\
							0x69,0xf1};	//OK
uint8_t RefereeID1SetRedToBlueData2[20]={0x5a,0x04,0x00,0x01,0x00,0x01,0x0b,0x02 ,\
							0x01,0x01,0x00,0x05,0x00,0x10,0x00,0x00,\
							0xff,0x00,0x34,0x95};	//OK
void CAN_SetID1Red2Blue_SendMsg(void)
{
	CAN_SendNormalMsg(&hcan,0x141,RefereeID1SetRedToBlueData1,18);
	HAL_Delay(1);
	CAN_SendNormalMsg(&hcan,0x141,RefereeID1SetRedToBlueData2,20);
}

/****************************************************
name:CAN_SetID2Red2Blue_SendMsg //OK
function:���Ͳ���ת��������
@param:void
@return:void
description:�����ݴ���TxMessage�ṹ������CAN_Transmit����
****************************************************/
uint8_t RefereeID2SetRedToBlueData1[18]={0x5a,0x02,0x00,0x00,0x00,0x01,0x0b,0x02 ,\
							0x02,0x01,0x00,0x04,0x00,0x3d,0x01,0x0b,\
							0xd4,0x6d};	//OK
uint8_t RefereeID2SetRedToBlueData2[20]={0x5a,0x04,0x00,0x01,0x00,0x01,0x0b,0x02 ,\
							0x02,0x01,0x00,0x05,0x00,0x5e,0x00,0x00,\
							0xff,0x00,0xc7,0x41};	//OK
void CAN_SetID2Red2Blue_SendMsg(void)
{
	CAN_SendNormalMsg(&hcan,0x142,RefereeID2SetRedToBlueData1,18);
	HAL_Delay(1);
	CAN_SendNormalMsg(&hcan,0x142,RefereeID2SetRedToBlueData2,20);
}

/****************************************************
name:CAN_SetID3Red2Blue_SendMsg //OK
function:���Ͳ���ת��������
@param:void
@return:void
description:�����ݴ���TxMessage�ṹ������CAN_Transmit����
****************************************************/
uint8_t RefereeID3SetRedToBlueData1[18]={0x5a, 0x02, 0x00, 0x00, 0x00, 0x01, 0x0b, 0x02  ,\
							0x03, 0x01, 0x00, 0x04, 0x00, 0xf0, 0x01, 0x0b ,\
							0x8e,0x19};	//OK
uint8_t RefereeID3SetRedToBlueData2[20]={0x5a, 0x04, 0x00, 0x01, 0x00, 0x01, 0x0b, 0x02  ,\
							0x03, 0x01, 0x00, 0x05, 0x00, 0x93, 0x00, 0x00 ,\
							0xff, 0x00, 0xe3, 0x5a };	//OK
void CAN_SetID3Red2Blue_SendMsg(void)
{
	CAN_SendNormalMsg(&hcan,0x143,RefereeID3SetRedToBlueData1,18);
	HAL_Delay(1);
	CAN_SendNormalMsg(&hcan,0x143,RefereeID3SetRedToBlueData2,20);
}

/****************************************************
name:CAN_SetID0Blue2Red_SendMsg
function:���Ͳ���ת�������
@param:void
@return:void
description:�����ݴ���TxMessage�ṹ������CAN_Transmit����
****************************************************/
//uint8_t RefereeID0SetBlueToRedData1[18]={0x5a,0x02,0x00,0x00,0x00,0x01,0x01,0x02 ,\
//							0x00,0x01,0x00,0x04,0x00,0x35,0x01,0x01,\
//							0x44, 0x3e};
//uint8_t RefereeID0SetBlueToRedData2[20]={0x5a,0x04,0x00,0x01,0x00,0x01,0x01,0x02 ,\
//							0x00,0x01,0x00,0x05,0x00,0x56,0xff,0x00,\
//							0x00, 0x00, 0xec, 0x71};	//���������ȫ����//��1498��
uint8_t RefereeID0SetBlueToRedData1[18]={0x5a,0x02,0x00,0x01,0x00,0x01,0x01,0x02 ,\
							0x00,0x01,0x00,0x04,0x00,0xfb,0x01,0x01,\
							0x68,0x21};	//��OK
uint8_t RefereeID0SetBlueToRedData2[20]={0x5a,0x04,0x00,0x02,0x00,0x01,0x01,0x02 ,\
							0x00,0x01,0x00,0x05,0x00,0x1d,0xff,0x00,\
							0x00,0x00,0x5c,0x2f};	//��OK
void CAN_SetID0Blue2Red_SendMsg(void)
{
	CAN_SendNormalMsg(&hcan,0x140,RefereeID0SetBlueToRedData1,18);
	HAL_Delay(1);
	CAN_SendNormalMsg(&hcan,0x140,RefereeID0SetBlueToRedData2,20);
}

/****************************************************
name:CAN_SetID1Blue2Red_SendMsg ����֤
function:���Ͳ���ת�������
@param:void
@return:void
description:�����ݴ���TxMessage�ṹ������CAN_Transmit����
****************************************************/
uint8_t RefereeID1SetBlueToRedData1[18]={0x5a, 0x02, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02,\
							0x01, 0x01, 0x00, 0x04, 0x00, 0xf8, 0x01, 0x01,\
							0x1e, 0x4a};
uint8_t RefereeID1SetBlueToRedData2[20]={0x5a, 0x04, 0x00, 0x01, 0x00, 0x01, 0x01, 0x02,\
							0x01, 0x01, 0x00, 0x05, 0x00, 0x9b, 0xff, 0x00,\
							0x00, 0x00, 0xc8, 0x6a };	//���������ȫ����//��1498��
void CAN_SetID1Blue2Red_SendMsg(void)
{
	CAN_SendNormalMsg(&hcan,0x141,RefereeID1SetBlueToRedData1,18);
	HAL_Delay(1);
	CAN_SendNormalMsg(&hcan,0x141,RefereeID1SetBlueToRedData2,20);
}

/****************************************************
name:CAN_SetID2Blue2Red_SendMsg ����֤
function:���Ͳ���ת�������
@param:void
@return:void
description:�����ݴ���TxMessage�ṹ������CAN_Transmit����
****************************************************/
uint8_t RefereeID2SetBlueToRedData1[18]={0x5a, 0x02, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02,\
							0x02, 0x01, 0x00, 0x04, 0x00, 0xb6, 0x01, 0x01,\
							0xa3, 0xd6};
uint8_t RefereeID2SetBlueToRedData2[20]={0x5a, 0x04, 0x00, 0x01, 0x00, 0x01, 0x01, 0x02,\
							0x02, 0x01, 0x00, 0x05, 0x00, 0xd5, 0xff, 0x00,\
							0x00, 0x00, 0x3b, 0xbe };	//���������ȫ����//��1498��
void CAN_SetID2Blue2Red_SendMsg(void)
{
	CAN_SendNormalMsg(&hcan,0x142,RefereeID2SetBlueToRedData1,18);
	HAL_Delay(1);
	CAN_SendNormalMsg(&hcan,0x142,RefereeID2SetBlueToRedData2,20);
}

/****************************************************
name:CAN_SetID3Blue2Red_SendMsg ����֤
function:���Ͳ���ת�������
@param:void
@return:void
description:�����ݴ���TxMessage�ṹ������CAN_Transmit����
****************************************************/
uint8_t RefereeID3SetBlueToRedData1[18]={0x5a, 0x02, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02,\
							0x03, 0x01, 0x00, 0x04, 0x00, 0x7b, 0x01, 0x01,\
							0xf9, 0xa2};
uint8_t RefereeID3SetBlueToRedData2[20]={0x5a, 0x04, 0x00, 0x01, 0x00, 0x01, 0x01, 0x02,\
							0x03, 0x01, 0x00, 0x05, 0x00, 0x18, 0xff, 0x00,\
							0x00, 0x00, 0x1f, 0xa5 };	//���������ȫ����//��1498��
void CAN_SetID3Blue2Red_SendMsg(void)
{
	CAN_SendNormalMsg(&hcan,0x143,RefereeID3SetBlueToRedData1,18);
	HAL_Delay(1);
	CAN_SendNormalMsg(&hcan,0x143,RefereeID3SetBlueToRedData2,20);
}

///////////////////////////////////////////������/////////////////////////////////////////////




/****************************************************
name:RefereeSetRedToBlueData
function:���Ͳ���ת������
@param:void
@return:void
description:�����ݴ���TxMessage�ṹ������CAN_Transmit����
****************************************************/
uint8_t RefereeSetRedToBlueData[20]={0x5a, 0x04, 0x00, 0x04, 0x00, 0x01, 0x0b, 0x02 ,\
							0xff, 0x01, 0x00, 0x06, 0x10, 0x01, 0x00, 0x00,\
							0x00, 0x00, 0x2c, 0x53};
void CAN_SetAllRed2Blue_SendMsg(void)
{
	//delay_us(50);
	HAL_Delay(1);
	CAN_SendNormalMsg(&hcan,0xfe,RefereeSetRedToBlueData,20);
}


/****************************************************
name:CAN_SetALLRedTurnOFF_SendMsg
function:���Ͳ��� Red Turn OFF������
@param:void
@return:void
description:�����ݴ���TxMessage�ṹ������CAN_Transmit����
****************************************************/
uint8_t RefereeSetALLRedTurnOFFData[20]={0x5a, 0x04, 0x00, 0x86, 0x01, 0x01, 0x01, 0x02 ,\
							0xff, 0x01, 0x00, 0x06, 0x10, 0xf9, 0x01, 0x00,\
							0x00, 0x00, 0x8d, 0x53};
void CAN_SetALLRedTurnOFF_SendMsg(void)
{
	CAN_SendNormalMsg(&hcan,0xfe,RefereeSetALLRedTurnOFFData,20);
}


/****************************************************
name:CAN_SetALLBlueTurnOFF_SendMsg???
function:���Ͳ���Blue Turn OFF������
@param:void
@return:void
description:�����ݴ���TxMessage�ṹ������CAN_Transmit����
****************************************************/
uint8_t RefereeSetALLBlueTurnOFFData[20]={0x5a, 0x04, 0x00, 0x8a, 0x01, 0x01, 0x0b, 0x02 ,\
							0xff, 0x01, 0x00, 0x06, 0x10, 0x47, 0x01, 0x00,\
							0x00, 0x00, 0x2d, 0x1d};
void CAN_SetALLBlueTurnOFF_SendMsg(void)
{
	CAN_SendNormalMsg(&hcan,0xfe,RefereeSetALLBlueTurnOFFData,20);
}


void ALLAutoTurnON(void)
{
	for(AimorIDEnum id=AIMORID_240;id<AIMORID_NUMS;id++)
	{
		switch(AimorData[id].state)	//���Գ����������е�����ָ����
		{
			case OFF_LINE:
			{
				//CAN_RefereeStart_SendMsg();
				break;
			}
			case ON_WAIT_SET:
			{
				CAN_SetIDxTurnON_Task(id);
				HAL_Delay(20);
				AimorData[id].state=ON_LINE;
				break;
			}
			case ON_LINE:
			{
				//CAN_SetAllRed2Blue_SendMsg();
				//CAN_HeartBeat_SendMsg();
				//delay_ms(200);

				break;
			}
		}

	}
	
}

/****************************************************
name:ArmorHit_CallBack
function:װ�װ屻����ʱ�����Ļص�
@param:id:װ�װ�ID����ֽ�
@param:type:�ܴ��������
@return:void
description:����ص����������д����������
****************************************************/
__weak void ArmorHit_CallBack(AimorIDEnum id,AimorHitTypeEnum type)
{
	
}

void Armor_Data_Analysis(AimorIDEnum id,uint8_t* pdata,uint8_t length)	//��������װ�װ�ID:240-244
{
	if(length==8)
	{
		if(*pdata==0x5a&&*(pdata+1)==0x00&&*(pdata+2)==0x00)	//ģ������ͨ������
		{
			AimorData[id].state=ON_WAIT_SET;
		}
		
		if(*pdata==0x5a&&*(pdata+1)==0x09&&*(pdata+2)==0x00)	//�˺�����
		{
			AimorData[id].numOfhits++;
			AimorData[id].hitcallback(id,HIT_BULLET17);
		}
	}
}

/**************** **************************
��������CAN1_Hit_Analysis
�������ܣ���װ�װ����ݽ��н���
          �õ���������
������������
��������ֵ����
������������
*******************************************/
uint32_t hit_count=0;
void CAN1_Hit_Analysis(CAN_RxHeaderTypeDef* rxMessage,CAN_RecvMsg *can_recvmsg)
{		
	//CAN_Receive(CAN1, CAN_FIFO0, rx_message);//��ȡ����  ������bsp��
	
	switch(rxMessage->StdId)
	{
		case 0x150:	//�����������
		{
			hit_count++;  
			break;
		}
		case 0x240:	//id=1װ������
		{
//			if(AimorData[0].state==OFF_LINE)
//			{
//				AimorData[0].state=ON_WAIT_SET;
//			}
//			else if(AimorData[0].state==ON_WAIT_SET)
//			{
//				//AimorData[0].state=ON_LINE;
//			}
			Armor_Data_Analysis(AIMORID_240,can_recvmsg->Data,rxMessage->DLC);
			break;
		}
		case 0x241:	//id=2װ������
		{
//			if(AimorData[1].state==OFF_LINE)
//			{
//				AimorData[1].state=ON_WAIT_SET;
//			}
//			else if(AimorData[1].state==ON_WAIT_SET)
//			{
//				//AimorData[1].state=ON_LINE;
//			}
			Armor_Data_Analysis(AIMORID_241,can_recvmsg->Data,rxMessage->DLC);
			break;
		}
		case 0x242:	//id=3װ������
		{
//			if(AimorData[2].state==OFF_LINE)
//			{
//				AimorData[2].state=ON_WAIT_SET;
//			}
//			else if(AimorData[2].state==ON_WAIT_SET)
//			{
//				//AimorData[2].state=ON_LINE;
//			}
			Armor_Data_Analysis(AIMORID_242,can_recvmsg->Data,rxMessage->DLC);
			break;
		}
		case 0x243:	//id=4װ������
		{
			
//			if(AimorData[3].state==OFF_LINE)
//			{
//				AimorData[3].state=ON_WAIT_SET;
//			}
//			else if(AimorData[3].state==ON_WAIT_SET)
//			{
//				//AimorData[3].state=ON_LINE;
//			}
			Armor_Data_Analysis(AIMORID_243,can_recvmsg->Data,rxMessage->DLC);
			break;
		}
		case 0x244:	//id=5��װ������
		{
//			if(AimorData[4].state==OFF_LINE)
//			{
//				AimorData[4].state=ON_WAIT_SET;
//			}
//			else if(AimorData[4].state==ON_WAIT_SET)
//			{
//				//AimorData[4].state=ON_LINE;
//			}
			Armor_Data_Analysis(AIMORID_244,can_recvmsg->Data,rxMessage->DLC);
			break;
		}
		default:
		break;
	}
}


///****************************************************
//name:CAN_HeartBeat_SendMsg
//function:���Ͳ�����������
//@param:void
//@return:void
//description:�����ݴ���TxMessage�ṹ������CAN_Transmit����
//****************************************************/
//void CAN_HeartBeat_SendMsg(void)
//{	
//	CanTxMsg TxMessage;
//	TxMessage.StdId = 0x200;      //֡IDΪ���������CAN_ID
//    TxMessage.IDE = CAN_ID_STD;    //��׼֡
//    TxMessage.RTR = CAN_RTR_DATA;  //����֡
//    TxMessage.DLC = 0x08;          //֡����Ϊ8
//    
//	TxMessage.Data={0};
//		 
//    CAN_Transmit(CAN1,&TxMessage);
//}


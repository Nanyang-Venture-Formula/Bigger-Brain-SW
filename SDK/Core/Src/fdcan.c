#include "can.h"

#if defined(HAL_FDCAN_MODULE_ENABLED)

void task_canhw(FDCAN_HandleTypeDef *hfdcan)
{
	static canbus_msg txmsg;
	txmsg.data32[0] = 0x01;
	txmsg.data32[1] = 0x01;
	txmsg.id = 0x44;

	canhw_send(hfdcan, &txmsg);
}

void init_canhw(FDCAN_HandleTypeDef *hfdcan)
{
   __fdcan_init(hfdcan);
    _canhw_set_filter(hfdcan);
    _canhw_start(hfdcan);
    _canhw_set_irq(hfdcan);
}

void __fdcan_init(FDCAN_HandleTypeDef *hfdcan)
{
    hfdcan->Init.FrameFormat = FDCAN_FRAME_CLASSIC;
    hfdcan->Init.Mode = FDCAN_MODE_NORMAL;
    hfdcan->Init.AutoRetransmission = DISABLE;
    hfdcan->Init.TransmitPause = DISABLE;
    hfdcan->Init.ProtocolException = DISABLE;
    hfdcan->Init.NominalPrescaler = 10;
    hfdcan->Init.NominalSyncJumpWidth = 1;
    hfdcan->Init.NominalTimeSeg1 = 2;
    hfdcan->Init.NominalTimeSeg2 = 2;
    hfdcan->Init.DataPrescaler = 1;
    hfdcan->Init.DataSyncJumpWidth = 1;
    hfdcan->Init.DataTimeSeg1 = 1;
    hfdcan->Init.DataTimeSeg2 = 1;
    hfdcan->Init.StdFiltersNbr = 1;
    hfdcan->Init.ExtFiltersNbr = 0;
    hfdcan->Init.RxFifo0ElmtsNbr = 1;
    hfdcan->Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_12;
    hfdcan->Init.RxFifo1ElmtsNbr = 0;
    hfdcan->Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8;
    hfdcan->Init.RxBuffersNbr = 0;
    hfdcan->Init.RxBufferSize = FDCAN_DATA_BYTES_8;
    hfdcan->Init.TxEventsNbr = 0;
    hfdcan->Init.TxBuffersNbr = 32;
    hfdcan->Init.TxFifoQueueElmtsNbr = 32;
    hfdcan->Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
    hfdcan->Init.TxElmtSize = FDCAN_DATA_BYTES_12;

    uint8_t ram_offset = 0;
    // ram offset +11 for each fdcan
    if (hfdcan->Instance == FDCAN2)
    {
        ram_offset = 11;
    }
    else if (hfdcan->Instance == FDCAN3)
    {
        ram_offset = 23;
    }

    hfdcan->Init.MessageRAMOffset = ram_offset;

    if (HAL_FDCAN_Init(hfdcan) != HAL_OK)
    {
        Error_Handler();
    }
}

void deinit_canhw(FDCAN_HandleTypeDef *hfdcan)
{
}

void _canhw_set_filter(FDCAN_HandleTypeDef *hfdcan)
{
    static FDCAN_FilterTypeDef sFilterConfig;


    // Configure global filter to accept all messages
    sFilterConfig.IdType = FDCAN_STANDARD_ID;
    sFilterConfig.FilterIndex = 0;
    sFilterConfig.FilterType = FDCAN_FILTER_RANGE;
    sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    sFilterConfig.FilterID1 = 0;
    sFilterConfig.FilterID2 = 0x7FF << 5;
    sFilterConfig.RxBufferIndex = 0;
    if (HAL_FDCAN_ConfigFilter(hfdcan, &sFilterConfig) != HAL_OK)
    {
        Error_Handler();
    }
}

void _canhw_set_irq(FDCAN_HandleTypeDef *hfdcan)
{
    /* Activate Rx FIFO 0 new message notification */
    if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
    {
        Error_Handler();
    }
}

void _canhw_start(FDCAN_HandleTypeDef *hfdcan)
{
    // Start the FDCAN peripheral
    if (HAL_FDCAN_Start(hfdcan) != HAL_OK)
    {
        Error_Handler();
    }
}

void _debug_canhw_senderror(HAL_StatusTypeDef can_txstatus)
{
    static char error_msg[32];
    switch (can_txstatus)
    {
    case HAL_ERROR:
        strcpy(error_msg, "CAN: HAL Error\n");
        break;
    case HAL_BUSY:
        strcpy(error_msg, "CAN: Peripheral Busy\n");
        break;
    case HAL_TIMEOUT:
        strcpy(error_msg, "CAN: Mailbox Timeout\n");
        break;
    default:
        strcpy(error_msg, "CAN: Unknown Error\n");
    }

    usb_vcptx(error_msg);
}

void _debug_can_print_msg(canbus_msg *cmsg)
{
    static char msg[64];
    sprintf(msg, "%08X::%08X::%08X -> %08X %08X \n", cmsg->ts_rx, cmsg->id, cmsg->dlc, cmsg->data32[0], cmsg->data32[1]);
    usb_vcptx(msg);
    return;
}

void cb_can_rx(FDCAN_HandleTypeDef *hfdcan, canbus_msg *rxmsg, FDCAN_RxHeaderTypeDef *rxHeader)
{
    /* Retreive Rx messages from RX FIFO0 */
    HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, rxHeader, rxmsg->data);
    rxmsg->dlc = rxHeader->DataLength;
    rxmsg->id = rxHeader->Identifier;
    rxmsg->ts_rx = rxHeader->RxTimestamp;

    rxmsg->which_can = 0;
    if (hfdcan->Instance == FDCAN1)
    {
    	rxmsg->which_can = 1;
    }
    else if (hfdcan->Instance == FDCAN2)
    {
    	rxmsg->which_can = 2;
    }
    else if (hfdcan->Instance == FDCAN3)
	{
		rxmsg->which_can = 3;
	}

    _debug_can_print_msg(rxmsg);
    return;
}

void canhw_send(FDCAN_HandleTypeDef *hfdcan, canbus_msg *msg)
{
    static FDCAN_TxHeaderTypeDef txHeader;
    static uint8_t txData[8];

    txHeader.Identifier = msg->id;
    txHeader.DataLength = FDCAN_DLC_BYTES_8;
    txHeader.IdType = FDCAN_STANDARD_ID;
    txHeader.TxFrameType = FDCAN_DATA_FRAME;

    memcpy(txData, msg->data, 8);

    HAL_StatusTypeDef txStatus = HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &txHeader, txData);

    // if no errors, exit from function
    if (txStatus == HAL_OK)
        return;

    // debug errors if exist
    _debug_canhw_senderror(txStatus);
    return;
}

#endif

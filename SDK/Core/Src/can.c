#include "can.h"

#if defined(HAL_CAN_MODULE_ENABLED)

void task_canhw(CAN_HandleTypeDef *hcan)
{
    
}

void init_canhw(CAN_HandleTypeDef *hcan)
{
    _canhw_set_filter(hcan);
    _canhw_set_irq(hcan);
    _canhw_start(hcan);
}

void deinit_canhw(CAN_HandleTypeDef *hcan)
{
}

void _canhw_set_filter(CAN_HandleTypeDef *hcan)
{
    static CAN_FilterTypeDef sFilterConfig;

    sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0; // set fifo assignment
    sFilterConfig.FilterIdLow = 0x000;                     // accept all from 0x00
    sFilterConfig.FilterIdHigh = 0x7FF << 5;               // accept all to 0x7FF
    sFilterConfig.FilterMaskIdHigh = 0x000;
    sFilterConfig.FilterMaskIdLow = 0x000;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT; // set filter scale
    sFilterConfig.FilterActivation = ENABLE;

    sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0; // set CAN FILTER to send to FIFO0

    HAL_CAN_ConfigFilter(hcan, &sFilterConfig);
    return;
}

void _canhw_set_irq(CAN_HandleTypeDef *hcan)
{
    HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
}

void _canhw_start(CAN_HandleTypeDef *hcan)
{
    HAL_CAN_Start(hcan);
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

void cb_can_rx(CAN_HandleTypeDef *hcan, canbus_msg *rxmsg, CAN_RxHeaderTypeDef *rxHeader)
{
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, rxHeader, rxmsg->data);
    rxmsg->dlc = rxHeader->DLC;
    rxmsg->id = rxHeader->StdId;
    rxmsg->ts_rx = rxHeader->Timestamp;

    usb_vcptx("rx: ");
    _debug_can_print_msg(rxmsg);
    return;
}

void canhw_send(CAN_HandleTypeDef *hcan, canbus_msg *msg)
{
    static CAN_TxHeaderTypeDef txHeader;
    static uint8_t txData[8];
    static uint32_t txMailbox;

    txHeader.StdId = msg->id;
    txHeader.IDE = CAN_ID_STD;
    txHeader.RTR = CAN_RTR_DATA;
    txHeader.DLC = msg->dlc;
    txHeader.TransmitGlobalTime = ENABLE;
    msg->ts_tx = HAL_GetTick();

    memcpy(txData, msg->data, 8);
//    usb_vcptx("tx: ");
//    _debug_can_print_msg(msg);
    HAL_StatusTypeDef txStatus = HAL_CAN_AddTxMessage(hcan, &txHeader, txData, &txMailbox);

    // if no errors, exit from function
    if (txStatus == HAL_OK)
        return;

    // debug errors if exist
    _debug_canhw_senderror(txStatus);
    return;
}

#endif

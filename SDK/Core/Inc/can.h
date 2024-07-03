
#ifndef INC_CAN_H_
#define INC_CAN_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "canbus.h"
#include "usb.h"

#if defined(STM32F0)
#include "stm32f0xx_hal.h"
void task_canhw(CAN_HandleTypeDef*);
void deinit_canhw(CAN_HandleTypeDef*);
void init_canhw(CAN_HandleTypeDef*);
void _canhw_set_filter(CAN_HandleTypeDef*);
void _canhw_set_irq(CAN_HandleTypeDef*);
void _canhw_start(CAN_HandleTypeDef*);
void cb_can_rx(CAN_HandleTypeDef*, canbus_msg*, CAN_RxHeaderTypeDef*);
void canhw_send(CAN_HandleTypeDef*, canbus_msg*);
#endif

#if defined(STM32H723xx)
#include "stm32h7xx_hal.h"
void task_canhw(FDCAN_HandleTypeDef*);
void deinit_canhw(FDCAN_HandleTypeDef*);
void init_canhw(FDCAN_HandleTypeDef*);
void _canhw_set_filter(FDCAN_HandleTypeDef*);
void _canhw_set_irq(FDCAN_HandleTypeDef*);
void _canhw_start(FDCAN_HandleTypeDef*);
void cb_can_rx(FDCAN_HandleTypeDef*, canbus_msg*, FDCAN_RxHeaderTypeDef*);
void canhw_send(FDCAN_HandleTypeDef*, canbus_msg*);
void __fdcan_init(FDCAN_HandleTypeDef*);
#endif

void _debug_canhw_senderror(HAL_StatusTypeDef);
void _debug_can_print_rxmsg(canbus_msg*);

#endif /* INC_CAN_H_ */

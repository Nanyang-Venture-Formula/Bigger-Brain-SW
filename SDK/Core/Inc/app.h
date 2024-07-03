/* inc copyright */

#ifndef INC_AUTO_APP
#define INC_AUTO_APP

#include "stm32h7xx_hal.h"
#include "nvf_canid.h"

/**
 * auto generate include headers for app
*/
#include "can.h"

void init_app(void);
void deinit_app(void);
void task_app();
void cb_app(canbus_msg*, FDCAN_RxHeaderTypeDef*);
void cb_app1(canbus_msg*, FDCAN_RxHeaderTypeDef*);
void cb_app2(canbus_msg*, FDCAN_RxHeaderTypeDef*);
void cb_app3(canbus_msg*, FDCAN_RxHeaderTypeDef*);

#endif/* INC_AUTO_APP */

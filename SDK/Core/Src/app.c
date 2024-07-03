/* inc copyright */

#include "app.h"
#include "main.h"

/* extern variables */
extern FDCAN_HandleTypeDef hfdcan1;
canbus_msg msg = {.id=0x44, .dlc=8};

void init_app(void)
{
    init_canhw(&hfdcan1);
}

void deinit_app(void)
{
    deinit_canhw(&hfdcan1);
}

void task_app()
{
    task_canhw(&hfdcan1);
}

void cb_app(canbus_msg *msg, FDCAN_RxHeaderTypeDef *rxHeader)

{
}

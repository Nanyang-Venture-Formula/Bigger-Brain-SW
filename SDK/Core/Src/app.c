/* inc copyright */

#include "app.h"
#include "main.h"

/* extern variables */
extern FDCAN_HandleTypeDef hfdcan1;
extern FDCAN_HandleTypeDef hfdcan2;
extern FDCAN_HandleTypeDef hfdcan3;

canbus_msg txmsg1 = {.id=MY_CAN_ID1, .dlc=8};
canbus_msg txmsg2 = {.id=MY_CAN_ID2, .dlc=8};
canbus_msg txmsg3 = {.id=MY_CAN_ID3, .dlc=8};

void init_app(void)
{
    init_canhw(&hfdcan1);
    init_canhw(&hfdcan2);
    init_canhw(&hfdcan3);
}

void deinit_app(void)
{
    deinit_canhw(&hfdcan1);
    deinit_canhw(&hfdcan2);
    deinit_canhw(&hfdcan3);
}

void task_app()
{
    task_canhw(&hfdcan1);
    task_canhw(&hfdcan2);
    task_canhw(&hfdcan3);

    // for vcu
    // heartbeat task
//    task_heartbeat_vcun();
//    task_heartbeat_pdln();
//    task_heartbeat_r2dn();

    // for vcu
    // R2D task
//    task_r2d_r2dn();            // verify if r2d is still active
//    task_r2d_vcun();            // rpt to vcun on r2d status
//    task_r2d_mcn();             // rpt to mcn on r2d status



    // for mcc
    // APPS task
    // task_apps_pdln();           // agg and verify the most viable pdls 
    // task_apps_mcn();            // send torque request

    // MC task

}

void cb_app(canbus_msg *msg, FDCAN_RxHeaderTypeDef *rxHeader)
{
    switch (msg->which_can)
    {
        case 1: cb_app1(msg, rxHeader);
            break;
        case 2: cb_app2(msg, rxHeader);
            break;
        case 3: cb_app3(msg, rxHeader);
            break;
    }   
}

void cb_app1(canbus_msg *msg, FDCAN_RxHeaderTypeDef *rxHeader)
{

}

void cb_app2(canbus_msg *msg, FDCAN_RxHeaderTypeDef *rxHeader)
{

}

void cb_app3(canbus_msg *msg, FDCAN_RxHeaderTypeDef *rxHeader)
{
    
}


#ifndef INC_NVF_CANID_H_
#define INC_NVF_CANID_H_

#define CAN_DATA_IDX_DATA1      0
#define CAN_DATA_IDX_DATA2      1
#define CAN_DATA_IDX_DATA3      2
#define CAN_DATA_IDX_DATA4      3
#define CAN_DATA_IDX_NETWORK    4   /* @ref CAN_network_identifier */
#define CAN_DATA_IDX_MSGTYPE	5   
#define CAN_DATA_IDX_TIME1		6
#define CAN_DATA_IDX_TIME2		7

/**
 * @ref CAN_network_identifier
*/
#define CAN_VCUN				0
#define CAN_R2DN				1
#define CAN_PDLN				2

/**
 * @ref CAN_R2DN Message Ids
*/
#define CAN_R2DN_DASH_STATUS 	0x01
#define CAN_R2DN_VCU_REPORT 	0x02

/**
 * @ref CAN R2DN Node Ids
*/
#define CAN_R2DN_VCU 			0x11
#define CAN_R2DN_DASH 			0x12

#define MY_CAN_ID1				CAN_R2DN_VCU
#define MY_CAN_ID2				CAN_R2DN_VCU
#define MY_CAN_ID3				CAN_R2DN_VCU

#endif /* INC_NVF_CANID_H_ */

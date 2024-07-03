
#include "usb.h"

void usb_vcptx(const char* msg)
{
    CDC_Transmit_HS(msg, strlen(msg));
}


#include "usb.h"

void usb_vcptx(const char* msg)
{
    CDC_Transmit_HS((uint8_t *) msg, strlen(msg));
}

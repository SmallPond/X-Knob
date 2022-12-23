#ifndef __USB_CDC_H__
#define __USB_CDC_H__
#include "USB.h"
#if ARDUINO_USB_CDC_ON_BOOT
#define HWSerial Serial0
#define USBSerial Serial
#else
#define HWSerial Serial
extern USBCDC USBSerial;
#endif

void usb_cdc_init(void);

#endif /* __USB_CDC_H__ */
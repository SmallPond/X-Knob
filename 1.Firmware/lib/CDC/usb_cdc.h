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
struct UsbBuffer_t
{
    uint8_t extraDataTx[32];
    uint8_t rxData[2][60 * 240 * 3 + 32]; // 43232bytes, 43200 of which are lcd buffer
    volatile uint16_t receivedPacketLen = 0;
    volatile uint8_t pingPongIndex = 0;
    volatile uint32_t rxDataOffset = 0;
};

void usb_cdc_init(void);
uint8_t* GetPingPongBufferPtr();
uint8_t* GetLcdBufferPtr();
uint8_t* GetExtraDataRxPtr();
void SwitchPingPongBuffer();
void SendUsbPacket(uint8_t* _data, uint32_t _len);
#endif /* __USB_CDC_H__ */
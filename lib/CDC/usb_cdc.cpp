#include <Arduino.h>
#include "usb_cdc.h"

#if ARDUINO_USB_MODE
#warning This sketch should be used when USB is in OTG mode
#else


#if ARDUINO_USB_CDC_ON_BOOT
#define HWSerial Serial0
#define USBSerial Serial
#else
#define HWSerial Serial
USBCDC USBSerial;
#endif

UsbBuffer_t usbBuffer;

uint8_t* GetPingPongBufferPtr()
{
    return &(usbBuffer.rxData[usbBuffer.pingPongIndex][usbBuffer.rxDataOffset]);
}


void SendUsbPacket(uint8_t* _data, uint32_t _len)
{
    uint8_t ret;
    do
    {
        ret = USBSerial.write(_data, _len);
    } while (ret != 0);
}


void SwitchPingPongBuffer()
{
    usbBuffer.pingPongIndex = (usbBuffer.pingPongIndex == 0 ? 1 : 0);
    usbBuffer.rxDataOffset = 0;
}


uint8_t* GetLcdBufferPtr()
{
    return usbBuffer.rxData[usbBuffer.pingPongIndex == 0 ? 1 : 0];
}

void ReceiveUsbPacketUntilSizeIs(uint32_t _count)
{
    while (usbBuffer.receivedPacketLen != _count);
    usbBuffer.receivedPacketLen = 0;
}

static void usbEventCallback(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data){
    if(event_base == ARDUINO_USB_EVENTS){
        arduino_usb_event_data_t * data = (arduino_usb_event_data_t*)event_data;
        switch (event_id){
            case ARDUINO_USB_STARTED_EVENT:
                HWSerial.println("USB PLUGGED");
                break;
            case ARDUINO_USB_STOPPED_EVENT:
                HWSerial.println("USB UNPLUGGED");
                break;
            case ARDUINO_USB_SUSPEND_EVENT:
                HWSerial.printf("USB SUSPENDED: remote_wakeup_en: %u\n", data->suspend.remote_wakeup_en);
                break;
            case ARDUINO_USB_RESUME_EVENT:
                HWSerial.println("USB RESUMED");
                break;
            
            default:
                break;
        }
    } else if(event_base == ARDUINO_USB_CDC_EVENTS){
        arduino_usb_cdc_event_data_t * data = (arduino_usb_cdc_event_data_t*)event_data;
        switch (event_id){
            case ARDUINO_USB_CDC_CONNECTED_EVENT:
                HWSerial.println("CDC CONNECTED");
                break;
            case ARDUINO_USB_CDC_DISCONNECTED_EVENT:
                HWSerial.println("CDC DISCONNECTED");
                break;
            case ARDUINO_USB_CDC_LINE_STATE_EVENT:
                HWSerial.printf("CDC LINE STATE: dtr: %u, rts: %u\n", data->line_state.dtr, data->line_state.rts);
                break;
            case ARDUINO_USB_CDC_LINE_CODING_EVENT:
                HWSerial.printf("CDC LINE CODING: bit_rate: %u, data_bits: %u, stop_bits: %u, parity: %u\n", data->line_coding.bit_rate, data->line_coding.data_bits, data->line_coding.stop_bits, data->line_coding.parity);
                break;
            case ARDUINO_USB_CDC_RX_EVENT:
                HWSerial.printf("CDC RX [%u]:", data->rx.len);
                {
                    uint8_t buf[data->rx.len];
                    size_t len = USBSerial.read(GetPingPongBufferPtr(), data->rx.len);
                    HWSerial.write(GetPingPongBufferPtr(), len);
                }
                HWSerial.println();
                break;
            case ARDUINO_USB_CDC_RX_OVERFLOW_EVENT:
                HWSerial.printf("CDC RX Overflow of %d bytes", data->rx_overflow.dropped_bytes);
                break;
            
            default:
                break;
        }
    }
}

void usb_cdc_init(void)
{
    USB.onEvent(usbEventCallback);
    USBSerial.onEvent(usbEventCallback);
    USBSerial.begin();
    USB.begin();
    // RxBufferSize 默认 256
    // USBSerial.setRxBufferSize(256)
}

#endif /* ARDUINO_USB_MODE */
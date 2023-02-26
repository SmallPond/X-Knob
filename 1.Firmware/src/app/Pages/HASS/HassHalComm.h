#ifndef __HASS_HAL_COMM_H__
#define __HASS_HAL_COMM_H__

void hass_hal_init(void);
int hass_hal_send(const char *device_name, int knob_value);
#endif /*   __HASS_HAL_COMM_H__ */
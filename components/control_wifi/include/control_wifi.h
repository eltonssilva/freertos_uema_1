#ifndef __CONTROL_WIFI__H__
#define __CONTROL_WIFI__H__


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_event_loop.h"
#include "freertos/event_groups.h"

#include "../../config.h"

//TaskHandle_t taskWifiHandle = NULL;

//void initflexTaskWifi(int sizeTask,  EventGroupHandle_t wifi_event_group);
void flex_init_wifi(EventGroupHandle_t wifi_event_group);
void vTaskWifi(void * parameter);
void wifi_init_sta( void );

#endif  //!__CONTROL_WIFI__H__
#ifndef __CONTROL_MQTT__H__
#define __CONTROL_MQTT__H__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "../../config.h"
 
void vTaskMQTT( void *pvParameters );
void flex_init_mqtt(EventGroupHandle_t wifi_event_group);

 void mqtt_start( void );


#endif  //!__CONTROL_MQTT__H__
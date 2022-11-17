#ifndef __CONFIG__H__
#define __CONFIG__H__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"  

extern EventGroupHandle_t event_group;
char mac_address_root_str[18];
extern char host_name[80]; 

#define CORE_0 0
#define CORE_1 1

#define DEBUG_ETH true
#define DEBUG true

#define WIFI_CONNECTED_BIT  BIT0
#define WIFI_AP_START_BIT   BIT1
#define MQTT_CONNECTED_BIT  BIT2
#define ETHE_CONNECTED_BIT  BIT3

#define WIFI_SSID      "Charlie Brown"
#define WIFI_PASSWORD  "comida05"

#define NET_IP      "192.168.1.70"
#define NET_GW      "192.168.1.1"
#define NET_NM      "255.255.255.0"
#define NET_DNS1    "8.8.8.8"
#define NET_DNS2    "8.8.4.4"

extern esp_netif_t *eth_netif;
QueueHandle_t xQueuePubMQTT;
#define TOPIC_SUBSCRIBE1  "iluflex/rx/3"
#define TOPIC_PUBLISH1  "iluflex/tx/3"

#define MQTT_URL  "mqtt://192.168.10.13"
#define ID_MQTT "007006aa-203a-4fe3-cfdd0c673a"
#define MQTT_USERNAME "autohome"
#define MQTT_PASSWORD "comida05"
#define MQTT_PORTA   "1883"


#endif  //!__CONFIG__H__
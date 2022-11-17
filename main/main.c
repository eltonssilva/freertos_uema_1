#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_vfs.h"
#include "esp_spiffs.h"
#include "nvs_flash.h"
#include "esp_err.h"
#include "esp_log.h"
#include "mdns.h"
#include "lwip/dns.h"
#include "driver/spi_master.h"

#include "http_server.h"


#include "control_wifi.h"
#include "control_mqtt.h"
#include "control_rele.h"

#define TAG	"MAIN"

EventGroupHandle_t event_group;

#define WIFI_CONNECTED_BIT 	BIT0
#define WIFI_FAIL_BIT		BIT1
char host_name[80];

esp_netif_t *eth_netif;

void init_main( void );


void app_main(void)
{
    init_main();

    xTaskCreatePinnedToCore( vTaskWifi, "vTaskWifi", configMINIMAL_STACK_SIZE + 2048, NULL, 4, NULL, CORE_0 );

    xTaskCreatePinnedToCore( vTaskMQTT, "vTaskMQTT", configMINIMAL_STACK_SIZE + 4000, NULL, 5, NULL, CORE_0 );

     xTaskCreatePinnedToCore( vTask_rele, "Task_rele", configMINIMAL_STACK_SIZE + 4000, NULL, 5, NULL, CORE_1 );

}

void init_main( void ){
	// Initialize NVS
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}

	ESP_ERROR_CHECK(ret);

	// Initialize SPIFFS



	uint8_t chipid[20];
	esp_efuse_mac_get_default( chipid );
	snprintf( mac_address_root_str, sizeof( mac_address_root_str ), ""MACSTR"", MAC2STR( chipid ) );
	printf( "MacAddress: %s \n", mac_address_root_str);

	event_group = xEventGroupCreate();

	 esp_event_loop_create_default();

	
	strcpy(host_name, "G4T Online");

}
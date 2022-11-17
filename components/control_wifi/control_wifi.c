#include <stdio.h>
#include "control_wifi.h"
#include <stdbool.h>
#include <string.h>


#include "esp_wifi.h"
#include "esp_event_loop.h"

 #include "freertos/FreeRTOS.h"
 #include "freertos/task.h"
 #include "freertos/event_groups.h"




 #include "esp_log.h"


// #include "esp_system.h"


static esp_err_t event_handler( void *ctx, system_event_t *event );
EventGroupHandle_t _wifi_event_group;
static const char *TAG = "Flex_Wifi: ";


void flex_init_wifi(EventGroupHandle_t wifi_event_group){
  _wifi_event_group = wifi_event_group;
}


void vTaskWifi(void * parameter){




wifi_init_sta();




while (true)
{
  xEventGroupWaitBits(event_group, WIFI_CONNECTED_BIT, false, true, portMAX_DELAY);	

  tcpip_adapter_ip_info_t ip_info;
	  ESP_ERROR_CHECK(tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info));  
  


  vTaskDelay(5000/portTICK_RATE_MS);
}

}

static esp_err_t event_handler( void *ctx, system_event_t *event )
{
    switch( event->event_id ) 
    {
		case SYSTEM_EVENT_STA_START:

		  if( DEBUG )
		  	ESP_LOGI(TAG, "WiFi OK, Init Connect!\r\n");
			
      esp_wifi_connect();
			break;
		case SYSTEM_EVENT_STA_GOT_IP:

		  if( DEBUG ) ESP_LOGI(TAG, "Wifi Connected!\r\n");
			if( DEBUG ) ESP_LOGI(TAG, "got ip:%s",ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));

			xEventGroupSetBits(event_group, WIFI_CONNECTED_BIT);
			break;

		case SYSTEM_EVENT_STA_DISCONNECTED:

		  if( DEBUG ) ESP_LOGI(TAG, "Wifi OffLine!\r\n");

			esp_wifi_connect();
			xEventGroupClearBits( event_group, WIFI_CONNECTED_BIT );
			break;

		default:
			break;
    }
    return ESP_OK;
}

void wifi_init_sta( void )
{
   // tcpip_adapter_init();
    ESP_ERROR_CHECK( esp_event_loop_init( event_handler, NULL ) );

    wifi_init_config_t wifi_cfg = WIFI_INIT_CONFIG_DEFAULT();
    eth_netif =  esp_netif_create_default_wifi_sta();
    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_wifi_init( &wifi_cfg ));
    
    wifi_config_t wifi_config = {0}; // Zero initialize all struct memberes
    wifi_config_t ap_config;

    memset(&ap_config, 0, sizeof(wifi_config_t));
    memcpy((char *)ap_config.ap.ssid,WIFI_SSID, strlen(WIFI_SSID));  
    memcpy((char *)ap_config.ap.password, WIFI_PASSWORD, strlen(WIFI_PASSWORD));
    ap_config.ap.max_connection = 2;


    strcpy((char *)wifi_config.sta.ssid, WIFI_SSID);
    strcpy((char *)wifi_config.sta.password, WIFI_PASSWORD);
  


    

    
    esp_netif_ip_info_t ip_info;





      ESP_ERROR_CHECK( esp_wifi_set_mode( WIFI_MODE_STA ) );
      ESP_ERROR_CHECK( esp_wifi_set_config( ESP_IF_WIFI_STA, &wifi_config ) );

    
    ESP_ERROR_CHECK( esp_wifi_start() );

    if( DEBUG ) ESP_LOGI(TAG, "wifi_init_sta configurada.\r\n");
    
}
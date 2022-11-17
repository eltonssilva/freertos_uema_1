#include <stdio.h>
#include <stdbool.h>
#include "string.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "mqtt_client.h"


#include "esp_log.h"
#include "control_mqtt.h"

static const char *TAG = "Flex_Mqtt: ";


bool mqttConectando = false;


 static esp_mqtt_client_handle_t client;


void vTaskMQTT( void *pvParameters ){

    char code_full[]  = "SRF,1,0,033,038,175";
    EventBits_t uxBits;
    BaseType_t xStatus;
    int delayMultiplicador = 100;

    while (true)
    {
        uxBits = xEventGroupWaitBits(event_group, MQTT_CONNECTED_BIT, false, false, 0);	

        if( (( uxBits & ETHE_CONNECTED_BIT ) != 0) && (( uxBits & MQTT_CONNECTED_BIT ) == 0))
        {
            delayMultiplicador++;
            if((delayMultiplicador >= 100) && !mqttConectando){
            if(DEBUG) ESP_LOGI(TAG, "Wifi Conectado, Agora é o MQTT");
            delayMultiplicador = 0;
            }

            if ( ! mqttConectando ) mqtt_start();
            
        }else if( (( uxBits & ETHE_CONNECTED_BIT ) == 0) && (( uxBits & MQTT_CONNECTED_BIT ) != 0))
        {
            printf("Wifi Desconectado, Desconectar o mqtt\n");
        }else if( (( uxBits & ETHE_CONNECTED_BIT ) != 0) && (( uxBits & MQTT_CONNECTED_BIT ) != 0))
        {
            xStatus = xQueueReceive( xQueuePubMQTT, &code_full, 0 );
            if( xStatus == pdPASS ){

            printf("code_full %s\n", code_full);
            esp_mqtt_client_publish(client, TOPIC_PUBLISH1, code_full, strlen( code_full ), 0, 0);
            }
        }

        vTaskDelay(10/portTICK_RATE_MS);
    }


}

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
    client = event->client;
    
    switch ( event->event_id ) 
    {
      case MQTT_EVENT_BEFORE_CONNECT: 
        if( DEBUG ) ESP_LOGI(TAG, "MQTT Conectando");
        mqttConectando = true;   
      break;
          
      case MQTT_EVENT_CONNECTED:
        if( DEBUG ) ESP_LOGI( TAG, "MQTT Conectado" );
          printf("MQTT Conectado");
          // Subscreve ao topico
          esp_mqtt_client_subscribe( client, TOPIC_SUBSCRIBE1, 0 );
          // Seta bit do grupo mqtt conectado
          xEventGroupSetBits( event_group, MQTT_CONNECTED_BIT );
        break;

      case MQTT_EVENT_DISCONNECTED:
        if(DEBUG) ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
          // Seta bit do grupo mqtt disconectado
        //  mqttConectando = false;  
          xEventGroupClearBits( event_group, MQTT_CONNECTED_BIT );
        break;

      case MQTT_EVENT_SUBSCRIBED:   
        if( DEBUG ) ESP_LOGI( TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id );
      break;

      case MQTT_EVENT_UNSUBSCRIBED:           
        if( DEBUG ) ESP_LOGI( TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id );
        break;

      case MQTT_EVENT_PUBLISHED:            
        if( DEBUG )ESP_LOGI( TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id );
        break;

      case MQTT_EVENT_DATA:

        /**
        * Se este case for chamado significa que alguma mensagem foi recebida pelo ESP32
        * ou seja, alguma mensagem foi publicada no tópico 'TOPIC_SUBSCRIBE';
        */
        if( DEBUG )
        {
          ESP_LOGI( TAG, "MQTT_EVENT_DATA" ); 
          /**
          * Imprime o nome do tópico no qual a mensagem foi publicada.
          */
          ESP_LOGI( TAG, "Topico = %.*s\r\n", event->topic_len, event->topic );
          /**
          * Imprime o conteúdo da mensagem recebida via MQTT;
          */
          ESP_LOGI( TAG, "Data = %.*s\r\n", event->data_len, event->data );              
        }  
          


      break;

      case MQTT_EVENT_ERROR:
        if( DEBUG ) ESP_LOGI( TAG, "MQTT_EVENT_ERROR" );
      break;
		
      case MQTT_EVENT_ANY: 
			  if( DEBUG ) ESP_LOGI(TAG, "MQTT_EVENT_ANY");
			break;

      default :
        if( DEBUG ) ESP_LOGI(TAG, "OUTRO EVENTO");
    }

    return ESP_OK;
}


static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
	if( DEBUG ) ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(event_data);
}


void mqtt_start( void )
{

    if( DEBUG ) ESP_LOGI( TAG, "chamado mqtt_start()\n" );
  

    esp_mqtt_client_config_t mqtt_cfg = {0}; // Zero initialize all struct memberes


    if( DEBUG ) ESP_LOGI( TAG, "Usando dados padrões\n" );
    mqtt_cfg.uri = MQTT_URL;
    mqtt_cfg.username = MQTT_USERNAME;
    mqtt_cfg.password = MQTT_PASSWORD;
   
    
    mqtt_cfg.client_id = ID_MQTT;


    /**
     * Carrega configuração do descritor e inicializa stack MQTT;
     */
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}


void mqtt_stop( void )
{
    if( DEBUG ) ESP_LOGI( TAG, "mqtt_stop()" );
    
   esp_mqtt_client_stop( client );
   esp_mqtt_client_destroy( client );
}





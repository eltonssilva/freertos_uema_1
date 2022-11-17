#include <stdio.h>
#include "control_switch.h"

void vTask(void *pvParameters)
{

int switch_1 = 21;
int switch_2 = 22;
int switch_3 = 23;
int switch_4 = 24;

gpio_pad_select_gpio( switch_1 );
gpio_pad_select_gpio( switch_2 );
gpio_pad_select_gpio( switch_3 );
gpio_pad_select_gpio( switch_4 );

gpio_set_direction( switch_1, GPIO_MODE_INPUT ); 
gpio_set_direction( switch_2, GPIO_MODE_INPUT );
gpio_set_direction( switch_3, GPIO_MODE_INPUT );
gpio_set_direction( switch_4, GPIO_MODE_INPUT );

bool status_input1 = false;
bool status_input1 = false;
bool status_input1 = false;
bool status_input1 = false;

bool status_rele1 = false;
bool status_rele2 = false;
bool status_rele3 = false;
bool status_rele4 = false;

info_rele_t info_status_rele;

    while(true){

        status_input1 = gpio_get_level( switch_1 ); 
        status_input2 = gpio_get_level( switch_1 ); 
        status_input3 = gpio_get_level( switch_1 ); 
        status_input4 = gpio_get_level( switch_1 );

        if(status_input1){
            status_rele1 = !status_rele1;
            info_status_rele.rele = 1
            info_status_rele.value = status_rele1
            xQueueSend( xQueue_switch, &info_status_rele, portMAX_DELAY);
        }
        
        if(status_input2){
            status_rele2 = !status_rele2;
            info_status_rele.rele = 2
            info_status_rele.value = status_rele2
            xQueueSend( xQueue_switch, &info_status_rele, portMAX_DELAY);
        }


        vTaskDelay( 250 / portTICK_PERIOD_MS );
    }

}
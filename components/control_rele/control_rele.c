#include <stdio.h>
#include "control_rele.h"

void vTask_rele(void *pvParameters)
{

info_rele_t info_status_rele;
int rele;
bool value;

    while(true){

        BaseType_t xStatus;
        xQueueReceive( xQueue_switch, &info_status_rele, portMAX_DELAY );

        rele = info_status_rele.rele;
        value = info_status_rele.value;

        switch (rele)
        {
        case 1;
            gpio_set_level( RELAY_1, value);
            break;
        case 2;
            gpio_set_level( RELAY_2, value);
            break;
        
        case 3;
            gpio_set_level( RELAY_3, value);
            break;

        case 4;
            gpio_set_level( RELAY_4, value);
            break;
        
        default:
            break;
        }
      
    
    }
}

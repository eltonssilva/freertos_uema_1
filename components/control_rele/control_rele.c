#include <stdio.h>
#include "control_rele.h"

void vTask_rele(void *pvParameters)
{



    while(true){

        printf("Estou Vivo kkkkk\n");
        vTaskDelay( 2000 / portTICK_PERIOD_MS );
    }
}

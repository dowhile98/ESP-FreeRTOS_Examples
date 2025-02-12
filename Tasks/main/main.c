/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
/*Includes ---------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_log.h"

/*Defines -----------------------------------------------------------------------------------------------------*/

/*Global variables --------------------------------------------------------------------------------------------*/
static const char *TAG = "main";

TaskHandle_t task1;
TaskHandle_t task2;
TaskHandle_t task3;

float temp1, temp2;
/*Function prototype ------------------------------------------------------------------------------------------*/
void vTaskExample1(void *params);
void vTaskExample2(void *params);

/*Entry point -------------------------------------------------------------------------------------------------*/
void app_main(void)
{
    portBASE_TYPE status;

    status = xTaskCreate(vTaskExample1, "Tarea 1", 2048,NULL, 10, &task1);
    //verificacion
    if(status != pdPASS){
        ESP_LOGE(TAG, "Error al crear tarea 1");
    }

    status = xTaskCreate(vTaskExample2, "Tarea 2", 2048,&temp1, 10, &task2);
    //verificacion
    if(status != pdPASS){
        ESP_LOGE(TAG, "Error al crear tarea 2");
    }

    status = xTaskCreate(vTaskExample2, "Tarea 3", 1024,&temp2, 12, &task3);
    //verificacion
    if(status != pdPASS){
        ESP_LOGE(TAG, "Error al crear tarea 3");
    }

    for(;;){
        //code

    }
}

/*Function defintion -----------------------------------------------------------------------------------------*/
void vTaskExample1(void *params){
    //Local variables 
    uint32_t lastTemp = xTaskGetTickCount();
    //init 
    ESP_LOGI(TAG, "Tarea 1 iniciado");
    //loop
    for(;;){
        //vTaskDelay(pdMS_TO_TICKS(1000));
        //code
        //vTaskDelay(pdMS_TO_TICKS(1000));       //realmente genera un retardo de 1seg?

        //millis()->arduino
        //HAL_GetTick
        TickType_t
        if((xTaskGetTickCount() - lastTemp)>= pdMS_TO_TICKS(2000)){//2seg
            lastTemp = xTaskGetTickCount(); //4100
            //freq
            
        }
        //code





    }
}

void isr(void){
    //semafaro entregas
}


void vTaskExample2(void *params){
    //Local variables 
    char num = 10;
    float *temp = (float *)params;
    //init 
    char *name;

    name =  pcTaskGetName(NULL);

    ESP_LOGI(TAG, "%s iniciado", name);
    //loop
    for(;;){

        //vTaskDelay(pdMS_TO_TICKS(100));

        //elimiaar tarea 1
    }

    //terminar una tarea
    vTaskDelete(NULL);
}
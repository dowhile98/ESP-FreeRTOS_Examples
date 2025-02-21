/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_log.h"
/*Global variables ---------------------------------------------------------------------------*/
SemaphoreHandle_t sem1;
static const char *TAG = "main";
/*Entry points -------------------------------------------------------------------------------*/


void taskA_entry(void *args);
void taskB_entry(void *args);
void taskC_entry(void *args);

void app_main(void)
{
    sem1 = xSemaphoreCreateBinary(); 
    if(sem1 == NULL){
        ESP_LOGE(TAG, "Error al crear el semaphore");
    }
    
    xTaskCreate(taskA_entry, "Task A", 4096, NULL, 5, NULL);
    xTaskCreate(taskB_entry, "Task B", 4096, NULL, 5, NULL);
    xTaskCreate(taskB_entry, "Task C", 4096, NULL, 5, NULL);

}


void taskA_entry(void *args){
    for(;;){

        xSemaphoreGive(sem1);

        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}
void taskB_entry(void *args){

    xSemaphoreTake(sem1, portMAX_DELAY);

    for(;;){
        if(xSemaphoreTake(sem1, pdMS_TO_TICKS(500) == pdPASS)){
            //
            ESP_LOGI(TAG, "Semaforo recibido");
            

        }else{
            ESP_LOGE(TAG, "Semaforo NO recibido");
        }
    }
}
void taskC_entry(void *args){

    for(;;){
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
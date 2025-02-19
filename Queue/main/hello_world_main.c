/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
/*Includes --------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_log.h"


/*Defines ----------------------------------------------------------------------------------------------*/


/*Global variables -------------------------------------------------------------------------------------*/
TaskHandle_t taskA;
TaskHandle_t taskB;

static const char *TAG = "main";

QueueHandle_t queue1;
/*Task entry points ------------------------------------------------------------------------------------*/
void taskA_entry(void *args);
void taskB_entry(void *args);


void app_main(void)
{
    portBASE_TYPE status;


    /*create queue*/
    queue1 = xQueueCreate(10/*cantidad de items*/, sizeof(uint8_t)/*cuntos bytes tiene cada item*/);
    if(queue1 == NULL){
        ESP_LOGE(TAG, "Error al crear el queue");
    } 
    /*Task create*/
    status = xTaskCreate(taskA_entry, "Task A", 4096, NULL, 5, &taskA);
    if(status != pdPASS){
        ESP_LOGE(TAG, "Error al crear tarea A");
    }
    status = xTaskCreate(taskB_entry, "Task B", 4096, NULL, 5, &taskB);
    if(status != pdPASS){
        ESP_LOGE(TAG, "Error al crear tarea B");
    }

    
}

/*Entry points definitions ----------------------------------------------------------------------------*/
void taskA_entry(void *args){
    uint8_t msgCount = 0;
    ESP_LOGI(TAG, "Tarea que envia el queue");
    for(;;){
        //send
        if(xQueueSend(queue1, (void *)&msgCount, pdMS_TO_TICKS(200) /*Tiempo de espera en cas se detecte queue lleno*/) != pdPASS){
            ESP_LOGE(TAG, "Error al enviar mensaje");
        }else{
            ESP_LOGI(TAG, "Mensaje enviado");
        }

        msgCount += 1;
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
void taskB_entry(void *args){
    uint8_t msgRcv;
    ESP_LOGI(TAG, "Tarea que recibe el queue");
    for(;;){

        if(xQueueReceive(queue1, (void *)&msgRcv, pdMS_TO_TICKS(100)) != pdPASS){
            ESP_LOGE(TAG, "Error al recibir el mensaje");
        }else{
            ESP_LOGI(TAG, "MSG: %u", msgRcv);
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
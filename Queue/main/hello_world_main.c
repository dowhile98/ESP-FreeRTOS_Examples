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
#include "string.h"

#define example 2
/*Defines ----------------------------------------------------------------------------------------------*/

/*Typedefs ---------------------------------------------------------------------------------------------*/
typedef struct QueueMsg{
    uint8_t cmd;
    uint32_t delay_ms;
    //texto
    char str[32];
}QueueMsg_t;


/*Global variables -------------------------------------------------------------------------------------*/
TaskHandle_t taskA;
TaskHandle_t taskB;

static const char *TAG = "main";

QueueHandle_t queue1;

/*Function prototype -----------------------------------------------------------------------------------*/
BaseType_t buzzer_cmd(uint8_t cmd, uint32_t delay, char *str);
/*Task entry points ------------------------------------------------------------------------------------*/
void taskA_entry(void *args);
void taskB_entry(void *args);


void app_main(void)
{
    portBASE_TYPE status;


    /*create queue*/
#if (example == 1)
    queue1 = xQueueCreate(10/*cantidad de items*/, sizeof(uint8_t)/*cuntos bytes tiene cada item*/);
#elif (example == 2)
    queue1 = xQueueCreate(10/*cantidad de items*/, sizeof(QueueMsg_t)/*cuntos bytes tiene cada item*/);
#endif
    
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

    //alto el pin
    //wait 100ms
    //bajo el pin
    
}

/*Entry points definitions ----------------------------------------------------------------------------*/
#if (example == 1)
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
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
void taskB_entry(void *args){
    uint8_t msgRcv;
    ESP_LOGI(TAG, "Tarea que recibe el queue");
    for(;;){

        if(xQueueReceive(queue1, (void *)&msgRcv, portMAX_DELAY) != pdPASS){
            ESP_LOGE(TAG, "Error al recibir el mensaje");
        }else{
            ESP_LOGI(TAG, "MSG: %u", msgRcv);
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

#elif (example == 2)
void taskA_entry(void *args){

    for(;;){
        buzzer_cmd(1, 500, "on");

        vTaskDelay(pdMS_TO_TICKS(3000));//tarea ingresa a estado bloqueado: no usa el tiempo de la cpu
    }
}
void taskB_entry(void *args){
    QueueMsg_t msg;

    ESP_LOGI(TAG, "Tarea iniciada");
    for(;;){

        xQueueReceive(queue1, (void *)&msg, portMAX_DELAY); //bloquea
        ESP_LOGI(TAG, "\r\n-----------------------------------------------------");
        ESP_LOGI(TAG, "MSG TEXT: %s", msg.str);

        switch (msg.cmd)
        {
        case 0:
            
            /* code */
            break;
        case 1: //sound
            //pin en alto
            ESP_LOGI(TAG, "buzzer on");
            //delay
            vTaskDelay(pdMS_TO_TICKS(msg.delay_ms));
            //pin en bajo
            ESP_LOGI(TAG, "buzzer off");
        default:
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

/*Function definition -----------------------------------------------------------------------------*/
BaseType_t buzzer_cmd(uint8_t cmd, uint32_t delay, char *str){
    BaseType_t ret = 0;
    QueueMsg_t msg;

    //se define el mensaje
    msg.cmd = cmd;
    msg.delay_ms = delay;
    
    strcpy(msg.str, str);
    //se envia el mensaje
    ret = xQueueSend(queue1, (void *)&msg, portMAX_DELAY);
    if(ret != pdPASS){
        ESP_LOGE(TAG, "Error al enviar mensaje a la tarea que maneja el buzzer");
    }

    return ret;
}
#endif
/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
/*Includes ---------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_log.h"
#include "main.h"
/*Defines -----------------------------------------------------------------------------------------------------*/
#define APP_BUTTON_SW1  4
#define APP_BUTTON_SW2  5
/*Global variables --------------------------------------------------------------------------------------------*/
static const char *TAG = "main";
app_button_t sw1;
app_button_t sw2;

TaskHandle_t sw1_task;
TaskHandle_t sw2_task;
/*Function prototype ------------------------------------------------------------------------------------------*/


/*Entry point -------------------------------------------------------------------------------------------------*/
void app_main(void)
{
    portBASE_TYPE status;
    //init
    sw1.config.mode = GPIO_MODE_INPUT;
    sw1.config.pin_bit_mask = 1<<APP_BUTTON_SW1;
    sw1.config.pull_up_en = GPIO_PULLUP_ENABLE;
    sw1.config.pull_down_en = 0;
    sw1.config.intr_type = 0;
    sw1.pin = APP_BUTTON_SW1;

    
    memcpy(&sw2, &sw1, sizeof(app_button_t));
    sw2.config.pin_bit_mask = 1<<APP_BUTTON_SW2;
    sw2.pin = APP_BUTTON_SW2;
    //sw1
    status = xTaskCreate(button_task_read_entry, "sw1", 4096, (void * const)&sw1, 3, &sw1_task);
    if(status != pdPASS){
        ESP_LOGE(TAG, "Error al crear tarea sw1");
    }
    //sw2
    status = xTaskCreate(button_task_read_entry, "sw2", 4096, (void * const)&sw2, 3, &sw2_task);
    if(status != pdPASS){
        ESP_LOGE(TAG, "Error al crear tarea sw1");
    }
    for(;;){

        //codigo
    }   
}

/*Function defintion -----------------------------------------------------------------------------------------*/





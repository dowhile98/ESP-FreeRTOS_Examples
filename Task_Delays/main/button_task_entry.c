/**
 * @file button_task_entry.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-02-11
 *
 * @copyright Copyright (c) 2025
 *
 */

/*Includes ------------------------------------------------------------------------------------------------ */
#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "main.h"
/**
 * 1. free
 * 2. Debounce
 * 3. onpress
 * 4. keepalive
 * 5. release
 * 
 */
/*Defines -------------------------------------------------------------------------------------------------*/
typedef enum{
    BUTTON_FREE = 0,
    BUTTON_DEBOUNCE,
    BUTTON_ONPRESS,
    BUTTON_KEEPALIVE,
    BUTTON_ONRELEASE,
}APP_Button_state_t;
/*global variables ----------------------------------------------------------------------------------------*/

/*Function prototype -------------------------------------------------------------------------------------*/

/*Task definition ----------------------------------------------------------------------------------------*/
void button_task_read_entry(void *params){
    /*Local variables*/
    app_button_t *obj;
    uint8_t state = BUTTON_FREE;
    uint32_t ticks = 0;
    obj = (app_button_t *)params;
    /*Init */
    gpio_config(&obj->config);

    for(;;){
        //state machine
        switch (state)
        {
        case BUTTON_FREE:
            if(gpio_get_level(obj->pin) == 0){
                vTaskDelay(pdMS_TO_TICKS(20));          //wait 20ms
                state = BUTTON_DEBOUNCE;
            }
            /* code */
            break;
        case BUTTON_DEBOUNCE:
            if(gpio_get_level(obj->pin) == 0){
                //presion valida

                //generate event (on press)

                //save ticks
                ticks = xTaskGetTickCount();

                state = BUTTON_KEEPALIVE;
            }else{
                state = BUTTON_FREE;
            }
        case BUTTON_KEEPALIVE:
            if(gpio_get_level(obj->pin) == 1){
                //on release

                state = BUTTON_FREE;
            }
            else if((xTaskGetTickCount() - ticks)>=pdMS_TO_TICKS(100)){
                ticks = xTaskGetTickCount();

                //generate event

            }
            break;
        
        default:
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}
/*Function definition -----------------------------------------------------------------------------------*/
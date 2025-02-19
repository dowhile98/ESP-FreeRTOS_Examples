#ifndef MAIN_H
#define MAIN_H

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
#include "freertos/event_groups.h"
typedef struct  app_button
{
    gpio_config_t config;
    uint8_t pin;
    uint8_t state;
    //event
    EventGroupHandle_t event;
}app_button_t;

/*Task entry points prototype ---------------------------------------------------------------------------------*/
void button_task_read_entry(void *params);
#endif
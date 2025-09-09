#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "../../include/globals.h"

static const char* TAG = "MAIN";
#include "usb_console.h"

void app_main(void)
{
    // Initialize NVS (needed by WiFi, BT, etc.)
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "USB Serial/JTAG console example started");
    printf("Type something in idf.py monitor...\n");

    // Start key receiver task
    xTaskCreate(usb_console_receive_task, "usb_console_receive_task", TaskStack2K + 512, NULL, tskIDLE_PRIORITY, NULL);
    // xTaskCreate(DS18B20_Task, "DS18B20_Task", (TaskStack3K), NULL, tskIDLE_PRIORITY, NULL);

    // Main loop (1 second timer)
    uint32_t counter = 0;
    TickType_t last_wake = xTaskGetTickCount();
    while (1) {
        ESP_LOGI(TAG, "Main loop tick %lu", counter++);
        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(100));
    }
}

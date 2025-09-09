#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"

static const char *TAG = "MAIN";

static void receive_task(void *arg)
{
    while (1) {
        int c = getchar();  // blocking read from USB Serial/JTAG
        if (c != EOF) {
            ESP_LOGI(TAG, "You pressed: '%c' (ASCII %d)", c, c);
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

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
    xTaskCreate(receive_task, "receive_task", 4096, NULL, 5, NULL);

    // Main loop (1 second timer)
    uint32_t counter = 0;
    TickType_t last_wake = xTaskGetTickCount();
    while (1) {
        ESP_LOGI(TAG, "Main loop tick %lu", counter++);
        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(3000));
    }
}

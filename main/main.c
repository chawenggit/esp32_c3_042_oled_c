#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include <string.h>

static const char *TAG = "MAIN";

#define CMD_BUF_LEN 128

static void receive_task(void *arg)
{
    char buf[CMD_BUF_LEN];
    size_t idx = 0;

    while (1) {
        int c = getchar();  // blocking read from USB Serial/JTAG
        if (c != EOF) {
            ESP_LOGI(TAG, "'%c'", c);
            // printf("=%c", c);
            if (c == '\n') {
                // End of line
                if (idx > 0) {
                    buf[idx] = '\0';  // null terminate
                    ESP_LOGI(TAG, "Received command: \"%s\"", buf);

                    // Example: simple command handling
                    if (strcmp(buf, "hello") == 0) {
                        ESP_LOGI(TAG, "Hi there!");
                    } else if (strcmp(buf, "led on") == 0) {
                        ESP_LOGI(TAG, "Turning LED ON...");
                        // gpio_set_level(GPIO_NUM_X, 1);
                    } else if (strcmp(buf, "led off") == 0) {
                        ESP_LOGI(TAG, "Turning LED OFF...");
                        // gpio_set_level(GPIO_NUM_X, 0);
                    } else {
                        ESP_LOGW(TAG, "Unknown command: %s", buf);
                    }
                }
                idx = 0; // reset for next line
            } else if (idx < CMD_BUF_LEN - 1) {
                buf[idx++] = (char)c;
            } else {
                ESP_LOGW(TAG, "Command too long, clearing buffer");
                idx = 0;
            }
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
        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(10000));
    }
}

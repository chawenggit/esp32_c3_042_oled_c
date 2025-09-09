#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "../../include/globals.h"

static const char *TAG = "MAIN";

#define CMD_BUF_LEN 128

static void receive_task(void *arg)
{
    char buf[CMD_BUF_LEN];
    size_t idx = 0;
    const gpio_num_t LED1_GPIO = LED_P; // example LED pin, change as needed

    // Configure LED pin as output
    gpio_reset_pin(LED1_GPIO);
    gpio_set_direction(LED1_GPIO, GPIO_MODE_OUTPUT);

    while (1) {
        int c = getchar();  // blocking read from USB Serial/JTAG
        if (c != EOF) {
            ESP_LOGI(TAG, "'%c'", c);
            // Echo character if you want: putchar(c);

            if (c == '\n' || c == '\r') {
                // End of line
                if (idx > 0) {
                    buf[idx] = '\0';  // null terminate
                    ESP_LOGI(TAG, "Received command: \"%s\"", buf);

                    // Parse LED command
                    if (strncmp(buf, "led1=", 5) == 0) {
                        if (buf[5] == '1') {
                            gpio_set_level(LED1_GPIO, 1);
                            ESP_LOGI(TAG, "LED1 turned ON");
                        } else if (buf[5] == '0') {
                            gpio_set_level(LED1_GPIO, 0);
                            ESP_LOGI(TAG, "LED1 turned OFF");
                        } else {
                            ESP_LOGW(TAG, "Invalid LED value: %c", buf[5]);
                        }
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

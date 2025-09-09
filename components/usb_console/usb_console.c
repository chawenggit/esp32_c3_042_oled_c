#include <string.h>
#include "../../include/globals.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

static const char *TAG = "USB_CONSOLE";

#define CMD_BUF_LEN 128

void usb_console_receive_task(void *arg)
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
            ESP_LOGI(TAG, "'%c' STACK FREE =%ld Bytes", c, (unsigned long int)uxTaskGetStackHighWaterMark(NULL));

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

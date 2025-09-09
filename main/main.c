#include "../../include/globals.h"

static const char* firmware_verion = "NODE 0.0.1";
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "main1.h"

void app_main()
{
    system_info();
    // initArduino();
    // pinMode(4, OUTPUT);
    // digitalWrite(4, HIGH);
    // // Do your own thing
    // init_nvs();
    // wifi_connection();
    // gpio_reset_pin(LED_P);
    // gpio_set_direction(LED_P, GPIO_MODE_OUTPUT);

    // gpio_config_t pwr_gpio_config = {};
    // pwr_gpio_config.mode = GPIO_MODE_OUTPUT;
    // pwr_gpio_config.pin_bit_mask = 1ULL << LED_P;
    // gpio_config(&pwr_gpio_config);

    // ESP_LOGW("INFO", "--------START");
    // init_spiffs();
    // // initialize_sntp();
    // // wait_for_time_sync();
    // ESP_LOGW("INFO", "------END");

    // // Blink loop
    // // bool blink = true;
    // xTaskCreate(task_test_SSD1306i2c, "task_test_SSD1306i2c", TaskStack4K, NULL, Priority1, NULL);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while (1) {
        // gpio_set_level(LED_P, 0);
        // vTaskDelay(10 / portTICK_PERIOD_MS); // Delay 1 second
        // gpio_set_level(LED_P, 1);

        vTaskDelayUntil(&xLastWakeTime, (1000 / portTICK_PERIOD_MS));
    }
}
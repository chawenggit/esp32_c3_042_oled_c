#include "../../include/globals.h"

static const char* firmware_verion = "NODE 0.0.1";
static const char* TAG = "MAIN";
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include "driver/usb_serial_jtag.h"
#include "driver/usb_serial_jtag_vfs.h"
// #include "esp_vfs_usb_serial_jtag.h"
#include <fcntl.h> // for fcntl()
#include "esp_vfs_dev.h"
#include "nvs_flash.h"
#include <fcntl.h>
// #include <cstdint>
// #include <cstdio>
#include "main1.h"

static void initUart()
{

    /* Disable buffering on stdin */
    setvbuf(stdin, NULL, _IONBF, 0);

    // Minicom, screen, idf_monitor send CR when ENTER key is pressed
    usb_serial_jtag_vfs_set_rx_line_endings(ESP_LINE_ENDINGS_CR);
    // Move the caret to the beginning of the next line on '\n'
    usb_serial_jtag_vfs_set_tx_line_endings(ESP_LINE_ENDINGS_CRLF);

    // Enable non-blocking mode on stdin and stdout
    fcntl(fileno(stdout), F_SETFL, 0);
    fcntl(fileno(stdin), F_SETFL, 0);

    usb_serial_jtag_driver_config_t usb_serial_jtag_config = {0};
    usb_serial_jtag_config.rx_buffer_size = 512;
    usb_serial_jtag_config.tx_buffer_size = 512;

    esp_err_t ret = ESP_OK;
    // Install USB-SERIAL-JTAG driver for interrupt-driven reads and writes
    ret = usb_serial_jtag_driver_install(&usb_serial_jtag_config);
    if (ret != ESP_OK) {
        return;
    }

    /* Tell vfs to use usb-serial-jtag driver */
    usb_serial_jtag_vfs_use_driver();
}

static void Receive_callback(void* argument)
{
    for (;;) {
        // uint8_t ch;
        // ch = fgetc(stdin);
        // if (ch != 0xFF) {
        //     // fputc(ch, stdout);
        //     printf("char received = %c \n", ch);
        // }
        int c = getchar();  // Blocking read from USB Serial/JTAG
        if (c != EOF) {
            ESP_LOGI(TAG, "You pressed: '%c' (ASCII %d)", c, c);
        }
        // ESP_LOGI(TAG, "show: '%c' (ASCII %d)", c, c);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    system_info();

    esp_err_t ret;
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    printf("hello world before init Uart\n");

    initUart();

    printf("hello world after init Uart\n");

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

    uint8_t looptimer = 0;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    static uint8_t timer1S = 0;
    ESP_LOGI(TAG, "----Main Start LOOP------");

    xTaskCreate(Receive_callback, "UART receive callback", 4096, NULL, 5, NULL); // receiving commands from main uart

    // xTaskCreate(UART0_task, "UART0_task", TaskStack4K, NULL, Priority1, NULL);
    while (1) { // 10 ms loop
        // uint8_t wr_val = (looptimer << 4) | 15;
        if (timer1S++ >= T1S) { // 1 sec loop
            timer1S = 0;

            looptimer++;
            ESP_LOGI(TAG, "Timer=%d STACK=%ld", looptimer, (unsigned long int)uxTaskGetStackHighWaterMark(NULL));
        }

        vTaskDelayUntil(&xLastWakeTime, (pdMS_TO_TICKS(MainLoopT)));
    }
}
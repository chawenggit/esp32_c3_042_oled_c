#include "../../include/globals.h"
#include <stdio.h>
#include <esp_log.h>
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_private/esp_clk.h"
#include "esp_psram.h" // <-- Add this at the top of your file
#include "esp_heap_caps.h" // for heap_caps_malloc if you use it
#include "esp_system.h"

#include "driver/uart.h"

static const char* TAG = "MAIN1";
void system_info()
{
    ESP_LOGI(TAG, "----------- SYS INFO --------");
    /* Print chip information */
    esp_chip_info_t chip_info;
    uint32_t flash_size;
    esp_chip_info(&chip_info);
    ESP_LOGI(TAG, "This is %s chip with %d CPU core(s), WiFi%s%s, ",
        CONFIG_IDF_TARGET,
        chip_info.cores,
        (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
        (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    unsigned major_rev = chip_info.revision / 100;
    unsigned minor_rev = chip_info.revision % 100;
    ESP_LOGI(TAG, "silicon revision v%d.%d, ", major_rev, minor_rev);
    if (esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
        ESP_LOGE(TAG, "Get flash size failed");
        return;
    }
    ESP_LOGI(TAG, "CPU Frequency: %d MHz", esp_clk_cpu_freq() / 1000000);

    ESP_LOGI(TAG, "%" PRIu32 "MB %s flash", flash_size / (uint32_t)(1024 * 1024),
        (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
    // PSRAM C3 not support PSRAM
    // if (esp_psram_is_initialized()) {
    //     size_t psram_size = esp_psram_get_size();
    //     ESP_LOGI(TAG, "PSRAM size: %d bytes", psram_size);

    // } else {
    //     ESP_LOGE(TAG, "No PSRAM detected.");
    // }

    ESP_LOGI(TAG, "Minimum free heap size: %" PRIu32 " bytes", esp_get_minimum_free_heap_size());

    ESP_LOGI(TAG, "Free Heap Size: %d bytes", heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
    ESP_LOGI(TAG, "Free PSRAM Size: %d bytes", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
    ESP_LOGI(TAG, "----------- SYS INFO END --------"); // todo why uncomment this , screen fail
}


#define UART0_COMMAND_LINE_MAX_SIZE 1024
void UART0_setup() {
    uart_config_t uart_config = {
        .baud_rate           = 115200,
        .data_bits           = UART_DATA_8_BITS,
        .parity              = UART_PARITY_DISABLE,
        .stop_bits           = UART_STOP_BITS_1,
        .flow_ctrl           = UART_HW_FLOWCTRL_DISABLE,
        //.rx_flow_ctrl_thresh = 122,
        //.use_ref_tick        = false,
    };

    // Configure UART parameters
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_0, &uart_config));
    // Set UART pins(TX: IO4, RX: IO5, RTS: IO18, CTS: IO19)
    //ESP_ERROR_CHECK(uart_set_pin(UART_NUM_0, 13, 26, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_0, UART0_COMMAND_LINE_MAX_SIZE, 0, 0, NULL, 0));
}


void UART0_task(void *argument)
{
	// UART0_setup();

  	// char command_line[UART0_COMMAND_LINE_MAX_SIZE];
  	for (;;)
	{	
        int c = getchar(); 
        putchar(c);
        if (c != -1)
            ESP_LOGI("UART", "Got key: %c (ASCII %d)", c, c);



		// int len = uart_read_bytes(UART_NUM_0, command_line, (UART0_COMMAND_LINE_MAX_SIZE - 1), 200 / portTICK_PERIOD_MS);
        // if (len) {
        //     command_line[len] = 0;
        //     ESP_LOGI("UART", "Got key: %c (ASCII %d) %s", command_line[0], command_line[0], command_line);
		// 	// ParseSystemCmd(command_line, len); // Line is complete. Execute it!
		// 	// memset(&command_line, 0, sizeof(command_line));
        // }
        // else{
        //     ESP_LOGI("UART", "Got no key:");
            
        // }
		vTaskDelay(1000/portTICK_PERIOD_MS);

  }

}
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mn12864k.h"

void vfd_draw(void) {
    for (;;) {
        for (uint8_t y = 0; y <= 63; y++) {
            for (uint8_t x = 0; x <= 127; x++) {
                vfd_draw_pixel(x, y, 1);
                vTaskDelay(20 / portTICK_PERIOD_MS);
            }
        }
        for (uint8_t y = 0; y <= 63; y++) {
            for (uint8_t x = 0; x <= 127; x++) {
                vfd_draw_pixel(x, y, 0);
                vTaskDelay(20 / portTICK_PERIOD_MS);
            }
        }
        /*vfd_draw_pixel(0, 0, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        vfd_draw_pixel(0, 31, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        vfd_draw_pixel(127, 0, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        vfd_draw_pixel(127, 31, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        vfd_draw_pixel(0, 0, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        vfd_draw_pixel(0, 31, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        vfd_draw_pixel(127, 0, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        vfd_draw_pixel(127, 31, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);*/
    }
}

void app_main(void) {
    vfd_start_send();
    vTaskDelay(100 / portTICK_PERIOD_MS);
    esp_rom_gpio_pad_select_gpio(HVCTR);
    esp_rom_gpio_pad_select_gpio(FLCTR);
    gpio_set_direction(HVCTR, GPIO_MODE_OUTPUT);
    gpio_set_direction(FLCTR, GPIO_MODE_OUTPUT);
    //xTaskCreatePinnedToCore(vfd_draw, "vfd_draw", 4096, NULL, 1, NULL, 0);
    all();
    esp_rom_gpio_pad_select_gpio(GPIO_NUM_0);
    gpio_set_direction(GPIO_NUM_0, GPIO_MODE_INPUT);
    uint8_t vfd_vout = 0;
    for (;;) {
        while (!gpio_get_level(GPIO_NUM_0)) {
            if (vfd_vout == 1) {
                gpio_set_level(HVCTR, 0);
                gpio_set_level(FLCTR, 0);
                vfd_vout = 0;
                vTaskDelay(1000 / portTICK_PERIOD_MS);
            } else if (vfd_vout == 0) {
                gpio_set_level(FLCTR, 1);
                vTaskDelay(1000 / portTICK_PERIOD_MS);
                gpio_set_level(HVCTR, 1);
                vfd_vout = 1;
                vTaskDelay(1000 / portTICK_PERIOD_MS);
            }
            vTaskDelay(5000 / portTICK_PERIOD_MS);
            gpio_set_level(HVCTR, 0);
            gpio_set_level(FLCTR, 0);
            vfd_vout = 0;
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
}

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mn12832j.h"

void vfd_draw(void) {
    for (;;) {
        vfd_draw_square(0, 0, 127, 31, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        vfd_draw_square(0, 0, 127, 31, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void) {
    vfd_start_send();
    xTaskCreatePinnedToCore(vfd_draw, "vfd_draw", 4096, NULL, 2, NULL, 0);
}

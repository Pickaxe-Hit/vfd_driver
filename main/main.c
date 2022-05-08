#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mn12832j.h"

void draw(void) {
    for (;;) {
        vfd_draw_square(0, 0, 127, 127, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        vfd_draw_square(0, 0, 127, 127, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void) {
    vfd_start_send();
    xTaskCreatePinnedToCore(draw, "draw", 2048, NULL, 2, NULL, 0);
}

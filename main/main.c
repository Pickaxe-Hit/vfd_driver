#include "mn12832j.h"

void app_main(void) {
    vfd_init();
    vfd_draw_square(0, 0, 127, 31, 1);
    for (;;) {
        vfd_send_data();
    }
}

#include "mn12832j.h"

rmt_item32_t rmt_clk_data[65];
rmt_item32_t rmt_si_data[33];
rmt_item32_t rmt_delay_high_data[2];
rmt_item32_t rmt_delay_low_data[2];

void rmt_data_init(void) {
    rmt_delay_high_data[0].duration0 = 5;
    rmt_delay_high_data[0].level0 = 1;
    rmt_delay_high_data[0].duration1 = 5;
    rmt_delay_high_data[0].level1 = 1;
    rmt_delay_high_data[1].duration0 = 0;
    rmt_delay_high_data[1].level0 = 0;
    rmt_delay_high_data[1].duration1 = 0;
    rmt_delay_high_data[1].level1 = 0;
    rmt_delay_low_data[0].duration0 = 5;
    rmt_delay_low_data[0].level0 = 0;
    rmt_delay_low_data[0].duration1 = 5;
    rmt_delay_low_data[0].level1 = 0;
    rmt_delay_low_data[1].duration0 = 0;
    rmt_delay_low_data[1].level0 = 0;
    rmt_delay_low_data[1].duration1 = 0;
    rmt_delay_low_data[1].level1 = 0;
    for (uint8_t i = 0; i < 65; i++) {
        if (i < 64) {
            rmt_clk_data[i].duration0 = 5;
            rmt_clk_data[i].level0 = 0;
            rmt_clk_data[i].duration1 = 5;
            rmt_clk_data[i].level1 = 1;
        } else if (i == 64) {
            rmt_clk_data[i].duration0 = 0;
            rmt_clk_data[i].level0 = 0;
            rmt_clk_data[i].duration1 = 0;
            rmt_clk_data[i].level1 = 0;
        };
        if (i < 32) {
            rmt_si_data[i].duration0 = 10;
            rmt_si_data[i].level0 = 0;
            rmt_si_data[i].duration1 = 10;
            rmt_si_data[i].level1 = 0;
        } else if (i == 32) {
            rmt_si_data[i].duration0 = 0;
            rmt_si_data[i].level0 = 0;
            rmt_si_data[i].duration1 = 0;
            rmt_si_data[i].level1 = 0;
        };
    };
}

void rmt_clk_init(void) {
    rmt_config_t config_clk = RMT_DEFAULT_CONFIG_TX(CLK, RMT_CLK_CHANNEL);
    config_clk.tx_config.carrier_en = false;
    config_clk.clk_div = 4;
    config_clk.tx_config.idle_level = true;
    config_clk.tx_config.idle_output_en = true;

    ESP_ERROR_CHECK(rmt_config(&config_clk));
    ESP_ERROR_CHECK(rmt_driver_install(config_clk.channel, 0, 0));
}

void rmt_si_init(void) {
    rmt_config_t config_si = RMT_DEFAULT_CONFIG_TX(SI, RMT_SI_CHANNEL);
    config_si.tx_config.carrier_en = false;
    config_si.clk_div = 4;
    config_si.tx_config.idle_level = false;
    config_si.tx_config.idle_output_en = true;

    ESP_ERROR_CHECK(rmt_config(&config_si));
    ESP_ERROR_CHECK(rmt_driver_install(config_si.channel, 0, 0));
}

void vfd_vout_enable(void) {
    for (;;) {
        esp_rom_gpio_pad_select_gpio(VOUTCTR);
        gpio_set_direction(VOUTCTR, GPIO_MODE_OUTPUT);
        for (;;) {
            gpio_set_level(VOUTCTR, 0);
            vTaskDelay(100 / portTICK_PERIOD_MS);
            gpio_set_level(VOUTCTR, 1);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }
    }
}

void vfd_gpio_init(void) {
    esp_rom_gpio_pad_select_gpio(FLCTR);
    esp_rom_gpio_pad_select_gpio(HVCTR);
    esp_rom_gpio_pad_select_gpio(BKG);
    esp_rom_gpio_pad_select_gpio(BK1);
    esp_rom_gpio_pad_select_gpio(BK2);
    esp_rom_gpio_pad_select_gpio(LATG);
    esp_rom_gpio_pad_select_gpio(LAT);
    esp_rom_gpio_pad_select_gpio(CLKG);
    esp_rom_gpio_pad_select_gpio(SIG);
    gpio_set_direction(FLCTR, GPIO_MODE_OUTPUT);
    gpio_set_direction(HVCTR, GPIO_MODE_OUTPUT);
    gpio_set_direction(BKG, GPIO_MODE_OUTPUT);
    gpio_set_direction(BK1, GPIO_MODE_OUTPUT);
    gpio_set_direction(BK2, GPIO_MODE_OUTPUT);
    gpio_set_direction(LATG, GPIO_MODE_OUTPUT);
    gpio_set_direction(LAT, GPIO_MODE_OUTPUT);
    gpio_set_direction(CLKG, GPIO_MODE_OUTPUT);
    rmt_clk_init();
    gpio_set_direction(SIG, GPIO_MODE_OUTPUT);
    rmt_si_init();
    gpio_set_level(FLCTR, 0);
    gpio_set_level(HVCTR, 0);
    gpio_set_level(CLKG, 1);
    rmt_write_items(RMT_CLK_CHANNEL,
                    rmt_delay_high_data,
                    sizeof(rmt_delay_high_data) / sizeof(rmt_delay_high_data[0]),
                    true);
    gpio_set_level(LATG, 1);
    gpio_set_level(LAT, 0);
    gpio_set_level(BKG, 1);
    gpio_set_level(BK1, 1);
    gpio_set_level(BK2, 1);
    gpio_set_level(SIG, 0);
    rmt_write_items(RMT_SI_CHANNEL,
                    rmt_delay_low_data,
                    sizeof(rmt_delay_low_data) / sizeof(rmt_delay_low_data[0]),
                    true);
    esp_rom_gpio_pad_select_gpio(GPIOCTR);
    gpio_set_direction(GPIOCTR, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIOCTR, 0);
    xTaskCreatePinnedToCore(vfd_vout_enable, "vout_enable", 2048, NULL, 2, NULL, 1);
}

uint8_t vfd_gram[128][4] = {0};

void vfd_send_data(void) {
    for (uint8_t cycle = 0; cycle < 63; cycle++) {
        for (uint8_t row = 0; row < 32; row++) {
            rmt_si_data[row].level1 =
                (vfd_gram[cycle * 2 + 1][row / 8] & (1 << (row & 7))) >= 1;
            rmt_si_data[row].level0 =
                (vfd_gram[cycle * 2 + 2][row / 8] & (1 << (row & 7))) >= 1;
        };
        gpio_set_level(BKG, 1);
        if (cycle & 1) {
            gpio_set_level(BK1, 1);
        } else {
            gpio_set_level(BK2, 1);
        };
        vTaskDelay(5 / (1000 * portTICK_PERIOD_MS));
        gpio_set_level(LAT, 1);
        if (cycle == 0 || cycle == 1) {
            gpio_set_level(SIG, 1);
            gpio_set_level(CLKG, 0);
            gpio_set_level(CLKG, 1);
            gpio_set_level(SIG, 0);
        } else {
            gpio_set_level(CLKG, 0);
            gpio_set_level(CLKG, 1);
        };
        vTaskDelay(5 / (1000 * portTICK_PERIOD_MS));
        gpio_set_level(LAT, 0);
        if (cycle & 1) {
            gpio_set_level(BK2, 0);
        } else {
            gpio_set_level(BK1, 0);
        };
        gpio_set_level(BKG, 0);
        rmt_add_channel_to_group(RMT_CLK_CHANNEL);
        rmt_add_channel_to_group(RMT_SI_CHANNEL);
        rmt_write_items(RMT_CLK_CHANNEL,
                        rmt_clk_data,
                        sizeof(rmt_clk_data) / sizeof(rmt_clk_data[0]),
                        false);
        rmt_write_items(RMT_SI_CHANNEL,
                        rmt_si_data,
                        sizeof(rmt_si_data) / sizeof(rmt_si_data[0]),
                        true);
    };

    for (uint8_t row = 0; row < 32; row++) {
        rmt_si_data[row].level1 = (vfd_gram[127][row / 8] & (1 << (row & 7))) >= 1;
        rmt_si_data[row].level0 = (vfd_gram[0][row / 8] & (1 << (row & 7))) >= 1;
    };
    gpio_set_level(BKG, 1);
    gpio_set_level(BK1, 1);
    for (int i = 0; i <= CONFIG_ESP32S3_DEFAULT_CPU_FREQ_MHZ; i++)
        ;
    ;
    gpio_set_level(LAT, 1);
    gpio_set_level(CLKG, 0);
    gpio_set_level(CLKG, 1);
    for (int i = 0; i <= CONFIG_ESP32S3_DEFAULT_CPU_FREQ_MHZ; i++)
        ;
    gpio_set_level(LAT, 0);
    gpio_set_level(BK2, 0);
    gpio_set_level(BKG, 0);
    rmt_add_channel_to_group(RMT_CLK_CHANNEL);
    rmt_add_channel_to_group(RMT_SI_CHANNEL);
    rmt_write_items(RMT_CLK_CHANNEL,
                    rmt_clk_data,
                    sizeof(rmt_clk_data) / sizeof(rmt_clk_data[0]),
                    false);
    rmt_write_items(RMT_SI_CHANNEL,
                    rmt_si_data,
                    sizeof(rmt_si_data) / sizeof(rmt_si_data[0]),
                    true);
}

void vfd_draw_pixel(uint8_t x, uint8_t y, uint8_t enable) {
    uint8_t byte_pos, bit_x, tmp = 0;
    byte_pos = y >> 3;
    bit_x = y & 7;
    tmp = 1 << bit_x;
    if (enable) {
        vfd_gram[x][byte_pos] |= tmp;
    } else {
        vfd_gram[x][byte_pos] &= ~tmp;
    }
}

void vfd_draw_cycle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t enable) {
    uint8_t x, y, R = 0;
    for (x = x0 - r; x <= x0 + r; x++) {
        for (y = y0 - r; y <= y0 + r; y++) {
            R = sqrt(pow(r, 2) - pow(x - x0, 2)) + y0;
            if ((y >= y0 && y <= R) || (y < y0 && y >= y0 * 2 - R) || enable == 0) {
                vfd_draw_pixel(x, y, enable);
            }
        }
    }
}

void vfd_draw_square(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t enable) {
    uint8_t x, y = 0;
    for (x = x1; x <= x2; x++) {
        for (y = y1; y <= y2; y++) {
            vfd_draw_pixel(x, y, enable);
        }
    }
}

void vfd_write_byte(uint8_t x, uint8_t y, uint8_t data) {
    vfd_gram[x][y] = data;
}

void vfd_init(void) {
    rmt_data_init();
    vfd_gpio_init();
}

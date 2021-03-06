#include "mn12864k.h"

static const char *TAG = "vfd_driver";

rmt_item32_t vfd_rmt_clk_data[385];
rmt_item32_t vfd_rmt_si_data[193];
rmt_item32_t vfd_rmt_clkg_data[49];
rmt_item32_t vfd_rmt_sig_data[23];
rmt_item32_t vfd_rmt_delay_high_data[2];
rmt_item32_t vfd_rmt_delay_low_data[2];

void vfd_rmt_data_init(void) {
    vfd_rmt_delay_high_data[0].duration0 = 5;
    vfd_rmt_delay_high_data[0].level0 = 1;
    vfd_rmt_delay_high_data[0].duration1 = 5;
    vfd_rmt_delay_high_data[0].level1 = 1;
    vfd_rmt_delay_high_data[1].duration0 = 0;
    vfd_rmt_delay_high_data[1].level0 = 0;
    vfd_rmt_delay_high_data[1].duration1 = 0;
    vfd_rmt_delay_high_data[1].level1 = 0;
    vfd_rmt_delay_low_data[0].duration0 = 5;
    vfd_rmt_delay_low_data[0].level0 = 0;
    vfd_rmt_delay_low_data[0].duration1 = 5;
    vfd_rmt_delay_low_data[0].level1 = 0;
    vfd_rmt_delay_low_data[1].duration0 = 0;
    vfd_rmt_delay_low_data[1].level0 = 0;
    vfd_rmt_delay_low_data[1].duration1 = 0;
    vfd_rmt_delay_low_data[1].level1 = 0;
    for (uint16_t i = 0; i < 385; i++) {
        if (i < 384) {
            vfd_rmt_clk_data[i].duration0 = 5;
            vfd_rmt_clk_data[i].level0 = 0;
            vfd_rmt_clk_data[i].duration1 = 5;
            vfd_rmt_clk_data[i].level1 = 1;
        } else if (i == 384) {
            vfd_rmt_clk_data[i].duration0 = 0;
            vfd_rmt_clk_data[i].level0 = 0;
            vfd_rmt_clk_data[i].duration1 = 0;
            vfd_rmt_clk_data[i].level1 = 0;
        };
        if (i < 48) {
            vfd_rmt_clkg_data[i].duration0 = 5;
            vfd_rmt_clkg_data[i].level0 = 0;
            vfd_rmt_clkg_data[i].duration1 = 5;
            vfd_rmt_clkg_data[i].level1 = 1;
        } else if (i == 48) {
            vfd_rmt_clkg_data[i].duration0 = 0;
            vfd_rmt_clkg_data[i].level0 = 0;
            vfd_rmt_clkg_data[i].duration1 = 0;
            vfd_rmt_clkg_data[i].level1 = 0;
        };
        if (i < 192) {
            vfd_rmt_si_data[i].duration0 = 10;
            vfd_rmt_si_data[i].level0 = 0;
            vfd_rmt_si_data[i].duration1 = 10;
            vfd_rmt_si_data[i].level1 = 0;
        } else if (i == 192) {
            vfd_rmt_si_data[i].duration0 = 0;
            vfd_rmt_si_data[i].level0 = 0;
            vfd_rmt_si_data[i].duration1 = 0;
            vfd_rmt_si_data[i].level1 = 0;
        };
        if (i < 22) {
            vfd_rmt_sig_data[i].duration0 = 10;
            vfd_rmt_sig_data[i].level0 = 0;
            vfd_rmt_sig_data[i].duration1 = 10;
            vfd_rmt_sig_data[i].level1 = 0;
        } else if (i == 22) {
            vfd_rmt_sig_data[i].duration0 = 0;
            vfd_rmt_sig_data[i].level0 = 0;
            vfd_rmt_sig_data[i].duration1 = 0;
            vfd_rmt_sig_data[i].level1 = 0;
        };
    };
}

void vfd_rmt_clk_init(void) {
    rmt_config_t vfd_rmt_clk_config = RMT_DEFAULT_CONFIG_TX(CLK, RMT_CLK_CHANNEL);
    vfd_rmt_clk_config.tx_config.carrier_en = false;
    vfd_rmt_clk_config.clk_div = 4;
    vfd_rmt_clk_config.tx_config.idle_level = true;
    vfd_rmt_clk_config.tx_config.idle_output_en = true;

    ESP_ERROR_CHECK(rmt_config(&vfd_rmt_clk_config));
    ESP_ERROR_CHECK(rmt_driver_install(vfd_rmt_clk_config.channel, 0, 0));
}

void vfd_rmt_si_init(void) {
    rmt_config_t vfd_rmt_si_config = RMT_DEFAULT_CONFIG_TX(SI, RMT_SI_CHANNEL);
    vfd_rmt_si_config.tx_config.carrier_en = false;
    vfd_rmt_si_config.clk_div = 4;
    vfd_rmt_si_config.tx_config.idle_level = false;
    vfd_rmt_si_config.tx_config.idle_output_en = true;

    ESP_ERROR_CHECK(rmt_config(&vfd_rmt_si_config));
    ESP_ERROR_CHECK(rmt_driver_install(vfd_rmt_si_config.channel, 0, 0));
}

/*void vfd_rmt_clkg_init(void) {
    rmt_config_t vfd_rmt_clkg_config = RMT_DEFAULT_CONFIG_TX(CLKG, RMT_CLKG_CHANNEL);
    vfd_rmt_clkg_config.tx_config.carrier_en = false;
    vfd_rmt_clkg_config.clk_div = 2;
    vfd_rmt_clkg_config.tx_config.idle_level = true;
    vfd_rmt_clkg_config.tx_config.idle_output_en = true;

    ESP_ERROR_CHECK(rmt_config(&vfd_rmt_clkg_config));
    ESP_ERROR_CHECK(rmt_driver_install(vfd_rmt_clkg_config.channel, 0, 0));
}

void vfd_rmt_sig_init(void) {
    rmt_config_t vfd_rmt_sig_config = RMT_DEFAULT_CONFIG_TX(SIG, RMT_SIG_CHANNEL);
    vfd_rmt_sig_config.tx_config.carrier_en = false;
    vfd_rmt_sig_config.clk_div = 2;
    vfd_rmt_sig_config.tx_config.idle_level = false;
    vfd_rmt_sig_config.tx_config.idle_output_en = true;

    ESP_ERROR_CHECK(rmt_config(&vfd_rmt_sig_config));
    ESP_ERROR_CHECK(rmt_driver_install(vfd_rmt_sig_config.channel, 0, 0));
}*/

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
    esp_rom_gpio_pad_select_gpio(BK);
    esp_rom_gpio_pad_select_gpio(SIG);
    esp_rom_gpio_pad_select_gpio(CLKG);
    esp_rom_gpio_pad_select_gpio(LATG);
    esp_rom_gpio_pad_select_gpio(LAT);
    gpio_set_direction(FLCTR, GPIO_MODE_OUTPUT);
    gpio_set_direction(HVCTR, GPIO_MODE_OUTPUT);
    gpio_set_direction(BKG, GPIO_MODE_OUTPUT);
    gpio_set_direction(BK, GPIO_MODE_OUTPUT);
    gpio_set_direction(SIG, GPIO_MODE_OUTPUT);
    gpio_set_direction(CLKG, GPIO_MODE_OUTPUT);
    gpio_set_direction(LATG, GPIO_MODE_OUTPUT);
    gpio_set_direction(LAT, GPIO_MODE_OUTPUT);
    vfd_rmt_clk_init();
    vfd_rmt_si_init();
    // vfd_rmt_clkg_init();
    // vfd_rmt_sig_init();
    gpio_set_level(FLCTR, 0);
    gpio_set_level(HVCTR, 0);
    rmt_write_items(RMT_CLK_CHANNEL,
                    vfd_rmt_delay_high_data,
                    sizeof(vfd_rmt_delay_high_data) / sizeof(vfd_rmt_delay_high_data[0]),
                    true);
    // rmt_write_items(RMT_CLKG_CHANNEL,
    //                 vfd_rmt_delay_high_data,
    //                 sizeof(vfd_rmt_delay_high_data) / sizeof(vfd_rmt_delay_high_data[0]),
    //                 true);
    gpio_set_level(LATG, 1);
    gpio_set_level(LAT, 0);
    gpio_set_level(BKG, 1);
    gpio_set_level(BK, 1);
    gpio_set_level(CLKG, 1);
    gpio_set_level(SIG, 0);
    rmt_write_items(RMT_SI_CHANNEL,
                    vfd_rmt_delay_low_data,
                    sizeof(vfd_rmt_delay_low_data) / sizeof(vfd_rmt_delay_low_data[0]),
                    true);
    // rmt_write_items(RMT_SIG_CHANNEL,
    //                 vfd_rmt_delay_low_data,
    //                 sizeof(vfd_rmt_delay_low_data) / sizeof(vfd_rmt_delay_low_data[0]),
    //                 true);
    esp_rom_gpio_pad_select_gpio(GPIOCTR);
    gpio_set_direction(GPIOCTR, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIOCTR, 0);
    // xTaskCreatePinnedToCore(vfd_vout_enable, "vfd_vout_enable", 2048, NULL, 2, NULL, 1);
}

uint8_t vfd_gram[128][8] = {0};

void vfd_send_data(void) {
    for (;;) {
        for (uint8_t gate = 0; gate <= 42; gate++) {
            if (gate == 42) { // T43
                for (uint8_t row = 0; row <= 63; row++) {
                    vfd_rmt_si_data[row * 3].level0 =
                        (vfd_gram[126][row / 8] & (1 << (row & 7))) >= 1;
                    vfd_rmt_si_data[row * 3].level1 = 0;
                    vfd_rmt_si_data[row * 3 + 1].level0 =
                        (vfd_gram[127][row / 8] & (1 << (row & 7))) >= 1;
                    vfd_rmt_si_data[row * 3 + 1].level1 = 0;
                    vfd_rmt_si_data[row * 3 + 2].level0 = 0;
                    vfd_rmt_si_data[row * 3 + 2].level1 = 0;
                }
                vfd_rmt_sig_data[20].level1 = 0;
                vfd_rmt_sig_data[21].level1 = 1;
            } else if (!(gate & 1)) { // T1,T3,T5...T41
                for (uint8_t row = 0; row <= 63; row++) {
                    vfd_rmt_si_data[row * 3].level0 =
                        (vfd_gram[gate * 3][row / 8] & (1 << (row & 7))) >= 1;
                    vfd_rmt_si_data[row * 3].level1 = 0;
                    vfd_rmt_si_data[row * 3 + 1].level0 =
                        (vfd_gram[gate * 3 + 1][row / 8] & (1 << (row & 7))) >= 1;
                    vfd_rmt_si_data[row * 3 + 1].level1 = 0;
                    vfd_rmt_si_data[row * 3 + 2].level0 =
                        (vfd_gram[gate * 3 + 2][row / 8] & (1 << (row & 7))) >= 1;
                    vfd_rmt_si_data[row * 3 + 2].level1 = 0;
                };
                if (gate == 0) {
                    vfd_rmt_sig_data[21].level1 = 0;
                    vfd_rmt_sig_data[0].level0 = 1;
                } else {
                    vfd_rmt_sig_data[(gate >> 1) - 1].level1 = 0;
                }
                // vfd_rmt_sig_data[gate>>1].level0=1;
                vfd_rmt_sig_data[gate >> 1].level1 = 1;
            } else if (gate & 1) { // T2,T4,T6...T40
                for (uint8_t row = 0; row <= 63; row++) {
                    vfd_rmt_si_data[row * 3 + 2].level0 = 0;
                    vfd_rmt_si_data[row * 3 + 2].level1 =
                        (vfd_gram[gate * 3][row / 8] & (1 << (row & 7))) >= 1;
                    vfd_rmt_si_data[row * 3 + 1].level0 = 0;
                    vfd_rmt_si_data[row * 3 + 1].level1 =
                        (vfd_gram[gate * 3 + 1][row / 8] & (1 << (row & 7))) >= 1;
                    vfd_rmt_si_data[row * 3].level0 = 0;
                    vfd_rmt_si_data[row * 3].level1 =
                        (vfd_gram[gate * 3 + 2][row / 8] & (1 << (row & 7))) >= 1;
                };
                vfd_rmt_sig_data[(gate - 1) >> 1].level0 = 0;
                vfd_rmt_sig_data[((gate - 1) >> 1) + 1].level0 = 1;
            }
            rmt_add_channel_to_group(RMT_CLK_CHANNEL);
            rmt_add_channel_to_group(RMT_SI_CHANNEL);
            rmt_write_items(RMT_CLK_CHANNEL,
                            vfd_rmt_clk_data,
                            sizeof(vfd_rmt_clk_data) / sizeof(vfd_rmt_clk_data[0]),
                            false);
            rmt_write_items(RMT_SI_CHANNEL,
                            vfd_rmt_si_data,
                            sizeof(vfd_rmt_si_data) / sizeof(vfd_rmt_si_data[0]),
                            true);
            for (uint8_t i = 0; i < gate; i++) {
                gpio_set_level(CLKG, 0);
                gpio_set_level(CLKG, 1);
            }
            gpio_set_level(CLKG, 0);
            gpio_set_level(SIG, 1);
            gpio_set_level(CLKG, 1);
            gpio_set_level(CLKG, 0);
            gpio_set_level(CLKG, 1);
            gpio_set_level(CLKG, 0);
            gpio_set_level(SIG, 0);
            gpio_set_level(CLKG, 1);
            for (uint8_t j = 0; j <= 44 - gate; j++) {
                gpio_set_level(CLKG, 0);
                gpio_set_level(CLKG, 1);
            }
            vTaskDelay(100 / (1000 * portTICK_PERIOD_MS));
            gpio_set_level(BKG, 1);
            gpio_set_level(BK, 1);
            vTaskDelay(100 / (1000 * portTICK_PERIOD_MS));
            gpio_set_level(LATG, 1);
            gpio_set_level(LAT, 1);
            vTaskDelay(100 / (1000 * portTICK_PERIOD_MS));
            gpio_set_level(LATG, 0);
            gpio_set_level(LAT, 0);
            gpio_set_level(BK, 0);
            gpio_set_level(BKG, 0);
        };
    }
}

void vfd_draw_pixel(uint8_t x, uint8_t y, uint8_t show) {
    if (x > 127 || y > 63) {
        ESP_LOGW(TAG, "Pixel (%d,%d) is out of the display.", x, y);
        goto end;
    }
    uint8_t byte_pos, bit_pos, temp = 0;
    byte_pos = y >> 3;
    bit_pos = y & 7;
    temp = 1 << bit_pos;
    if (show) {
        vfd_gram[x][byte_pos] |= temp;
    } else {
        vfd_gram[x][byte_pos] &= ~temp;
    }

end:;
}

void vfd_draw_horz_line(uint8_t x, uint8_t y, uint8_t w, uint8_t show) {
    uint8_t x0;
    for (x0 = 0; x0 < w; x0++) {
        vfd_draw_pixel(x + x0, y, show);
    }
}

void vfd_draw_vert_line(uint8_t x, uint8_t y, uint8_t h, uint8_t show) {
    uint8_t y0;
    for (y0 = 0; y0 < h; y0++) {
        vfd_draw_pixel(x, y + y0, show);
    }
}

void vfd_draw_gate(uint8_t x0, uint8_t y0, uint8_t r, uint8_t show) {
    uint8_t x, y, R = 0;
    for (x = x0 - r; x <= x0 + r; x++) {
        for (y = y0 - r; y <= y0 + r; y++) {
            R = sqrt(pow(r, 2) - pow(x - x0, 2)) + y0;
            if ((y >= y0 && y <= R) || (y < y0 && y >= y0 * 2 - R) || show == 0) {
                vfd_draw_pixel(x, y, show);
            }
        }
    }
}

void vfd_draw_square(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t show) {
    uint8_t x, y = 0;
    for (x = x1; x <= x2; x++) {
        for (y = y1; y <= y2; y++) {
            vfd_draw_pixel(x, y, show);
        }
    }
}

void vfd_write_byte(uint8_t x, uint8_t y, uint8_t data) {
    vfd_gram[x][y] = data;
}

void vfd_init(void) {
    vfd_rmt_data_init();
    vfd_gpio_init();
}

void vfd_start_send(void) {
    vfd_init();
    xTaskCreatePinnedToCore(vfd_send_data,
                            "vfd_send_data",
                            2048,
                            NULL,
                            configMAX_PRIORITIES - 1,
                            NULL, 1);
}

void all(void) {
    for (uint8_t i = 0; i <= 127; i++) {
        for (uint8_t j = 0; j <= 7; j++) {
            vfd_gram[i][j] = 0xff;
        }
    }
}

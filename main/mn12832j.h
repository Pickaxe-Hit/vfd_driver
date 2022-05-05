#include "driver/gpio.h"
#include "driver/rmt.h"
#include <math.h>
#include <stdint.h>

#define RMT_CLK_CHANNEL RMT_CHANNEL_0
#define RMT_SI_CHANNEL RMT_CHANNEL_1

#define BK1 GPIO_NUM_4
#define BK2 GPIO_NUM_5
#define LAT GPIO_NUM_6
#define CLK GPIO_NUM_7
#define SI GPIO_NUM_15
#define BKG GPIO_NUM_16
#define LATG GPIO_NUM_17
#define CLKG GPIO_NUM_18
#define SIG GPIO_NUM_8
#define FLCTR GPIO_NUM_10
#define HVCTR GPIO_NUM_9
#define GPIOCTR GPIO_NUM_11
#define VOUTCTR GPIO_NUM_12

void rmt_data_init(void);

void rmt_clk_init(void);

void vfd_gpio_init(void);

void vfd_send_data(void);

void vfd_draw_pixel(uint8_t x, uint8_t y, uint8_t enable);

void vfd_draw_cycle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t enable);

void vfd_draw_square(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t enable);

void vfd_write_byte(uint8_t x, uint8_t y, uint8_t data);

void vfd_init(void);
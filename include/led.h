#ifndef LED_H
#define LED_H

#define LED_CAN 0
#define LED_DBG 1
#define LED_ERR 2

void set_LED(uint8_t L, uint8_t state);
void update_LEDS(uint16_t count);
void set_RGB(uint8_t *R, uint8_t *G, uint8_t *B);

#endif
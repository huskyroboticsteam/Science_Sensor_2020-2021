#ifndef ENCODER_H
#define ENCODER_H

int32_t get_encoder_ticks(uint8_t which);
void set_encoder_ticks(uint8_t which, int32_t ticks);
void init_encoders();
void reset_encoder(uint8_t which);

#endif
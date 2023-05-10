#ifndef __TSIC50X_DRIVER_H__
#define __TSIC50X_DRIVER_H__

#include <stdint.h>
#include <string.h>
#include <Arduino.h>

typedef struct tsic_oper  {
    void (*tsic_delayus)(uint32_t num);
    void (*tsic_delayms)(uint32_t num);
    void (*tsic_power)(uint8_t sensor_status);
    uint8_t (*tsic_data_output)(uint8_t pinNum);
}tsic_oper_t;

void Tsic_Init(tsic_oper_t *remap_tsic_oper);
uint8_t read_tsic506_byte(uint8_t pinNum, float *value);

#endif

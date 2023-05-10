#include "tsic50x_driver.h"

uint8_t wait_edge_trigger(uint8_t pinNum, uint8_t sensor_status);

tsic_oper_t tsic_oper;


void Tsic_Init(tsic_oper_t *remap_tsic_oper)
{
    memcpy(&tsic_oper, remap_tsic_oper, sizeof(tsic_oper));
}

void tsic_start(uint8_t pinNum)
{
    wait_edge_trigger(pinNum, 0);  

}

uint16_t tsic_data(uint8_t pinNum, uint8_t *val)
{
    uint8_t i = 0;
    uint16_t temp = 0;
    uint8_t check = 0;
    int timerValue = 10000;
        
    for(i = 0; i < 9; i++)
    {
        while(tsic_oper.tsic_data_output(pinNum) && timerValue-- > 0 )
            continue;
        tsic_oper.tsic_delayus(52);
        if(tsic_oper.tsic_data_output(pinNum))
            temp |= 1 << (8 - i);
        else
            wait_edge_trigger(pinNum, 0);
    }
    
    for(i = 0; i < 9; i++)
    {
        if(temp & (1 << i))
            check++;
    }
    
    if(check % 2)
    {
        return 1;
    }

    *val = (temp >> 1);
    return 0;
}

void tsic_stop(uint8_t pinNum)
{
    wait_edge_trigger(pinNum, 1);
}


uint8_t read_tsic506_byte(uint8_t pinNum, float *value)
{
    uint8_t temp1 = 0,temp2 = 0;
    uint32_t data = 0;
    if(wait_edge_trigger(pinNum, 1) == 2)
        return 2;

    if(wait_edge_trigger(pinNum, 0) == 2)
        return 2;

    tsic_oper.tsic_delayms(50);
    wait_edge_trigger(pinNum, 1);
    wait_edge_trigger(pinNum, 0); 

    if(tsic_data(pinNum, &temp1))
    {
        return 1;
    }

    tsic_stop(pinNum);
        
    tsic_start(pinNum);
        
    if(tsic_data(pinNum, &temp2))
    {
        return 1;
    }

    tsic_oper.tsic_power(0);

    data |= temp2;
    data |= temp1 << 8;

    *value = ((double) data) / 2047 * 70 - 10;

    return 0;
}

uint8_t wait_edge_trigger(uint8_t pinNum, uint8_t status)
{
    int i = 100000;

    while(tsic_oper.tsic_data_output(pinNum) == status && i)
    {
        tsic_oper.tsic_delayus(1);
        i--;
    }
    if(i == 0)
    {
        return 2;
    }
  
    return 0;
}

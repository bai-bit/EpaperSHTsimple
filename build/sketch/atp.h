#line 1 "F:\\EpaperSHTsimple\\EpaperSHTsimple\\atp.h"
#ifndef __ATP_H__
#define __ATP_H__

#include <stdint.h>
#include <stdbool.h>

typedef struct  
{
    char		*name;
    int         (*cmd)(int argc, char * const argv[]);
}ATCMD_t;

extern const ATCMD_t ATCMD[];

void atp_init(ATCMD_t *CMD, uint32_t size);
void atp_put(uint8_t ch, void *param, void *data);

#endif
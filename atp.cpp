#include <string.h>
#include <stdio.h>
#include "atp.h"

#define BUF_SIZE        (128)

#if !defined(ARRAY_SIZE)
    #define ARRAY_SIZE(x)	(sizeof(x) / sizeof((x)[0]))
#endif

#if !defined(MAX)
    #define MAX(a, b)       ((a) > (b) ? (a) : (b))
#endif
    
enum
{
    kAT_CMDA,
    kAT_CMDT,
    kAT_DATA,
    kAT_END,
};

typedef struct
{
    ATCMD_t *cmd;
    uint32_t cnt;
}atp_t;

static atp_t atp;

void atp_init(ATCMD_t *CMD, uint32_t size)
{
    atp.cnt = size;
    atp.cmd = CMD;
}

void atp_put(uint8_t ch, void *param, void *data)
{
    uint8_t ret = 0;
    static uint8_t stat = kAT_CMDA;
    static char buf[BUF_SIZE] = "";
    static uint32_t i = 0;
    uint32_t num = 0, len = 0;
    char argc = 0;
    char *argv[4] = {0};
    
    switch(stat)
    {
        case kAT_CMDA:
            if('A' == ch)
                stat = kAT_CMDT;
            break;
        case kAT_CMDT:
            'T' == ch ? stat = kAT_DATA : (stat = kAT_CMDA);
            i = 0;
            break;
        case kAT_DATA:
            '\r' == ch || '\n' == ch || i == BUF_SIZE ? stat = kAT_END : (buf[i++] = ch); 
            break;
        case kAT_END:
            if('\r' == ch || '\n' == ch)
            {
                buf[i] = '\0';
                for(num = 0; num < atp.cnt; num++)
                {
                    strchr(buf, '=') ? len = (strchr(buf, '=') - buf) : (len = i);
                    
                    if(!strncmp(atp.cmd[num].name, buf, MAX(len, strlen(atp.cmd[num].name))))
                    {
                        argc = 3;
                        argv[0] = (char *)param;
                        argv[1] = (char *)data;
                        argv[2] = (char *)(buf + strlen(atp.cmd[num].name));

                        ret = atp.cmd[num].cmd(argc, argv);
                        if(0 == ret)
                            printf("OK\r\n");
                        memset(buf, 0, sizeof(buf));      
                        break;
                    }
                }
            }
            stat = kAT_CMDA;
            break;
        default:
            break;
    }

}



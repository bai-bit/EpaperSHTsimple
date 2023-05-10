#line 1 "D:\\chaohe\\HiPNUC\\EpaperSHTsimple\\EpaperSHTsimple\\atcmd.cpp"
#include "atp.h"
#include <stdio.h>
#include <string.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x)	(sizeof(x) / sizeof((x)[0]))
#endif

#define AT_QUEUE_MAX_SIZE   (64)

typedef struct
{
    uint8_t len;
    uint8_t buf[AT_QUEUE_MAX_SIZE];
}at_queue_t;

int RST(int argc, char * const argv[]);
int INFO(int argc, char * const argv[]);
int ODR(int argc, char * const argv[]);
int BAUD(int argc, char * const argv[]);
int ID(int argc, char * const argv[]);
int MODE(int argc, char * const argv[]);
int CLEAN(int argc, char * const argv[]);
int BUILD(int argc, char *const argv[]);
int CMD(int argc, char *const argv[]);
int QUIT(int argc, char *const argv[]);
int WINAME(int argc, char *const argv[]);
int WIPASSWORD(int argc, char *const argv[]);
int MQSERVER(int argc, char *const argv[]);
int MQDEVID(int argc, char *const argv[]);
int MQPUBID(int argc, char *const argv[]);
int MQPASSWORD(int argc, char *const argv[]);
int MQPNUM(int argc, char *const argv[]);
int USFACTOR(int argc, char *const argv[]);
int TIMESLEEP(int argc, char *const argv[]);

/* AT command list */
const ATCMD_t ATCMD[] = 
{
    {"+RST",       RST        }, /* 模块复位 */
    {"+BAUD",      BAUD       }, /* 配置波特率 */
    {"+INFO",      INFO       }, /* 显示信息 */
    {"+ID",        ID         }, /* 显示id号 */
    {"+MODE",      MODE       }, /* 配置工作模式，目前这个功能没有使用 */
    {"+CLEAN",     CLEAN      }, /* 恢复默认配置 */
    {"+BUILD",     BUILD      }, /* 打印固件编译日期 */
    {"+CMD",       CMD        }, /* 进入配置模式*/
    {"+QUIT",      QUIT       }, /* 退出配置模式*/
    {"+WIFIN",     WINAME     }, /* 配置wifi账号密码 */
    {"+WIFIPW",    WIPASSWORD }, /* 配置wifi密码 */
    {"+MQSERVER",  MQSERVER   }, /* 配置mqtt服务器地址*/
    {"+MQDEVID",   MQDEVID    }, /* 配置mqtt设备号*/
    {"+MQPUBID",   MQPUBID    }, /* 配置mqtt产品号*/
    {"+MQPW",      MQPASSWORD }, /* 配置mqtt鉴权信息*/
    {"+MQPNUM",    MQPNUM     }, /* 配置mqtt 服务器端口号*/
    {"+USFACTOR",  USFACTOR   }, /* 配置时间分频因子*/
    {"+TIMESLEEP", TIMESLEEP  }, /* 配置睡眠时间*/
};


void at_thread_putchar(uint8_t *buf, uint32_t len, void *param, void *data)
{
    int i;

    at_queue_t queue;
    queue.len = len;
    memcpy(queue.buf, buf, len);

    if(len > AT_QUEUE_MAX_SIZE)
        len = AT_QUEUE_MAX_SIZE;

    for(i=0; i<queue.len; i++)
    {
        atp_put(queue.buf[i], (void *)param, (void *)data);
    }
}


void atCmd_init(void)
{
    atp_init((ATCMD_t*)ATCMD, ARRAY_SIZE(ATCMD));
}
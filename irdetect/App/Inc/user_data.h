#ifndef __USER_DATA_H__
#define __USER_DATA_H__
#include <stdint.h>

typedef struct {
    uint8_t opt;
    void (*func)(uint8_t *data, uint8_t length);
}cmd_query_t;

typedef struct {
    uint8_t  *cmdEnable;
    uint16_t *cmdDelay;
    void (*func)(void);
}cmd_report_t;

extern cmd_query_t query_cmd[];
extern cmd_query_t factory_query_cmd[];
extern cmd_report_t report_cmd[];

/* misc */

void user_database_init(void);

void user_database_save(void);

void user_reply_handle(void);

int getQueryCmdIndex(uint16_t opt);

void printSetting(void);

void user_check_report_delay(void);

/* listen function */
void onCmdGetAllStatus(uint8_t *data, uint8_t length);

void onCmdSetSingleLedState(uint8_t *data, uint8_t length);

void onCmdSingleSetAllLedState(uint8_t *data, uint8_t length);

void onCmdModifyBaseSetting(uint8_t *data, uint8_t length);

void onCmdMultiSetAutoReportFlag(uint8_t *data, uint8_t length);

void onCmdMultiSetSingleLedState(uint8_t *data, uint8_t length);

void onCmdMultiSetAllLedState(uint8_t *data, uint8_t length);

void onCmdSingleModifyBaudRate(uint8_t *data, uint8_t length);

void onCmdMultiModifyBaudRate(uint8_t *data, uint8_t length);

/* report function */
void onReportAllStatus(void);

void onReportSetSingleLedState(void);

void onReportSetAllLedState(void);

void onReportAlarmType(void);

void onReportBaseSetting(void);

void onReportAddrAndUid(void);

void onReportSingleModifyBaudRate(void);

#endif


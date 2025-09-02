#ifndef __COMMON_H__
#define __COMMON_H__
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h> 
// #include <math.h>
#include "main.h"
#include "flash_if.h"

#define DATABASE_START_ADDR         (0x0800F000)
#define DATABASE_MAGIC              (0xaaaa)


#define DEFAULT_AUTO_REPORT_FLAG    (1)
#define DEFAULT_BAUD_RATE_INDEX     (4)

#define DELAY_BASE                  (10)//100ms*10 = 1s
#define FLASH_FREQ                  (200)//ms


#define DEV_NUM						(12)

#define VERSION						(0x0803)//v8.3

#define BROADCAST_ADDR              (0xFF)
#define CHECK_ADDR_INVALID(addr)    (BROADCAST_ADDR != addr && addr != myDevice.address)
#define CHECK_ACK(addr)             (addr == myDevice.address)
#define IS_ADDR_INVALID(addr)        (addr != myDevice.address)
#define IS_UID_INVALID(uid0, uid1, uid2)        (uid0 != myDevice.uid0 || uid1 != myDevice.uid1 || uid2 != myDevice.uid2)


enum {
    STATUS_REQUEST_UPGRADE = 1,
    STATUS_UPGRADE_GOING,
    STATUS_UPGRADE_SUCCESS,
};
#define PACKET_SIZE                 (32)

enum {
    CMD_DISABLE = 0,
    CMD_ENABLE,
};

enum {
	STATE_DISABLE = 0,
	STATE_ENABLE,
};

typedef struct {
    uint8_t sendCmdEnable;
    uint16_t sendCmdDelay;
}cmd_setting_t;

typedef struct {
    cmd_setting_t singleQueryStatus;
    cmd_setting_t singleSetLedState;
	cmd_setting_t setAllLedsState;
	cmd_setting_t autoAlarm;
    cmd_setting_t singleBasicSetting;
	cmd_setting_t reportAddrAndUid;
    cmd_setting_t singleModifyBaudRate;
	cmd_setting_t setAddrByUid;
	cmd_setting_t setAddrByAddr;
	cmd_setting_t getInfo;
	cmd_setting_t upgrade;
	cmd_setting_t clrUartBuffer;
	cmd_setting_t factoryCmd;
}cmd_control_t;

typedef struct {
	uint8_t mode;
	uint8_t irEnable;
	uint8_t ledState;
	uint8_t outState;
	uint8_t lastOutState;
	uint8_t outCnt;
	uint8_t outStateInitFlag;
	uint8_t flashStatus;
	uint8_t send;
	uint8_t port;
	uint16_t ledFlashCnt;
}dev_ctrl_t;

typedef struct {
	uint8_t enable;
	uint8_t type;
}report_ctrl_t;


typedef struct {
    uint8_t  autoReportFlag;
    uint8_t  address;
	uint8_t  oldAddress;
	uint16_t checkPeriod;
    uint32_t uid0;
    uint32_t uid1;
    uint32_t uid2;
    uint16_t baudRateIndex;
    cmd_control_t cmdControl;
	dev_ctrl_t devCtrl[DEV_NUM];
	report_ctrl_t repCtrl[DEV_NUM];
}dev_ctx_t;


enum {
	LED_STATE_OFF = 0,
	LED_STATE_RED_ON,
	LED_STATE_GREEN_ON,
	LED_STATE_RED_FLASH,
	LED_STATE_GREEN_FLASH,
	LED_STATE_RED_GREEN_FLASH,
};

enum {
	LED_OFF = 0,
    LED_ON,
};

enum {
    LED_RED = 0,
    LED_GREEN,
};

enum {
	LED_MODE_AUTO = 0,
	LED_MODE_MANUAL,
};

typedef struct {
    uint16_t magic;
    uint16_t address;
    uint16_t autoReportFlag;
    uint16_t baudRateIndex;
}database_t;

typedef struct {
    uint16_t magic;
    uint16_t address;
    uint16_t deviceCmd;
    uint16_t baudIndex;
    uint32_t upgradeFlag;
    uint32_t packetIndex;
    uint32_t packetSize;
    uint8_t packetData[PACKET_SIZE];
    uint16_t upgradeStatus;
}upgrade_t;

extern dev_ctx_t myDevice;

void tim_interrupt_callback(void);

#endif

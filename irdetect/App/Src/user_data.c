#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user_data.h"
#include "common.h"
#include "user_protocol.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_flash_ex.h"  
#include "flash_if.h"
#include "gpio.h"


static uint32_t packetIndex;
static uint32_t packetSize;
static uint8_t upgradeStatus;
static uint8_t packetData[PACKET_SIZE];
/**
 * @brief 添加监听指令步骤
 * （1）在user_protocol.h中定义opt
 * （2）在user_data.c中修改getQueryCmdIndex，增加index
 * （3）编写监听解析函数
 * （4）在query_cmd增加对应接口
 */

cmd_query_t query_cmd[] = {
    {OPT_CODE_SINGLE_DEV_QUERY_STATUS,                  onCmdGetAllStatus},
    {OPT_CODE_SINGLE_DEV_SET_SINGLE_LED_STATE,          onCmdSetSingleLedState},
    {OPT_CODE_SINGLE_DEV_SET_MULTI_LED_STATE,           onCmdSingleSetAllLedState},
    {OPT_CODE_SINGLE_DEV_MODIFY_BASE_SETTING,           onCmdModifyBaseSetting},
    {OPT_CODE_MULTI_DEV_SET_AUTO_REPORT_FLAG,           onCmdMultiSetAutoReportFlag},
    {OPT_CODE_MULTI_DEV_SET_SINGLE_LED_STATE,           onCmdMultiSetSingleLedState},
    {OPT_CODE_MULTI_DEV_SET_MULTI_LED_STATE,            onCmdMultiSetAllLedState},
    {OPT_CODE_SINGLE_MODIFY_BAUDRATE,   				onCmdSingleModifyBaudRate},
    {OPT_CODE_MULTI_MODIFY_BAUDRATE,    				onCmdMultiModifyBaudRate},	
    {OPT_CODE_SET_ADDR_BY_UID,                          onCmdSetAddrByUid},
    {OPT_CODE_SET_ADDR_BY_ADDR,                         onCmdSetAddrByAddr},
    {OPT_CODE_GET_INFO_BY_ADDR,                         onCmdGetInfoByAddr},
    {OPT_CODE_CLEAR_UART_BUFFER,                        onCmdClearUartBuffer},
    {OPT_CODE_REQUEST_UPGRADE,                          onCmdRequestUpgrade},
    {0, NULL}, // 结束标志,
};

cmd_query_t factory_query_cmd[] = {
    {OPT_CODE_FACTORY_QUERY,            onCmdFactoryQuery},
    {0, NULL},//must end with NULL
};

/**
 * @brief 添加发布指令步骤
 * （1）在user_protocol.h中定义opt
 * （2）修改common.h中结构体cmd_control_t，增加对应的发布指令
 * （3）编写监听解析函数
 * （4）在report_cmd增加对应接口
 */

cmd_report_t report_cmd[] = {
    {&myDevice.cmdControl.singleQueryStatus.sendCmdEnable,          &myDevice.cmdControl.singleQueryStatus.sendCmdDelay,        onReportAllStatus},
    {&myDevice.cmdControl.singleSetLedState.sendCmdEnable,          &myDevice.cmdControl.singleSetLedState.sendCmdDelay,        onReportSetSingleLedState},
    {&myDevice.cmdControl.setAllLedsState.sendCmdEnable,       		&myDevice.cmdControl.setAllLedsState.sendCmdDelay,          onReportSetAllLedState},
    {&myDevice.cmdControl.autoAlarm.sendCmdEnable,          		&myDevice.cmdControl.autoAlarm.sendCmdDelay,          		onReportAlarmType},
    {&myDevice.cmdControl.singleBasicSetting.sendCmdEnable,         &myDevice.cmdControl.singleBasicSetting.sendCmdDelay,       onReportBaseSetting},
    {&myDevice.cmdControl.reportAddrAndUid.sendCmdEnable,           &myDevice.cmdControl.reportAddrAndUid.sendCmdDelay,         onReportAddrAndUid},
    {&myDevice.cmdControl.singleModifyBaudRate.sendCmdEnable,		&myDevice.cmdControl.singleModifyBaudRate.sendCmdDelay,		onReportSingleModifyBaudRate},
    {&myDevice.cmdControl.setAddrByUid.sendCmdEnable,               &myDevice.cmdControl.setAddrByUid.sendCmdDelay,             onReportSetAddrByUid},
    {&myDevice.cmdControl.setAddrByAddr.sendCmdEnable,              &myDevice.cmdControl.setAddrByAddr.sendCmdDelay,            onReportSetAddrByAddr},
    {&myDevice.cmdControl.getInfo.sendCmdEnable,                    &myDevice.cmdControl.getInfo.sendCmdDelay,                  onReportGetInfoByAddr},
    {&myDevice.cmdControl.clrUartBuffer.sendCmdEnable,              &myDevice.cmdControl.clrUartBuffer.sendCmdDelay,            onReportClearUartBuffer},
    {&myDevice.cmdControl.factoryCmd.sendCmdEnable,                 &myDevice.cmdControl.factoryCmd.sendCmdDelay,               onReportFactoryCmd},
    {&myDevice.cmdControl.upgrade.sendCmdEnable,                    &myDevice.cmdControl.upgrade.sendCmdDelay,                  onReportRequestUpgrade},
    {NULL, NULL, NULL}, // 结束标志
};

void onCmdGetAllStatus(uint8_t *data, uint8_t length)
{
    uint8_t pos = 0;
    uint8_t addr = 0;

    addr = data[pos++];

    if(IS_ADDR_INVALID(addr)){
        printf("[%s]addr is not matched!\r\n", __FUNCTION__);
        return;
    }

    /* send dev status here */
    myDevice.cmdControl.singleQueryStatus.sendCmdEnable = CMD_ENABLE;
    myDevice.cmdControl.singleQueryStatus.sendCmdDelay = 0;
}

void onCmdSingleSetAllLedState(uint8_t *data, uint8_t length)
{
    uint8_t pos = 0;
    uint8_t mode = 0;
    uint8_t addr = 0;
    uint8_t ledState[DEV_NUM] = {0};

    /* mode */
	mode = (data[pos++] << 8);
	mode += data[pos++];
    /* led state */
	for(int i=0;i< DEV_NUM;i+=2){
		ledState[DEV_NUM - 1 - i] = (data[pos] >> 4) & 0x0f;
		ledState[DEV_NUM - 2 - i] = (data[pos++] & 0x0f);
	}
    /* addr */
    addr = data[pos++];
    if(CHECK_ADDR_INVALID(addr)){
        printf("[%s]addr is not matched!\r\n", __FUNCTION__);
        return;
    }

	/* set dev state here */
	for(int i=0;i<DEV_NUM;i++){
		myDevice.devCtrl[i].mode = (mode >> i) & 0x01;
		if(myDevice.devCtrl[i].mode){
			myDevice.devCtrl[i].ledState = ledState[i];
		}else{
			myDevice.devCtrl[i].ledState = myDevice.devCtrl[i].outState ? LED_STATE_GREEN_ON : LED_STATE_RED_ON;
		}
	}
	/* send ack msg here */
    if(CHECK_ACK(addr)){
        myDevice.cmdControl.setAllLedsState.sendCmdEnable = CMD_ENABLE;
        myDevice.cmdControl.setAllLedsState.sendCmdDelay = 0;
    }	
	data[0] = 0;
}

void onCmdSetSingleLedState(uint8_t *data, uint8_t length)
{
    uint8_t addr = 0;
    uint8_t pos = 0;
    uint8_t status;
    uint8_t port;

    /* port */
    port = data[pos++] - 1;
    /* mode and state */
    status = data[pos++];
    /* addr */
    addr = data[pos++];
    if(CHECK_ADDR_INVALID(addr)){
        printf("[%s]addr is not matched!\r\n", __FUNCTION__);
        return;
    }

	if(port >= DEV_NUM){
		printf("[%s]invalid port num!!!\r\n", __FUNCTION__);
		return;
	}

    /* set dev state here */
   	myDevice.devCtrl[port].mode = (status >> 4) & 0x0f;
	if(myDevice.devCtrl[port].mode){
		myDevice.devCtrl[port].ledState = status & 0x0f;
	}else{
		myDevice.devCtrl[port].ledState = myDevice.devCtrl[port].outState ? LED_STATE_GREEN_ON : LED_STATE_RED_ON;
	}
	myDevice.devCtrl[port].send = CMD_ENABLE;
    /* send ack msg here */
    if(CHECK_ACK(addr)){
        myDevice.cmdControl.singleSetLedState.sendCmdEnable = CMD_ENABLE;
        myDevice.cmdControl.singleSetLedState.sendCmdDelay = 0;
    } 
}

void onCmdModifyBaseSetting(uint8_t *data, uint8_t length)
{
    uint8_t pos = 0;
    uint8_t address;
    uint8_t autoReportFlag;

	/* autoReportFlag */
    autoReportFlag = data[pos++];
	/* address */
    address = data[pos++];

    if(CHECK_ADDR_INVALID(address)){
        printf("[%s]addr is not matched!\r\n", __FUNCTION__);
        return;
    }

    /* set dev status */
    myDevice.address = address;
    myDevice.autoReportFlag = autoReportFlag;
	/* save setting */
	user_database_save();
    /* send dev status here */
    if(CHECK_ACK(address)){
        myDevice.cmdControl.singleBasicSetting.sendCmdEnable = CMD_ENABLE;
        myDevice.cmdControl.singleBasicSetting.sendCmdDelay = 0;
    }  
}

void onCmdMultiSetAutoReportFlag(uint8_t *data, uint8_t length)
{
    uint8_t pos = 0;
    uint8_t autoReportFlag;

    if(length < 1){
       // printf("[%s]length error!\r\n", __FUNCTION__);
        return;
    }

    autoReportFlag = data[pos++];

    myDevice.autoReportFlag = autoReportFlag;

    user_database_save();
}

void onCmdMultiSetSingleLedState(uint8_t *data, uint8_t length)
{
    uint8_t pos = 0;
    uint8_t status;
    uint8_t port;

    if(NULL == data){
        //printf("[%s]data is null!\r\n", __FUNCTION__);
        return;
    }

    /* port */
    port = data[pos++] - 1;
    /* mode and state */
    status = data[pos++];
 

	if(port >= DEV_NUM){
		//printf("[%s]invalid port num!!!\r\n", __FUNCTION__);
		return;
	}

    /* set dev state here */
   	myDevice.devCtrl[port].mode = (status >> 4) & 0x0f;
	if(myDevice.devCtrl[port].mode){
		myDevice.devCtrl[port].ledState = status & 0x0f;
	}else{
		myDevice.devCtrl[port].ledState = myDevice.devCtrl[port].outState ? LED_STATE_GREEN_ON : LED_STATE_RED_ON;
	}
	myDevice.devCtrl[port].send = CMD_DISABLE;
}

void onCmdMultiSetAllLedState(uint8_t *data, uint8_t length)
{
    uint8_t pos = 0;
    uint16_t mode = 0;
    uint8_t ledState[DEV_NUM] = {0};
    uint8_t i;

    if(NULL == data){
      //  printf("[%s]data is null!\r\n", __FUNCTION__);
        return;
    }

    /* mode */
	mode = (data[pos++] << 8);
	mode += data[pos++];
    /* led state */
	for(i=0;i<DEV_NUM;i+=2){
		ledState[DEV_NUM - 1 - i] = (data[pos] >> 4) & 0x0f;
		ledState[DEV_NUM - 2 - i] = (data[pos++] & 0x0f);
	}


    /* set dev state here */
	for(i=0;i<DEV_NUM;i++){
		myDevice.devCtrl[i].mode = (mode >> i) & 0x01;
		if(myDevice.devCtrl[i].mode){
			myDevice.devCtrl[i].ledState = ledState[i];
		}else{
			myDevice.devCtrl[i].ledState = myDevice.devCtrl[i].outState ? LED_STATE_GREEN_ON : LED_STATE_RED_ON;
		}
	}
}

void onCmdSingleModifyBaudRate(uint8_t *data, uint8_t length)
{
    uint8_t addr = 0;
    uint8_t pos = 0;
    uint8_t baudRateIndex = 0;

    baudRateIndex = data[pos++];

    /* addr */
    addr = data[pos++];
    if(IS_ADDR_INVALID(addr)){
        printf("[%s]addr is not matched!\r\n", __FUNCTION__);
        return;
    }
 
    myDevice.baudRateIndex = baudRateIndex;

    user_database_save();

    /* send ack msg */
    myDevice.cmdControl.singleModifyBaudRate.sendCmdEnable = CMD_ENABLE;
    myDevice.cmdControl.singleModifyBaudRate.sendCmdDelay = 0;
}

void onCmdMultiModifyBaudRate(uint8_t *data, uint8_t length)
{
    uint16_t pos = 0;
    uint8_t baudRateIndex = 0;

    baudRateIndex = data[pos++];

    myDevice.baudRateIndex = baudRateIndex;

    user_database_save();
    
    while(1);//wait for watchdog reset  
}

void onCmdSetAddrByUid(uint8_t *data, uint8_t length)
{
    uint8_t pos = 0;
    uint8_t addr = 0;
    uint32_t uid0, uid1, uid2;

    /* uid */
    uid0 = (data[pos++] << 24) + (data[pos++] << 16) + (data[pos++] << 8) + data[pos++];
    uid1 = (data[pos++] << 24) + (data[pos++] << 16) + (data[pos++] << 8) + data[pos++];
    uid2 = (data[pos++] << 24) + (data[pos++] << 16) + (data[pos++] << 8) + data[pos++];

    if(IS_UID_INVALID(uid0, uid1, uid2)){
        printf("[%s]uid is not matched!\r\n", __FUNCTION__);
        return;

    }

    /* addr */
    addr = data[pos++];
    if(BROADCAST_ADDR == addr){
        printf("[%s]can not set broadcast addr!\r\n", __FUNCTION__);
        return;
    }

    myDevice.address = addr;

    user_database_save();

    /* send ack msg */
    myDevice.cmdControl.setAddrByUid.sendCmdEnable = CMD_ENABLE;
    myDevice.cmdControl.setAddrByUid.sendCmdDelay = 0;
}

void onCmdSetAddrByAddr(uint8_t *data, uint8_t length)
{
    uint8_t pos = 0;
    uint8_t addr = 0;
    uint8_t newAddr = 0;

    /* addr */
    newAddr = data[pos++];
    addr = data[pos++];

    if(IS_ADDR_INVALID(addr)){
        printf("[%s]addr is not matched!\r\n", __FUNCTION__);
        return;
    }

    if(BROADCAST_ADDR == newAddr){
        printf("[%s]can not set broadcast addr!\r\n", __FUNCTION__);
        return;
    }
    myDevice.address = newAddr;
    myDevice.oldAddress = addr;
    user_database_save();
    /* send ack msg */
    myDevice.cmdControl.setAddrByAddr.sendCmdEnable = CMD_ENABLE;
    myDevice.cmdControl.setAddrByAddr.sendCmdDelay = 0;
}

void onCmdGetInfoByAddr(uint8_t *data, uint8_t length)
{
    uint8_t pos = 0;
    uint8_t addr = 0;

    /* addr */
    addr = data[pos++];
    if(IS_ADDR_INVALID(addr)){
        printf("[%s]addr is not matched!\r\n", __FUNCTION__);
        return;
    }

    /* send ack msg */
    myDevice.cmdControl.getInfo.sendCmdEnable = CMD_ENABLE;
    myDevice.cmdControl.getInfo.sendCmdDelay = 0;
}

void onCmdClearUartBuffer(uint8_t *data, uint8_t length)
{
    uint8_t addr = 0;
    uint8_t pos = 0;

    addr = data[pos++];
    if(IS_ADDR_INVALID(addr)){
        printf("[%s]address is not matched!\r\n", __FUNCTION__);
        return;
    }

    /* clear uart buffer here */
    user_set_clear_buffer_flag(1);
    
    /* send ack msg here */
    myDevice.cmdControl.clrUartBuffer.sendCmdEnable = CMD_ENABLE;
    myDevice.cmdControl.clrUartBuffer.sendCmdDelay = 0;
}

void onCmdFactoryQuery(uint8_t *data, uint8_t length)
{
    /* send ack msg here */
    myDevice.cmdControl.factoryCmd.sendCmdEnable = CMD_ENABLE;
    myDevice.cmdControl.factoryCmd.sendCmdDelay = 0;
}

void onCmdRequestUpgrade(uint8_t *data, uint8_t length)
{
    uint8_t addr = 0;
    uint8_t pos = 0;

    addr = data[pos++];
    upgradeStatus = data[pos++];
    packetSize = (data[pos++] << 24);
    packetSize += (data[pos++] << 16);
    packetSize += (data[pos++] << 8);
    packetSize += data[pos++];

    packetIndex = (data[pos++] << 24);
    packetIndex += (data[pos++] << 16);
    packetIndex += (data[pos++] << 8);
    packetIndex += data[pos++];

    for(uint8_t i=0;i<PACKET_SIZE;i++){
        packetData[i] = data[pos++];
    }

    if(IS_ADDR_INVALID(addr)){
        printf("[%s]address is not matched!\r\n", __FUNCTION__);
        return;
    }

    if(upgradeStatus != STATUS_REQUEST_UPGRADE && upgradeStatus != STATUS_UPGRADE_GOING){
        printf("[%s]request upgrade failed!\r\n", __FUNCTION__);
        return;
    }

    /* send ack msg here */
    myDevice.cmdControl.upgrade.sendCmdEnable = CMD_ENABLE;
    myDevice.cmdControl.upgrade.sendCmdDelay = 0;
}

void onReportAllStatus(void)
{
    uint8_t buffer[100];
    uint8_t pos = 0;
	uint16_t onFlag = 0;
	uint16_t mode = 0;
    
	/* auto report flag */
	buffer[pos++] = myDevice.autoReportFlag;
 	/* baudRate */
	buffer[pos++] = myDevice.baudRateIndex;
	/* switch onoff flag */
	for(uint8_t i=0;i<DEV_NUM;i++){
		onFlag += (myDevice.devCtrl[i].outState & 0x01) << i;
		mode += (myDevice.devCtrl[i].mode & 0x01) << i;
	}
	buffer[pos++] = (onFlag >> 8) & 0xff;
	buffer[pos++] = onFlag & 0xff;
	/* led control mode */
	buffer[pos++] = (mode >> 8) & 0xff;
	buffer[pos++] = mode & 0xff;
	/* led state */
	buffer[pos++] = ((myDevice.devCtrl[11].ledState << 4) & 0xf0) + (myDevice.devCtrl[10].ledState & 0x0f);
	buffer[pos++] = ((myDevice.devCtrl[9].ledState << 4) & 0xf0) + (myDevice.devCtrl[8].ledState & 0x0f);
	buffer[pos++] = ((myDevice.devCtrl[7].ledState << 4) & 0xf0) + (myDevice.devCtrl[6].ledState & 0x0f);
	buffer[pos++] = ((myDevice.devCtrl[5].ledState << 4) & 0xf0) + (myDevice.devCtrl[4].ledState & 0x0f);
	buffer[pos++] = ((myDevice.devCtrl[3].ledState << 4) & 0xf0) + (myDevice.devCtrl[2].ledState & 0x0f);
	buffer[pos++] = ((myDevice.devCtrl[1].ledState << 4) & 0xf0) + (myDevice.devCtrl[0].ledState & 0x0f);	
    /* addr */
    buffer[pos++] = myDevice.address;

    user_protocol_send_data(CMD_ACK, OPT_CODE_SINGLE_DEV_QUERY_STATUS, buffer, pos);       
}

void onReportSetSingleLedState(void)
{
    uint8_t buffer[50];
    uint8_t pos = 0;
    uint8_t port = 0;
	while(1){
		pos = 0;
		if(myDevice.devCtrl[port].send == CMD_DISABLE){
			port ++;
			if(port >= DEV_NUM) 
				return;
			else 
				continue;
		}

		/* port */
    	buffer[pos++] = port + 1;
		/* mode and state */
		buffer[pos++] = ((myDevice.devCtrl[port].mode << 4) & 0xf0) + (myDevice.devCtrl[port].ledState & 0x0f);
		/* addr */
        buffer[pos++] = myDevice.address;

		user_protocol_send_data(CMD_ACK, OPT_CODE_SINGLE_DEV_SET_SINGLE_LED_STATE, buffer, pos); 

		myDevice.devCtrl[port].send = CMD_DISABLE;

		port ++;
		if(port >= DEV_NUM) return;
	}
}

void onReportSetAllLedState(void)
{
    uint8_t buffer[100];
    uint8_t pos = 0;
    
	/* mode */
	buffer[pos] = 0;
	for(int i;i<DEV_NUM;i++){
		buffer[pos] += ((myDevice.devCtrl[i].mode & 0x01) << i);
	}
	pos ++;
    /* led state */
	buffer[pos++] = (myDevice.devCtrl[11].ledState << 4) + (myDevice.devCtrl[10].ledState & 0x0f);
	buffer[pos++] = (myDevice.devCtrl[9].ledState << 4) + (myDevice.devCtrl[8].ledState & 0x0f);
	buffer[pos++] = (myDevice.devCtrl[7].ledState << 4) + (myDevice.devCtrl[6].ledState & 0x0f);
	buffer[pos++] = (myDevice.devCtrl[5].ledState << 4) + (myDevice.devCtrl[4].ledState & 0x0f);
	buffer[pos++] = (myDevice.devCtrl[3].ledState << 4) + (myDevice.devCtrl[2].ledState & 0x0f);
	buffer[pos++] = (myDevice.devCtrl[1].ledState << 4) + (myDevice.devCtrl[0].ledState & 0x0f);
    /* addr */
    buffer[pos++] = myDevice.address;

    user_protocol_send_data(CMD_ACK, OPT_CODE_SINGLE_DEV_SET_MULTI_LED_STATE, buffer, pos);     
}

void onReportAlarmType(void)
{
    uint8_t buffer[100];
    uint8_t pos = 0;
	uint8_t port = 0;
	while(1){
		pos = 0;
		if(myDevice.repCtrl[port].enable){
			/* port */
		    buffer[pos++] = port + 1;
			/* type */
			buffer[pos++] = myDevice.repCtrl[port].type;
			/* mode and led state */
			buffer[pos++] = (myDevice.devCtrl[port].mode << 4) + (myDevice.devCtrl[port].ledState & 0x0f);
			/* address */
		    buffer[pos++] = myDevice.address;

		    user_protocol_send_data(CMD_QUERY, OPT_CODE_REPORT_DEV_ALARM, buffer, pos);  

			myDevice.repCtrl[port].enable = 0;
		}
		port ++;
		if(port >= DEV_NUM) return;
	}
}

void onReportBaseSetting(void)
{
    uint8_t buffer[100];
    uint8_t pos = 0;
	
	/* auto report flag */
    buffer[pos++] = myDevice.autoReportFlag;
	/* address */
    buffer[pos++] = myDevice.address;

    user_protocol_send_data(CMD_ACK, OPT_CODE_SINGLE_DEV_MODIFY_BASE_SETTING, buffer, pos);     
}

void onReportAddrAndUid(void)
{
    uint8_t buffer[100];
    uint8_t pos = 0;
	/* address */
    buffer[pos++] = myDevice.address;
	/* button state, not used */
    buffer[pos++] = 1;
	/* uid */
    buffer[pos++] = (myDevice.uid0 >> 24)& 0xff;
    buffer[pos++] = (myDevice.uid0 >> 16) & 0xff;
    buffer[pos++] = (myDevice.uid0 >> 8) & 0xff;
    buffer[pos++] = myDevice.uid0 & 0xff;
    buffer[pos++] = (myDevice.uid1 >> 24)& 0xff;
    buffer[pos++] = (myDevice.uid1 >> 16) & 0xff;
    buffer[pos++] = (myDevice.uid1 >> 8) & 0xff;
    buffer[pos++] = myDevice.uid1 & 0xff;
    buffer[pos++] = (myDevice.uid2 >> 24)& 0xff;
    buffer[pos++] = (myDevice.uid2 >> 16) & 0xff;
    buffer[pos++] = (myDevice.uid2 >> 8) & 0xff;
    buffer[pos++] = myDevice.uid2 & 0xff;

    user_protocol_send_data(CMD_QUERY, OPT_CODE_REPORT_DEV_ADDR_AND_UID, buffer, pos); 

}

void onReportSingleModifyBaudRate(void)
{
    uint8_t buffer[23];
    uint8_t pos = 0;
   
    /* lock baudRateIndex */
    buffer[pos++] = myDevice.baudRateIndex;
     /* addr */
    buffer[pos++] = myDevice.address;

    user_protocol_send_data(CMD_ACK, OPT_CODE_SINGLE_MODIFY_BAUDRATE, buffer, pos); 

    while(1);//wait for watchdog reset 
}

void onReportSetAddrByUid(void)
{
    uint8_t buffer[32];
    uint8_t pos = 0;

    /* uid */
    buffer[pos++] = (myDevice.uid0 >> 24)& 0xff;
    buffer[pos++] = (myDevice.uid0 >> 16) & 0xff;
    buffer[pos++] = (myDevice.uid0 >> 8) & 0xff;
    buffer[pos++] = myDevice.uid0 & 0xff;
    buffer[pos++] = (myDevice.uid1 >> 24)& 0xff;
    buffer[pos++] = (myDevice.uid1 >> 16) & 0xff;
    buffer[pos++] = (myDevice.uid1 >> 8) & 0xff;
    buffer[pos++] = myDevice.uid1 & 0xff;
    buffer[pos++] = (myDevice.uid2 >> 24)& 0xff;
    buffer[pos++] = (myDevice.uid2 >> 16) & 0xff;
    buffer[pos++] = (myDevice.uid2 >> 8) & 0xff;
    buffer[pos++] = myDevice.uid2 & 0xff;

    /* addr */
    buffer[pos++] = myDevice.address;

    user_protocol_send_data(CMD_ACK, OPT_CODE_SET_ADDR_BY_UID, buffer, pos);
}

void onReportSetAddrByAddr(void)
{
    uint8_t buffer[32];
    uint8_t pos = 0;

    /* addr */
    buffer[pos++] = myDevice.address;
    buffer[pos++] = myDevice.oldAddress;

    user_protocol_send_data(CMD_ACK, OPT_CODE_SET_ADDR_BY_ADDR, buffer, pos);
}

void onReportGetInfoByAddr(void)
{
    uint8_t buffer[32];
    uint8_t pos = 0;

    /* uid */
    buffer[pos++] = (myDevice.uid0 >> 24)& 0xff;
    buffer[pos++] = (myDevice.uid0 >> 16) & 0xff;
    buffer[pos++] = (myDevice.uid0 >> 8) & 0xff;
    buffer[pos++] = myDevice.uid0 & 0xff;
    buffer[pos++] = (myDevice.uid1 >> 24)& 0xff;
    buffer[pos++] = (myDevice.uid1 >> 16) & 0xff;
    buffer[pos++] = (myDevice.uid1 >> 8) & 0xff;
    buffer[pos++] = myDevice.uid1 & 0xff;
    buffer[pos++] = (myDevice.uid2 >> 24)& 0xff;
    buffer[pos++] = (myDevice.uid2 >> 16) & 0xff;
    buffer[pos++] = (myDevice.uid2 >> 8) & 0xff;
    buffer[pos++] = myDevice.uid2 & 0xff;

    /* version */
    buffer[pos++] = (VERSION >> 8U) & 0xff;
    buffer[pos++] = VERSION & 0xff;

    /* addr */
    buffer[pos++] = myDevice.address;

    user_protocol_send_data(CMD_ACK, OPT_CODE_GET_INFO_BY_ADDR, buffer, pos);
}

void onReportClearUartBuffer(void)
{
    uint8_t buffer[23];
    uint8_t pos = 0;
    
    /* addr */
    buffer[pos++] = myDevice.address;

    user_protocol_send_data(CMD_ACK, OPT_CODE_CLEAR_UART_BUFFER, buffer, pos);     
}

void onReportFactoryCmd(void)
{
    uint8_t buffer[50];
    uint8_t pos = 0;

    /* type */
    buffer[pos++] = CMD_ACK;

    /* addr */
    buffer[pos++] = myDevice.address;

    /* uid */
    buffer[pos++] = (myDevice.uid0 >> 24)& 0xff;
    buffer[pos++] = (myDevice.uid0 >> 16) & 0xff;
    buffer[pos++] = (myDevice.uid0 >> 8) & 0xff;
    buffer[pos++] = myDevice.uid0 & 0xff;
    buffer[pos++] = (myDevice.uid1 >> 24)& 0xff;
    buffer[pos++] = (myDevice.uid1 >> 16) & 0xff;
    buffer[pos++] = (myDevice.uid1 >> 8) & 0xff;
    buffer[pos++] = myDevice.uid1 & 0xff;
    buffer[pos++] = (myDevice.uid2 >> 24)& 0xff;
    buffer[pos++] = (myDevice.uid2 >> 16) & 0xff;
    buffer[pos++] = (myDevice.uid2 >> 8) & 0xff;
    buffer[pos++] = myDevice.uid2 & 0xff;

    user_protocol_send_data(CMD_FACTORY_ACK, OPT_CODE_FACTORY_QUERY, buffer, pos); 
}

void onReportRequestUpgrade(void)
{

	printf("[%s]status: %d, packet index: %d, packet num: %d\r\n", __FUNCTION__, upgradeStatus, packetIndex, packetSize);
    uint8_t buffer[50];
    uint8_t pos = 0;

    switch(upgradeStatus){
        case STATUS_REQUEST_UPGRADE:{
             if(0 != write_upgrade_flag()){
                printf("Write upgrade flag fail!");
                return;
            }
            HAL_NVIC_SystemReset();//boot再回复
            return;
        }

        case STATUS_UPGRADE_GOING:{
            if(packetSize && packetSize == packetIndex){
                upgradeStatus = STATUS_UPGRADE_SUCCESS;
            }else{
                return;
            }
            break;
        }

        case STATUS_UPGRADE_SUCCESS:break;
        default:return;
    }

    /* 地址 */
    buffer[pos++] = myDevice.address;
    /* 状态 */
    buffer[pos++] = upgradeStatus;
    /* 包总数 */
    buffer[pos++] = (packetSize >> 24) & 0xFF;
    buffer[pos++] = (packetSize >> 16) & 0xFF;
    buffer[pos++] = (packetSize >> 8) & 0xFF;
    buffer[pos++] = packetSize & 0xFF;
    /* 包序号 */
    buffer[pos++] = (packetIndex >> 24) & 0xFF;
    buffer[pos++] = (packetIndex >> 16) & 0xFF;
    buffer[pos++] = (packetIndex >> 8) & 0xFF;
    buffer[pos++] = packetIndex & 0xFF;
    /* 数据 */
    for(int i =0; i< PACKET_SIZE;i++){
        buffer[pos++] = packetData[i];
    }

    // printf("[%s]status: %d, packet index: %d, packet num: %d\r\n", __FUNCTION__, upgradeStatus, packetIndex, packetSize);
	user_protocol_send_data(CMD_ACK, OPT_CODE_REQUEST_UPGRADE, buffer, pos);  
}


uint16_t user_read_flash(uint32_t address)
{
    return *(__IO uint16_t*)address;
}

void user_database_init(void)
{
    uint16_t i;
    uint16_t lenOfDataBase = sizeof(database_t) / sizeof(uint16_t);
    database_t readDataBase;
    uint16_t *pData = (uint16_t *)&readDataBase;

    memset((uint8_t *)&myDevice, 0, sizeof(myDevice));

   // printf("\r\n******************** App Start ********************\r\n");

    myDevice.uid0 = HAL_GetUIDw0();
    myDevice.uid1 = HAL_GetUIDw1();
    myDevice.uid2 = HAL_GetUIDw2();
    //printf("Read data:\r\n");
    for(i=0;i<lenOfDataBase;i++){
        pData[i] = user_read_flash(DATABASE_START_ADDR + 2U*i);
       //printf("%d\t", pData[i]);
    }
    //printf("\r\n");

    if(DATABASE_MAGIC != readDataBase.magic){
       // printf("Init Database!!!\r\n");
        myDevice.autoReportFlag = DEFAULT_AUTO_REPORT_FLAG;
        myDevice.baudRateIndex = DEFAULT_BAUD_RATE_INDEX;
        user_database_save();
    }else{
      //  printf("Read database from flash!!!\r\n");
        myDevice.address = (uint8_t )readDataBase.address;
        myDevice.autoReportFlag = (uint8_t)readDataBase.autoReportFlag;
        myDevice.baudRateIndex = (readDataBase.baudRateIndex == 0xffff) ? DEFAULT_BAUD_RATE_INDEX : readDataBase.baudRateIndex;
    }

    for(int i=0;i<DEV_NUM;i++){
        myDevice.devCtrl[i].lastOutState = 0xff;
        myDevice.devCtrl[i].outState     = 0xff;
    }
}

void user_database_save(void)
{
    uint16_t i;
    uint16_t lenOfDataBase = sizeof(database_t) / sizeof(uint16_t);
    database_t writeDataBase;
    uint16_t *pData = (uint16_t *)&writeDataBase;
    HAL_StatusTypeDef status;
    FLASH_EraseInitTypeDef flashEraseInitType;
    uint32_t PageError;

    writeDataBase.magic = DATABASE_MAGIC;
    writeDataBase.address = myDevice.address;
    writeDataBase.autoReportFlag = myDevice.autoReportFlag;

    writeDataBase.baudRateIndex = myDevice.baudRateIndex;

    HAL_FLASH_Unlock();

    flashEraseInitType.TypeErase = FLASH_TYPEERASE_PAGES;
    flashEraseInitType.PageAddress = DATABASE_START_ADDR;
    flashEraseInitType.NbPages = 1;
    status = HAL_FLASHEx_Erase(&flashEraseInitType, &PageError);
    
    if(HAL_OK != status){
        HAL_FLASH_Lock();
       // printf("Flash erase error: %d\r\n", status);
        return;
    }
    //printf("save data:\r\n");
    for(i=0;i<lenOfDataBase;i++){
        //printf("%d\t", pData[i]);
       if(HAL_OK != HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, DATABASE_START_ADDR + 2U*i, pData[i])){
         //   printf("[%s]write data[%d] fail!\r\n", __FUNCTION__, i);
       } 
    }
    //printf("\r\n");
    HAL_FLASH_Lock();
}

void user_reply_handle(void)
{
    uint16_t funcNum = sizeof(report_cmd) / sizeof(cmd_report_t);
    for(uint16_t i = 0; i < funcNum; i++){
        if(*report_cmd[i].cmdEnable && !(*report_cmd[i].cmdDelay)){
            *report_cmd[i].cmdEnable = CMD_DISABLE;
            report_cmd[i].func();
        }
    }
}

void user_check_report_delay(void)
{
    for(uint8_t i=0; report_cmd[i].cmdEnable; i++){
        if(*report_cmd[i].cmdEnable && (*report_cmd[i].cmdDelay > 0)){
            *report_cmd[i].cmdDelay --;
        }
    }
}

void printSetting(void)
{
	printf("Version: 0x%04x\r\n", VERSION);
	printf("Chip uuid: 0x%04x%04x%04x\r\n", myDevice.uid0, myDevice.uid1, myDevice.uid2);
	printf("address: 0x%02X\r\n", myDevice.address);
	printf("autoReportFlag: %d\r\n", myDevice.autoReportFlag);
	printf("baudRateIndex: %d\r\n", myDevice.baudRateIndex);

    sync_boot_env();
}

int user_write_flash(uint32_t address, uint16_t *data, uint16_t size)
{
    HAL_StatusTypeDef status;
    FLASH_EraseInitTypeDef flashEraseInitType;
    uint32_t PageError;
    int ret = 0;

    if(NULL == data)    return -1;

    HAL_FLASH_Unlock();

    flashEraseInitType.TypeErase = FLASH_TYPEERASE_PAGES;
    flashEraseInitType.PageAddress = address;
    flashEraseInitType.NbPages = 1;
    status = HAL_FLASHEx_Erase(&flashEraseInitType, &PageError);
    
    if(HAL_OK != status){
        HAL_FLASH_Lock();
        printf("Flash erase error: %d\r\n", status);
        return -1;
    }

    for(uint16_t i=0;i<size;i++){
        if(HAL_OK != HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address + 2U*i, data[i])){
            printf("Write data[%d] fail!\r\n", i);
            ret = -1;
        }
    }

    HAL_FLASH_Lock();

    return ret;
}

int write_upgrade_flag(void)
{
    upgrade_t upgradeData;
    uint16_t *pData = NULL;
    uint16_t lenOfDataBase = sizeof(upgrade_t) / sizeof(uint16_t);

    upgradeData.magic = DATABASE_MAGIC;
    upgradeData.address = myDevice.address;
    upgradeData.deviceCmd = CMD_QUERY;
    upgradeData.baudIndex = myDevice.baudRateIndex;
    upgradeData.upgradeFlag = APP_UPGREQ_IS_VALID;

    pData = (uint16_t *)&upgradeData;

    return user_write_flash(APP_UPGRADE_FLAG_ADDRESS, pData, lenOfDataBase);
}

void sync_boot_env(void)
{
    uint16_t i;
    uint16_t lenOfDataBase = sizeof(upgrade_t) / sizeof(uint16_t);
    upgrade_t readDataBase;
    uint16_t *pData = (uint16_t *)&readDataBase;

    for(i=0;i<lenOfDataBase;i++){
        pData[i] = user_read_flash(APP_UPGRADE_FLAG_ADDRESS + 2U*i);
    }

    if(DATABASE_MAGIC == readDataBase.magic && readDataBase.upgradeStatus == STATUS_UPGRADE_SUCCESS){
        /* sync boot env */
        upgradeStatus = STATUS_UPGRADE_SUCCESS;
        packetSize = readDataBase.packetSize;
        packetIndex = readDataBase.packetIndex;
        for(uint8_t i=0;i<PACKET_SIZE;i++){
            packetData[i] = readDataBase.packetData[i];
        }
        /* send msg */
        myDevice.cmdControl.upgrade.sendCmdEnable = CMD_ENABLE;
        myDevice.cmdControl.upgrade.sendCmdDelay = 0;
        /* clear upgrade status */
        readDataBase.upgradeStatus = 0;
        user_write_flash(APP_UPGRADE_FLAG_ADDRESS, pData, lenOfDataBase);
        printf("Upgrade done!!!!!\r\n");
    }
}


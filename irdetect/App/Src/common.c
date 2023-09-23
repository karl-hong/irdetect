#include "main.h"
#include "common.h"
#include "user_data.h"

dev_ctx_t myDevice;
static uint16_t timeBase = 0;

void tim_interrupt_callback(void)
{
    /* 1ms */
    timeBase ++;   
    if(timeBase >= 100){
        /* 100ms */
        timeBase = 0;
			
				user_check_report_delay();
				
    }
		
	if(myDevice.checkPeriod > 0)													myDevice.checkPeriod --;
		
	for(int i=0;i<DEV_NUM;i++){
		if(myDevice.devCtrl[i].ledFlashCnt > 0)							myDevice.devCtrl[i].ledFlashCnt --;
	}
}





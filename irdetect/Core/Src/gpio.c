/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */
#include "common.h"
typedef struct {
	GPIO_TypeDef *greenLedPort;
	uint16_t	  greenLedPin;
	GPIO_TypeDef *redLedPort;
	uint16_t 	  redLedPin;
	GPIO_TypeDef *irEnPort;
	uint16_t	  irEnPin;
	GPIO_TypeDef *outPort;
	uint16_t	  outPin;	
}gpio_ctrl_t;
/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */
gpio_ctrl_t pinCtrl[DEV_NUM] = {
	{LEDG1_GPIO_Port, LEDG1_Pin, LEDR1_GPIO_Port, LEDR1_Pin, EN1_GPIO_Port, EN1_Pin, OUT1_GPIO_Port, OUT1_Pin},
	{LEDG2_GPIO_Port, LEDG2_Pin, LEDR2_GPIO_Port, LEDR2_Pin, EN2_GPIO_Port, EN2_Pin, OUT2_GPIO_Port, OUT2_Pin},
	{LEDG3_GPIO_Port, LEDG3_Pin, LEDR3_GPIO_Port, LEDR3_Pin, EN3_GPIO_Port, EN3_Pin, OUT3_GPIO_Port, OUT3_Pin},
	{LEDG4_GPIO_Port, LEDG4_Pin, LEDR4_GPIO_Port, LEDR4_Pin, EN4_GPIO_Port, EN4_Pin, OUT4_GPIO_Port, OUT4_Pin},
	{LEDG5_GPIO_Port, LEDG5_Pin, LEDR5_GPIO_Port, LEDR5_Pin, EN5_GPIO_Port, EN5_Pin, OUT5_GPIO_Port, OUT5_Pin},
	{LEDG6_GPIO_Port, LEDG6_Pin, LEDR6_GPIO_Port, LEDR6_Pin, EN6_GPIO_Port, EN6_Pin, OUT6_GPIO_Port, OUT6_Pin},
	{LEDG7_GPIO_Port, LEDG7_Pin, LEDR6B13_GPIO_Port, LEDR6B13_Pin, EN7_GPIO_Port, EN7_Pin, OUT7_GPIO_Port, OUT7_Pin},
	{LEDG8_GPIO_Port, LEDG8_Pin, LEDR8_GPIO_Port, LEDR8_Pin, EN8_GPIO_Port, EN8_Pin, OUT8_GPIO_Port, OUT8_Pin},
	{LEDG9_GPIO_Port, LEDG9_Pin, LEDR9_GPIO_Port, LEDR9_Pin, EN9_GPIO_Port, EN9_Pin, OUT9_GPIO_Port, OUT9_Pin},
	{LEDG10_GPIO_Port, LEDG10_Pin, LEDR10_GPIO_Port, LEDR10_Pin, EN10_GPIO_Port, EN10_Pin, OUT10_GPIO_Port, OUT10_Pin},
	{LEDG11_GPIO_Port, LEDG11_Pin, LEDR11_GPIO_Port, LEDR11_Pin, EN11_GPIO_Port, EN11_Pin, OUT11_GPIO_Port, OUT11_Pin},
	{LEDG12_GPIO_Port, LEDG12_Pin, LEDR12_GPIO_Port, LEDR12_Pin, EN12_GPIO_Port, EN12_Pin, OUT12_GPIO_Port, OUT12_Pin},

};
/* USER CODE END 1 */

/** Configure pins
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LEDG1_Pin|LEDR1_Pin|LEDR2_Pin|EN2_Pin
                          |LEDG3_Pin|LEDR5_Pin|LEDG8_Pin|LEDR8_Pin
                          |EN8_Pin|LEDR10_Pin|EN10_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, EN1_Pin|LEDG2_Pin|EN3_Pin|LEDG4_Pin
                          |EN9_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LEDR3_Pin|LEDR4_Pin|EN4_Pin|LEDG5_Pin
                          |LEDG9_Pin|LEDR9_Pin|LEDG10_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, EN5_Pin|LEDG6_Pin|LEDR6_Pin|EN6_Pin
                          |LEDG7_Pin|LEDR6B13_Pin|EN7_Pin|LEDR11_Pin
                          |EN11_Pin|LEDG12_Pin|LEDR12_Pin|EN12_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LEDG11_GPIO_Port, LEDG11_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PCPin PCPin */
  GPIO_InitStruct.Pin = LEDG1_Pin|LEDR1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PCPin PCPin PCPin PCPin
                           PCPin */
  GPIO_InitStruct.Pin = OUT1_Pin|OUT2_Pin|OUT5_Pin|OUT8_Pin
                          |OUT10_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PFPin PFPin PFPin PFPin
                           PFPin */
  GPIO_InitStruct.Pin = EN1_Pin|LEDG2_Pin|EN3_Pin|LEDG4_Pin
                          |EN9_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PCPin PCPin PCPin PCPin
                           PCPin PCPin PCPin PCPin
                           PCPin */
  GPIO_InitStruct.Pin = LEDR2_Pin|EN2_Pin|LEDG3_Pin|LEDR5_Pin
                          |LEDG8_Pin|LEDR8_Pin|EN8_Pin|LEDR10_Pin
                          |EN10_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin PAPin PAPin
                           PAPin PAPin PAPin */
  GPIO_InitStruct.Pin = LEDR3_Pin|LEDR4_Pin|EN4_Pin|LEDG5_Pin
                          |LEDG9_Pin|LEDR9_Pin|LEDG10_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin PAPin */
  GPIO_InitStruct.Pin = OUT3_Pin|UID_Pin|OUT4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin PBPin
                           PBPin PBPin PBPin PBPin
                           PBPin PBPin PBPin PBPin */
  GPIO_InitStruct.Pin = EN5_Pin|LEDG6_Pin|LEDR6_Pin|EN6_Pin
                          |LEDG7_Pin|LEDR6B13_Pin|EN7_Pin|LEDR11_Pin
                          |EN11_Pin|LEDG12_Pin|LEDR12_Pin|EN12_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin PBPin */
  GPIO_InitStruct.Pin = OUT6_Pin|OUT7_Pin|OUT11_Pin|OUT12_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = OUT9_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(OUT9_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = LEDG11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LEDG11_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */
/* led state function */
void user_set_led_state(uint8_t port, uint8_t led, uint8_t state)
{
	if(port >= DEV_NUM){
		return;
	}
	
	switch(led){
		case LED_RED:{
			HAL_GPIO_WritePin(pinCtrl[port].redLedPort, pinCtrl[port].redLedPin, (GPIO_PinState)state);
			break;
		}

		case LED_GREEN:{
			HAL_GPIO_WritePin(pinCtrl[port].greenLedPort, pinCtrl[port].greenLedPin, (GPIO_PinState)state);
			break;
		}

		default:break;
	}
}

/* enable ir function */
void user_enable_ir(uint8_t port, uint8_t state)
{
	if(port >= DEV_NUM){
		return;
	}
	HAL_GPIO_WritePin(pinCtrl[port].irEnPort, pinCtrl[port].irEnPin, (GPIO_PinState)state);
}

/* get out status */
uint8_t user_get_out_status(uint8_t port)
{
	if(port >= DEV_NUM){
		return 0;
	}

	return (uint8_t)HAL_GPIO_ReadPin(pinCtrl[port].outPort, pinCtrl[port].outPin);
}

static void red_led_flash(dev_ctrl_t *dev_ctr)
{
	if(0 < dev_ctr->ledFlashCnt) return;

	dev_ctr->flashStatus = !dev_ctr->flashStatus;

	user_set_led_state(dev_ctr->port, LED_GREEN, LED_OFF);
	user_set_led_state(dev_ctr->port, LED_RED, dev_ctr->flashStatus);

	dev_ctr->ledFlashCnt = FLASH_FREQ;
}

static void green_led_flash(dev_ctrl_t *dev_ctr)
{
	if(0 < dev_ctr->ledFlashCnt) return;

	dev_ctr->flashStatus = !dev_ctr->flashStatus;

	user_set_led_state(dev_ctr->port, LED_RED, LED_OFF);
	user_set_led_state(dev_ctr->port, LED_GREEN, dev_ctr->flashStatus);

	dev_ctr->ledFlashCnt = FLASH_FREQ;
}

static void red_green_led_flash(dev_ctrl_t *dev_ctr)
{
	if(0 < dev_ctr->ledFlashCnt) return;
	
	dev_ctr->flashStatus = !dev_ctr->flashStatus;

	if(dev_ctr->flashStatus){
		user_set_led_state(dev_ctr->port, LED_GREEN, LED_ON);
		user_set_led_state(dev_ctr->port, LED_RED, LED_OFF);
	}else{
		user_set_led_state(dev_ctr->port, LED_RED, LED_ON);
		user_set_led_state(dev_ctr->port, LED_GREEN, LED_OFF);
	}

	dev_ctr->ledFlashCnt = FLASH_FREQ;
}

static void led_auto_mode(dev_ctrl_t *dev_ctr)
{
	if(dev_ctr->outState){
		/* 设备在位*/
		user_set_led_state(dev_ctr->port, LED_GREEN, LED_ON);
		user_set_led_state(dev_ctr->port, LED_RED, LED_OFF);
	}else{
		/* 设备不在�? */
		user_set_led_state(dev_ctr->port, LED_RED, LED_ON);
		user_set_led_state(dev_ctr->port, LED_GREEN, LED_OFF);
	}
}

static void led_manual_mode(dev_ctrl_t *dev_ctr)
{
	switch(dev_ctr->ledState){
		case LED_STATE_OFF:{
			user_set_led_state(dev_ctr->port, LED_RED, LED_OFF);
			user_set_led_state(dev_ctr->port, LED_GREEN, LED_OFF);
			break;
		}

		case LED_STATE_RED_ON:{
			user_set_led_state(dev_ctr->port, LED_RED, LED_ON);
			user_set_led_state(dev_ctr->port, LED_GREEN, LED_OFF);
			break;
		}

		case LED_STATE_GREEN_ON:{
			user_set_led_state(dev_ctr->port, LED_RED, LED_OFF);
			user_set_led_state(dev_ctr->port, LED_GREEN, LED_ON);
			break;
		}

		case LED_STATE_RED_FLASH:{
			red_led_flash(dev_ctr);
			break;
		}

		case LED_STATE_GREEN_FLASH:{
			green_led_flash(dev_ctr);
			break;
		}

		case LED_STATE_RED_GREEN_FLASH:{
			red_green_led_flash(dev_ctr);
			break;
		}

		default:break;
	}
}

static void _led_task(dev_ctrl_t *dev_ctr)
{
	switch(dev_ctr->mode){
		case LED_MODE_AUTO:{
			led_auto_mode(dev_ctr);
			break;
		}

		case LED_MODE_MANUAL:{
			led_manual_mode(dev_ctr);
			break;
		}

		default:break;
	}
}

void user_led_task(void)
{
	for(int i=0;i<DEV_NUM;i++){
		myDevice.devCtrl[i].port = i;
		_led_task(&myDevice.devCtrl[i]);
	}
}

void user_check_device_status(void)
{
	static uint8_t checkIndex = 0;
	
	if(myDevice.devCtrl[checkIndex].irEnable == STATE_ENABLE){
		myDevice.devCtrl[checkIndex].lastOutState = user_get_out_status(checkIndex);
		
		if(myDevice.devCtrl[checkIndex].outState != myDevice.devCtrl[checkIndex].lastOutState){
			
			myDevice.devCtrl[checkIndex].outCnt ++;
			
			if(myDevice.devCtrl[checkIndex].outCnt >= 100){
					myDevice.devCtrl[checkIndex].outState = myDevice.devCtrl[checkIndex].lastOutState;

					if(myDevice.devCtrl[checkIndex].mode == 0){
						if(myDevice.devCtrl[checkIndex].outState)	myDevice.devCtrl[checkIndex].ledState = LED_STATE_GREEN_ON;
						else										myDevice.devCtrl[checkIndex].ledState = LED_STATE_RED_ON;
					}
						
					if(myDevice.autoReportFlag && myDevice.devCtrl[checkIndex].outStateInitFlag){
						
						myDevice.cmdControl.autoAlarm.sendCmdEnable = CMD_ENABLE;
						myDevice.cmdControl.autoAlarm.sendCmdDelay = 0;
						myDevice.repCtrl[checkIndex].enable = CMD_ENABLE;
						myDevice.repCtrl[checkIndex].type = myDevice.devCtrl[checkIndex].outState;
						
					}
					
					myDevice.devCtrl[checkIndex].outStateInitFlag = 1;
					myDevice.devCtrl[checkIndex].outCnt = 0;
			}	
		}else{
			myDevice.devCtrl[checkIndex].outCnt = 0;
		}
	}

	
	if(myDevice.checkPeriod) return;
	
	//disable last ir port
	user_enable_ir(checkIndex, STATE_DISABLE);
	myDevice.devCtrl[checkIndex].irEnable = STATE_DISABLE;
	
	checkIndex ++;
	if(checkIndex >= DEV_NUM) checkIndex = 0;
	//enable next ir port
	user_enable_ir(checkIndex, STATE_ENABLE);
	myDevice.devCtrl[checkIndex].irEnable = STATE_ENABLE;
	//reset time
	myDevice.checkPeriod = 50;//50ms
	
	
//	for(int i=0;i<DEV_NUM;i++){
//		myDevice.devCtrl[i].irEnable = STATE_ENABLE;
//		myDevice.devCtrl[i].port = i;
//	
//		user_enable_ir(i, STATE_ENABLE);

//		myDevice.devCtrl[i].outState = user_get_out_status(i);

//		
//		if(myDevice.devCtrl[i].outState != myDevice.devCtrl[i].lastOutState){
//			myDevice.devCtrl[i].lastOutState = myDevice.devCtrl[i].outState;
//			if(myDevice.autoReportFlag && myDevice.devCtrl[i].outStateInitFlag){
//				myDevice.cmdControl.autoAlarm.sendCmdEnable = CMD_ENABLE;
//				myDevice.cmdControl.autoAlarm.sendCmdDelay = 0;
//				myDevice.repCtrl[i].enable = CMD_ENABLE;
//				myDevice.repCtrl[i].type = myDevice.devCtrl[i].outState;
//			}
//			myDevice.devCtrl[i].outStateInitFlag = 1;
//		}

//		user_enable_ir(i, STATE_DISABLE);
//	}
}

void user_UID_Task(void)
{
	static uint8_t send = 0;
	static uint8_t curState = 0;
	static uint16_t cnt = 0;

	curState = HAL_GPIO_ReadPin(UID_GPIO_Port, UID_Pin);


	if(!curState ){
		if(send)	return;
		cnt ++;
		if(cnt > 100) return;

		cnt = 0;

		send = 1;

		if(myDevice.autoReportFlag){
			myDevice.cmdControl.reportAddrAndUid.sendCmdEnable = CMD_ENABLE;
			myDevice.cmdControl.reportAddrAndUid.sendCmdDelay = 0;
		}
		
	}else{
		cnt = 0;
		send = 0;
	}
	
}
/* USER CODE END 2 */

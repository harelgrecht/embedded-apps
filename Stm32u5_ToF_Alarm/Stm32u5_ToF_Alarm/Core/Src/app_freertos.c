/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : FreeRTOS applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "app_freertos.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
distanceHandler_t globalPayload;
extern ADC_HandleTypeDef hadc1;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for taskToF */
osThreadId_t taskToFHandle;
const osThreadAttr_t taskToF_attributes = {
  .name = "taskToF",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024 * 4
};
/* Definitions for alarmTask */
osThreadId_t alarmTaskHandle;
const osThreadAttr_t alarmTask_attributes = {
  .name = "alarmTask",
  .priority = (osPriority_t) osPriorityAboveNormal,
  .stack_size = 1024 * 4
};
/* Definitions for monitorTask */
osThreadId_t monitorTaskHandle;
const osThreadAttr_t monitorTask_attributes = {
  .name = "monitorTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024 * 4
};
/* Definitions for alarmMutex */
osMutexId_t alarmMutexHandle;
const osMutexAttr_t alarmMutex_attributes = {
  .name = "alarmMutex"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  /* USER CODE END Init */
  /* creation of alarmMutex */
  alarmMutexHandle = osMutexNew(&alarmMutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of taskToF */
  taskToFHandle = osThreadNew(startToF, NULL, &taskToF_attributes);

  /* creation of alarmTask */
  alarmTaskHandle = osThreadNew(startAlarm, NULL, &alarmTask_attributes);

  /* creation of monitorTask */
  monitorTaskHandle = osThreadNew(startMonitor, NULL, &monitorTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}
/* USER CODE BEGIN Header_StartDefaultTask */
/**
* @brief Function implementing the defaultTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN defaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END defaultTask */
}

/* USER CODE BEGIN Header_startToF */
/**
* @brief Function implementing the taskToF thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_startToF */
void startToF(void *argument)
{
  /* USER CODE BEGIN taskToF */
	if(initToF() != HAL_OK) {
		printf("Failed to init ToF module. Halting task\r\n");
		for(;;) osDelay(osWaitForever);
	}

	if (startToFSampling(SAMPLE_MODE_CONTINUOUS, IRQ_MODE_ENABLED) != HAL_OK) {
		printf("Failed to start ToF sampling. Halting task.\r\n");
		for(;;)
			osDelay(osWaitForever);
	}

//	distanceHandler_t localPayload;
	MovingAverageFilter_t distanceFilter;
	initMovingAverage(&distanceFilter);
	double distanceMeters;

	/* Infinite loop */
	for(;;) {
		performToFCalibration();
		distanceMeters = readToFDistance();
		if (distanceMeters >= 0) {
			if(osMutexAcquire(alarmMutexHandle, MUTEX_TIMEOUT) == osOK) {
				globalPayload.upTimeStamp = HAL_GetTick();
				globalPayload.distanceCM = updateMovingAverage(&distanceFilter, distanceMeters * M_TO_CM);
				osMutexRelease(alarmMutexHandle);
			} else {
				printf("Measurement failed.\r\n");
			}
		}
	}
	osDelay(100);
  /* USER CODE END taskToF */
}

/* USER CODE BEGIN Header_startAlarm */
/**
* @brief Function implementing the alarmTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_startAlarm */
void startAlarm(void *argument)
{
  /* USER CODE BEGIN alarmTask */
	printf("task alarm started\n\r");
	distanceHandler_t alarmTaskPayload;
	uint32_t hours, minutes, seconds, ms;
	int slowBlinkCnt = 0;
	/* Infinite loop */
	for(;;) {
		if(osMutexAcquire(alarmMutexHandle, MUTEX_TIMEOUT) == osOK) {
			alarmTaskPayload = globalPayload;
			osMutexRelease(alarmMutexHandle);

			calcUptime(alarmTaskPayload.upTimeStamp, &hours, &minutes, &seconds, &ms);

//			printf("Distance: %.2f cm | CPU Temp: ", alarmTaskPayload.distanceCM);
//			if (alarmTaskPayload.cpuTemp < -90.0f)
//			    printf("N/A");
//			 else
//			    printf("%.1f C", alarmTaskPayload.cpuTemp);
//			printf(" | Uptime: %02lu:%02lu:%02lu.%02lu\n\r", hours, minutes, seconds, ms);

			if(alarmTaskPayload.distanceCM < ALARM_10_CM) {
				printf("ALARM: Object is too close!\n\r");
				BSP_LED_Toggle(LED_RED);
				BSP_LED_Off(LED_BLUE);
				slowBlinkCnt = 0;
			} else if(alarmTaskPayload.distanceCM < ALARM_30_CM) {
				slowBlinkCnt++;
				if(slowBlinkCnt >= LED_LOGIC_LOOPS) {
					printf("Warnning: Object is getting close!\n\r");
					BSP_LED_Toggle(LED_BLUE);
					slowBlinkCnt = 0;
				}
				BSP_LED_Off(LED_RED);
			} else {
				BSP_LED_Off(LED_RED);
				BSP_LED_Off(LED_GREEN);
				BSP_LED_Off(LED_BLUE);
				slowBlinkCnt = 0;
			}
		}
		osDelay(50);
	}
  /* USER CODE END alarmTask */
}

/* USER CODE BEGIN Header_startMonitor */
/**
* @brief Function implementing the monitorTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_startMonitor */
void startMonitor(void *argument)
{
  /* USER CODE BEGIN monitorTask */
	float newTemp;
	uint32_t adcValue;
	if(osMutexAcquire(alarmMutexHandle, 100) == osOK) {
	    globalPayload.cpuTemp = -99.9f;
	    osMutexRelease(alarmMutexHandle);
	  }

  /* Infinite loop */
  for(;;) {
      if(HAL_ADC_Start(&hadc1) == HAL_OK) {
    	  if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK) {
    		  adcValue = HAL_ADC_GetValue(&hadc1);
    		  printf("startMonitor: Poll OK. Raw ADC Value = %lu\n\r", adcValue);

    		  newTemp = ((float)adcValue - *TEMPSENSOR_CAL1_ADDR) * \
    				  (TEMPSENSOR_CAL2_TEMP - TEMPSENSOR_CAL1_TEMP) / \
					  (*TEMPSENSOR_CAL2_ADDR - *TEMPSENSOR_CAL1_ADDR) + \
					  TEMPSENSOR_CAL1_TEMP;

    		  if(osMutexAcquire(alarmMutexHandle, 100) == osOK) {
    			  globalPayload.cpuTemp = newTemp;
    			  osMutexRelease(alarmMutexHandle);
    			  printf("startMonitor: SUCCESS! New temp: %.1f C\n\r", newTemp);
    		  }
      }
	  HAL_ADC_Stop(&hadc1);
	  osDelay(5000); // Wait 5 seconds
      }
  }
  /* USER CODE END monitorTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void blinkLed() {
	BSP_LED_On(LED_RED);
	BSP_LED_On(LED_GREEN);
	BSP_LED_On(LED_BLUE);
	osDelay(500); // ms
	BSP_LED_Off(LED_RED);
	BSP_LED_Off(LED_GREEN);
	BSP_LED_Off(LED_BLUE);
	osDelay(500);
}

void calcUptime(uint32_t timestamp, uint32_t *hh, uint32_t *mm, uint32_t *ss, uint32_t *ms) {
	uint32_t totalSeconds = timestamp / 1000;
	*hh = totalSeconds / 3600;
	*mm = (totalSeconds % 3600) / 60;
	*ss = totalSeconds % 60;
	*ms = timestamp / 100;
}
/* USER CODE END Application */


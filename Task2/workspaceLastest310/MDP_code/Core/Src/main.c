/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
DMA_HandleTypeDef hdma_adc1;
DMA_HandleTypeDef hdma_adc2;

I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;
TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim8;

UART_HandleTypeDef huart3;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for MotorTask */
osThreadId_t MotorTaskHandle;
const osThreadAttr_t MotorTask_attributes = {
  .name = "MotorTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for GyroTask */
osThreadId_t GyroTaskHandle;
const osThreadAttr_t GyroTask_attributes = {
  .name = "GyroTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for gyroBias */
osThreadId_t gyroBiasHandle;
const osThreadAttr_t gyroBias_attributes = {
  .name = "gyroBias",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for pid_controller */
osThreadId_t pid_controllerHandle;
const osThreadAttr_t pid_controller_attributes = {
  .name = "pid_controller",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for EncoderTask */
osThreadId_t EncoderTaskHandle;
const osThreadAttr_t EncoderTask_attributes = {
  .name = "EncoderTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for moveDistTask */
osThreadId_t moveDistTaskHandle;
const osThreadAttr_t moveDistTask_attributes = {
  .name = "moveDistTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for OledTask */
osThreadId_t OledTaskHandle;
const osThreadAttr_t OledTask_attributes = {
  .name = "OledTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for FRTask */
osThreadId_t FRTaskHandle;
const osThreadAttr_t FRTask_attributes = {
  .name = "FRTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for commandTask */
osThreadId_t commandTaskHandle;
const osThreadAttr_t commandTask_attributes = {
  .name = "commandTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for BRTask */
osThreadId_t BRTaskHandle;
const osThreadAttr_t BRTask_attributes = {
  .name = "BRTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for TestTask */
osThreadId_t TestTaskHandle;
const osThreadAttr_t TestTask_attributes = {
  .name = "TestTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for DispatchTask */
osThreadId_t DispatchTaskHandle;
const osThreadAttr_t DispatchTask_attributes = {
  .name = "DispatchTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for FLTask */
osThreadId_t FLTaskHandle;
const osThreadAttr_t FLTask_attributes = {
  .name = "FLTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for BLTask */
osThreadId_t BLTaskHandle;
const osThreadAttr_t BLTask_attributes = {
  .name = "BLTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for TurnRightAngle */
osThreadId_t TurnRightAngleHandle;
const osThreadAttr_t TurnRightAngle_attributes = {
  .name = "TurnRightAngle",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Navigate_Obst */
osThreadId_t Navigate_ObstHandle;
const osThreadAttr_t Navigate_Obst_attributes = {
  .name = "Navigate_Obst",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for ultrasonicTask */
osThreadId_t ultrasonicTaskHandle;
const osThreadAttr_t ultrasonicTask_attributes = {
  .name = "ultrasonicTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for FastestTask */
osThreadId_t FastestTaskHandle;
const osThreadAttr_t FastestTask_attributes = {
  .name = "FastestTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* USER CODE BEGIN PV */
// PID
float targetAngle = 0;
float angleNow = 0;
float speedScale = 1;

uint8_t readGyroZData[2];
int16_t gyroZ;
uint16_t newDutyL, newDutyR;
uint32_t last_curTask_tick = 0;

float targetDist1, targetDist2, targetDist3, targetDist4 = 0;
uint16_t curDistTick = 0;
uint16_t targetDistTick = 0;
uint16_t dist_dL = 0;
uint16_t lastDistTick_L = 0;

uint8_t aRxBuffer[4];
uint8_t Buffer[9];//test



uint8_t RX_BUFFER_SIZE = 4;

int flag=0;

typedef struct _command {
	uint8_t index;
	uint16_t val;
} Command;

uint8_t CMD_BUFFER_SIZE = 12;
typedef struct _commandQueue {
	uint8_t head;
	uint8_t tail;
	uint8_t size;
	Command buffer[12];
} CommandQueue;

CommandQueue cQueue;

Command curCmd;
uint8_t rxMsg[16];
char ch[16];

uint8_t manualMode = 0;

//PID
typedef struct _pidConfig {
	float Kp;
	float Ki;
	float Kd;
	float ek1;
	float ekSum;
} PIDConfig;

PIDConfig pidSlow, pidTSlow, pidFast, pidFast_b;


//Ultrasound Global Variables
uint32_t Echo_Val1 = 0;
uint32_t Echo_Val2 = 0;
uint32_t Difference = 0;
uint8_t Is_First_Captured = 0;
uint8_t Distance=999;
uint8_t UD=999;
int same=0;
uint16_t final_distance = 60;
int stime = 0;
double ultra_Distance  = 0; //Ultrasound distance

float obs_a, x, angle_left, angle_right;

uint16_t obsTick_IR = 0;

float obsDist_IR = 0, obsDist_US = 0;
//float IR_data_raw_acc = 0, dataPoint = 0;
uint16_t dataPoint = 0; uint32_t IR_data_raw_acc = 0;

float totalDistance = 0;

typedef struct _commandConfig {
	uint16_t leftDuty;
	uint16_t rightDuty;
	float servoTurnVal;
	float targetAngle;
	uint8_t direction;
} CmdConfig;

#define CONFIG_FL00 7
#define CONFIG_FR00 8
#define CONFIG_BL00 9
#define CONFIG_BR00 10

#define CONFIG_FL20 11
#define CONFIG_FR20 12
#define CONFIG_BR30 18

#define CONFIG_BL20 13
#define CONFIG_BR20 14

#define CONFIG_FL30 15
#define CONFIG_FR30 16
#define CONFIG_BL30 17
CmdConfig cfgs[20] = {
	{0,0,SERVO_CENTER,0, DIR_FORWARD}, // STOP
	{1200, 1200, SERVO_CENTER, 0, DIR_FORWARD}, // FW00
	{1200, 1200, SERVO_CENTER, 0, DIR_BACKWARD}, // BW00

	{800, 1200, 100, 0, DIR_FORWARD}, // FL--
	{1200, 800, 190, 0, DIR_FORWARD}, // FR--
	{800, 1200, 100, 0, DIR_BACKWARD}, // BL--
	{1200, 800, 190, 0, DIR_BACKWARD}, // BR--

	{700, 1800, 100, 89, DIR_FORWARD}, // FL00
	{1800, 400, 190 ,-87, DIR_FORWARD}, // FR00
	{500, 1700, 100, -88, DIR_BACKWARD}, // BL00
	{1800, 500, 190, 89, DIR_BACKWARD}, // BR00,

	{800, 1800, 101.85, 89, DIR_FORWARD}, // FL20
	{1800, 900, 190 ,-87, DIR_FORWARD}, // FR20 MODIFIED 115-190,50+50
	{700, 1800, 100, -89, DIR_BACKWARD}, // BL20 MODIFIED
	{1800, 700, 190, 89, DIR_BACKWARD}, // BR20, MODIFIED

	{1500, 1500, 103, 87.5, DIR_FORWARD}, // FL30 MODIFIED
	{1500, 1500, 190, -86.5, DIR_FORWARD}, // FR30 MODIFIED
	{1500, 1500, 101, -87.5, DIR_BACKWARD}, // BL30 MODIFIED
	{1500, 1100, 115, 88, DIR_BACKWARD}, // BR30
	{1800, 400, 190 ,-93, DIR_FORWARD}
};

enum TASK_TYPE{
	TASK_MOVE,
	TASK_MOVE_BACKWARD,
	TASK_FL,
	TASK_FR,
	TASK_BL,
	TASK_BR,
	TASK_ADC,
	TASK_MOVE_OBS,
	TASK_FASTESTPATH,
	TASK_FASTESTPATH_V2,
	TASK_BUZZER,
	TASK_NONE,
	TASK_FW_DIST,
	TASK_BW_DIST,
	TASK_TURN_ANGLE,//turn left
	TASK_NAVIGATE,
	TASK_TURN_ANGLE_RIGHT,
	TASK_ACK,
};
enum TASK_TYPE curTask = TASK_NONE, prevTask = TASK_NONE;

enum MOVE_MODE {
	SLOW,
	FAST
};
 enum MOVE_MODE moveMode = FAST;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM1_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM8_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC2_Init(void);
static void MX_TIM5_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM7_Init(void);
static void MX_TIM4_Init(void);
void StartDefaultTask(void *argument);
void motorTask(void *argument);
void gyroTask(void *argument);
void gyroBiasTask(void *argument);
void PidTask(void *argument);
void encoder_task(void *argument);
void runMoveDistTask(void *argument);
void runOledTask(void *argument);
void runFRTask(void *argument);
void runCmdTask(void *argument);
void runBRTask(void *argument);
void runTestTask(void *argument);
void runDispatchTask(void *argument);
void runFLTask(void *argument);
void runBLTask(void *argument);
void runTurnRightTask(void *argument);
void runNavigateObst(void *argument);
void runUltrasonicTask(void *argument);
void runFastestTask(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_TIM1_Init();
  MX_USART3_UART_Init();
  MX_I2C1_Init();
  MX_TIM8_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_TIM5_Init();
  MX_TIM3_Init();
  MX_TIM7_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
    OLED_Init();
    HAL_TIM_Base_Start(&htim7);
    HAL_UART_Receive_IT(&huart3, aRxBuffer,RX_BUFFER_SIZE);
    //HAL_UART_Receive_IT(&huart3, Buffer,RX_BUFFER_SIZE);
    ICM20948_init(&hi2c1,0,GYRO_FULL_SCALE_2000DPS);

    // initialise command queue
    curCmd.index = 100;
    curCmd.val = 0;

    cQueue.head = 0;
    cQueue.tail = 0;
    cQueue.size = CMD_BUFFER_SIZE;
    for (int i = 0; i < CMD_BUFFER_SIZE;i++) {
  	  Command cmd;
  	  cmd.index = 100;
  	  cmd.val = 0;
  	  cQueue.buffer[i] = cmd;
    }

    PIDConfigInit(&pidTSlow, 3.5, 0.0,0.8);
    PIDConfigInit(&pidSlow, 2.5, 0.0,0);
    PIDConfigInit(&pidFast, 1.48, 0.0,0);//backward: 1.522
    //PIDConfigInit(&pidFast_b, 1.522, 0.0,0);//indoor
    PIDConfigInit(&pidFast_b, 1.522, 0.0,0);//outdoor
	// servo motor turn
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
	// motor backwheel move
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);

	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

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

  /* Create the thread(s) */
  /* creation of defaultTask */
//  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);
//
//  /* creation of MotorTask */
//  MotorTaskHandle = osThreadNew(motorTask, NULL, &MotorTask_attributes);

  /* creation of GyroTask */
//  GyroTaskHandle = osThreadNew(gyroTask, NULL, &GyroTask_attributes);
////
////  /* creation of gyroBias */
//  gyroBiasHandle = osThreadNew(gyroBiasTask, NULL, &gyroBias_attributes);
//
//  /* creation of pid_controller */
//  pid_controllerHandle = osThreadNew(PidTask, NULL, &pid_controller_attributes);
////
////  /* creation of EncoderTask */
////  EncoderTaskHandle = osThreadNew(encoder_task, NULL, &EncoderTask_attributes);
////
////  /* creation of moveDistTask */
//  moveDistTaskHandle = osThreadNew(runMoveDistTask, NULL, &moveDistTask_attributes);
////
////  /* creation of OledTask */
//  OledTaskHandle = osThreadNew(runOledTask, NULL, &OledTask_attributes);
////
////  /* creation of FRTask */
//  FRTaskHandle = osThreadNew(runFRTask, NULL, &FRTask_attributes);
//
//  /* creation of commandTask */
//  commandTaskHandle = osThreadNew(runCmdTask, NULL, &commandTask_attributes);
////
////  /* creation of BRTask */
//  BRTaskHandle = osThreadNew(runBRTask, NULL, &BRTask_attributes);
//
//  /* creation of TestTask */
//  TestTaskHandle = osThreadNew(runTestTask, NULL, &TestTask_attributes);
////
////  /* creation of DispatchTask */
//  DispatchTaskHandle = osThreadNew(runDispatchTask, NULL, &DispatchTask_attributes);
////
////  /* creation of FLTask */
//  FLTaskHandle = osThreadNew(runFLTask, NULL, &FLTask_attributes);
////
////  /* creation of BLTask */
//  BLTaskHandle = osThreadNew(runBLTask, NULL, &BLTask_attributes);
////
////  /* creation of TurnRightAngle */
//  TurnRightAngleHandle = osThreadNew(runTurnRightTask, NULL, &TurnRightAngle_attributes);
////
////  /* creation of Navigate_Obst */
//  Navigate_ObstHandle = osThreadNew(runNavigateObst, NULL, &Navigate_Obst_attributes);

  /* creation of ultrasonicTask */
  ultrasonicTaskHandle = osThreadNew(runUltrasonicTask, NULL, &ultrasonicTask_attributes);

  /* creation of TestTask */
//    TestTaskHandle = osThreadNew(runTestTask, NULL, &TestTask_attributes);
//
//  /* creation of FastestTask */
  FastestTaskHandle = osThreadNew(runFastestTask, NULL, &FastestTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_11;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc2.Init.Resolution = ADC_RESOLUTION_12B;
  hadc2.Init.ScanConvMode = DISABLE;
  hadc2.Init.ContinuousConvMode = DISABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 1;
  hadc2.Init.DMAContinuousRequests = ENABLE;
  hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_12;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 160;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 1000;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_ENABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 16-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_IC_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief TIM5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 0;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 4294967295;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_IC_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim5, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */

  /* USER CODE END TIM5_Init 2 */

}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 16-1;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 65535;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

}

/**
  * @brief TIM8 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM8_Init(void)
{

  /* USER CODE BEGIN TIM8_Init 0 */

  /* USER CODE END TIM8_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM8_Init 1 */

  /* USER CODE END TIM8_Init 1 */
  htim8.Instance = TIM8;
  htim8.Init.Prescaler = 0;
  htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim8.Init.Period = 7199;
  htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim8.Init.RepetitionCounter = 0;
  htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim8) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim8, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim8) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim8, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM8_Init 2 */

  /* USER CODE END TIM8_Init 2 */
  HAL_TIM_MspPostInit(&htim8);

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, OLED_SCL_Pin|OLED_SDA_Pin|OLED_RST_Pin|OLED_DC_Pin
                          |LED3_Pin|Trig_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, MOTOR_AIN2_Pin|MOTOR_AIN1_Pin|MOTOR_BIN1_Pin|MOTOR_BIN2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : OLED_SCL_Pin OLED_SDA_Pin OLED_RST_Pin OLED_DC_Pin
                           LED3_Pin Trig_Pin */
  GPIO_InitStruct.Pin = OLED_SCL_Pin|OLED_SDA_Pin|OLED_RST_Pin|OLED_DC_Pin
                          |LED3_Pin|Trig_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : MOTOR_AIN2_Pin MOTOR_AIN1_Pin MOTOR_BIN1_Pin MOTOR_BIN2_Pin */
  GPIO_InitStruct.Pin = MOTOR_AIN2_Pin|MOTOR_AIN1_Pin|MOTOR_BIN1_Pin|MOTOR_BIN2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PD8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/*
 * Test
 */
void delay_us(uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&htim7,0);  // set the counter value a 0
	while (__HAL_TIM_GET_COUNTER(&htim7) < us);  // wait for the counter to reach the us input in the parameter
}


void HCSR04_Read (void)
{
	HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, GPIO_PIN_SET);  // pull the TRIG pin HIGH
	delay_us(10);  // wait f or 10 us
	HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, GPIO_PIN_RESET);  // pull the TRIG pin low

	__HAL_TIM_ENABLE_IT(&htim3, TIM_IT_CC2);
	//delay_us(50);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart) {
	// prevent unused argument(s) compilation warning
	UNUSED(huart);
	int val;

	val = (aRxBuffer[2] - 48) * 10 + (aRxBuffer[3] - 48);

	if (aRxBuffer[1] >= '0' && aRxBuffer[1] <= '9') val += (aRxBuffer[1] - 48) * 100;

	manualMode = 0;

	if (aRxBuffer[0] == 'S' && aRxBuffer[1] == 'T') {
		__ON_TASK_END(&htim8, prevTask, curTask);
		  angleNow = 0; gyroZ = 0;
		PIDConfigReset(&pidTSlow);
		PIDConfigReset(&pidSlow);
		PIDConfigReset(&pidFast);
		curDistTick = 0;
		if (__COMMAND_QUEUE_IS_EMPTY(cQueue)) {
			__CLEAR_CURCMD(curCmd);
			__ACK_TASK_DONE(&huart3, rxMsg);
		}
		else {
			__READ_COMMAND(cQueue, curCmd, rxMsg);
		}
	}
	else if (aRxBuffer[0] == 'F' && (aRxBuffer[1] == 'W' || aRxBuffer[1] == 'S')) { //FW or FS
		manualMode = aRxBuffer[2] == '-' && aRxBuffer[3] == '-';
		moveMode = aRxBuffer[1] == 'S' ? SLOW : FAST;
		__ADD_COMMAND(cQueue, 1, val);
	}
	else if (aRxBuffer[0] == 'B' && (aRxBuffer[1] == 'W' || aRxBuffer[1] == 'S')) { //BW or BS
		manualMode = aRxBuffer[2] == '-' && aRxBuffer[3] == '-';
		moveMode = aRxBuffer[1] == 'S' ? SLOW : FAST;
		__ADD_COMMAND(cQueue, 2, val);
	}
	else if (aRxBuffer[0] == 'F' && aRxBuffer[1] == 'L') { // FL
		manualMode = aRxBuffer[2] == '-' && aRxBuffer[3] == '-';
		__ADD_COMMAND(cQueue, 3 + (manualMode ? 0 : 4), val);
	}
	else if (aRxBuffer[0] == 'F' && aRxBuffer[1] == 'R') { // FR
		manualMode = aRxBuffer[2] == '-' && aRxBuffer[3] == '-';
		__ADD_COMMAND(cQueue, 4 + (manualMode ? 0 : 4), val);

	}
	else if (aRxBuffer[0] == 'B' && aRxBuffer[1] == 'L') { // BL
		manualMode = aRxBuffer[2] == '-' && aRxBuffer[3] == '-';
		__ADD_COMMAND(cQueue, 5 + (manualMode ? 0 : 4), val);
	}
	else if (aRxBuffer[0] == 'B' && aRxBuffer[1] == 'R') { // BR
		manualMode = aRxBuffer[2] == '-' && aRxBuffer[3] == '-';
		__ADD_COMMAND(cQueue, 6 + (manualMode ? 0 : 4), val);
	}
	else if (aRxBuffer[0] == 'F') {
		val = (aRxBuffer[1] - 48) * 100+(aRxBuffer[2] - 48) * 10 + (aRxBuffer[3] - 48);
		__ADD_COMMAND(cQueue, 18, val);
	}//test
	else if (aRxBuffer[0] == 'B') {
		val = (aRxBuffer[1] - 48) * 100+(aRxBuffer[2] - 48) * 10 + (aRxBuffer[3] - 48);
		__ADD_COMMAND(cQueue, 19, val);
	}
	else if (aRxBuffer[0] == 'X' && aRxBuffer[1] == '0'&& aRxBuffer[2] == '0'&& aRxBuffer[3] == '0') {
		__ADD_COMMAND(cQueue, 23, val);
	}
	else if (aRxBuffer[0] == 'Z' || aRxBuffer[0] == 'Y') {
				val = (aRxBuffer[1] - 48) * 100+(aRxBuffer[2] - 48) * 10 + (aRxBuffer[3] - 48);
				if(aRxBuffer[0] == 'Z'){
					__ADD_COMMAND(cQueue, 20, val);
				}else{
					__ADD_COMMAND(cQueue, 22, val);
				}

		}
	else if (aRxBuffer[0] == 'T' && aRxBuffer[1] == 'L') __ADD_COMMAND(cQueue, 11, val); // TL turn left max
	else if (aRxBuffer[0] == 'T' && aRxBuffer[1] == 'R') __ADD_COMMAND(cQueue, 12, val); // TR turn right max
	else if (aRxBuffer[0] == 'I' && aRxBuffer[1] == 'R') __ADD_COMMAND(cQueue, 13, val); // test IR sensor
	else if (aRxBuffer[0] == 'D' && aRxBuffer[1] == 'T') __ADD_COMMAND(cQueue, 14, val); // DT move until specified distance from obstacle
	//else if (aRxBuffer[0] == 'Z' && aRxBuffer[1] == 'Z') __ADD_COMMAND(cQueue, 15, val); // ZZ buzzer
	//else if (aRxBuffer[0] == 'W' && aRxBuffer[1] == 'X') __ADD_COMMAND(cQueue, 16, val); // WN fastest path
	//else if (aRxBuffer[0] == 'W' && aRxBuffer[1] == 'N') __ADD_COMMAND(cQueue, 17, val); // WN fastest path v2
	else if (aRxBuffer[0] == 'A') __ADD_COMMAND(cQueue, 88, val); // anti-clockwise rotation with variable
	else if (aRxBuffer[0] == 'C') __ADD_COMMAND(cQueue, 89, val); // clockwise rotation with variable
	else if (aRxBuffer[0] == 'N' && aRxBuffer[1] == 'V'&& aRxBuffer[2] == 'G'&& aRxBuffer[3] == 'T') {
		__ADD_COMMAND(cQueue, 21, val);
	}
	else if (aRxBuffer[0] == 'P' && aRxBuffer[1] == 'A'&& aRxBuffer[2] == 'T'&& aRxBuffer[3] == 'H') {
		__ADD_COMMAND(cQueue, 24, val);
	}
	if (!__COMMAND_QUEUE_IS_EMPTY(cQueue)) {
		__READ_COMMAND(cQueue, curCmd, rxMsg);
		//OLED_ShowNumber(10,30,curCmd.index,10,12);
		//OLED_Refresh_Gram();
	}
	//OLED_ShowNumber(10,30,curCmd.val,10,12);
	//OLED_Refresh_Gram();
	// clear aRx buffer
	  __HAL_UART_FLUSH_DRREGISTER(&huart3);
	  HAL_UART_Receive_IT(&huart3, aRxBuffer, RX_BUFFER_SIZE);
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	//OLED_ShowString(10,10,"helloCallback");
	uint8_t dist[20];
	//OLED_Refresh_Gram();
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)  // if the interrupt source is channel1
	{
//		sprintf(dist, "Triggered", Echo_Val2);
//		OLED_ShowString(10,20,dist);
		if (Is_First_Captured==0) // if the first value is not captured
		{
			Echo_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2); // read the first value
			Is_First_Captured = 1;  // set the first captured as true
			// Now change the polarity to falling edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_FALLING);
		}

		else if (Is_First_Captured==1)   // if the first is already captured
		{
			Echo_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);  // read second value
			__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter

			if (Echo_Val2 > Echo_Val1)
			{
				Difference = Echo_Val2-Echo_Val1;
			}

			else if (Echo_Val1 > Echo_Val2)
			{
				Difference = (0xffff - Echo_Val1) + Echo_Val2;
				//Difference = 0;
			}
//			sprintf(dist, "Echo2: %5d CM", Echo_Val2);
//			OLED_ShowString(10,30,dist);
//			sprintf(dist, "Diff: %5d CM", Difference);
//			OLED_ShowString(10,40,dist);
			UD = (Difference * 0.034)/2;

			Is_First_Captured = 0; // set it back to false

			// set polarity to rising edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim3, TIM_IT_CC2);
		}
	}
}


int clickOnce = 0;
int targetD = 5;
uint8_t tempDir = 1 ;
int8_t step = 0;
uint8_t turnMode = 2;


void PIDConfigInit(PIDConfig * cfg, const float Kp, const float Ki, const float Kd) {
	cfg->Kp = Kp;
	cfg->Ki = Ki;
	cfg->Kd = Kd;
	cfg->ek1 = 0;
	cfg->ekSum = 0;
}

void PIDConfigReset(PIDConfig * cfg) {
	cfg->ek1 = 0;
	cfg->ekSum = 0;
}


int8_t dir = 1;
int correction = 0;
//PIDConfig curPIDConfig;

void StraightLineMove(const uint8_t speedMode) {
	__Gyro_Read_Z(&hi2c1, readGyroZData, gyroZ); // polling
	dir = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2) ? 1 : -1; // use only one of the wheel to determine car direction
	//angleNow += ((gyroZ >= -10 && gyroZ <= 4) ? 0 : gyroZ); // / GRYO_SENSITIVITY_SCALE_FACTOR_2000DPS * 0.01;
	angleNow += ((gyroZ >= -10 && gyroZ <= 8) ? 0 : gyroZ);
	//angleNow += gyroZ;
	__PID_SPEED_2(pidFast, angleNow, correction, dir, newDutyL, newDutyR);


	__SET_MOTOR_DUTY(&htim8, newDutyL, newDutyR);
}

void StraightLineMoveBack(const uint8_t speedMode) {
	__Gyro_Read_Z(&hi2c1, readGyroZData, gyroZ); // polling
	dir = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2) ? 1 : -1; // use only one of the wheel to determine car direction
	//angleNow += ((gyroZ >= -10 && gyroZ <= 4) ? 0 : gyroZ); // / GRYO_SENSITIVITY_SCALE_FACTOR_2000DPS * 0.01;
	angleNow += ((gyroZ >= -10 && gyroZ <= 8) ? 0 : gyroZ);
	//angleNow += gyroZ;
	__PID_SPEED_2Back(pidFast, angleNow, correction, dir, newDutyL, newDutyR);
	__SET_MOTOR_DUTY(&htim8, newDutyL, newDutyR);
}

void StraightLineMoveSpeedScale(const uint8_t speedMode, float * speedScale) {
	__Gyro_Read_Z(&hi2c1, readGyroZData, gyroZ); // polling
	dir = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2) ? 1 : -1; // use only one of the wheel to determine car direction
	//if (dir==1) angleNow += ((gyroZ >= -4 && gyroZ <= 3) ? 0 : gyroZ);//forward indoor
//	if (dir==1) angleNow += ((gyroZ >= -6 && gyroZ <= 0.3) ? 0 : gyroZ); //ours
	if (dir==1) angleNow += ((gyroZ >= -15 && gyroZ <= 15) ? 0 : gyroZ);
	//else if (dir==-1) angleNow += ((gyroZ >= -2 && gyroZ <= 5) ? 0 : gyroZ);//indoor
	else if (dir==-1) angleNow += ((gyroZ >= -6 && gyroZ <= 6) ? 0 : gyroZ);
	if (speedMode == SPEED_MODE_1) {
		__PID_SPEED_1(pidSlow, angleNow, correction, dir, newDutyL, newDutyR);
	}else if (speedMode == SPEED_MODE_2){
		if (dir==1) __PID_SPEED_2(pidFast, angleNow, correction, dir, newDutyL, newDutyR);
		else if (dir==-1) __PID_SPEED_2(pidFast_b, angleNow, correction, dir, newDutyL, newDutyR);
	}

	__SET_MOTOR_DUTY(&htim8, newDutyL * (*speedScale), newDutyR * (*speedScale));
}

void StraightLineMoveSpeedScaleUS(const uint8_t speedMode, float * speedScale) {
	__Gyro_Read_Z(&hi2c1, readGyroZData, gyroZ); // polling
	dir = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2) ? 1 : -1; // use only one of the wheel to determine car direction
	//if (dir==1) angleNow += ((gyroZ >= -4 && gyroZ <= 3) ? 0 : gyroZ);//forward indoor
//	if (dir==1) angleNow += ((gyroZ >= -6 && gyroZ <= 0.3) ? 0 : gyroZ); //ours
	if (dir==1) angleNow += ((gyroZ >= -10 && gyroZ <= 10) ? 0 : gyroZ);
	//else if (dir==-1) angleNow += ((gyroZ >= -2 && gyroZ <= 5) ? 0 : gyroZ);//indoor
	else if (dir==-1) angleNow += ((gyroZ >= -6 && gyroZ <= 6) ? 0 : gyroZ);
	if (speedMode == SPEED_MODE_2){
		if (dir==1) __PID_SPEED_2US(pidFast, angleNow, correction, dir, newDutyL, newDutyR);
		else if (dir==-1) __PID_SPEED_2US(pidFast_b, angleNow, correction, dir, newDutyL, newDutyR);
	}

	__SET_MOTOR_DUTY(&htim8, newDutyL * (*speedScale), newDutyR * (*speedScale));
}

void StraightLineMoveSpeedScaleBack(const uint8_t speedMode, float * speedScale) {
	__Gyro_Read_Z(&hi2c1, readGyroZData, gyroZ); // polling
	dir = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2) ? 1 : -1; // use only one of the wheel to determine car direction
	//if (dir==1) angleNow += ((gyroZ >= -4 && gyroZ <= 3) ? 0 : gyroZ);//forward indoor
//	if (dir==1) angleNow += ((gyroZ >= -6 && gyroZ <= 0.3) ? 0 : gyroZ); //ours
	if (dir==1) angleNow += ((gyroZ >= -10 && gyroZ <= 10) ? 0 : gyroZ);
	//else if (dir==-1) angleNow += ((gyroZ >= -2 && gyroZ <= 5) ? 0 : gyroZ);//indoor
	else if (dir==-1) angleNow += ((gyroZ >= -6 && gyroZ <= 6) ? 0 : gyroZ);
	if (speedMode == SPEED_MODE_2){
		if (dir==1) __PID_SPEED_2Back(pidFast, angleNow, correction, dir, newDutyL, newDutyR);
		else if (dir==-1) __PID_SPEED_2Back(pidFast_b, angleNow, correction, dir, newDutyL, newDutyR);
	}

	__SET_MOTOR_DUTY(&htim8, newDutyL * (*speedScale), newDutyR * (*speedScale));
}

void RobotMoveDist(float * targetDist, const uint8_t dir, const uint8_t speedMode) {
	angleNow = 0; gyroZ = 0;
	PIDConfigReset(&pidTSlow);
	PIDConfigReset(&pidSlow);
	PIDConfigReset(&pidFast);
	curDistTick = 0;

	__GET_TARGETTICK(*targetDist, targetDistTick);

	last_curTask_tick = HAL_GetTick();
	__SET_MOTOR_DIRECTION(dir);
	__SET_ENCODER_LAST_TICK(&htim2, lastDistTick_L);
	//OLED_ShowNumber(10,10,lastDistTick_L,10,12);
	do {
		__GET_ENCODER_TICK_DELTA(&htim2, lastDistTick_L, dist_dL);
		curDistTick += dist_dL;
		//show number
		uint16_t hello = 0;
		uint16_t hello2=0;

				 //sprintf(hello,"%d\0",curDistTick);
				 //OLED_ShowNumber(10,10,curDistTick,10,12);
				 //sprintf(hello2,"%d\0",targetDistTick);
				 //OLED_ShowNumber(10,30,targetDistTick,10,12);
//				 OLED_Refresh_Gram();
		if (curDistTick >= targetDistTick) break;

		if (HAL_GetTick() - last_curTask_tick >= 10) {
			if (speedMode == SPEED_MODE_T) {
				StraightLineMove(SPEED_MODE_T);
			} else {
				speedScale = abs(curDistTick - targetDistTick) / 990;
				if (speedMode == SPEED_MODE_1) speedScale = speedScale > 1 ? 1 : (speedScale < 0.75 ? 0.75 : speedScale);
				else if (speedMode == SPEED_MODE_2)speedScale = speedScale > 1 ? 1 : (speedScale < 0.6 ? 0.5 : speedScale);
				StraightLineMoveSpeedScale(speedMode, &speedScale);
			}

			last_curTask_tick = HAL_GetTick();
		}
	} while (1);
	__SET_MOTOR_DUTY(&htim8, 0, 0);
}

void RobotMoveDistBack(float * targetDist, const uint8_t dir, const uint8_t speedMode) {
	angleNow = 0; gyroZ = 0;
	PIDConfigReset(&pidTSlow);
	PIDConfigReset(&pidSlow);
	PIDConfigReset(&pidFast);
	curDistTick = 0;

	__GET_TARGETTICK(*targetDist, targetDistTick);

	last_curTask_tick = HAL_GetTick();
	__SET_MOTOR_DIRECTION(dir);
	__SET_ENCODER_LAST_TICK(&htim2, lastDistTick_L);
	//OLED_ShowNumber(10,10,lastDistTick_L,10,12);
	do {
		__GET_ENCODER_TICK_DELTA(&htim2, lastDistTick_L, dist_dL);
		curDistTick += dist_dL;
		//show number
		uint16_t hello = 0;
		uint16_t hello2=0;

				 //sprintf(hello,"%d\0",curDistTick);
				 //OLED_ShowNumber(10,10,curDistTick,10,12);
				 //sprintf(hello2,"%d\0",targetDistTick);
				 //OLED_ShowNumber(10,30,targetDistTick,10,12);
//				 OLED_Refresh_Gram();
		if (curDistTick >= targetDistTick) break;

		if (HAL_GetTick() - last_curTask_tick >= 10) {
			if (speedMode == SPEED_MODE_T) {
				StraightLineMoveBack(SPEED_MODE_T);
			} else {
				speedScale = abs(curDistTick - targetDistTick) / 990;
				if (speedMode == SPEED_MODE_1) speedScale = speedScale > 1 ? 1 : (speedScale < 0.75 ? 0.75 : speedScale);
				else if (speedMode == SPEED_MODE_2)speedScale = speedScale > 1 ? 1 : (speedScale < 0.6 ? 0.5 : speedScale);
				StraightLineMoveSpeedScaleBack(speedMode, &speedScale);
			}

			last_curTask_tick = HAL_GetTick();
		}
	} while (1);
	__SET_MOTOR_DUTY(&htim8, 0, 0);
}


void RobotMoveDistObstacle(float * targetDist, const uint8_t speedMode) {
	angleNow = 0; gyroZ = 0;
	PIDConfigReset(&pidTSlow);
	PIDConfigReset(&pidSlow);
	PIDConfigReset(&pidFast);
//	Distance = 1000;
//	uint8_t dist[20];
//	HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
	last_curTask_tick = HAL_GetTick();
//	int cnt = 0;
	do {
//	  HCSR04_Read();
//	  Distance=UD;
//	  if(cnt==0)
//	  {
//		  totalDistance += Distance;
//		  cnt++;
//	  }
//	  sprintf(dist, "DIST: %5d CM", Distance);
//	  OLED_ShowString(10,50,dist);
//	  OLED_Refresh_Gram();
	  if (abs(*targetDist - Distance) <= 2) break;
	  __SET_MOTOR_DIRECTION(Distance >= *targetDist);
	  if (HAL_GetTick() - last_curTask_tick >=20) {
		  if (speedMode == SPEED_MODE_1) {
			  speedScale = abs(Distance - *targetDist) / 15; // slow down at 15cm
			  speedScale = speedScale > 1 ? 1 : (speedScale < 0.75 ? 0.75 : speedScale);
			  StraightLineMoveSpeedScale(SPEED_MODE_1, &speedScale);
		  } else {
			  speedScale = abs(Distance - *targetDist) / 15; // slow down at 15cm
			  speedScale = speedScale > 1 ? 1 : (speedScale < 0.4 ? 0.4 : speedScale);
			  StraightLineMoveSpeedScale(SPEED_MODE_2, &speedScale);
		  }
//		  speedScale = 1;
//		  StraightLineMoveSpeedScale(SPEED_MODE_2, &speedScale);

		  last_curTask_tick = HAL_GetTick();
	  }

	} while (1);

	__SET_MOTOR_DUTY(&htim8, 0, 0);
}

void RobotMoveDistObstacleUS(float * targetDist, const uint8_t speedMode) {
	angleNow = 0; gyroZ = 0;
	PIDConfigReset(&pidTSlow);
	PIDConfigReset(&pidSlow);
	PIDConfigReset(&pidFast);
//	Distance = 1000;
//	uint8_t dist[20];
//	HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
	last_curTask_tick = HAL_GetTick();
//	int cnt = 0;
	do {
//	  HCSR04_Read();
//	  Distance=UD;
//	  if(cnt==0)
//	  {
//		  totalDistance += Distance;
//		  cnt++;
//	  }
//	  sprintf(dist, "DIST: %5d CM", Distance);
//	  OLED_ShowString(10,50,dist);
//	  OLED_Refresh_Gram();
	  if (abs(*targetDist - Distance) <= 2) break;
	  __SET_MOTOR_DIRECTION(Distance >= *targetDist);
	  if (HAL_GetTick() - last_curTask_tick >=20) {
		  speedScale = abs(Distance - *targetDist) / 20; // slow down at 15cm
		  speedScale = speedScale > 1 ? 1 : (speedScale < 0.4 ? 0.3 : speedScale);
		  StraightLineMoveSpeedScaleUS(SPEED_MODE_2, &speedScale);
//		  speedScale = 1;
//		  StraightLineMoveSpeedScale(SPEED_MODE_2, &speedScale);

		  last_curTask_tick = HAL_GetTick();
	  }

	} while (1);

	__SET_MOTOR_DUTY(&htim8, 0, 0);
}

void RobotMoveDistIR(const uint8_t speedMode, int isRight) {
	angleNow = 0; gyroZ = 0;
	PIDConfigReset(&pidTSlow);
	PIDConfigReset(&pidSlow);
	PIDConfigReset(&pidFast);
	Distance = 1000;
	uint8_t dist[20];
//	HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
	last_curTask_tick = HAL_GetTick();

	do {
		if (isRight){
			__ADC_Read_Dist(&hadc1, dataPoint, IR_data_raw_acc, obsDist_IR, obsTick_IR);
		}else{
			__ADC_Read_Dist(&hadc2, dataPoint, IR_data_raw_acc, obsDist_IR, obsTick_IR);
		}
//	  Distance=UD;
		uint32_t dist=obsDist_IR;

	  if (dist > 60 || dist == 0) break;
	  float dir = 1.0;
	  __SET_MOTOR_DIRECTION(&dir);
	  if (HAL_GetTick() - last_curTask_tick >=20) {
		  speedScale = 1;
		  StraightLineMoveSpeedScale(SPEED_MODE_2, &speedScale);
		  last_curTask_tick = HAL_GetTick();
	  }

	} while (1);

	__SET_MOTOR_DUTY(&htim8, 0, 0);
}

void RobotMoveDistIR2(const uint8_t speedMode, int isRight) {
	angleNow = 0; gyroZ = 0;
	PIDConfigReset(&pidTSlow);
	PIDConfigReset(&pidSlow);
	PIDConfigReset(&pidFast);
	Distance = 1000;
//	uint8_t dist[20];
//	HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
	last_curTask_tick = HAL_GetTick();

	do {
		if (isRight){
			__ADC_Read_Dist(&hadc1, dataPoint, IR_data_raw_acc, obsDist_IR, obsTick_IR);
		}else{
			__ADC_Read_Dist(&hadc2, dataPoint, IR_data_raw_acc, obsDist_IR, obsTick_IR);
		}
//	  Distance=UD;
		uint32_t dist=obsDist_IR;
	  if (dist < 70 && dist > 10) break;
	  float dir = 1.0;
	  __SET_MOTOR_DIRECTION(&dir);
	  if (HAL_GetTick() - last_curTask_tick >=20) {
		  speedScale = 1;
		  StraightLineMoveSpeedScale(SPEED_MODE_2, &speedScale);
		  last_curTask_tick = HAL_GetTick();
	  }
	} while (1);

	__SET_MOTOR_DUTY(&htim8, 0, 0);
}


void RobotTurn(float * targetAngle) {
	angleNow = 0; gyroZ = 0;
	last_curTask_tick = HAL_GetTick();
	do {
	  if (HAL_GetTick() - last_curTask_tick >= 10) { // sample gyro every 10ms
		  __Gyro_Read_Z(&hi2c1, readGyroZData, gyroZ);
		  angleNow += gyroZ * 0.01 / GRYO_SENSITIVITY_SCALE_FACTOR_2000DPS ;
//		  angleNow += gyroZ;

		  //OLED_ShowNumber(10,20,gyroZ,10,12);
		  if (abs(angleNow - *targetAngle) < 0.01) {
			  //OLED_ShowNumber(10,10,(int)angleNow,10,12);
			  //OLED_ShowNumber(10,20,(int)(*targetAngle),10,12);
			  break;

		  }
		  last_curTask_tick = HAL_GetTick();
	  }
	} while(1);
	__SET_MOTOR_DUTY(&htim8, 0, 0);
	__RESET_SERVO_TURN(&htim1);
//	osDelay(200);
}

void RobotTurnWithoutReset(float * targetAngle) {
	angleNow = 0; gyroZ = 0;
	last_curTask_tick = HAL_GetTick();
	do {
	  if (HAL_GetTick() - last_curTask_tick >= 10) { // sample gyro every 10ms
		  __Gyro_Read_Z(&hi2c1, readGyroZData, gyroZ);
		  angleNow += gyroZ * 0.01 / GRYO_SENSITIVITY_SCALE_FACTOR_2000DPS ;
//		  angleNow += gyroZ;

		  //OLED_ShowNumber(10,20,gyroZ,10,12);
		  if (abs(angleNow - *targetAngle) < 0.01) {
			  //OLED_ShowNumber(10,10,(int)angleNow,10,12);
			  //OLED_ShowNumber(10,20,(int)(*targetAngle),10,12);
			  break;

		  }
		  last_curTask_tick = HAL_GetTick();
	  }
	} while(1);
	__SET_MOTOR_DUTY(&htim8, 0, 0);
}

void RobotTurnFastest(float * targetAngle) {
	angleNow = 0; gyroZ = 0;
	last_curTask_tick = HAL_GetTick();
	do {
	  if (HAL_GetTick() - last_curTask_tick >= 10) { // sample gyro every 10ms
		  __Gyro_Read_Z(&hi2c1, readGyroZData, gyroZ);
		  angleNow += gyroZ / GRYO_SENSITIVITY_SCALE_FACTOR_2000DPS * 0.01;
		  if (abs(angleNow - *targetAngle) < 0.01) break;
		  last_curTask_tick = HAL_GetTick();
	  }
	} while(1);
	__SET_MOTOR_DUTY(&htim8, 0, 0);
	__RESET_SERVO_TURN_FAST(&htim1);
}

//void FASTESTPATH_TURN_LEFT_90() {
//	targetAngle = 79.25;
//	__SET_SERVO_TURN(&htim1, 80);
//	__SET_MOTOR_DIRECTION(1);
//	__SET_MOTOR_DUTY(&htim8, 2000, 3500);
//	RobotTurn(&targetAngle);
//}

//void FASTESTPATH_TURN_RIGHT_90() {
//	targetAngle = -85;
//	__SET_SERVO_TURN(&htim1, 230);
//	__SET_MOTOR_DIRECTION(1);
//	__SET_MOTOR_DUTY(&htim8, 3000, 800);
//	RobotTurn(&targetAngle);
//}

void FASTESTPATH_TURN_LEFT_90() {
	// 3 bars
//	targetAngle = 75;
	// 2 bars
	targetAngle = 78;
	__SET_SERVO_TURN(&htim1, 80);
	__SET_MOTOR_DIRECTION(1);
	__SET_MOTOR_DUTY(&htim8, 800, 5500);
	RobotTurn(&targetAngle);
}

void FASTESTPATH_TURN_RIGHT_90() {
	//3 bars
//	targetAngle = -77;
	// 2 bars
	targetAngle = -80;
	__SET_SERVO_TURN(&htim1, 230);
	__SET_MOTOR_DIRECTION(1);
	__SET_MOTOR_DUTY(&htim8, 5500, 800);
	RobotTurnWithoutReset(&targetAngle);
	htim1.Instance->CCR4=120;
	osDelay(500);
	__RESET_SERVO_TURN(&htim1);
}

//void FASTESTPATH_TURN_RIGHT_180() {
//	targetAngle = -176;
//	__SET_SERVO_TURN(&htim1, 115);
//	__SET_MOTOR_DIRECTION(1);
//	__SET_MOTOR_DUTY(&htim8, 3000, 800);
//	RobotTurn(&targetAngle);
//}

//void FASTESTPATH_TURN_LEFT_180() {
//	targetAngle = 50;
//	__SET_SERVO_TURN(&htim1, 110);
//	__SET_MOTOR_DIRECTION(1);
//	__SET_MOTOR_DUTY(&htim8, 1000, 4500);
//	RobotTurnWithoutReset(&targetAngle);
//	scan();
//	targetAngle = -105;
//	__SET_SERVO_TURN(&htim1, 200);
//	__SET_MOTOR_DIRECTION(1);
//	__SET_MOTOR_DUTY(&htim8, 4500, 800);
//	RobotTurnWithoutReset(&targetAngle);
//	targetAngle = 50;
//	__SET_SERVO_TURN(&htim1, 110);
//	__SET_MOTOR_DIRECTION(1);
//	__SET_MOTOR_DUTY(&htim8, 1000, 4500);
//	RobotTurnWithoutReset(&targetAngle);
//	__RESET_SERVO_TURN(&htim1);
//}

void FASTESTPATH_TURN_LEFT_180() {
	targetAngle = 50;
	__SET_SERVO_TURN(&htim1, 110);
	__SET_MOTOR_DIRECTION(1);
	__SET_MOTOR_DUTY(&htim8, 1000, 4500);
	RobotTurnWithoutReset(&targetAngle);
	scan();
	targetAngle = -105;
	__SET_SERVO_TURN(&htim1, 200);
	__SET_MOTOR_DIRECTION(1);
	__SET_MOTOR_DUTY(&htim8, 4500, 800);
	RobotTurnWithoutReset(&targetAngle);
	targetAngle = 50;
	__SET_SERVO_TURN(&htim1, 110);
	__SET_MOTOR_DIRECTION(1);
	__SET_MOTOR_DUTY(&htim8, 1000, 4500);
	RobotTurnWithoutReset(&targetAngle);
	__RESET_SERVO_TURN(&htim1);
}

void FASTESTPATH_TURN_RIGHT_180() {
	targetAngle = -45;
	__SET_SERVO_TURN(&htim1, 200);
	__SET_MOTOR_DIRECTION(1);
	__SET_MOTOR_DUTY(&htim8, 3000, 800);
	RobotTurnWithoutReset(&targetAngle);
	scan();
	targetAngle = 87;
	__SET_SERVO_TURN(&htim1, 110);
	__SET_MOTOR_DIRECTION(1);
	__SET_MOTOR_DUTY(&htim8, 1000, 4500);
	RobotTurnWithoutReset(&targetAngle);
	targetAngle = -42;
	__SET_SERVO_TURN(&htim1, 200);
	__SET_MOTOR_DIRECTION(1);
	__SET_MOTOR_DUTY(&htim8, 3000, 800);
	RobotTurnWithoutReset(&targetAngle);
	htim1.Instance->CCR4=120;
	osDelay(300);
	__RESET_SERVO_TURN(&htim1);
//	osDelay(200);
}

void FASTESTPATH_TURN_LEFT_AROUND() {
	targetAngle = 170;
	__SET_SERVO_TURN(&htim1, 95);
	__SET_MOTOR_DIRECTION(1);
	__SET_MOTOR_DUTY(&htim8, 800, 5500);
	RobotTurn(&targetAngle);
}

void FASTESTPATH_TURN_RIGHT_AROUND() {
	targetAngle = -180;
	__SET_SERVO_TURN(&htim1, 80);
	__SET_MOTOR_DIRECTION(1);
	__SET_MOTOR_DUTY(&htim8, 2000, 3500);
	RobotTurnWithoutReset(&targetAngle);
	htim1.Instance->CCR4=120;
	osDelay(500);
	__RESET_SERVO_TURN(&htim1);
}

void RobotMoveUntilIROvershoot() {
	obsDist_IR = 0;
	angleNow = 0; gyroZ = 0;
	  last_curTask_tick = HAL_GetTick();
	  do {
		  __ADC_Read_Dist(&hadc1, dataPoint, IR_data_raw_acc, obsDist_IR, obsTick_IR);
		  if (obsDist_IR > 40) break;
		  if (HAL_GetTick() - last_curTask_tick >= 10) {
			  StraightLineMove(SPEED_MODE_2);
			  last_curTask_tick = HAL_GetTick();
		  }
	  } while (1);
	  __SET_MOTOR_DUTY(&htim8, 0, 0);
}

void RobotMoveUntilIRHit() {
	obsDist_IR = 1000;
	angleNow = 0; gyroZ = 0;
	  last_curTask_tick = HAL_GetTick();
	  do {
		  __ADC_Read_Dist(&hadc1, dataPoint, IR_data_raw_acc, obsDist_IR, obsTick_IR);
		  if (obsDist_IR < 40) break;
		  if (HAL_GetTick() - last_curTask_tick >= 10) {
			  StraightLineMove(SPEED_MODE_2);
			  last_curTask_tick = HAL_GetTick();
		  }
	  } while (1);
	  __SET_MOTOR_DUTY(&htim8, 0, 0);
}

void scan(){
	char scan = 'A';
	HAL_UART_Transmit(&huart3, (uint8_t*)&scan,1, 0xFFFF);
	osDelay(10);

//	for (;;){
//		char key = aRxBuffer[0];
//		if (key != ' '){
//			return key;
//		}
//	}
}

char receiveFromBuffer(){
	for (;;){
		char key = aRxBuffer[0];
		if (key == 'L' || key == 'R'){
			aRxBuffer[0] = '-';
			return key;
		}
	}
}

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  //uint8_t ch = 'X';
  for(;;)
  {
	  if (curTask != TASK_ACK) osDelay(1000);
	  else {
		  //HAL_UART_Transmit(&huart3,(uint8_t*)&ch,1,0xFFFF);
		  clickOnce = 0;
		  prevTask = curTask;
		  curTask = TASK_NONE;

		if (__COMMAND_QUEUE_IS_EMPTY(cQueue)) {
			__CLEAR_CURCMD(curCmd);
			__ACK_TASK_DONE(&huart3, rxMsg);
			} else {
			 __READ_COMMAND(cQueue, curCmd, rxMsg);
			}
	  }
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_motorTask */
/**
* @brief Function implementing the MotorTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_motorTask */
void motorTask(void *argument)
{
  /* USER CODE BEGIN motorTask */
  /* Infinite loop */
  //uint16_t pwmVal = 0;
  //FASTESTPATH_TURN_LEFT_90();
  //osDelay(10);
	targetDist1=20;
//	RobotMoveDistObstacle(&targetDist, SPEED_MODE_1);

  /*for(;;)
  {
	  while(pwmVal<2000)
	  		{
	  		 // AIN2_Pin|AIN1_Pin
	  		HAL_GPIO_WritePin(GPIOA, AIN2_Pin, GPIO_PIN_RESET);
	  		HAL_GPIO_WritePin(GPIOA, AIN1_Pin, GPIO_PIN_SET);
	  		HAL_GPIO_WritePin(GPIOA, BIN2_Pin, GPIO_PIN_RESET);
	  		HAL_GPIO_WritePin(GPIOA, BIN1_Pin, GPIO_PIN_SET);
	  		pwmVal++;
	  		__HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_1, pwmVal); // Modify the comparison value for duty cycle of timer
	  		__HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_2, pwmVal);
	  		osDelay(10);
	  		}A

	  			 while(pwmVal>0)
	  			  {
	  			 HAL_GPIO_WritePin(GPIOA, AIN2_Pin, GPIO_PIN_RESET);
	  			 HAL_GPIO_WritePin(GPIOA, AIN1_Pin, GPIO_PIN_SET);
	  			 HAL_GPIO_WritePin(GPIOA, BIN2_Pin, GPIO_PIN_RESET);
	  			 HAL_GPIO_WritePin(GPIOA, BIN1_Pin, GPIO_PIN_SET);
	  			pwmVal--;
	  			  __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_1, pwmVal);
	  			  __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_2, pwmVal);
	  			 osDelay(10);
	  			 }

    osDelay(1);
  }*/
  /*HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
  	htim1.Instance->CCR4=210;
  	osDelay(5000);
  	htim1.Instance->CCR4=147;
      osDelay(5000);
      htim1.Instance->CCR4=110;
      osDelay(5000);
      htim1.Instance->CCR4=147;
      osDelay(5000);*/
  /* USER CODE END motorTask */
}

/* USER CODE BEGIN Header_gyroTask */
/**
* @brief Function implementing the GyroTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_gyroTask */
void gyroTask(void *argument)
{
  /* USER CODE BEGIN gyroTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END gyroTask */
}

/* USER CODE BEGIN Header_gyroBiasTask */
/**
* @brief Function implementing the gyroBias thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_gyroBiasTask */
void gyroBiasTask(void *argument)
{
  /* USER CODE BEGIN gyroBiasTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END gyroBiasTask */
}

/* USER CODE BEGIN Header_PidTask */
/**
* @brief Function implementing the pid_controller thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_PidTask */
void PidTask(void *argument)
{
  /* USER CODE BEGIN PidTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END PidTask */
}

/* USER CODE BEGIN Header_encoder_task */
/**
* @brief Function implementing the EncoderTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_encoder_task */
void encoder_task(void *argument)
{
  /* USER CODE BEGIN encoder_task */
  /* Infinite loop */
	HAL_TIM_Encoder_Start(&htim2,TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_ALL);
	int cnt1,cnt2,diff1;
	int cnt3,cnt4,diff2;
	uint32_t tick;

	cnt1=__HAL_TIM_GET_COUNTER(&htim2);
	cnt3=__HAL_TIM_GET_COUNTER(&htim3);
	tick=HAL_GetTick();
	uint8_t hello[8];
	uint16_t dir;
  for(;;)
  {
	  if(HAL_GetTick()-tick>1000L){
		  cnt2=__HAL_TIM_GET_COUNTER(&htim2);
		  cnt4=__HAL_TIM_GET_COUNTER(&htim3);
		  if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2)){
			  if(cnt2<cnt1)
				  diff1=cnt1-cnt2;
			  else
				  diff1=(65535-cnt2)+cnt1;
		  }else{
			  if(cnt2>cnt1)
				  diff1=cnt2-cnt1;
			  else
				  diff1=(65535-cnt1)+cnt2;
		  }
		  if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim3)){
		  			  if(cnt3<cnt4)
		  				  diff2=cnt3-cnt4;
		  			  else
		  				  diff2=(65535-cnt4)+cnt3;
		  		  }else{
		  			  if(cnt3>cnt4)
		  				  diff2=cnt4-cnt3;
		  			  else
		  				  diff2=(65535-cnt3)+cnt4;
		  		  }
		  sprintf(hello,"Speed1:%5d\0",diff1);
		  OLED_ShowString(10,20,hello);
		  cnt1=__HAL_TIM_GET_COUNTER(&htim2);
		  dir =__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2);
		  sprintf(hello,"Speed2:%5d\0",diff2);
		  OLED_ShowString(10,30,hello);
		  cnt3=__HAL_TIM_GET_COUNTER(&htim3);
		  tick= HAL_GetTick();
	  }

	  OLED_Refresh_Gram();
	  osDelay(250);
  }
  /* USER CODE END encoder_task */
}

/* USER CODE BEGIN Header_runMoveDistTask */
/**
* @brief Function implementing the moveDistTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_runMoveDistTask */
void runMoveDistTask(void *argument)
{
  /* USER CODE BEGIN runMoveDistTask */
  /* Infinite loop */
	int flag=0;
  for(;;)
  {
	  flag++;
	  //OLED_ShowNumber(10,10,flag,10,12);
	  //OLED_Refresh_Gram();
	  if (curTask != TASK_FW_DIST && curTask != TASK_BW_DIST){

	  		  osDelay(100);
	  }else{

		  htim1.Instance->CCR4=147;
		  float v= (float)curCmd.val;
		  /*outdoor*/
		  if(v>169){
			  v=v;
		  }else if(v>159){
			  v=v-10;
		  }else if(v>149){
			  v=v-6.5;
		  }else if(v>139){
			  v=v-3;
		  }else if(v>129){
			  v=v-5.5-10;
		  }else if(v>99){
			  v=v+10;
		  }else if(v>89){
			  v=v-5;
		  }else if(v>79){
			  v=v-9;
		  }else if(v>69){
			  v=v;
		  }else if(v>59){
			  v=v-5;
		  }else if(v>15){
			  v=v-5;
		  }else if(v>6.5){
			  v=v-3.5;
		  }else if(v>3.5){
			  v=v;
		  }else{
			  v=3;
		  }
		  /*indoor*/
		  /*if(v>169){
			  v=v-8;
		  }else if(v>129){
			  v=v-16;
	      }else if(v>100){
			  v=v-9;
		  }else if(v>79){
			  v=v-6;
		  }else if(v>49){
			  v=v-4.5;
		  }else if(v>29){
			  v=v-2;
		  }else if(v>19){
			  v=v-4;
		  }else if(v>6.5){
			  v=v-3.5;
		  }else if(v>3.5){
			  v=v-2.5;
		  }else{
			  v=1.5;
		  }*/

		  RobotMoveDist(&v,(curTask == TASK_FW_DIST)?DIR_FORWARD:DIR_BACKWARD,SPEED_MODE_2);
		  osDelay(1000);
		  clickOnce = 0;
		  	  prevTask = curTask;
		  	  curTask = TASK_NONE;
		  	  if (__COMMAND_QUEUE_IS_EMPTY(cQueue)) {
				__CLEAR_CURCMD(curCmd);
				__ACK_TASK_DONE(&huart3, rxMsg);
		  	  } else{
		  		  __READ_COMMAND(cQueue, curCmd, rxMsg);
		  	  }
	  }

	  //OLED_ShowString(10,20,"hello");
	  			/*angleNow = 0; gyroZ = 0; // reset angle for PID
	  			PIDConfigReset(&pidTSlow);
	  			PIDConfigReset(&pidSlow);
	  			PIDConfigReset(&pidFast);

	  			__SET_MOTOR_DIRECTION(DIR_FORWARD);

	  			last_curTask_tick = HAL_GetTick();
	  			do {
	  				//if (!manualMode) break;
	  				if (HAL_GetTick() - last_curTask_tick >= 10) {
	  					StraightLineMove(SPEED_MODE_T);
	  					last_curTask_tick = HAL_GetTick();
	  				}
	  				osDelay(5);
	  			} while (1);*/
	  /*if(flag==0){
		  float a=50.0;
		  RobotMoveDist(&a,DIR_FORWARD,SPEED_MODE_1);
		  flag=1;
	  }else{
		  osDelay(1000);
	  }*/


  }
  /* USER CODE END runMoveDistTask */
}

/* USER CODE BEGIN Header_runOledTask */
/**
* @brief Function implementing the OledTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_runOledTask */
void runOledTask(void *argument)
{
  /* USER CODE BEGIN runOledTask */
  /* Infinite loop */
  for(;;)
  {

	 uint8_t hello[20] = "Hello world\0";
	 sprintf(hello,"%s\0",aRxBuffer);
	 OLED_ShowString(0,40,hello);
	 OLED_Refresh_Gram();
	 osDelay(1);



  }
  /* USER CODE END runOledTask */
}

/* USER CODE BEGIN Header_runFRTask */
/**
* @brief Function implementing the FRTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_runFRTask */
void runFRTask(void *argument)
{
  /* USER CODE BEGIN runFRTask */
  /* Infinite loop */
  for(;;)
  {
	  /*targetDist = 80;
	  			  RobotMoveDist(&targetDist, DIR_FORWARD, SPEED_MODE_T);
	  			  osDelay(10);
	  			  __SET_CMD_CONFIG(cfgs[16], &htim8, &htim1, targetAngle);
	  			  RobotTurn(&targetAngle);
	  			//FASTESTPATH_TURN_LEFT_90();
	  			  osDelay(10);

	  			  targetDist = 18;
	  			  RobotMoveDist(&targetDist, DIR_FORWARD, SPEED_MODE_T);
	  			  osDelay(10);
	  			  break;*/
	  	  if (curTask != TASK_FR) osDelay(1000);
	  	  else {
	  //		  osDelay(3000); // video demo only
	  		  switch(curCmd.val) {
	  		  case 30: // FR30 (4x2)

	  			  __SET_CMD_CONFIG(cfgs[CONFIG_FR30], &htim8, &htim1, targetAngle);
	  			  RobotTurn(&targetAngle);
	  			  osDelay(10);
	  			  targetDist1 = 4;
	  			  RobotMoveDist(&targetDist1, DIR_BACKWARD, SPEED_MODE_T);
	  			  osDelay(10);
	  			  break;
	  		  case 20: // FR20 (outdoor 3x1)
	  			  targetDist1 = 4;
	  			  RobotMoveDist(&targetDist1, DIR_BACKWARD, SPEED_MODE_T);
	  			  osDelay(10);
	  			  __SET_CMD_CONFIG(cfgs[CONFIG_FR20], &htim8, &htim1, targetAngle);
	  			  RobotTurn(&targetAngle);
	  			  osDelay(10);
	  			  targetDist1 = 8;
	  			  RobotMoveDist(&targetDist1, DIR_BACKWARD, SPEED_MODE_T);
	  			  osDelay(10);
	  			  break;
	  		  default: // FR00 (indoor 3x1)
	  			  targetDist1 = 3;
	  			  RobotMoveDist(&targetDist1, DIR_BACKWARD, SPEED_MODE_T);
	  			  osDelay(10);
	  			  __SET_CMD_CONFIG(cfgs[CONFIG_FR00], &htim8, &htim1, targetAngle);
	  			  RobotTurn(&targetAngle);
	  			  osDelay(10);
	  			  targetDist1 = 6.5;
	  			  RobotMoveDist(&targetDist1, DIR_BACKWARD, SPEED_MODE_T);
	  			  osDelay(10);
	  			  break;
	  		  }


	  		  clickOnce = 0;
	  		  prevTask = curTask;
	  		  curTask = TASK_NONE;
	  		  if (__COMMAND_QUEUE_IS_EMPTY(cQueue)) {
	  				__CLEAR_CURCMD(curCmd);
	  				__ACK_TASK_DONE(&huart3, rxMsg);
	  		  } else __READ_COMMAND(cQueue, curCmd, rxMsg);
	  	  }
  }
  /* USER CODE END runFRTask */
}

/* USER CODE BEGIN Header_runCmdTask */
/**
* @brief Function implementing the commandTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_runCmdTask */
void runCmdTask(void *argument)
{
  /* USER CODE BEGIN runCmdTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END runCmdTask */
}

/* USER CODE BEGIN Header_runBRTask */
/**
* @brief Function implementing the BRTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_runBRTask */
void runBRTask(void *argument)
{
  /* USER CODE BEGIN runBRTask */
  /* Infinite loop */
  for(;;)
  {
	  if (curTask != TASK_BR) osDelay(1000);
	  	  else {
	  //		  osDelay(3000); // video demo only
	  		  switch(curCmd.val) {
	  		  case 30: // BR30 (4x2)
	  			  __SET_CMD_CONFIG(cfgs[CONFIG_BR30], &htim8, &htim1, targetAngle);
	  			  RobotTurn(&targetAngle);
	  			  osDelay(10);
	  			  targetDist1 = 5;
	  			  RobotMoveDist(&targetDist1, DIR_BACKWARD, SPEED_MODE_T);
	  			  osDelay(10);
	  			  break;
	  		  case 20: // BR20 (outdoor 3x1)
	  			  targetDist1 = 7;
	  			  RobotMoveDist(&targetDist1, DIR_FORWARD, SPEED_MODE_T);
	  			  osDelay(10);
	  			  __SET_CMD_CONFIG(cfgs[CONFIG_BR20], &htim8, &htim1, targetAngle);
	  			  RobotTurn(&targetAngle);
	  			  osDelay(10);
	  			  targetDist1 = 3;
	  			  RobotMoveDist(&targetDist1, DIR_FORWARD, SPEED_MODE_T);
	  			  osDelay(10);
	  			  break;
	  		  default: // BR00 (indoor 3x1)
	  			  targetDist1 = 7;
	  			  RobotMoveDist(&targetDist1, DIR_FORWARD, SPEED_MODE_T);
	  			  osDelay(10);
	  			  __SET_CMD_CONFIG(cfgs[CONFIG_BR00], &htim8, &htim1, targetAngle);
	  			  RobotTurn(&targetAngle);
	  			  osDelay(10);
	  			  targetDist1 = 3;
	  			  RobotMoveDist(&targetDist1, DIR_FORWARD, SPEED_MODE_T);
	  			  osDelay(10);
	  			  break;
	  		  }


	  		  clickOnce = 0;
	  		  prevTask = curTask;
	  		  curTask = TASK_NONE;
	  		  if (__COMMAND_QUEUE_IS_EMPTY(cQueue)) {
	  				__CLEAR_CURCMD(curCmd);
	  				__ACK_TASK_DONE(&huart3, rxMsg);
	  		  } else __READ_COMMAND(cQueue, curCmd, rxMsg);
	  	  }

  }
  /* USER CODE END runBRTask */
}

/* USER CODE BEGIN Header_runDispatchTask */
/**
* @brief Function implementing the DispatchTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_runDispatchTask */
void runDispatchTask(void *argument)
{
  /* USER CODE BEGIN runDispatchTask */
  /* Infinite loop */
	//int flag=0;
	 //OLED_ShowString(10,10,"hello1");
	 //OLED_Refresh_Gram();
	for(;;)
		  {
			  /*if(flag==0 && curCmd.index<99){
				  OLED_ShowNumber(10,10,curCmd.index,10,12);
				  OLED_Refresh_Gram();
				  flag=1;
			  }*/
			  //OLED_ShowString(10,30,"hello2");
			 // OLED_Refresh_Gram();
			  switch(curCmd.index) {
			  //	  	 case 0: // STOP handled in UART IRQ directly
			  //	  	  	  break;
			  	  	 case 1: //FW
			  	  	 case 2: //BW
			  	  		curTask = curCmd.index == 1 ? TASK_MOVE : TASK_MOVE_BACKWARD;
			  	  		__PEND_CURCMD(curCmd);
			  	  		 break;
			  	  	case 3: //FL manual
			  		case 4: //FR manual
			  		case 5: //BL manual
			  		case 6: //BR manual
			  			__SET_CMD_CONFIG(cfgs[curCmd.index], &htim8, &htim1, targetAngle);
			  			if (__COMMAND_QUEUE_IS_EMPTY(cQueue)) {
			  				__CLEAR_CURCMD(curCmd);
			  				__ACK_TASK_DONE(&huart3, rxMsg);
			  			} else __READ_COMMAND(cQueue, curCmd, rxMsg);
			  			__PEND_CURCMD(curCmd);
			  			 break;
			  	  	 case 7: // FL
			  	  		 curTask = TASK_FL;
			  	  		__PEND_CURCMD(curCmd);
			  	  		 break;
			  	  	 case 8: // FR
			  	  		OLED_ShowString(10,10,"hello,case8");
			  	  		OLED_Refresh_Gram();
			  	  		curTask = TASK_FR;
			  	  		__PEND_CURCMD(curCmd);
			  	  		break;
			  	  	 case 9: // BL
			  	  		curTask = TASK_BL;
			  	  		__PEND_CURCMD(curCmd);
			  	  		break;
			  	  	 case 10: //BR
			  	  		curTask = TASK_BR;
			  	  		__PEND_CURCMD(curCmd);
			  	  		break;
			  	  	 case 11: // TL
			  	  	 case 12: // TR
			  	  		 __SET_SERVO_TURN_MAX(&htim1, curCmd.index - 11 ? 1 : 0);
			  	  		__CLEAR_CURCMD(curCmd);
			  			__ACK_TASK_DONE(&huart3, rxMsg);
			  	  		 break;
			  	  	 case 13: // debug IR sensor
			  	  		 curTask = TASK_ADC;
			  	  		 break;
			  	  	 case 14: // DT move until specified distance from obstacle
			  	  		  curTask = TASK_MOVE_OBS;
			  	  		  __PEND_CURCMD(curCmd);
			  	  		 break;
			  	  	 case 15:
			  	  		 curTask = TASK_BUZZER;
			  	  		__PEND_CURCMD(curCmd);
			  	  		break;
			  	  	 /*case 16:
			  	  		 curTask = TASK_FASTESTPATH;
			  	  		__PEND_CURCMD(curCmd);
			  	  		 break;
			  	  	 case 17:
			  	  		 curTask = TASK_FASTESTPATH_V2;
			  	  		__PEND_CURCMD(curCmd);
			  	  		 break;*/
			  	  	 case 18:
			  	  				  	  		 curTask = TASK_FW_DIST;
			  	  				  	  		__PEND_CURCMD(curCmd);
			  	  				  	  		break;
			  	  	 case 19:
										curTask = TASK_BW_DIST;
										__PEND_CURCMD(curCmd);
										break;
			  	  	 case 20:
			  	  			  	  			curTask = TASK_TURN_ANGLE;
			  	  			  	  			__PEND_CURCMD(curCmd);
			  	  			  	  			break;
			  	  	 case 21:
			  	  								curTask = TASK_NAVIGATE;
			  	  								__PEND_CURCMD(curCmd);
			  	  								break;
			  	  	 case 22:
							curTask = TASK_TURN_ANGLE_RIGHT;
							__PEND_CURCMD(curCmd);
							break;
			  	  	 case 23:
							curTask = TASK_ACK;
							__PEND_CURCMD(curCmd);
							break;
			  	  	 case 24:
							curTask = TASK_FASTESTPATH;
							//OLED_ShowNumber(10,10,curCmd.val,10,12);
							//OLED_Refresh_Gram();
							__PEND_CURCMD(curCmd);
							break;

			  	  	 case 88: // Axxx, rotate left by xxx degree
			  	  	 case 89: // Cxxx, rotate right by xxx degree
			  	  		 __SET_SERVO_TURN_MAX(&htim1, curCmd.index - 88);
			  	  		 __SET_MOTOR_DIRECTION(DIR_FORWARD);
			  	  		 if (curCmd.index == 88) {
			  	  			 targetAngle = curCmd.val;
			  	  			 __SET_MOTOR_DUTY(&htim8, 800, 1200);
			  	  		 } else {
			  	  			targetAngle = -curCmd.val;
			  	  			 __SET_MOTOR_DUTY(&htim8, 1200, 800);
			  	  		 }
			  	  		__PEND_CURCMD(curCmd);
			  	  		 RobotTurn(&targetAngle);
			  	  		 break;
			  	  	 case 99:
			  	  		 break;
			  	  	 case 100:
			  	  		 break;
			  	  	 default:
			  	  //		 curCmd.index = 99;
			  	  		 break;
			  	  	 }

			  	  osDelay(100);
		  }
  /* USER CODE END runDispatchTask */
}

/* USER CODE BEGIN Header_runFLTask */
/**
* @brief Function implementing the FLTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_runFLTask */
void runFLTask(void *argument)
{
  /* USER CODE BEGIN runFLTask */
  /* Infinite loop */
	//OLED_ShowString(10,30,"Hello,world");
			  //OLED_Refresh_Gram();
  for(;;)
  {
	  if (curTask != TASK_FL){

		  osDelay(1000);
	  }
	  	  else {
	  //		  osDelay(3000); // video demo only
	  		  switch(curCmd.val) {
	  		  case 30: // FL30 (4x2)
	  			  __SET_CMD_CONFIG(cfgs[CONFIG_FL30], &htim8, &htim1, targetAngle);
	  			  RobotTurn(&targetAngle);
	  			  osDelay(10);
	  			  targetDist1 = 4;
	  			  RobotMoveDist(&targetDist1, DIR_BACKWARD, SPEED_MODE_T);
	  			  osDelay(10);
	  			  break;
	  		  case 20: // FL20 (outdoor 3x1)
	  			  targetDist1 = 4;
	  			  RobotMoveDist(&targetDist1, DIR_BACKWARD, SPEED_MODE_T);
	  			  osDelay(10);
	  			  __SET_CMD_CONFIG(cfgs[CONFIG_FL20], &htim8, &htim1, targetAngle);
	  			  RobotTurn(&targetAngle);
	  			  osDelay(10);
	  			  targetDist1 = 7;
	  			  RobotMoveDist(&targetDist1, DIR_BACKWARD, SPEED_MODE_T);
	  			  osDelay(10);
	  			  break;
	  		  default: // FL00 (indoor 3x1)
	  			  targetDist1 = 4;
	  			  RobotMoveDist(&targetDist1, DIR_BACKWARD, SPEED_MODE_T);
	  			  osDelay(10);
	  			  __SET_CMD_CONFIG(cfgs[CONFIG_FL00], &htim8, &htim1, targetAngle);
	  			  RobotTurn(&targetAngle);
	  			  osDelay(10);
	  			  targetDist1 = 7;
	  			  RobotMoveDist(&targetDist1, DIR_BACKWARD, SPEED_MODE_T);
	  			  osDelay(10);
	  			  break;
	  		  }

	  		  clickOnce = 0;
	  		  prevTask = curTask;
	  		  curTask = TASK_NONE;

	  		  if (__COMMAND_QUEUE_IS_EMPTY(cQueue)) {
	  				__CLEAR_CURCMD(curCmd);
	  				__ACK_TASK_DONE(&huart3, rxMsg);
	  		  } else __READ_COMMAND(cQueue, curCmd, rxMsg);
	  	  }
  }
  /* USER CODE END runFLTask */
}

/* USER CODE BEGIN Header_runBLTask */
/**
* @brief Function implementing the BLTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_runBLTask */
void runBLTask(void *argument)
{
  /* USER CODE BEGIN runBLTask */
  /* Infinite loop */
  for(;;)
  {
	  if (curTask != TASK_BL) osDelay(1000);
	  	  else {
	  //		  osDelay(3000); // video demo only
	  		  switch(curCmd.val) {
	  		  case 30: // BL30 (4x2)
	  			  __SET_CMD_CONFIG(cfgs[CONFIG_BL30], &htim8, &htim1, targetAngle);
	  			  RobotTurn(&targetAngle);
	  			  osDelay(10);
	  			  targetDist1 = 4.5;
	  			  RobotMoveDist(&targetDist1, DIR_BACKWARD, SPEED_MODE_T);
	  			  osDelay(10);
	  			  break;
	  		  case 20: // BL20 (outdoor 3x1)
	  			  targetDist1 = 6;
	  			  RobotMoveDist(&targetDist1, DIR_FORWARD, SPEED_MODE_T);
	  			  osDelay(10);
	  			  __SET_CMD_CONFIG(cfgs[CONFIG_BL20], &htim8, &htim1, targetAngle);
	  			  RobotTurn(&targetAngle);
	  			  osDelay(10);
	  			  targetDist1 = 2;
	  			  RobotMoveDist(&targetDist1, DIR_FORWARD, SPEED_MODE_T);
	  			  osDelay(10);
	  			  break;
	  		  default: // BL00 (indoor 3x1)
	  			  targetDist1 = 6;
	  			  RobotMoveDist(&targetDist1, DIR_FORWARD, SPEED_MODE_T);
	  			  osDelay(10);
	  			  __SET_CMD_CONFIG(cfgs[CONFIG_BL00], &htim8, &htim1, targetAngle);
	  			  RobotTurn(&targetAngle);
	  			  osDelay(10);
	  			  targetDist1 = 2.5;
	  			  RobotMoveDist(&targetDist1, DIR_FORWARD, SPEED_MODE_T);
	  			  osDelay(10);
	  			  break;
	  		  }


	  		  clickOnce = 0;
	  		  prevTask = curTask;
	  		  curTask = TASK_NONE;
	  		  if (__COMMAND_QUEUE_IS_EMPTY(cQueue)) {
	  				__CLEAR_CURCMD(curCmd);
	  				__ACK_TASK_DONE(&huart3, rxMsg);
	  		  } else __READ_COMMAND(cQueue, curCmd, rxMsg);
	  	  }
  }
  /* USER CODE END runBLTask */
}

/* USER CODE BEGIN Header_runTurnRightTask */
/**
* @brief Function implementing the TurnRightAngle thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_runTurnRightTask */
void runTurnRightTask(void *argument)
{
  /* USER CODE BEGIN runTurnRightTask */
  /* Infinite loop */
	for(;;)
	  {
		  if (curTask != TASK_TURN_ANGLE && curTask !=TASK_TURN_ANGLE_RIGHT){
			  osDelay(100);

		  }else{

			  targetDist1 = 30;
			  		  //RobotMoveDist(&targetDist1, DIR_FORWARD, SPEED_MODE_T);

			  		//OLED_ShowNumber(10,10,curCmd.val,10,12);
			  		//OLED_Refresh_Gram();
			  		if(curTask == TASK_TURN_ANGLE){
			  			__SET_SERVO_TURN(&htim1, 90);
			  			targetAngle =(float)(curCmd.val-7);
			  		}else{
			  			__SET_SERVO_TURN(&htim1, 215);
			  			targetAngle =(float)(curCmd.val-8);//outdoor
			  			//targetAngle =(float)(curCmd.val-8);//indoor
			  		}
			  		    //OLED_ShowNumber(10,30,(int)targetAngle,10,12);
			  		    //OLED_Refresh_Gram();
			  		  	__SET_MOTOR_DIRECTION(DIR_FORWARD);

			  		   if(curTask == TASK_TURN_ANGLE){
			  			      __SET_MOTOR_DUTY(&htim8, 2000, 3500);
			  			  	  RobotTurn(&targetAngle);
						}else{
							  targetAngle=-targetAngle;
							  __SET_MOTOR_DUTY(&htim8, 3500, 2000);
							  RobotTurn(&targetAngle);
						}
			  		 //targetDist1 = 30;
			  		 //RobotMoveDist(&targetDist1, DIR_FORWARD, SPEED_MODE_2);
			  		  osDelay(1000);
			  		  clickOnce = 0;
					  prevTask = curTask;
					  curTask = TASK_NONE;

					  if (__COMMAND_QUEUE_IS_EMPTY(cQueue)) {
							__CLEAR_CURCMD(curCmd);
							__ACK_TASK_DONE(&huart3, rxMsg);
					  } else {
						  __READ_COMMAND(cQueue, curCmd, rxMsg);
					  }
			  	  }

		  }


  /* USER CODE END runTurnRightTask */
}

/* USER CODE BEGIN Header_runNavigateObst */
/**
* @brief Function implementing the Navigate_Obst thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_runNavigateObst */
void runNavigateObst(void *argument)
{
  /* USER CODE BEGIN runNavigateObst */
  /* Infinite loop */
//int i=0;
	uint8_t ch;
	ch='D';
  for(;;)
  {
	  if (curTask != TASK_NAVIGATE){
		  osDelay(1000);
	  }else{

		  	  //while(i<3){
		  targetDist1 = 40;
		  //HCSR04_Read();
		  //osDelay(10);
		  //Distance=UD;
		  //RobotMoveDistObstacle(&targetDist1, SPEED_MODE_2);
		  uint8_t dist[20];
		  for(int a=0;a<1000;a++){
			  sprintf(dist, "DIST:CM %5d", a);
			  				  OLED_ShowString(10,20,dist);
			  				  OLED_Refresh_Gram();
			  				  osDelay(20);
		  }
		  /*if((float)Distance-targetDist1>0.1 && flag==0){
			  RobotMoveDistObstacle(&targetDist1, SPEED_MODE_1);
			  targetDist1=12;
			  RobotMoveDist(&targetDist1, DIR_FORWARD, SPEED_MODE_T);
			  HAL_UART_Transmit(&huart3,(uint8_t*)&ch,1,0xFFFF);
			  osDelay(10);
			  flag=1;
		  }else{
			  targetDist1 = 10;
			  RobotMoveDist(&targetDist1, DIR_BACKWARD, SPEED_MODE_T);
			  __SET_CMD_CONFIG(cfgs[CONFIG_FL00], &htim8, &htim1, targetAngle);
			  RobotTurn(&targetAngle);
			  targetDist1 = 30;
			  RobotMoveDist(&targetDist1, DIR_FORWARD, SPEED_MODE_T);
			  __SET_CMD_CONFIG(cfgs[CONFIG_FR00], &htim8, &htim1, targetAngle);
			  RobotTurn(&targetAngle);
			  angleNow=0;
			  targetDist1 = 26;
			  RobotMoveDist(&targetDist1, DIR_BACKWARD, SPEED_MODE_T);
			  __SET_CMD_CONFIG(cfgs[19], &htim8, &htim1, targetAngle);
			  RobotTurn(&targetAngle);
			  //targetDist1 = 8;
			  //RobotMoveDistObstacle(&targetDist1, SPEED_MODE_1);
			  targetDist1=15;
			  RobotMoveDist(&targetDist1, DIR_FORWARD, SPEED_MODE_T);
			  HAL_UART_Transmit(&huart3,(uint8_t*)&ch,1,0xFFFF);
			  osDelay(10);

		  }*/
		  	clickOnce = 0;
		  	prevTask = curTask;
		  	curTask = TASK_NONE;

		  	if (__COMMAND_QUEUE_IS_EMPTY(cQueue)) {
		  		__CLEAR_CURCMD(curCmd);
		  		__ACK_TASK_DONE(&huart3, rxMsg);
		  		} else {
		  		 __READ_COMMAND(cQueue, curCmd, rxMsg);
		  		}
	  }






  }
  /* USER CODE END runNavigateObst */
}

/* USER CODE BEGIN Header_runUltrasonicTask */
/**
* @brief Function implementing the ultrasonicTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_runUltrasonicTask */
void runUltrasonicTask(void *argument)
{
  /* USER CODE BEGIN runUltrasonicTask */
  /* Infinite loop */
	HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
  for(;;)
  {
//	  float d = 100;
//	  RobotMoveDist(&d,1,SPEED_MODE_1);
//	  __SET_MOTOR_DUTY(&htim8, 600, 1000);
	  HCSR04_Read();
	  uint8_t dist[20];
////	  osDelay(10);
	  Distance=UD;
//	  sprintf(dist, "US DIST: %5d", Distance);
//	  OLED_ShowString(10,10,dist);
//	  __ADC_Read_Dist(&hadc1, dataPoint, IR_data_raw_acc, obsDist_IR, obsTick_IR);
//	  uint32_t dist1=obsDist_IR;
//	  sprintf(dist, "IR1 DIST: %5d", dist1);
//	  OLED_ShowString(10,20,dist);
//	  __ADC_Read_Dist(&hadc2, dataPoint, IR_data_raw_acc, obsDist_IR, obsTick_IR);
//	  uint32_t dist2=obsDist_IR;
//	  sprintf(dist, "IR2 DIST: %5d", dist2);
//	  OLED_ShowString(10,30,dist);
//
//	  sprintf(dist, "Buffer: %s", aRxBuffer);
//	  OLED_ShowString(10,40,dist);
//
//	  sprintf(dist, "totDis: %5d", (int)totalDistance);
//	  OLED_ShowString(10,50,dist);
//	  OLED_Refresh_Gram();
	  osDelay(10);
  }
  /* USER CODE END runUltrasonicTask */
}

/* USER CODE BEGIN Header_runFastestTask */
/**
* @brief Function implementing the FastestTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_runFastestTask */
void runFastestTask(void *argument)
{
  /* USER CODE BEGIN runFastestTask */

	__RESET_SERVO_TURN(&htim1);

	while (aRxBuffer[0] != 'S'){
		continue;
	}
	osDelay(10);
	totalDistance += Distance;
	// take first pic
	scan();
	targetDist1 = 30;
	// move straight till obstacle
	RobotMoveDistObstacleUS(&targetDist1, SPEED_MODE_2);

// Receive first image
    char resp = receiveFromBuffer();
//	char resp = 'R';

	if (resp == 'L'){
		FASTESTPATH_TURN_LEFT_180();
		scan();
	}  else{
		FASTESTPATH_TURN_RIGHT_180();
		scan();
	}

	// move straight till obstacle
	totalDistance += Distance;
//	__RESET_SERVO_TURN(&htim1);
	targetDist2 = 30;
	RobotMoveDistObstacleUS(&targetDist2, SPEED_MODE_2);
//	osDelay(500);
	osDelay(10);


	// Receive 2nd picture
	 resp = receiveFromBuffer();
//	resp = 'R';
	if (resp == 'L'){
//		OLED_ShowString(10,10,dist);
//		__RESET_SERVO_TURN(&htim1);
		FASTESTPATH_TURN_LEFT_90();
//		osDelay(10);
		//TODO
		osDelay(10);
		// go straight IR
//		__RESET_SERVO_TURN(&htim1);
		RobotMoveDistIR(SPEED_MODE_2, 1);

		// go straight
		float a = 10.0;
//		__RESET_SERVO_TURN(&htim1);
		RobotMoveDist(&a,DIR_FORWARD,SPEED_MODE_T);
		osDelay(200);

		// go right
//		__RESET_SERVO_TURN(&htim1);
		FASTESTPATH_TURN_RIGHT_90();
		osDelay(10);
//		htim1.Instance->CCR4=120;
//		osDelay(200);
//		__RESET_SERVO_TURN(&htim1);

		// go straight
		a = 15.0;
//		__RESET_SERVO_TURN(&htim1);
		RobotMoveDist(&a,DIR_FORWARD,SPEED_MODE_T);
//		osDelay(10);
		//TODO
		osDelay(200);

		// go right
//		__RESET_SERVO_TURN(&htim1);
		FASTESTPATH_TURN_RIGHT_90();
		osDelay(10);
//		htim1.Instance->CCR4=120;
//		osDelay(200);
//		__RESET_SERVO_TURN(&htim1);
//		osDelay(2000);

		a = 5.0;
//		__RESET_SERVO_TURN(&htim1);
		RobotMoveDist(&a,DIR_FORWARD,SPEED_MODE_T);
		//TODO
		osDelay(200);

		// go straight IR
//		__RESET_SERVO_TURN(&htim1);
		RobotMoveDistIR(SPEED_MODE_2, 1);
		osDelay(10);

		// go straight
		a = 15.0;
//		__RESET_SERVO_TURN(&htim1);
		RobotMoveDist(&a,DIR_FORWARD,SPEED_MODE_T);
//		osDelay(10);
		//TODO
		osDelay(200);

		// go right
//		__RESET_SERVO_TURN(&htim1);
		FASTESTPATH_TURN_RIGHT_90();
		osDelay(10);
//		osDelay(10);
//		htim1.Instance->CCR4=120;
//		osDelay(200);

		// go straight
//		__RESET_SERVO_TURN(&htim1);
		if (totalDistance > 200){
			totalDistance *= 1.15;
		}else{
			totalDistance *= 1.2;
		}
//		totalDistance *= 1.2;
		RobotMoveDistBack(&totalDistance,DIR_FORWARD,SPEED_MODE_T);
		osDelay(200);

		// go right
//		__RESET_SERVO_TURN(&htim1);
		FASTESTPATH_TURN_RIGHT_90();

		osDelay(10);
//		htim1.Instance->CCR4=120;
//		osDelay(200);
//		__RESET_SERVO_TURN(&htim1);

		// go back abit
		a = 45.0;
//		__RESET_SERVO_TURN(&htim1);
		RobotMoveDist(&a,DIR_BACKWARD,SPEED_MODE_T);
		osDelay(500);

		// go straight till IR
//		__RESET_SERVO_TURN(&htim1);
		RobotMoveDistIR2(SPEED_MODE_2, 0);
		osDelay(500);

		// go straight
		a = 20.0;
//		__RESET_SERVO_TURN(&htim1);
		RobotMoveDist(&a,DIR_FORWARD,SPEED_MODE_T);
		osDelay(10);


		// go left
//		__RESET_SERVO_TURN(&htim1);
		FASTESTPATH_TURN_LEFT_90();
//		osDelay(10);

		// go straight till us
		targetDist3 = 20;
		RobotMoveDistObstacleUS(&targetDist3, SPEED_MODE_2);

	} else {
		// go right
//		__RESET_SERVO_TURN(&htim1);
		FASTESTPATH_TURN_RIGHT_90();
		osDelay(10);
		// go straight IR
//		__RESET_SERVO_TURN(&htim1);
//		osDelay(2000);
		RobotMoveDistIR(SPEED_MODE_2, 0);
//		osDelay(10);

		// go straight
		float a = 5.0;
//		__RESET_SERVO_TURN(&htim1);
		RobotMoveDist(&a,DIR_FORWARD,SPEED_MODE_T);
		osDelay(200);

		// go left
//		__RESET_SERVO_TURN(&htim1);
		FASTESTPATH_TURN_LEFT_90();
		osDelay(10);

		// go straight
		a = 15.0;
//		__RESET_SERVO_TURN(&htim1);
		RobotMoveDist(&a,DIR_FORWARD,SPEED_MODE_T);
		osDelay(200);

		// go left
//		__RESET_SERVO_TURN(&htim1);
		FASTESTPATH_TURN_LEFT_90();
		osDelay(10);

		a = 5.0;
//		__RESET_SERVO_TURN(&htim1);
		RobotMoveDist(&a,DIR_FORWARD,SPEED_MODE_T);
		//TODO
		osDelay(200);

		// go straight IR
//		__RESET_SERVO_TURN(&htim1);
		RobotMoveDistIR(SPEED_MODE_2, 0);
		osDelay(200);

		// go straight
//		a = 5.0;
//		__RESET_SERVO_TURN(&htim1);
//		RobotMoveDist(&a,DIR_FORWARD,SPEED_MODE_T);
//		osDelay(10);

		// go left
//		__RESET_SERVO_TURN(&htim1);
		FASTESTPATH_TURN_LEFT_90();
		osDelay(10);

		// go straight
//		__RESET_SERVO_TURN(&htim1);
		if (totalDistance > 200){
			totalDistance *= 1.15;
		}else{
			totalDistance *= 1.2;
		}
//		totalDistance *= 1.2;
		RobotMoveDistBack(&totalDistance,DIR_FORWARD,SPEED_MODE_T);
		osDelay(200);

		// go left
//		__RESET_SERVO_TURN(&htim1);
		FASTESTPATH_TURN_LEFT_90();
		osDelay(10);

		// go back abit
		a = 40.0;
//		__RESET_SERVO_TURN(&htim1);
		RobotMoveDist(&a,DIR_BACKWARD,SPEED_MODE_T);
		osDelay(500);

		// go straight till IR
//		__RESET_SERVO_TURN(&htim1);
		RobotMoveDistIR2(SPEED_MODE_2,1);
		osDelay(500);
//		osDelay(10);

		// go straight
		a = 20.0;
//		__RESET_SERVO_TURN(&htim1);
		RobotMoveDist(&a,DIR_FORWARD,SPEED_MODE_T);
		osDelay(10);

		// go right
//		__RESET_SERVO_TURN(&htim1);
		FASTESTPATH_TURN_RIGHT_90();
		osDelay(10);
//		htim1.Instance->CCR4=120;
//		osDelay(200);
//		__RESET_SERVO_TURN(&htim1);


		// go straight till us
		targetDist4 = 20;
		RobotMoveDistObstacleUS(&targetDist4, SPEED_MODE_2);
	}

	//


//	uint8_t hadOvershoot = 0;
//	uint8_t dist[20];
//	sprintf(dist, "%8d Triggered");
//	OLED_ShowString(10,20,dist);
//	int cnt = 0;
//	  /* Infinite loop */
//	  for(;;)
//	  {
//		  curTask = TASK_FASTESTPATH;
//		  step = 0	;
//		  if (curTask != TASK_FASTESTPATH) osDelay(1000);
//		  else {
//			  if (step == 0) {
//				  targetDist = 50;
////				  RobotMoveDistObstacle(&targetDist, SPEED_MODE_2);
//				  __RESET_SERVO_TURN(&htim1);
////				  RobotMoveDist(&targetDist, 1, SPEED_MODE_2);
////				  __SET_MOTOR_DUTY(&htim8, 800, 3000);
////				  __SET_SERVO_TURN(&htim1, 90);
//			  } else if (step == 1) {
////				  //2:  turn left by 90 degree, record down angle when US sensor overshoot
//				  hadOvershoot = 0;
//				  angleNow = 0; gyroZ = 0;
//				  angle_left = 0;
//				  targetAngle = 90;
//				  obsDist_US = 0;
//				  __SET_SERVO_TURN_MAX(&htim1, 0);
//				  __SET_MOTOR_DUTY(&htim8, 800, 3000);
//				  __SET_MOTOR_DIRECTION(1);
////				  HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_2);
//				  last_curTask_tick = HAL_GetTick();
//				  do {
//					  __Gyro_Read_Z(&hi2c1, readGyroZData, gyroZ);
//					  if (!hadOvershoot) {
//						  HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, GPIO_PIN_SET);  // pull the TRIG pin HIGH
////						  __delay_us(&htim4, 10); // wait for 10us
//						  osDelay(10);
//						  HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, GPIO_PIN_RESET);  // pull the TRIG pin low
//						  __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_CC2);
//						  osDelay(5); // give timer interrupt chance to update obsDist_US value
//					  }
//
//
//					  if (HAL_GetTick() - last_curTask_tick >=10) {
//	//					  __Gyro_Read_Z(&hi2c1, readGyroZData, gyroZ);
//						  angleNow += gyroZ / GRYO_SENSITIVITY_SCALE_FACTOR_2000DPS * 0.01;
//						  if (obsDist_US > 55 && !hadOvershoot) {
//							  angle_left = angleNow;
//							  hadOvershoot = 1;
//						  }
//
//						  if (abs(targetAngle - angleNow) < 0.01) break;
//						  last_curTask_tick = HAL_GetTick();
//					  }
//
//					} while (1);
//				  __SET_MOTOR_DUTY(&htim8, 0, 0);
//				  __RESET_SERVO_TURN(&htim1);
//				  osDelay(10);
//
//				  obs_a = 30 * tanf(angle_left * PI / 180);
//				  angle_right = atanf((60 - obs_a) / 30) * 180 / PI;
//				  x = sqrtf((60 - obs_a) * (60 - obs_a) + 900) - 23; // 23 robot length offset
//
//			  } else if (step == 2) {
//				  // 3: move forward until IR overshoot
//				  cnt = 0;
//				  if(cnt==0)
//				  {
//					  RobotMoveUntilIROvershoot();
//					  osDelay(10);
//					  cnt++;
//				  }
//			  }else if (step == 3) {
//				  // 4: Turn right by 180 degree
//				  FASTESTPATH_TURN_RIGHT_180();
//				  osDelay(10);
//			  } else if (step == 4){
//				  // 5: move forward until right beside obstacle
//				  RobotMoveUntilIRHit();
//				  osDelay(10);
//			  }else if (step == 5) {
//				  // 6: move forward until IR overshoot
//				  RobotMoveUntilIROvershoot();
//				  osDelay(10);
//			  }else if (step == 6) {
//				  // 7: Turn right by 90 degree
//				  FASTESTPATH_TURN_RIGHT_90();
//				  osDelay(10);
//			  }else if (step == 7) {
//				  // 8: move forward until IR overshoot
//				  RobotMoveUntilIROvershoot();
//				  osDelay(10);
//			  }else if (step == 8) {
//				  // 9: turn right by angle_right
//				  __SET_SERVO_TURN_MAX(&htim1, 1);
//				  __SET_MOTOR_DUTY(&htim8, 2000, 1000);
//				  targetAngle = angle_right *-1;
//				  RobotTurn(&targetAngle);
//			  }else if (step == 9) {
//				  //10: move until center of the original path
//				  targetDist = x;
//				  RobotMoveDist(&targetDist, 1, SPEED_MODE_T);
//			  }else if (step == 10) {
//				  //11: turn left to face the carpark
//				  __SET_SERVO_TURN_MAX(&htim1, 0);
//				  __SET_MOTOR_DUTY(&htim8, 1000, 2000);
//				  targetAngle = angle_right;
//				  RobotTurn(&targetAngle);
//			  } else if (step == 11) {
//				  //12: back to the carpark
//				  targetDist = 15;
//				  RobotMoveDistObstacle(&targetDist, SPEED_MODE_2);
//			  }
////			  obsDist_IR=1000;
////			  for(int a=0;a<1000;a++){
////				  __ADC_Read_Dist(&hadc1, dataPoint, IR_data_raw_acc, obsDist_IR, obsTick_IR);
////				/*uint32_t aaa;
////				HAL_ADC_Start(&hadc1);
////				if(HAL_ADC_PollForConversion(&hadc1,20)!=HAL_OK){
////					aaa=999;
////				}else{
////					aaa=HAL_ADC_GetValue(&hadc1);
////				}*/
////				  //OLED_ShowString(10,20,"hello1");z
////					  //OLED_Refresh_Gram();
////				  uint32_t dist1=obsDist_IR;
////				  sprintf(dist, "%8d CM",  dist1);
////				  OLED_ShowString(10,20,dist);
////				  OLED_Refresh_Gram();
////				  osDelay(20);
////			  }
////
////			  clickOnce = 0;
////			  prevTask = curTask;
//			  curTask = TASK_NONE;
//			  __CLEAR_CURCMD(curCmd);
//			  __ACK_TASK_DONE(&huart3, rxMsg);

//		  }
//	  }
  /* USER CODE END runFastestTask */
}

/* USER CODE BEGIN Header_runTestTask */
/**
* @brief Function implementing the TestTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_runTestTask */
void runTestTask(void *argument)
{
  /* USER CODE BEGIN runTestTask */
  /* Infinite loop */
	__RESET_SERVO_TURN(&htim1);

	//functions
//	targetDist1 = 50.0;
//	RobotMoveDistObstacle(&targetDist1, SPEED_MODE_2);
//	RobotMoveDistIR2(SPEED_MODE_2, 0);
	FASTESTPATH_TURN_RIGHT_90();
	FASTESTPATH_TURN_LEFT_90();
//	RobotMoveDistIR2(SPEED_MODE_2, 0);
//	RobotMoveDistIR(SPEED_MODE_2, 0);
//	FASTESTPATH_TURN_LEFT_AROUND();
//	FASTESTPATH_TURN_RIGHT_AROUND();
//	FASTESTPATH_TURN_RIGHT_180();
//	FASTESTPATH_TURN_LEFT_180();

	//Testing
//	totalDistance = 100;
//	RobotMoveDistBack(&totalDistance,DIR_FORWARD,SPEED_MODE_T);
	//Delay
	osDelay(10000);
  /* USER CODE END runTestTask */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

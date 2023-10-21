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
#include "oled.h"
#include <stdbool.h>
#include "ICM20948.h"
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
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim8;

UART_HandleTypeDef huart3;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for motorTask */
osThreadId_t motorTaskHandle;
const osThreadAttr_t motorTask_attributes = {
  .name = "motorTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for oledTask */
osThreadId_t oledTaskHandle;
const osThreadAttr_t oledTask_attributes = {
  .name = "oledTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for rpiTask */
osThreadId_t rpiTaskHandle;
const osThreadAttr_t rpiTask_attributes = {
  .name = "rpiTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for gyroTask */
osThreadId_t gyroTaskHandle;
const osThreadAttr_t gyroTask_attributes = {
  .name = "gyroTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow1,
};
/* Definitions for bulleyesTask */
osThreadId_t bulleyesTaskHandle;
const osThreadAttr_t bulleyesTask_attributes = {
  .name = "bulleyesTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for encoderRightTas */
osThreadId_t encoderRightTasHandle;
const osThreadAttr_t encoderRightTas_attributes = {
  .name = "encoderRightTas",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for encoderLeftTask */
osThreadId_t encoderLeftTaskHandle;
const osThreadAttr_t encoderLeftTask_attributes = {
  .name = "encoderLeftTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for showTask */
osThreadId_t showTaskHandle;
const osThreadAttr_t showTask_attributes = {
  .name = "showTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM8_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM4_Init(void);
void StartDefaultTask(void *argument);
void StartMotorTask(void *argument);
void StartOledTask(void *argument);
void StartRpiTask(void *argument);
void StartGyroTask(void *argument);
void StartBulleyesTask(void *argument);
void StartEncoderRightTask(void *argument);
void StartEncoderLeftTask(void *argument);
void show(void *argument);
char scanAndReceive();
void moveLeftSide();

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// communication
uint8_t aRxBuffer[20] = {0};
uint8_t old_Buff[20] = {0};
uint8_t old_Buff1[20] = {0};

uint8_t testBuffer[20] = {0};
char testKey;

int flagDone = 0;
char key;
char direction;
int magnitude = 0;

int cntS = 0;

char resp = ' ';

// movement
uint16_t pwmVal_servo = 146;
uint16_t pwmVal_R = 0;
uint16_t pwmVal_L = 0;
int times_acceptable=0;
int e_brake = 0;


// MotorEncoder
int32_t rightEncoderVal = 0, leftEncoderVal = 0;
int32_t rightTarget = 0, leftTarget = 0;
double target_angle=0;


// Gyro
double total_angle=0;
uint8_t gyroBuffer[20];
uint8_t ICMAddress = 0x68;

//PID
int8_t dir = 1;
int correction = 0;
uint16_t newDutyL, newDutyR = 0;
uint8_t readGyroZData[2];
int16_t gyroZ;
float targetAngle = 0;
float angleNow = 0;

//float Kp = 2.5;
//float Ki = 0.0;
//float Kd 0.8;
//float ek1 = 0;
//float ekSum = 0;

typedef struct _pidConfig {
	float Kp;
	float Ki;
	float Kd;
	float ek1;
	float ekSum;
} PIDConfig;
//inside
PIDConfig pid_1 = {.Kp =1.8, .Ki = 0.0, .Kd = 0.000, .ek1 = 0.0, .ekSum = 0.0};
PIDConfig pid_3 = {.Kp = 1.6, .Ki = 0.0, .Kd = 0.0, .ek1 = 0.0, .ekSum = 0.0};


//outside
//PIDConfig pid_1 = {.Kp =1.5, .Ki = 0.0, .Kd = 0.00, .ek1 = 0.0, .ekSum = 0.0};
//PIDConfig pid_3 = {.Kp = 1.2, .Ki = 0.0, .Kd = 0.0, .ek1 = 0.0, .ekSum = 0.0};


int16_t angular_speed = 0;
//double offset = 0;

// Oled
char instructBuffer[40][5]={0};

//UserButton
int start = 0;

//Ultrasound
int Is_First_Captured = 0;
int32_t IC_Val1 = 0;
int32_t IC_Val2 = 0;
double Difference =0;
double uDistance = 0;






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
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM8_Init();
  MX_USART3_UART_Init();
  MX_I2C1_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  OLED_Init();
  ICM20948_init(&hi2c1,0,GYRO_FULL_SCALE_2000DPS);

  // for debug
  //HAL_UART_Receive_IT(&huart3, (uint8_t *) aRxBuffer, 1);
  // for real task
  HAL_UART_Receive_IT(&huart3, (uint8_t *) aRxBuffer, 5);

//  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);
//
//  /* creation of MotorTask */
//  motorTaskHandle = osThreadNew(StartMotorTask, NULL, &motorTask_attributes);
//
//  /* creation of oledTask */
//  oledTaskHandle = osThreadNew(StartOledTask, NULL, &oledTask_attributes);
//
//  /* creation of rpiTask */
//  rpiTaskHandle = osTreadNew(StartRpiTask, NULL, &rpiTask_attributes);
//
//  /* creation of gyroTask */
//  gyroTaskHandle = osTreadNew(StartGyroTask, NULL, &gyroTask_attributes);
//
//  /* creation of bulleyesTask */
//  bulleyesTaskHandle = osThreadNew(StartBulleyesTask, NULL, &bulleyesTask_attributes);
//
//	/* creation of encoderRightTas */
//	encoderRightTasHandle = osThreadNew(StartEncoderRightTask, NULL, &encoderRightTas_attributes);
//
//  /* creation of encoderLeftTask */
//  encoderLeftTaskHandle = osThreadNew(StartEncoderLeftTask, NULL, &encoderLeftTask_attributes);

//    defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

    /* creation of MotorTask */
//    motorTaskHandle = osThreadNew(StartMotorTask, NULL, &motorTask_attributes);

    /* creation of oledTask */
//    oledTaskHandle = osThreadNew(StartOledTask, NULL, &oledTask_attributes);

    /* creation of rpiTask */
//    rpiTaskHandle = osThreadNew(StartRpiTask, NULL, &rpiTask_attributes);

    /* creation of gyroTask */
//    gyroTaskHandle = osThreadNew(StartGyroTask, NULL, &gyroTask_attributes);

    /* creation of bulleyesTask */
//    bulleyesTaskHandle = osThreadNew(StartBulleyesTask, NULL, &bulleyesTask_attributes);

	/* creation of encoderRightTas */
//	encoderRightTasHandle = osThreadNew(StartEncoderRightTask, NULL, &encoderRightTas_attributes);

	/* creation of encoderLeftTask */
//	encoderLeftTaskHandle = osThreadNew(StartEncoderLeftTask, NULL, &encoderLeftTask_attributes);


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

  /* creation of motorTask */
  motorTaskHandle = osThreadNew(StartMotorTask, NULL, &motorTask_attributes);

  /* creation of oledTask */
//  oledTaskHandle = osThreadNew(StartOledTask, NULL, &oledTask_attributes);

  /* creation of rpiTask */
  rpiTaskHandle = osThreadNew(StartRpiTask, NULL, &rpiTask_attributes);

  /* creation of gyroTask */
  gyroTaskHandle = osThreadNew(StartGyroTask, NULL, &gyroTask_attributes);

  /* creation of bulleyesTask */
//  bulleyesTaskHandle = osThreadNew(StartBulleyesTask, NULL, &bulleyesTask_attributes);

  /* creation of encoderRightTas */
  encoderRightTasHandle = osThreadNew(StartEncoderRightTask, NULL, &encoderRightTas_attributes);

  /* creation of encoderLeftTask */
  encoderLeftTaskHandle = osThreadNew(StartEncoderLeftTask, NULL, &encoderLeftTask_attributes);

  /* creation of showTask */
  showTaskHandle = osThreadNew(show, NULL, &showTask_attributes);

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
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
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
  sConfig.IC1Filter = 10;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 10;
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

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 10;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 10;
  if (HAL_TIM_Encoder_Init(&htim3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
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
  if (HAL_TIM_IC_ConfigChannel(&htim4, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, OLED_SCL_Pin|OLED_SDA_Pin|OLED_RST_Pin|OLED_DC_Pin
                          |LED3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, AIN2_Pin|AIN1_Pin|BIN1_Pin|BIN2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Trigger_GPIO_Port, Trigger_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : OLED_SCL_Pin OLED_SDA_Pin OLED_RST_Pin OLED_DC_Pin
                           LED3_Pin */
  GPIO_InitStruct.Pin = OLED_SCL_Pin|OLED_SDA_Pin|OLED_RST_Pin|OLED_DC_Pin
                          |LED3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : AIN2_Pin AIN1_Pin BIN1_Pin BIN2_Pin */
  GPIO_InitStruct.Pin = AIN2_Pin|AIN1_Pin|BIN1_Pin|BIN2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB10 */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : USER_PB_Pin */
  GPIO_InitStruct.Pin = USER_PB_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_PB_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Trigger_Pin */
  GPIO_InitStruct.Pin = Trigger_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Trigger_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	/* to prevent unused argument(s) compilation warning */
	UNUSED(huart);
	// for debug
	//HAL_UART_Receive_IT (&huart3, aRxBuffer, 1);
	// for real task
	HAL_UART_Receive_IT (&huart3, aRxBuffer, 5);
}

void HAL_GPIO_EXTI_Callback( uint16_t GPIO_Pin ) {
	if (GPIO_Pin == USER_PB_Pin) {
		if (start == 0){
			start = 1;
		    }
		else
			start = 0;
 	    }
}

void delay(uint16_t time)
{
 __HAL_TIM_SET_COUNTER(&htim4, 0);
 while (__HAL_TIM_GET_COUNTER (&htim4) < time);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
 if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
 {
  if (Is_First_Captured == 0)
  {
   IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
   Is_First_Captured = 1;
   __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
  }
  else if (Is_First_Captured == 1)
  {
   IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
   __HAL_TIM_SET_COUNTER(htim,0);

   if (IC_Val2 > IC_Val1)
   {
    Difference = IC_Val2 - IC_Val1;
   }

   else if (IC_Val1 > IC_Val2)
   {
    Difference = (65535 - IC_Val1) + IC_Val2;
   }

   uDistance = Difference * .0343/2;

   Is_First_Captured = 0;

   __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
   __HAL_TIM_DISABLE_IT(&htim4, TIM_IT_CC1);
  }
 }
}

void HCSR04_Read (void) //Call when want to get reading from US
{
 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
 delay(10);
 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
 __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_CC1);
}

void buzzerBeep()
{
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10); //Buzzer On
	HAL_Delay(500);
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10); //Buzzer Off
}

char scanAndReceive() {
    // transmit S -> scan
    char scan = 'S';
    aRxBuffer[0] = ' ';
    HAL_UART_Transmit(&huart3, (uint8_t *)&scan, 1, 0xFFFF);

    // waiting for interrupt
    for (;;){
        testKey = aRxBuffer[0];
        if (testKey!=' '){
          return testKey;
        }
    }
}

void moveCarStraight(double distance)
{
	distance = distance*75;
//	pwmVal_servo = 124;
//	osDelay(100);
	pwmVal_servo = 146;
//	osDelay(500);
	osDelay(100);
	e_brake = 0;
	times_acceptable=0;
	rightEncoderVal = 75000;
	leftEncoderVal = 75000;
	rightTarget = 75000;
	leftTarget = 75000;
	rightTarget += distance;
	leftTarget += distance;
}

void moveCarStraightOverall(double totalDistance)
{
	double maxDistance = 30;
	while (totalDistance != 0){
		if (maxDistance > totalDistance){
			maxDistance = totalDistance;
		}
		moveCarStraight(maxDistance);
		totalDistance -= maxDistance;
		while(finishCheck());


	}
}

void moveCarStop()
{
	e_brake = 0;
	pwmVal_servo = 146;
//	osDelay(300);
	osDelay(10);
}

void moveCarRight(double angle)
{
//	pwmVal_servo = 230;
	pwmVal_servo = 225;
//	osDelay(500);
	osDelay(50);
	e_brake = 0;
	times_acceptable=0;
	target_angle -= angle;
//	if(target_angle<=0)
//		target_angle = 0;
}

void moveCarLeft(double angle)
{
	pwmVal_servo = 98;
//	osDelay(500);
	osDelay(50);
	e_brake = 0;
	times_acceptable=0;
	target_angle += angle;
}

void moveCarSlideRight(int forward){
	int sign;
	if(forward == 1){
		sign = 1;
	}else{
		sign = -1;
	}
	e_brake = 0;
	times_acceptable=0;
	moveCarStraight(2*sign);
//	moveCarStraight(0);
	while(finishCheck());
	osDelay(500);
//	osDelay(50);
	times_acceptable=0;
	moveCarRight(29*sign);
	while(finishCheck());
	osDelay(500);
	times_acceptable=0;
	if(sign==1)
		moveCarLeft(28*sign);
	else
		moveCarLeft(31*sign);
	while(finishCheck());
//	osDelay(500);
	osDelay(50);
	pwmVal_servo = 120; //143
	osDelay(500);
	times_acceptable=0;
	if(sign==1)
		moveCarStraight(1*sign);
	else
		moveCarStraight(0);
//	moveCarStraight();
	while(finishCheck());
	osDelay(50);
//	if(sign==1)
//	{
//		times_acceptable=0;
//		moveCarRight(21*sign);
//		while(finishCheck());
//		osDelay(50);
//		times_acceptable=0;
//		moveCarLeft(30.5*sign);
//		while(finishCheck());
//		//	osDelay(500);
//		osDelay(500);
//		pwmVal_servo = 120; //143
//		osDelay(1000);
//		times_acceptable=0;
//		moveCarStraight(10*sign);
//		while(finishCheck());
//		osDelay(500);
//	}else
//	{
//		times_acceptable=0;
//		moveCarRight(21*sign);
//		while(finishCheck());
//		osDelay(50);
//		times_acceptable=0;
//		moveCarLeft(30.5*sign);
//		while(finishCheck());
//		//	osDelay(500);
//		osDelay(500);
//		pwmVal_servo = 120; //143
//		osDelay(1000);
//		times_acceptable=0;
//		moveCarStraight(10*sign);
//		while(finishCheck());
//		osDelay(500);
//	}
}

void moveCarSlideLeft(int forward){
	int sign;
	if(forward == 1){
		sign = 1;
	}else{
		sign = -1;
	}
	e_brake = 0;
	times_acceptable=0;
	moveCarStraight(2*sign);
	while(finishCheck());
	osDelay(50);
	times_acceptable=0;
	if(sign==1)
		moveCarLeft(26*sign);
	else
		moveCarLeft(28*sign);
	while(finishCheck());
	osDelay(50);
	times_acceptable=0;
	if(sign==1)
		moveCarRight(33*sign);
	else
		moveCarRight(28*sign);
	while(finishCheck());
	osDelay(50);
	pwmVal_servo = 120; //143
	osDelay(500);
	times_acceptable=0;
	if(sign==1)
		moveCarStraight(1*sign);
	else
		moveCarStraight(3*sign);
	while(finishCheck());
//	osDelay(500);
//	osDelay(1000);
//	if(sign==1)
//	{
//		times_acceptable=0;
//		moveCarLeft(20*sign);
//		while(finishCheck());
//		osDelay(50);
//		times_acceptable=0;
//		moveCarRight(31*sign);
//		while(finishCheck());
//		//	osDelay(500);
//		osDelay(500);
//		pwmVal_servo = 120; //143
//		osDelay(1000);
//		times_acceptable=0;
//		moveCarStraight(10*sign);
//		while(finishCheck());
//		osDelay(500);
//	}else
//	{
//		times_acceptable=0;
//		moveCarLeft(22*sign);
//		while(finishCheck());
//		osDelay(50);
//		times_acceptable=0;
//		moveCarRight(35*sign);
//		while(finishCheck());
//		//	osDelay(500);
//		osDelay(500);
//		pwmVal_servo = 120; //143
//		osDelay(1000);
//		times_acceptable=0;
//		moveCarStraight(10*sign);
//		while(finishCheck());
//		osDelay(500);
//	}
}

void moveCarRight90(double angle, int dir)
{
	e_brake = 0;
	int sign = 1;
	if(angle<0){
		sign=-1;
	}
	pwmVal_servo = 146; //146
	times_acceptable=0;
	if(dir>0)
//		moveCarStraight(10);
		moveCarStraight(0);
	else
		moveCarStraight(3);///
	while(finishCheck());
	osDelay(500);
//	osDelay(50);
	times_acceptable=0;
	if(sign==1)
		moveCarRight(angle-0.5);
	else
		moveCarRight(angle);
	while(finishCheck());
	osDelay(50);
	times_acceptable=0;
	if(dir>0)
	{
		pwmVal_servo = 110; //143
		osDelay(1000);
		pwmVal_servo = 148;
		osDelay(1000);
		moveCarStraight(-2);
	}
	else
	{
		pwmVal_servo = 120; //143
		osDelay(1000);
		moveCarStraight(-7);
	}
	while(finishCheck());
//	osDelay(500);
	osDelay(500);
}

void moveCarLeft90(double angle, int dir)
{
	e_brake = 0;
	int sign = 1;
	if(angle<0){
		sign=-1;
	}
	pwmVal_servo = 146; //146
	times_acceptable=0;
	if(dir>0)
		moveCarStraight(1);
	else
		moveCarStraight(2);
	while(finishCheck());
	osDelay(500);
//	osDelay(50);
	times_acceptable=0;
	moveCarLeft(angle);
	while(finishCheck());
	osDelay(50);
	times_acceptable=0;
	if(dir>0)
	{
//		pwmVal_servo = 120; //143
//		osDelay(1000);

		moveCarStraight(-2);
	}
	else
	{
//		pwmVal_servo = 120; //143
//		osDelay(1000);
		moveCarStraight(-4);
	}
	while(finishCheck());
	osDelay(500);
//	osDelay(50);
}

void moveLeftSide(){
    const char* instructions[] = {"SF000", "LF090", "SF030", "RF090", "SB020", "RF090"};
    size_t arraySize = sizeof(instructions) / sizeof(instructions[0]);

    int Ryan_sign_fuckmylife = 1;
    // Loop through the array of strings
    for (size_t i = 0; i < arraySize; i++) {
    	aRxBuffer[0] = instructions[i][0];
	  aRxBuffer[1] = instructions[i][1];
	  aRxBuffer[2] = instructions[i][2];
	  aRxBuffer[3] = instructions[i][3];
	  aRxBuffer[4] = instructions[i][4];

        magnitude = 0;
        key = aRxBuffer[0];
        direction = aRxBuffer[1];
        magnitude = ((int)(aRxBuffer[2])-48)*100 + ((int)(aRxBuffer[3])-48)*10 + ((int)(aRxBuffer[4])-48);
        Ryan_sign_fuckmylife = 1;

        if(direction == 'B' || direction == 'b'){
          magnitude *= -1;
          Ryan_sign_fuckmylife = -1;
       }

      osDelay(500);
	switch (key){
	 case 'D':
	  break;
	 case 'S':
	  times_acceptable=0;
	  moveCarStraight(magnitude);
	  while(finishCheck());
	  osDelay(100);

	  break;
	 case 'R':
	  times_acceptable=0;
	  moveCarRight90(90*Ryan_sign_fuckmylife, Ryan_sign_fuckmylife);
	  while(finishCheck());
	  osDelay(100);
	  break;
	 case 'L':
	  times_acceptable=0;
	  moveCarLeft90(90*Ryan_sign_fuckmylife, Ryan_sign_fuckmylife);
	  while(finishCheck());
//	  flagDone=1;
	  osDelay(100);
	  break;
	}
  }
}

void PIDConfigReset(PIDConfig * cfg) {
	cfg->ek1 = 0;
	cfg->ekSum = 0;
}

void PID_SPEED_1(PIDConfig *cfg, int error, int correction, int8_t dir)
{
	correction = (cfg)->Kp * error + (cfg)->Ki * (cfg)->ekSum + (cfg)->Kd * (error - (cfg)->ek1);
	(cfg)->ek1 = error;
	(cfg)->ekSum += error;
	uint8_t hello [20] = "Hello World!\0";
	sprintf(hello, "Corr: %d\0", (int)correction);
	OLED_ShowString(10, 30, hello);
	correction = correction > 700 ? 700 : (correction < -700 ? -700 : correction);
//	correction = correction > 400 ? 400 : (correction < -400 ? -400 : correction);
//	correction = correction > 400 ? 800 : (correction < -400 ? -0 : abs(correction*8));
	pwmVal_L = (3000 + correction*dir);
	pwmVal_R = (3000 - correction*dir);
//	pwmVal_L = 2000;
//	pwmVal_R = 2000;
}

void PID_SPEED_2(PIDConfig *cfg, int error, int correction, int8_t dir)
{
	correction = (cfg)->Kp * error + (cfg)->Ki * (cfg)->ekSum + (cfg)->Kd * (error - (cfg)->ek1);
	(cfg)->ek1 = error;
	(cfg)->ekSum += error;
	correction = correction > 700 ? 700 : (correction < -700 ? -700 : correction);
//	correction = correction > 400 ? 400 : (correction < -400 ? -400 : correction);
	newDutyL = 2300 + correction*dir;
	newDutyR = 2300 - correction*dir;
}

void PID_SPEED_3(PIDConfig *cfg, int error, int correction, int8_t dir)
{
	correction = (cfg)->Kp * error + (cfg)->Ki * (cfg)->ekSum + (cfg)->Kd * (error - (cfg)->ek1);
	(cfg)->ek1 = error;
	(cfg)->ekSum += error;
	correction = correction > 700 ? 700 : (correction < -700 ? -700 : correction);
//	correction = correction > 400 ? 400 : (correction < -400 ? -400 : correction);
	newDutyL = 1000 + correction*dir;
	newDutyR = 1000 - correction*dir;
}

int PID_Control(int error, int right)
{
	if(right){//rightMotor
		if(error>0){//go forward
			HAL_GPIO_WritePin(GPIOA, BIN1_Pin, GPIO_PIN_SET); // set direction of rotation for wheel B- forward
			HAL_GPIO_WritePin(GPIOA, BIN2_Pin, GPIO_PIN_RESET);
		}else{//go backward
		    HAL_GPIO_WritePin(GPIOA, BIN1_Pin, GPIO_PIN_RESET); // set direction of rotation for wheel B - reverse
			HAL_GPIO_WritePin(GPIOA, BIN2_Pin, GPIO_PIN_SET);
		}
	}else{//leftMotor
		if(error>0){//go forward
			HAL_GPIO_WritePin(GPIOA, AIN1_Pin, GPIO_PIN_SET); // set direction of rotation for wheel A - forward
			HAL_GPIO_WritePin(GPIOA, AIN2_Pin, GPIO_PIN_RESET);
		}else{//go backward
		    HAL_GPIO_WritePin(GPIOA, AIN1_Pin, GPIO_PIN_RESET); // set direction of rotation for wheel A - reverse
			HAL_GPIO_WritePin(GPIOA, AIN2_Pin, GPIO_PIN_SET);
		}
	}

	error = abs(error);
	if(error > 2000){
		return 3000;
	}else if(error > 500){
		return 2000;
	}else if(error > 200){
		return 1400;
	}else if(error > 100){
		return 1000;
	}else if(error > 2){
		times_acceptable++;
		return 500;
	}else if(error >=1){
		times_acceptable++;
		return 0;
	}else{
		times_acceptable++;
		return 0;
	}

//	if(error > 100){
//		return 3000;
//	}else if(error > 2){
//		times_acceptable++;
//		return 500;
//	}else if(error >=1){
//		times_acceptable++;
//		return 0;
//	}else{
//		times_acceptable++;
//		return 0;
//	}
}

int PID_Angle(double errord, int right)
{
	int error = (int)(errord*10);
	if(right){//rightMotor
		if(error>0){//go forward
			HAL_GPIO_WritePin(GPIOA, BIN1_Pin, GPIO_PIN_SET); // set direction of rotation for wheel B- forward
			HAL_GPIO_WritePin(GPIOA, BIN2_Pin, GPIO_PIN_RESET);
		}else{//go backward
		    HAL_GPIO_WritePin(GPIOA, BIN1_Pin, GPIO_PIN_RESET); // set direction of rotation for wheel B - reverse
			HAL_GPIO_WritePin(GPIOA, BIN2_Pin, GPIO_PIN_SET);
		}
	}else{//leftMotor
		if(error<0){//go forward
			HAL_GPIO_WritePin(GPIOA, AIN1_Pin, GPIO_PIN_SET); // set direction of rotation for wheel A - forward
			HAL_GPIO_WritePin(GPIOA, AIN2_Pin, GPIO_PIN_RESET);
		}else{//go backward
		    HAL_GPIO_WritePin(GPIOA, AIN1_Pin, GPIO_PIN_RESET); // set direction of rotation for wheel A - reverse
			HAL_GPIO_WritePin(GPIOA, AIN2_Pin, GPIO_PIN_SET);
		}
	}

	error = abs(error);
	if(error > 300){
		return 3000;
	}else if(error > 200){
		return 2000;
	}else if(error > 150){
		return 1600;
	}else if(error > 100){
		return 1400;
	}else if(error >10){
		return 1000;
	}else if(error >=2){
		times_acceptable++;
		return 600;
	}else{
		times_acceptable++;
		return 0;
	}
}

int finishCheck(){
	if (times_acceptable > 20){
		e_brake = 1;
		pwmVal_L = pwmVal_R = 0;
		leftTarget = leftEncoderVal;
		rightTarget = rightEncoderVal;
		times_acceptable = 0;
//	    osDelay(300);
		osDelay(50);
		return 0;
	}
	return 1;
}


void readByte(uint8_t addr, uint8_t* data){
	gyroBuffer[0] = addr;
	HAL_I2C_Master_Transmit(&hi2c1, ICMAddress<<1, gyroBuffer, 1, 10);
	HAL_I2C_Master_Receive(&hi2c1, ICMAddress<<1, data, 2, 20);
}

void readPID(){
	HAL_I2C_Mem_Read(&hi2c1,ICM20948__I2C_SLAVE_ADDRESS_1 << 1, ICM20948__USER_BANK_0__GYRO_ZOUT_H__REGISTER, I2C_MEMADD_SIZE_8BIT, readGyroZData, 2, 0xFFFF);
	gyroZ = readGyroZData[0] << 8 | readGyroZData[1];
}

void writeByte(uint8_t addr, uint8_t data){
	gyroBuffer[0] = addr;
	gyroBuffer[1] = data;
	HAL_I2C_Master_Transmit(&hi2c1, ICMAddress << 1, gyroBuffer, 2, 20);
}

void setDirection(int error, int right)
{
	if(right){//rightMotor
		if(error>0){//go forward
			HAL_GPIO_WritePin(GPIOA, BIN1_Pin, GPIO_PIN_SET); // set direction of rotation for wheel B- forward
			HAL_GPIO_WritePin(GPIOA, BIN2_Pin, GPIO_PIN_RESET);
		}else{//go backward
			HAL_GPIO_WritePin(GPIOA, BIN1_Pin, GPIO_PIN_RESET); // set direction of rotation for wheel B - reverse
			HAL_GPIO_WritePin(GPIOA, BIN2_Pin, GPIO_PIN_SET);
		}
	}else{//leftMotor
		if(error>0){//go forward
			HAL_GPIO_WritePin(GPIOA, AIN1_Pin, GPIO_PIN_SET); // set direction of rotation for wheel A - forward
			HAL_GPIO_WritePin(GPIOA, AIN2_Pin, GPIO_PIN_RESET);
		}else{//go backward
			HAL_GPIO_WritePin(GPIOA, AIN1_Pin, GPIO_PIN_RESET); // set direction of rotation for wheel A - reverse
			HAL_GPIO_WritePin(GPIOA, AIN2_Pin, GPIO_PIN_SET);
		}
	}
}

void StraightLineMove(int error1, int error2) {
	error1 = abs(error1);
	error2 = abs(error2);
	int error = 0;
	if(error1 <= error2)
		error = error1;
	else
		error = error2;
	if(error > 500){
		angleNow = 0; gyroZ = 0; // reset angle for PID
		PIDConfigReset(&pid_1);
		readPID(); // polling
		dir = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2) ? 1 : -1; // use only one of the wheel to determine car direction
//		angleNow += ((gyroZ >= -4 && gyroZ <= 11) ? 0 : gyroZ); // / GRYO_SENSITIVITY_SCALE_FACTOR_2000DPS * 0.01;
//		angleNow += ((gyroZ >= -50 && gyroZ <= 0) ? 0 : gyroZ);
		angleNow += gyroZ;

		PID_SPEED_1(&pid_1, angleNow, correction, dir);
//		pwmVal_L = newDutyL;
//		pwmVal_R = newDutyR;
	}
//	else if(error >=1000){
//		angleNow = 0; gyroZ = 0; // reset angle for PID
//		PIDConfigReset(&pid_2);
//		readPID(); // polling
//		dir = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2) ? 1 : -1; // use only one of the wheel to determine car direction
////		angleNow += ((gyroZ >= -4 && gyroZ <= 11) ? 0 : gyroZ); // / GRYO_SENSITIVITY_SCALE_FACTOR_2000DPS * 0.01;
//		angleNow += ((gyroZ >= -4 && gyroZ <= 8) ? 0 : gyroZ);
//
//		PID_SPEED_2(&pid_2, angleNow, correction, dir);
//		pwmVal_L = newDutyL;
//		pwmVal_R = newDutyR;
//	}
	else if(error>=50){
		angleNow = 0; gyroZ = 0; // reset angle for PID
		PIDConfigReset(&pid_3);
		readPID(); // polling
		dir = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2) ? 1 : -1; // use only one of the wheel to determine car direction
//		angleNow += ((gyroZ >= -4 && gyroZ <= 11) ? 0 : gyroZ); // / GRYO_SENSITIVITY_SCALE_FACTOR_2000DPS * 0.01;
//		angleNow += ((gyroZ >= -4 && gyroZ <= 8) ? 0 : gyroZ);
		angleNow += gyroZ;

		PID_SPEED_3(&pid_3, angleNow, correction, dir);
		pwmVal_L = newDutyL;
		pwmVal_R = newDutyR;
	}
	else if(error>=1){
		times_acceptable++;
		pwmVal_R = pwmVal_L = 0;
	}else{
		pwmVal_R = pwmVal_L = 0;
		times_acceptable++;
	}

//	angleNow = 0; gyroZ = 0; // reset angle for PID
//	PIDConfigReset(&pidC);
//	readPID(); // polling
//	dir = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2) ? 1 : -1; // use only one of the wheel to determine car direction
//	angleNow += ((gyroZ >= -4 && gyroZ <= 11) ? 0 : gyroZ); // / GRYO_SENSITIVITY_SCALE_FACTOR_2000DPS * 0.01;
//
//	PID_SPEED(&pidC, angleNow, correction, dir, newDutyL, newDutyR);
//	pwmVal_R = newDutyL;
//	pwmVal_R = newDutyR;
}

void gyroInit(){
	writeByte(0x06, 0x00);
	osDelay(10);
	writeByte(0x03, 0x80);
	osDelay(10);
	writeByte(0x07, 0x07);
	osDelay(10);
	writeByte(0x06, 0x01);
	osDelay(10);
	writeByte(0x7F, 0x20);
	osDelay(10);
	writeByte(0x01, 0x2F);
	osDelay(10);
	writeByte(0x0, 0x00);
	osDelay(10);
	writeByte(0x7F, 0x00);
	osDelay(10);
	writeByte(0x07, 0x00);
	osDelay(10);
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
	uint8_t msg [20];
	HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_1);  // HC-SR04 Sensor


  /* Infinite loop */
  uint8_t ch = 'A';
  for(;;)
  {
	    //HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1,0xFFFF);
//	    sprintf(msg, "%c    \0", ch);
//	    if (ch <'F'){
//	    	ch++;
//	    }else{
//	    	ch = 'A';
//	    }
	    osDelay(2000);

		HAL_GPIO_TogglePin(LED3_GPIO_Port,LED3_Pin);

  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartMotorTask */
/**
* @brief Function implementing the motorTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMotorTask */
void StartMotorTask(void *argument)
{
  /* USER CODE BEGIN StartMotorTask */
	pwmVal_R = 0;
	pwmVal_L = 0;
	int straightCorrection=0;
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
	htim1.Instance->CCR4 = 146; //Centre

	HAL_GPIO_WritePin(GPIOA, BIN1_Pin, GPIO_PIN_SET); // set direction of rotation for wheel B- forward
	HAL_GPIO_WritePin(GPIOA, BIN2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, AIN1_Pin, GPIO_PIN_SET); // set direction of rotation for wheel A - forward
	HAL_GPIO_WritePin(GPIOA, AIN2_Pin, GPIO_PIN_RESET);
//	osDelay(1000);
	osDelay(50);


  /* Infinite loop */
  for(;;)
  {
		htim1.Instance->CCR4 = pwmVal_servo;
		double error_angle = target_angle - total_angle;
//		if(aRxBuffer[0] = ' '){
//			pwmVal_L = pwmVal_R = 0;
//
//		}else
			if (pwmVal_servo < 127){ //106 //TURN LEFT
//			pwmVal_R = PID_Angle(error_angle, 1)*1.072;  //right is master
			pwmVal_R = PID_Angle(error_angle, 1)*1.5;
//			pwmVal_L = pwmVal_R*(0.59); //left is slave
			pwmVal_L = pwmVal_R*(0.3);

			if(error_angle>0){
				//go forward
				HAL_GPIO_WritePin(GPIOA, AIN1_Pin, GPIO_PIN_SET); // set direction of rotation for wheel A- forward
				HAL_GPIO_WritePin(GPIOA, AIN2_Pin, GPIO_PIN_RESET);
			}
			else{
				//go backward
			    HAL_GPIO_WritePin(GPIOA, AIN1_Pin, GPIO_PIN_RESET); // set direction of rotation for wheel A - reverse
				HAL_GPIO_WritePin(GPIOA, AIN2_Pin, GPIO_PIN_SET);
			}
		}

		else if (pwmVal_servo > 189){ //230 //TURN RIGHT
//			pwmVal_L = PID_Angle(error_angle, 0);
			pwmVal_L = PID_Angle(error_angle, 0)*1.5;
//			pwmVal_R = pwmVal_L*(0.59); //right is slave
			pwmVal_R = pwmVal_L*(0.3); //right is slave

			if(error_angle<0){
				//go forward
				HAL_GPIO_WritePin(GPIOA, BIN1_Pin, GPIO_PIN_SET); // set direction of rotation for wheel B- forward
				HAL_GPIO_WritePin(GPIOA, BIN2_Pin, GPIO_PIN_RESET);
			}
			else{
				//go backward
			    HAL_GPIO_WritePin(GPIOA, BIN1_Pin, GPIO_PIN_RESET); // set direction of rotation for wheel B - reverse
				HAL_GPIO_WritePin(GPIOA, BIN2_Pin, GPIO_PIN_SET);
			}
		}

		else {
//			//outdoor
//			pwmVal_R = PID_Control(leftTarget - leftEncoderVal, 0)*1.0;
			//indoor
//			pwmVal_R = PID_Control(rightTarget - rightEncoderVal, 1)*0.988;

//			if (abs(leftTarget - leftEncoderVal)>abs(rightTarget - rightEncoderVal)){
//				straightCorrection++;
//			}else{ straightCorrection--;}
//			if (abs(leftTarget - leftEncoderVal)<100){
//				straightCorrection=0;
//			}
//			//original
//			pwmVal_L = (PID_Control(rightTarget - rightEncoderVal, 1))*1.1+straightCorrection;
			//real
//			pwmVal_L = PID_Control(leftTarget - leftEncoderVal, 0);

//			if ((leftTarget - leftEncoderVal)<0){
//				if (error_angle>5){ // if turn left, 106. right 230. left +. right -.
//							pwmVal_servo=((19*5)/5 + 146);
//						}
//						else if(error_angle<-5){
//							pwmVal_servo=((-19*5)/5 + 146);
//						}else{
//							pwmVal_servo=((19*error_angle)/5 + 146);
//						}
//			}else{
//				if (error_angle>5){ // if turn left, 106. right 230. left +. right -.
//							pwmVal_servo=((-19*5)/5 + 146);
//						}
//						else if(error_angle<-5){
//							pwmVal_servo=((19*5)/5 + 146);
//						}else{
//							pwmVal_servo=((-19*error_angle)/5 + 146);
//						}
//			}
//			//line correction equation is pwmVal = (19*error)/5 + 146

			// PID motor test
			setDirection(leftTarget - leftEncoderVal, 1);
			setDirection(rightTarget - rightEncoderVal, 0);
			StraightLineMove(leftTarget - leftEncoderVal, rightTarget - rightEncoderVal);
		}

		if(e_brake){
			pwmVal_L = pwmVal_R = 0;
			leftTarget = leftEncoderVal;
			rightTarget = rightEncoderVal;
		}

		__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,pwmVal_L);
		__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_2,pwmVal_R);
		osDelay(1);

		if (times_acceptable>1000){
			times_acceptable=1001;
		}

  }
  /* USER CODE END StartMotorTask */
}

/* USER CODE BEGIN Header_StartOledTask */
/**
* @brief Function implementing the oledTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartOledTask */
void StartOledTask(void *argument)
{
  /* USER CODE BEGIN StartOledTask */
	uint8_t hello [20] = "Hello World!\0";
	uint8_t clear[20] = {0};
	uint8_t lefty[20] = {0};
	uint8_t righty[20] = {0};
	uint8_t motorD[20] = {0};
	uint8_t check[20] = {0};
	uint8_t debugMsg[20] = "hello\0";

  /* Infinite loop */
  for(;;)
  {


	//sprintf(clear, "L:%d | R:%d     ", (int)(leftTarget - leftEncoderVal), (int)(rightTarget-rightEncoderVal));
	sprintf(hello, "gyro: %d\0", total_angle);

	OLED_ShowString(10, 10, hello);


	//sprintf(righty,"Gyro: %d \0", (int)total_angle);
//	int decimals = abs((int)((total_angle-(int)(total_angle))*1000));
//	sprintf(righty,"Gyro: %d.%d \0", (int)total_angle, decimals);
//	OLED_ShowString(0, 20, righty);
//ti'm
//	//sprintf(lefty, "US: %d\0", (int)uDistance);
//
//	sprintf(lefty, "S: %c %c %c%c%c \0", old_Buff1[0], old_Buff1[1], old_Buff1[2], old_Buff1[3], old_Buff1[4]);
//
//	OLED_ShowString(0, 30, lefty);
//
//
//
//
//	sprintf(motorD, "O: %c %c %c%c%c \0", old_Buff[0], old_Buff[1], old_Buff[2], old_Buff[3], old_Buff[4]);
//	OLED_ShowString(0, 40, motorD);
//
//	sprintf(check, "K: %c %c %c%c%c \0", aRxBuffer[0], aRxBuffer[1], aRxBuffer[2], aRxBuffer[3], aRxBuffer[4]);
//	OLED_ShowString(0, 50, check);

	//memset(clear, 0, 20*sizeof(uint8_t));

	OLED_Refresh_Gram();
	osDelay(10);
  }
  /* USER CODE END StartOledTask */
}

/* USER CODE BEGIN Header_StartRpiTask */
/**
* @brief Function implementing the rpiTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartRpiTask */
void StartRpiTask(void *argument)
{
  /* USER CODE BEGIN StartRpiTask */
	// A5
//	resp = scanAndReceive();
////	resp = 'B';
//	uint8_t hello [20] = "Hello World!\0";
//	for(;;)
//	{
//		if (resp != ' ')
//		{
//			sprintf(hello, "test: %c\0", resp);
//			OLED_ShowString(10, 40, hello);
//			moveLeftSide();
//			resp = scanAndReceive();
////			resp = 'B';
//			osDelay(10);
//		}
//	}

	char ch = 'A';
	char old = ')';
	int Ryan_sign_fuckmylife = 1;
  /* Infinite loop */
	  aRxBuffer[0] = '-';
	  aRxBuffer[1] = 'W';
	  aRxBuffer[2] = 'A';
	  aRxBuffer[3] = 'I';
	  aRxBuffer[4] = 'T';

	  const char *strings[] = {"SF000", "LF090", "LF090", "SF050" };
	  size_t numStrings = sizeof(strings) / sizeof(strings[0]);
//	  for(;;)
	  for (size_t i = 0; i < numStrings; i++)
	{
	  	aRxBuffer[0] = strings[i][0];
	  aRxBuffer[1] = strings[i][1];
	  aRxBuffer[2] = strings[i][2];
	  aRxBuffer[3] = strings[i][3];
	  aRxBuffer[4] = strings[i][4];

	  magnitude = 0;
	  key = aRxBuffer[0];
	  direction = aRxBuffer[1];
	  magnitude = ((int)(aRxBuffer[2])-48)*100 + ((int)(aRxBuffer[3])-48)*10 + ((int)(aRxBuffer[4])-48);
	  Ryan_sign_fuckmylife = 1;

	  if(direction == 'B' || direction == 'b'){
		  magnitude *= -1;
		  Ryan_sign_fuckmylife = -1;
	  }

//	  if(aRxBuffer[0] != old){
//
//		if (aRxBuffer[0]!='D'){
//			old_Buff1[0] = old_Buff[0];
//			old_Buff1[1] = old_Buff[1];
//			old_Buff1[2] = old_Buff[2];
//			old_Buff1[3] = old_Buff[3];
//			old_Buff1[4] = old_Buff[4];
//		old_Buff[0] = aRxBuffer[0];
//		old_Buff[1] = aRxBuffer[1];
//		old_Buff[2] = aRxBuffer[2];
//		old_Buff[3] = aRxBuffer[3];
//		old_Buff[4] = aRxBuffer[4];
//
//		}

		 osDelay(2000);
//	  	  osDelay(2000);
		  switch (key){
			  case 'D':
				  break;
			  case 'S':
				  times_acceptable=0;
//				  moveCarStraightOverall(magnitude);
				  moveCarStraight(magnitude);
				  while(finishCheck());
				  flagDone=1;
				  aRxBuffer[0] = 'D';
				  aRxBuffer[1] = 'O';
				  aRxBuffer[2] = 'N';
				  aRxBuffer[3] = 'E';
				  aRxBuffer[4] = '!';
//				  cntS++;

//				  osDelay(100);
				  osDelay(50);

				  break;
			  case 'R':
				  times_acceptable=0;
				  moveCarRight90(89*Ryan_sign_fuckmylife, Ryan_sign_fuckmylife);
				  while(finishCheck());
				  flagDone=1;
				  aRxBuffer[0] = 'D';
				  aRxBuffer[1] = 'O';
				  aRxBuffer[2] = 'N';
				  aRxBuffer[3] = 'E';
				  aRxBuffer[4] = '!';
//				  osDelay(100);
				  osDelay(50);

				  break;
			  case 'L':
				  times_acceptable=0;
				  moveCarLeft90(89*Ryan_sign_fuckmylife, Ryan_sign_fuckmylife);
				  while(finishCheck());
				  flagDone=1;
				  aRxBuffer[0] = 'D';
				  aRxBuffer[1] = 'O';
				  aRxBuffer[2] = 'N';
				  aRxBuffer[3] = 'E';
				  aRxBuffer[4] = '!';
//				  osDelay(100);
				  osDelay(50);

				  break;
			  case 'J':
				  times_acceptable=0;
				  moveCarSlideRight(Ryan_sign_fuckmylife);
				  while(finishCheck());
				  flagDone=1;
				  aRxBuffer[0] = 'D';
				  aRxBuffer[1] = 'O';
				  aRxBuffer[2] = 'N';
				  aRxBuffer[3] = 'E';
				  aRxBuffer[4] = '!';
//				  osDelay(100);
				  osDelay(50);

				  break;
			  case 'K':
				  times_acceptable=0;
				  moveCarSlideLeft(Ryan_sign_fuckmylife);
				  while(finishCheck());
				  flagDone=1;
				  aRxBuffer[0] = 'D';
				  aRxBuffer[1] = 'O';
				  aRxBuffer[2] = 'N';
				  aRxBuffer[3] = 'E';
				  aRxBuffer[4] = '!';
//				  osDelay(100);
				  osDelay(50);

				  break;
		  	  case 'P':
//		  		  buzzerBeep();
		  		  flagDone=1;
		  		  times_acceptable=0;
				  aRxBuffer[0] = 'D';
				  aRxBuffer[1] = 'O';
				  aRxBuffer[2] = 'N';
				  aRxBuffer[3] = 'E';
				  aRxBuffer[4] = '!';
//				  osDelay(100);
				  osDelay(50);
				  break;
			  default:
				  break;
		  }
		  old = aRxBuffer[0];

//
//
//	  // send ack back to rpi and ready for next instruction
		if(flagDone==1){
			osDelay(100);
			HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1,0xFFFF);
			flagDone = 0;
		}
//		osDelay(1);
//
  }
}
  /* USER CODE END StartRpiTask */


/* USER CODE BEGIN Header_StartGyroTask */
/**
* @brief Function implementing the gyroTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartGyroTask */
void StartGyroTask(void *argument)
{
  /* USER CODE BEGIN StartGyroTask */
	gyroInit();
	uint8_t val[2] = {0,0};

//	int16_t angular_speed = 0;

	uint32_t tick = 0;
	double offset = 0;
	double trash= 0;
	int i=0;
	while(i<100){
		osDelay(50);
		readByte(0x37, val);

		//test read GyroZ
//		readPID();

		angular_speed = ((int16_t)((int8_t)val[0]))*256 +  (uint16_t)val[1];
		angular_speed = (val[0] << 8) | val[1];
		trash +=(double)((double)angular_speed)*((HAL_GetTick() - tick)/16400.0);
		offset += angular_speed;
		tick = HAL_GetTick();
		i++;
	}
	offset = offset/i;


//	uint8_t hello [20] = "Hello World!\0";
//	sprintf(hello, "offset: %d\0", (int)offset);
//	OLED_ShowString(10, 30, hello);
//	buzzerBeep();
//	LEDShow();

  /* Infinite loop */
  for(;;)
  {
		osDelay(50);
		readByte(0x37, val);

		// test read GyroZ
		readPID();

		angular_speed = (val[0] << 8) | val[1];
		angular_speed = ((int16_t)((int8_t)val[0]))*256 +  (uint16_t)val[1];
		total_angle +=(double)((double)angular_speed - offset)*((HAL_GetTick() - tick)/16400.0);
//		total_angle +=(double)((double)angular_speed + 5)*((HAL_GetTick() - tick)/16400.0);
		i -= angular_speed;
		tick = HAL_GetTick();
		i++;

//		char hello[50] = {0};
//		double diff = total_angle - old;
//		int decimals = abs((int)((diff-(int)(diff))*10000));
//		int offdeci = abs((int)((offset-(int)(offset))*10000));
//		sprintf(hello,"G%d.%d: %d.%d \0", (int)offset,offdeci,(int)diff, decimals);
//		old = total_angle;
//		HAL_UART_Transmit(&huart3, hello, 20,0xFFFF);
  }
  /* USER CODE END StartGyroTask */
}

/* USER CODE BEGIN Header_StartBulleyesTask */
/**
* @brief Function implementing the bulleyesTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartBulleyesTask */
void StartBulleyesTask(void *argument)
{
  /* USER CODE BEGIN StartBulleyesTask */
	int i=0;

//	strncpy(instructBuffer[0],"SF080"  , 5);
//	strncpy(instructBuffer[1],"JF000"  , 5);
//	strncpy(instructBuffer[2],"P___2"  , 5);
//	strncpy(instructBuffer[3],"SB040"  , 5);
//	strncpy(instructBuffer[4],"RF090"  , 5);
//	strncpy(instructBuffer[5],"SF010"  , 5);
//	strncpy(instructBuffer[6],"P___6"  , 5);
//	strncpy(instructBuffer[7],"KB000"  , 5);
//	strncpy(instructBuffer[8],"SF020"  , 5);
//	strncpy(instructBuffer[9],"RF090"  , 5);
//	strncpy(instructBuffer[10],"P___5"  , 5);
//	strncpy(instructBuffer[11],"SB020"  , 5);
//	strncpy(instructBuffer[12],"LB090"  , 5);
//	strncpy(instructBuffer[13],"SB030"  , 5);
//	strncpy(instructBuffer[14],"LB090"  , 5);
//	strncpy(instructBuffer[15],"SB050"  , 5);
//	strncpy(instructBuffer[16],"LB090"  , 5);
//	strncpy(instructBuffer[17],"SF010"  , 5);
//	strncpy(instructBuffer[18],"P___3"  , 5);
//	strncpy(instructBuffer[19],"SB020"  , 5);
//	strncpy(instructBuffer[20],"LF090"  , 5);
//	strncpy(instructBuffer[21],"SF020"  , 5);
//	strncpy(instructBuffer[22],"RF090"  , 5);
//	strncpy(instructBuffer[23],"SB010"  , 5);
//	strncpy(instructBuffer[24],"LF090"  , 5);
//	strncpy(instructBuffer[25],"P___1"  , 5);
//	strncpy(instructBuffer[26],"LB090"  , 5);
//	strncpy(instructBuffer[27],"SB030"  , 5);
//	strncpy(instructBuffer[28],"LF090"  , 5);
//	strncpy(instructBuffer[29],"SF010"  , 5);
//	strncpy(instructBuffer[30],"LF090"  , 5);
//	strncpy(instructBuffer[31],"P___0"  , 5);
//
//	strncpy(instructBuffer[0],"RF090"  , 5);
//	strncpy(instructBuffer[1],"SF050"  , 5);
//	strncpy(instructBuffer[2],"LF090"  , 5);
//	strncpy(instructBuffer[3],"SB020"  , 5);
//	strncpy(instructBuffer[4],"LF090"  , 5);
//	strncpy(instructBuffer[5],"P___5"  , 5);
//	strncpy(instructBuffer[6],"SB050"  , 5);
//	strncpy(instructBuffer[7],"RB090"  , 5);
//	strncpy(instructBuffer[8],"SF020"  , 5);
//	strncpy(instructBuffer[9],"P___3"  , 5);
//	strncpy(instructBuffer[10],"SB040"  , 5);
//	strncpy(instructBuffer[11],"JB000"  , 5);
//	strncpy(instructBuffer[12],"RB090"  , 5);
//	strncpy(instructBuffer[13],"SF020"  , 5);
//	strncpy(instructBuffer[14],"P___1"  , 5);
//	strncpy(instructBuffer[15],"RB090"  , 5);
//	strncpy(instructBuffer[16],"KB000"  , 5);
//	strncpy(instructBuffer[17],"KF000"  , 5);
//	strncpy(instructBuffer[18],"P___0"  , 5);
//	strncpy(instructBuffer[19],"SB020"  , 5);
//	strncpy(instructBuffer[20],"LF090"  , 5);
//	strncpy(instructBuffer[21],"SF010"  , 5);
//	strncpy(instructBuffer[22],"LB000"  , 5);
//	strncpy(instructBuffer[23],"SF010"  , 5);
//	strncpy(instructBuffer[24],"LB000"  , 5);
//	strncpy(instructBuffer[25],"P___6"  , 5);
//	strncpy(instructBuffer[26],"SB020"  , 5);
//	strncpy(instructBuffer[27],"RB090"  , 5);
//	strncpy(instructBuffer[28],"SF050"  , 5);
//	strncpy(instructBuffer[29],"P___2"  , 5);



  /* Infinite loop */
  for(;;)
  { osDelay(1000);
  HCSR04_Read();

  	 // if(i<2){
//

  //	  }


//  	  while (i<50){
//		  aRxBuffer[0] = instructBuffer[i][0];
//		  aRxBuffer[1] = instructBuffer[i][1];
//		  aRxBuffer[2] = instructBuffer[i][2];
//		  aRxBuffer[3] = instructBuffer[i][3];
//		  aRxBuffer[4] = instructBuffer[i][4];
//		  while(aRxBuffer[0] != ' ' || aRxBuffer[0] != 'P');
//
//  		  i++;
//  	  }
  }
  /* USER CODE END StartBulleyesTask */
}

/* USER CODE BEGIN Header_StartEncoderRightTask */
/**
* @brief Function implementing the encoderRightTas thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartEncoderRightTask */
void StartEncoderRightTask(void *argument)
{
  /* USER CODE BEGIN StartEncoderRightTask */
	HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_ALL);
	int cnt1;
	int dirR = 1;
	int diff;
	uint32_t tick = HAL_GetTick();
  /* Infinite loop */
  for(;;)
  {
		if(HAL_GetTick()-tick > 10L)
		{
			cnt1 = __HAL_TIM_GET_COUNTER(&htim3);
			if(cnt1 > 32000){
				dirR = 1;
				diff = (65536 - cnt1);
			} else {
				dirR = -1;
				diff = cnt1;

			}

			if(dirR == 1){
				rightEncoderVal -= diff;
			} else {
				rightEncoderVal += diff;
			}

			__HAL_TIM_SET_COUNTER(&htim3, 0);

			tick = HAL_GetTick();
		}
  }
  /* USER CODE END StartEncoderRightTask */
}

/* USER CODE BEGIN Header_StartEncoderLeftTask */
/**
* @brief Function implementing the encoderLeftTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartEncoderLeftTask */
void StartEncoderLeftTask(void *argument)
{
  /* USER CODE BEGIN StartEncoderLeftTask */
	HAL_TIM_Encoder_Start(&htim2,TIM_CHANNEL_ALL);
	int cnt2;
	int dirL = 1;
	int diff;
	uint32_t tick = HAL_GetTick();
  /* Infinite loop */
  for(;;)
  {
		if(HAL_GetTick()-tick > 10L)
		{
			cnt2 = __HAL_TIM_GET_COUNTER(&htim2);

			if(cnt2 > 32000){
				dirL = 1;
				diff = (65536 - cnt2);
			} else {
				dirL = -1;
				diff = cnt2;
			}
			if(dirL == 1){
				leftEncoderVal += diff;
			} else {
				leftEncoderVal -= diff;
			}

			__HAL_TIM_SET_COUNTER(&htim2, 0);

			tick = HAL_GetTick();
		}
  }





  /* USER CODE END StartEncoderLeftTask */
}

/* USER CODE BEGIN Header_show */
/**
* @brief Function implementing the showTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_show */
void show(void *argument)
{
  /* USER CODE BEGIN show */
	uint8_t hello [20] = "Hello World!\0";
  /* Infinite loop */
  for(;;)
  {
	sprintf(hello, "buffer: %s\0", aRxBuffer);
	OLED_ShowString(10, 10, hello);
	sprintf(hello, "timeA: %d\0", times_acceptable);
	OLED_ShowString(10, 20, hello);
//	sprintf(hello, "corr: %d\0", correction);
//	OLED_ShowString(10, 30, hello);
	sprintf(hello, "gyroZ: %d\0", (int)gyroZ);
	OLED_ShowString(10, 40, hello);
	sprintf(hello, "a2: %d\0", (int)target_angle);
	OLED_ShowString(10, 50, hello);
	OLED_Refresh_Gram();
    osDelay(1);
  }
  /* USER CODE END show */
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

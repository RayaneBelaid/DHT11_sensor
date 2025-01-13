/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DHT11_GPIO_PORT GPIOA
#define DHT11_GPIO_PIN GPIO_PIN_1
#define PRINTF2UART2 int __io_putchar(int ch)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart2;

uint8_t data[5];

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */
void DHT11_ReadData(float *temperature, float *humidity);
uint8_t check_response(void);
void start_signal (void);
void delay_us ( uint16_t us);
uint8_t read_byte(void);
void process_sensor_data(void);

char msg[50] ;
char message1[16];
char message1[16];
uint8_t TOUT =0 , CheckSum , i;
uint8_t T_Byte1 , T_Byte2 , RH_Byte1 , RH_Byte2 ;

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
  float temperature, humidity;
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
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start(&htim1);
  printf("Initilization complet \n\r");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    //DHT11_ReadData(&temperature, &humidity);
    // print temperature and humidity values
    //printf("START ======>\n ");
    //printf("Temperature: %.1fC\r\n", temperature);
    //printf("Humidity: %.1f%%\r\n", humidity);


    // wait for 1 second
    HAL_Delay(1000);
    start_signal();
    uint8_t check = check_response();
	  if (!check){
		  printf("No responce from the sensor \r\n");
	  } else {
		  printf("process sensor data \r\n");
		  process_sensor_data();
	  }

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/* USER CODE BEGIN 4 */

PRINTF2UART2
{
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}


/* USER CODE END 4 */


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_TIM1;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
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

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 71;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PA1 LD2_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_1|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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


// function to read data from DHT11
void DHT11_ReadData1(float *temperature, float *humidity)
{
    uint8_t i;
    printf("Start reading data: \n");
    // send start signal
    HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_RESET);
    HAL_Delay(18);
    HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_SET);

    // wait for response
    HAL_Delay(40);

    // initialize data array
    memset(data, 0, sizeof(data));

    // read 40 bits of data
    for(i=0; i<40; i++)
    {
        // wait for low pulse
        while(!HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN));

        // wait for high pulse
        uint32_t t = 0;
        while(HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN))
        {
        	printf("HAL_GPIO_ReadPin is HIGH == \n", HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN));
            t++;
            HAL_Delay(1);
        }

        // store bit value in data array
        if(t > 30)
            data[i/8] |= (1 << (7 - (i % 8)));
    }

    // verify checksum
    if(data[4] == (data[0] + data[1] + data[2] + data[3]))
    {
        // convert temperature and humidity values
        *humidity = (data[0] << 8 | data[1]) / 10.0;
        *temperature = ((data[2] & 0x7F) << 8 | data[3]) / 10.0;
        if (data[2] & 0x80) *temperature *= -1;
    }
}

void DHT11_ReadData2(float *temperature, float *humidity)
{
    uint8_t i; // Loop index for reading 40 bits
    printf("Starting DHT11 data read...\n");

    // Step 1: Send start signal
    printf("Sending start signal to DHT11...\n");
    HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_RESET); // Set pin LOW
    HAL_Delay(18); // Hold LOW for at least 18ms
    HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_SET);   // Set pin HIGH
    printf("Start signal sent. Waiting for DHT11 response...\n");

    // Step 2: Wait for the sensor to respond
    HAL_Delay(40); // Sensor typically needs time for response

    // Step 3: Clear data buffer before reading
    printf("Clearing the data buffer...\n");
    memset(data, 0, sizeof(data));

    printf("Reading 40 bits from the DHT11 sensor...\n");
    // Step 4: Read 40 bits from the sensor
    for(i = 0; i < 40; i++)
    {
        // Wait for a low pulse indicating the start of a bit
        printf("Waiting for low pulse for bit %d...\n", i + 1);
        while(!HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN)); // Wait for pin to go HIGH

        // Measure duration of the high pulse
        uint32_t t = 0; // Pulse duration counter
        printf("Measuring high pulse duration for bit %d...\n", i + 1);

        while(HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN)) // Stay in loop while pin is HIGH
        {
        	printf("HAL_GPIO_ReadPin == \n", HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN));
            t++;
            HAL_Delay(1); // Small delay to match timing (adjustable for microcontroller clock)
        }

        // Determine if the bit is 0 or 1 based on pulse duration
        if(t > 30) // Adjust threshold if necessary
        {
            data[i / 8] |= (1 << (7 - (i % 8))); // Set the corresponding bit in the byte
            printf("Bit %d: 1 (t = %lu)\n", i + 1, t);
        }
        else
        {
            printf("Bit %d: 0 (t = %lu)\n", i + 1, t);
        }
    }

    // Step 5: Verify the checksum to ensure data integrity
    printf("Verifying checksum...\n");
    uint8_t checksum = data[0] + data[1] + data[2] + data[3];
    if(data[4] == checksum)
    {
        printf("Checksum valid: 0x%X (received) == 0x%X (calculated)\n", data[4], checksum);

        // Convert humidity and temperature data
        *humidity = (data[0] << 8 | data[1]) / 10.0; // Combine two bytes for humidity
        *temperature = ((data[2] & 0x7F) << 8 | data[3]) / 10.0; // Combine two bytes for temperature

        // Handle negative temperatures
        if(data[2] & 0x80) *temperature *= -1;

        printf("Data read successfully:\n");
        printf("Humidity: %.1f%%\n", *humidity);
        printf("Temperature: %.1f°C\n", *temperature);
    }
    else
    {
        // If checksum fails, print error and reset output values
        printf("Checksum failed! 0x%X (received) != 0x%X (calculated)\n", data[4], checksum);
        *humidity = 0;
        *temperature = 0;
    }

    printf("DHT11 data read complete.\n");
}
void DHT11_ReadData(float *temperature, float *humidity)
{
    uint8_t i; // Loop index for reading 40 bits
    printf("Starting DHT11 data read...\r\n");

    // Step 1: Send start signal
    printf("Sending start signal to DHT11...\r\n");
    HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_RESET); // Set pin LOW
    HAL_Delay(18); // Hold LOW for at least 18ms
    HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_SET);   // Set pin HIGH
    printf("Start signal sent. Waiting for DHT11 response...\r\n");

    // Step 2: Wait for the sensor to respond
    HAL_Delay(40); // Sensor typically needs time for response

    // Step 3: Clear data buffer before reading
    printf("Clearing the data buffer...\r\n");
    memset(data, 0, sizeof(data));

    printf("Reading 40 bits from the DHT11 sensor...\r\n");
    // Step 4: Read 40 bits from the sensor
    for(i = 0; i < 40; i++)
    {
        // Wait for a low pulse indicating the start of a bit
        printf("Waiting for low pulse for bit %d...\r\n", i + 1);
        while(!HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN)); // Wait for pin to go HIGH

        // Measure duration of the high pulse
        uint32_t t = 0; // Pulse duration counter
        printf("Measuring high pulse duration for bit %d...\r\n", i + 1);
        while(HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN)) // Stay in loop while pin is HIGH
        {
        	printf("t =  %d...\r\n", t);
            t++;
            HAL_Delay(1); // Small delay to match timing (adjustable for microcontroller clock)
        }

        // Determine if the bit is 0 or 1 based on pulse duration
        if(t > 30) // Adjust threshold if necessary
        {
            data[i / 8] |= (1 << (7 - (i % 8))); // Set the corresponding bit in the byte
            printf("Bit %d: 1 (t = %lu)\r\n", i + 1, t);
        }
        else
        {
            printf("Bit %d: 0 (t = %lu)\r\n", i + 1, t);
        }
    }

    // Step 5: Verify the checksum to ensure data integrity
    printf("Verifying checksum...\r\n");
    uint8_t checksum = data[0] + data[1] + data[2] + data[3];
    if(data[4] == checksum)
    {
        printf("Checksum valid: 0x%X (received) == 0x%X (calculated)\r\n", data[4], checksum);

        // Convert humidity and temperature data
        *humidity = (data[0] << 8 | data[1]) / 10.0; // Combine two bytes for humidity
        *temperature = ((data[2] & 0x7F) << 8 | data[3]) / 10.0; // Combine two bytes for temperature

        // Handle negative temperatures
        if(data[2] & 0x80) *temperature *= -1;

        printf("Data read successfully:\r\n");
        printf("Humidity: %.1f%%\r\n", *humidity);
        printf("Temperature: %.1f°C\r\n", *temperature);
    }
    else
    {
        // If checksum fails, print error and reset output values
        printf("Checksum failed! 0x%X (received) != 0x%X (calculated)\r\n", data[4], checksum);
        *humidity = 0;
        *temperature = 0;
    }

    printf("DHT11 data read complete.\r\n");
}

void start_signal (void){
     GPIO_InitTypeDef GPIO_InitStruct = {0};


      GPIO_InitStruct.Pin = DHT11_GPIO_PIN;
      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      HAL_GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStruct);

      HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_RESET);
      HAL_Delay(18);
      HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_SET);
      delay_us(30);

      GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
      HAL_GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStruct);

}
void delay_us ( uint16_t us)
{
    __HAL_TIM_SET_COUNTER(&htim1,0);
    while(__HAL_TIM_GET_COUNTER(&htim1) < us);
}


uint8_t check_response(void){
    TOUT=0;
    __HAL_TIM_SET_COUNTER(&htim1,0);
    while(!HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN) && (__HAL_TIM_GET_COUNTER(&htim1) < 100)) {};
    if(__HAL_TIM_GET_COUNTER(&htim1)>= 100){
        return 0; //timeout
    }
    while(HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN) && (__HAL_TIM_GET_COUNTER(&htim1) < 100)) {};
        if(__HAL_TIM_GET_COUNTER(&htim1)>= 100){
            return 0; //timeout
        }
    return 1;
}

uint8_t read_byte(void){
    uint8_t num =0 ;
    for(i=0 ;i<8; i++){
        while(!HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN))  {};
        __HAL_TIM_SET_COUNTER(&htim1,0);
        while(HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN))  {};
        if(__HAL_TIM_GET_COUNTER(&htim1) > 40)
        {
            num |= (1 << (7 - i));
        }

    }
    return num;
}

void process_sensor_data(void){
    RH_Byte1 = read_byte();
    RH_Byte2 = read_byte();
    T_Byte1 = read_byte();
    T_Byte2 = read_byte();
    CheckSum = read_byte();

    uint8_t humidity_integer =  RH_Byte1 ;
    uint8_t humidity_decimal =  RH_Byte2 / 10 ;

    uint8_t temperature_integer =  T_Byte1 ;
    uint8_t temperature_decimal =  T_Byte2 / 10 ;

    if(CheckSum ==((RH_Byte1+RH_Byte2 +T_Byte1+T_Byte2)& 0xff)){
        snprintf(msg,sizeof(msg),"RH = %d.%d %%\r\n",humidity_integer,humidity_decimal);
        printf(msg);
        snprintf(msg,sizeof(msg),"temp = %d.%d C\r\n",temperature_integer,temperature_decimal);
        printf(msg);

    }else
    {
        printf("Checksum Errors ! Trying Again ...\r\n");
    }
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

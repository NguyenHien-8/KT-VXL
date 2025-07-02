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
#include "lcd_16x2.h"
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

/* USER CODE BEGIN PV */
uint16_t ADC_LM35 = 0;
uint8_t TemperatureC = 0;
// Khai bao bien setup thoi gian gan nhatb
uint32_t start_time_tang = 0;
uint32_t start_time_giam = 0;
uint32_t start_time_ok = 0;
uint32_t warning_start_time = 0;

uint8_t display = 0;
// Khai bao bien canh bao
uint8_t warning_lm35 = 0;
// Khai bao bien cong tru menu
uint8_t Menu_tong = 0;
uint8_t Menu_value = 0;
uint8_t Setup_LM35 = 0;
uint8_t Mode_LM35 = 0;
uint8_t Mode_Warning = 0;

// Khai bao bien dem button
uint8_t demtong = 0;
uint8_t count_lm35 = 0;
// Khai bao bien trang thai nut nhan
volatile uint8_t nut_tang = 0;
volatile uint8_t nut_giam = 0;
volatile uint8_t nut_ok = 0;
// Khai bao bien luu trang thai
uint8_t save_value_lm35 = 34;
uint8_t save_mode_lm35 = 0;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    uint32_t now = HAL_GetTick(); // Lay thoi diem hien tai (ms)

    if (GPIO_Pin == GPIO_PIN_15) // NUT LEN
    {
        if (now - start_time_tang > 200)
        {
            nut_tang = 1;
            start_time_tang = now;
        }
    }
    else if (GPIO_Pin == GPIO_PIN_13) // NUT XUONG
    {
        if (now - start_time_giam > 200)
        {
            nut_giam = 1;
            start_time_giam = now;
        }
    }
    else if (GPIO_Pin == GPIO_PIN_14) // NUT OK
    {
        if (now - start_time_ok > 200)
        {
            nut_ok = 1;
            start_time_ok = now;
        }
    }
}
void Display_Warning()
{
    switch (Mode_Warning)
    {
    case 1:
        if (!display)
        {
            Lcd_clear_display();
            HAL_Delay(20);
        }
        Lcd_gotoxy(0, 0);
        Lcd_write_string("====================");
        Lcd_gotoxy(6, 1);
        Lcd_write_string("WARNING!");
        Lcd_gotoxy(3, 2);
        Lcd_write_string("QUA NHIET CHIP");
        Lcd_gotoxy(0, 3);
        Lcd_write_string("====================");
        display = 1;
        break;
    case 2:
        if (!display)
        {
            Lcd_clear_display();
        }
        Lcd_gotoxy(0, 0);
        Lcd_write_string("====================");
        Lcd_gotoxy(5, 1);
        Lcd_write_string("WAR");
        Lcd_gotoxy(0, 2);
        Lcd_write_string("WARNING QUA ");
        Lcd_gotoxy(0, 3);
        Lcd_write_string("====================");
        display = 1;
        break;
    }
}
void Display_All_Sensor_Values()
{
    switch (Menu_value)
    {
    case 0:
        if (!display)
        {
            Lcd_clear_display();
        }
        Lcd_gotoxy(0, 0);
        Lcd_write_string("TEMP(LM35):");
        Lcd_gotoxy(12, 0);
        Lcd_write_int(TemperatureC);
        Lcd_gotoxy(0, 1);
        Lcd_write_string("TEMP(DHT11):");
        Lcd_gotoxy(0, 2);
        Lcd_write_string("HUM(DHT11):");
        Lcd_gotoxy(0, 3);
        Lcd_write_string("LIGHT(LDR):");
        display = 1;
        break;
    case 1:
        if (!display)
        {
            Lcd_clear_display();
        }
        Lcd_gotoxy(0, 0);
        Lcd_write_string("VIBRATION:");
        Lcd_gotoxy(0, 1);
        Lcd_write_string("AIR QUALITY:");
        Lcd_gotoxy(0, 2);
        Lcd_write_string("WATER FLOW:");
        display = 1;
        break;
    }
}
void Manhinhchinh()
{
    if (!display)
    {
        Lcd_clear_display();
    }
    Lcd_gotoxy(0, 0);
    Lcd_write_string("**TRUONG BACH KHOA**");
    Lcd_gotoxy(0, 2);
    Lcd_write_string("GVHD: TRAN HUU DANH");
    Lcd_gotoxy(0, 3);
    Lcd_write_string("KHOA: DTVT");
    display = 1;
}
void Menu_Tong()
{
    switch (Menu_tong)
    {
    case 0:
        if (!display)
        {
            Lcd_clear_display();
        }
        Lcd_gotoxy(0, 0);
        Lcd_write_string(">CAI DAT CB LM35");
        Lcd_gotoxy(0, 1);
        Lcd_write_string(" CAI DAT CB DHT11");
        Lcd_gotoxy(0, 2);
        Lcd_write_string(" CAI DAT CB LDR");
        Lcd_gotoxy(0, 3);
        Lcd_write_string(" CAI DAT CB RUNG");
        display = 1;
        break;
    case 1:
        if (!display)
        {
            Lcd_clear_display();
        }
        Lcd_gotoxy(0, 0);
        Lcd_write_string(" CAI DAT CB LM35");
        Lcd_gotoxy(0, 1);
        Lcd_write_string(">CAI DAT CB DHT11");
        Lcd_gotoxy(0, 2);
        Lcd_write_string(" CAI DAT CB LDR");
        Lcd_gotoxy(0, 3);
        Lcd_write_string(" CAI DAT CB RUNG");
        display = 1;
        break;
    case 2:
        if (!display)
        {
            Lcd_clear_display();
        }
        Lcd_gotoxy(0, 0);
        Lcd_write_string(" CAI DAT CB LM35");
        Lcd_gotoxy(0, 1);
        Lcd_write_string(" CAI DAT CB DHT11");
        Lcd_gotoxy(0, 2);
        Lcd_write_string(">CAI DAT CB LDR");
        Lcd_gotoxy(0, 3);
        Lcd_write_string(" CAI DAT CB RUNG");
        display = 1;
        break;
    case 3:
        if (!display)
        {
            Lcd_clear_display();
        }
        Lcd_gotoxy(0, 0);
        Lcd_write_string(" CAI DAT CB LM35");
        Lcd_gotoxy(0, 1);
        Lcd_write_string(" CAI DAT CB DHT11");
        Lcd_gotoxy(0, 2);
        Lcd_write_string(" CAI DAT CB LDR");
        Lcd_gotoxy(0, 3);
        Lcd_write_string(">CAI DAT CB RUNG");
        display = 1;
        break;
    case 4:
        if (!display)
        {
            Lcd_clear_display();
        }
        Lcd_gotoxy(0, 0);
        Lcd_write_string(">CAI DAT LED MATRIX");
        Lcd_gotoxy(0, 1);
        Lcd_write_string(" CAI DAT COI");
        Lcd_gotoxy(0, 2);
        Lcd_write_string(" CAI DAT AM THANH");
        Lcd_gotoxy(0, 3);
        Lcd_write_string(" BACK");
        display = 1;
        break;
    case 5:
        if (!display)
        {
            Lcd_clear_display();
        }
        Lcd_gotoxy(0, 0);
        Lcd_write_string(" CAI DAT LED MATRIX");
        Lcd_gotoxy(0, 1);
        Lcd_write_string(">CAI DAT COI");
        Lcd_gotoxy(0, 2);
        Lcd_write_string(" CAI DAT AM THANH");
        Lcd_gotoxy(0, 3);
        Lcd_write_string(" BACK");
        display = 1;
        break;
    case 6:
        if (!display)
        {
            Lcd_clear_display();
        }
        Lcd_gotoxy(0, 0);
        Lcd_write_string(" CAI DAT LED MATRIX");
        Lcd_gotoxy(0, 1);
        Lcd_write_string(" CAI DAT COI");
        Lcd_gotoxy(0, 2);
        Lcd_write_string(">CAI DAT AM THANH");
        Lcd_gotoxy(0, 3);
        Lcd_write_string(" BACK");
        display = 1;
        break;
    case 7:
        if (!display)
        {
            Lcd_clear_display();
        }
        Lcd_gotoxy(0, 0);
        Lcd_write_string(" CAI DAT LED MATRIX");
        Lcd_gotoxy(0, 1);
        Lcd_write_string(" CAI DAT COI");
        Lcd_gotoxy(0, 2);
        Lcd_write_string(" CAI DAT AM THANH");
        Lcd_gotoxy(0, 3);
        Lcd_write_string(">BACK");
        display = 1;
        break;
    }
}
void Setup_Temp_LM35()
{
    switch (Setup_LM35)
    {
    case 0:
        if (!display)
        {
            Lcd_clear_display();
        }
        Lcd_gotoxy(0, 0);
        Lcd_write_string(">CAI DAT NGUONG");
        Lcd_gotoxy(0, 1);
        Lcd_write_string(" CHE DO COI");
        Lcd_gotoxy(0, 2);
        Lcd_write_string(" BACK");
        Lcd_gotoxy(0, 3);
        Lcd_write_string("***TRAN HUU DANH***");
        display = 1;
        break;
    case 1:
        if (!display)
        {
            Lcd_clear_display();
        }
        Lcd_gotoxy(0, 0);
        Lcd_write_string(" CAI DAT NGUONG");
        Lcd_gotoxy(0, 1);
        Lcd_write_string(">CHE DO COI");
        Lcd_gotoxy(0, 2);
        Lcd_write_string(" BACK");
        Lcd_gotoxy(0, 3);
        Lcd_write_string("***TRAN HUU DANH***");
        display = 1;
        break;
    case 2:
        if (!display)
        {
            Lcd_clear_display();
        }
        Lcd_gotoxy(0, 0);
        Lcd_write_string(" CAI DAT NGUONG");
        Lcd_gotoxy(0, 1);
        Lcd_write_string(" CHE DO COI");
        Lcd_gotoxy(0, 2);
        Lcd_write_string(">BACK");
        Lcd_gotoxy(0, 3);
        Lcd_write_string("***TRAN HUU DANH***");
        display = 1;
        break;
    }
}
void MODE_LM35()
{
    switch (Mode_LM35)
    {
    case 0:
        if (!display)
        {
            Lcd_clear_display();
        }
        Lcd_gotoxy(0, 0);
        Lcd_write_string("===CAI DAT NGUONG===");
        Lcd_gotoxy(0, 2);
        Lcd_write_string("NGUONG: -    +");
        Lcd_gotoxy(10, 2);
        Lcd_write_int(save_value_lm35);
        display = 1;
        break;
    case 1:
        if (!display)
        {
            Lcd_clear_display();
        }
        Lcd_gotoxy(0, 0);
        Lcd_write_string("=====CHE DO COI=====");
        Lcd_gotoxy(0, 2);
        Lcd_write_string("COI:     (ON/OFF)");
        Lcd_gotoxy(5, 2);
        Lcd_write_string(save_mode_lm35 ? "ON" : "OFF");
        display = 1;
        break;
    }
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC1)
    {
        ADC_LM35 = HAL_ADC_GetValue(hadc);
    }
}
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
    MX_ADC1_Init();
    /* USER CODE BEGIN 2 */
    Lcd_Init();
    Lcd_clear_display();
    HAL_ADC_Start_IT(&hadc1);
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    Manhinhchinh();
    HAL_Delay(2000);
    Lcd_clear_display();
    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        float voltage = (ADC_LM35 * 3.0f) / 4095.0f;
        TemperatureC = (uint8_t)(voltage * 100.0f);
        /////////////////////////////////// MENU HIEN THI CANH BAO ///////////////////////////////////

        if (TemperatureC >= save_value_lm35)
        {
            if (warning_lm35 == 0)
            {
                warning_lm35 = 1;
                Mode_Warning = 1;
                display = 0;
                Display_Warning();
                warning_start_time = HAL_GetTick(); // Lay thoi diem start warning
            }
        }
        else if (TemperatureC < save_value_lm35)
        {
            warning_lm35 = 0;
        }
        if (Mode_Warning == 1)
        {
            if (HAL_GetTick() - warning_start_time >= 3000)
            {
                Mode_Warning = 0;
                warning_lm35 = 1;
                display = 0;
                Lcd_clear_display();
                Display_All_Sensor_Values();
            }
            else
            {
                Display_Warning(); // Giu hien thi canh bao
            }
            continue; // Bỏ qua phần menu để chỉ hiển thị cảnh báo
        }

        /////////////////////////////////// MENU HIEN THI VALUES ///////////////////////////////////
        if (demtong == 0)
        {
            Display_All_Sensor_Values();
            if (nut_tang)
            {
                nut_tang = 0;
                if (Menu_value >= 1)
                {
                    display = 0;
                    Menu_value = 0;
                }
                else
                {
                    display = 0;
                    Menu_value++;
                }
                Display_All_Sensor_Values();
            }
            if (nut_giam)
            {
                nut_giam = 0;
                if (Menu_value <= 0)
                {
                    display = 0;
                    Menu_value = 1;
                }
                else
                {
                    display = 0;
                    Menu_value--;
                }
                Display_All_Sensor_Values();
            }
        }

        ///////////////////////////////// DIEU KHIEN CON TRO ///////////////////////////////////
        /////////////////////// NUT LEN ////////////////////////
        if (nut_tang)
        {
            nut_tang = 0;
            if (demtong == 1) // LEN MENU TONG
            {
                if (Menu_tong >= 7)
                {
                    display = 0;
                    Menu_tong = 0;
                }
                else
                {
                    display = 0;
                    Menu_tong++;
                }
                Menu_Tong();
            }
            else if (demtong == 2 && Menu_tong == 0)
            { // LEN MENU Setup_Temp_LM35
                if (Setup_LM35 >= 2)
                {
                    display = 0;
                    Setup_LM35 = 0;
                }
                else
                {
                    display = 0;
                    Setup_LM35++;
                }
                Setup_Temp_LM35();
            }
            else if (demtong == 3 && Setup_LM35 == 0)
            { // CONG GIA TRI NGUONG CUA LM35
                if (save_value_lm35 >= 99)
                {
                    display = 0;
                    save_value_lm35 = 0;
                }
                else
                {
                    display = 0;
                    save_value_lm35++;
                }
                MODE_LM35();
            }
            else if (demtong == 3 && Setup_LM35 == 1)
            { // ON/OFF MODE LM35
                if (save_mode_lm35 >= 1)
                {
                    display = 0;
                    save_mode_lm35 = 0;
                }
                else
                {
                    display = 0;
                    save_mode_lm35++;
                }
                MODE_LM35();
            }
        }
        /////////////////////// NUT XUONG ////////////////////////
        if (nut_giam)
        {
            nut_giam = 0;
            if (demtong == 1) // XUONG MENU TONG
            {
                if (Menu_tong <= 0)
                {
                    display = 0;
                    Menu_tong = 7;
                }
                else
                {
                    display = 0;
                    Menu_tong--;
                }
                Menu_Tong();
            }
            else if (demtong == 2 && Menu_tong == 0)
            { // XUONG MENU Setup_Temp_LM35
                if (Setup_LM35 <= 0)
                {
                    display = 0;
                    Setup_LM35 = 2;
                }
                else
                {
                    display = 0;
                    Setup_LM35--;
                }
                Setup_Temp_LM35();
            }
            else if (demtong == 3 && Setup_LM35 == 0)
            { // CONG GIA TRI NGUONG CUA LM35
                if (save_value_lm35 <= 0)
                {
                    display = 0;
                    save_value_lm35 = 99;
                }
                else
                {
                    display = 0;
                    save_value_lm35--;
                }
                MODE_LM35();
            }
            else if (demtong == 3 && Setup_LM35 == 1)
            { // ON/OFF MODE LM35
                if (save_mode_lm35 <= 0)
                {
                    display = 0;
                    save_mode_lm35 = 1;
                }
                else
                {
                    display = 0;
                    save_mode_lm35--;
                }
                MODE_LM35();
            }
        }
        ////////////////////////////////////// BUTTON //////////////////////////////////////
        if (nut_ok)
        {
            nut_ok = 0;
            demtong++;
            if (demtong == 1) // Ở Menu Tổng
            {
                display = 0;
                Menu_Tong();
            }
            else if (demtong == 2 && Menu_tong == 7)
            { // Nhấn BACK Từ MENU Tổng Về Display_All_Sensor_Values
                display = 0;
                demtong = 0;
                Menu_tong = 0;
                Display_All_Sensor_Values();
            }
            else if (demtong == 2 && Menu_tong == 0)
            { // Ở MENU Setup_Temp_LM35
                display = 0;
                Setup_Temp_LM35();
            }
            else if (demtong == 3 && Setup_LM35 == 2)
            { // Nhấn BACK Từ Menu Setup_Temp_LM35 Về Menu Tổng
                display = 0;
                demtong = 1;
                Setup_LM35 = 0;
                Menu_tong = 0;
                Menu_Tong();
            }
            else if (demtong == 3 && Setup_LM35 == 0)
            { // Ở MENU MODE_LM35 1
                display = 0;
                Mode_LM35 = 0;
                count_lm35++;
                MODE_LM35();
            }
            else if (demtong == 5 && Setup_LM35 == 0 && count_lm35 == 1)
            { // Nhấn BACK Từ MENU MODE_LM35 1 Về Menu Setup_Temp_LM35
                display = 0;
                demtong = 2;
                Setup_LM35 = 0;
                count_lm35 = 0;
                Setup_Temp_LM35();
            }
            else if (demtong == 3 && Setup_LM35 == 1)
            { // Ở MENU MODE_LM35 2
                display = 0;
                Mode_LM35 = 1;
                count_lm35++;
                MODE_LM35();
            }
            else if (demtong == 5 && Setup_LM35 == 1 && count_lm35 == 1)
            { // // Nhấn BACK Từ MENU MODE_LM35 2 Về Menu Setup_Temp_LM35
                display = 0;
                demtong = 2;
                Setup_LM35 = 1;
                count_lm35 = 0;
                Setup_Temp_LM35();
            }
        }
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
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
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
    hadc1.Init.ContinuousConvMode = ENABLE;
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
    sConfig.Channel = ADC_CHANNEL_1;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN ADC1_Init 2 */

    /* USER CODE END ADC1_Init 2 */
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
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_8 | GPIO_PIN_9, GPIO_PIN_RESET);

    /*Configure GPIO pins : PC13 PC14 PC15 */
    GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : PB10 PB12 PB13 PB14
                             PB15 PB8 PB9 */
    GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

    /* USER CODE BEGIN MX_GPIO_Init_2 */

    /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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

#ifdef USE_FULL_ASSERT
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

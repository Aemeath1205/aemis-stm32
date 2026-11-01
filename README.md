# STM32F103 外设实验合集

基于 STM32F103ZE、STM32 HAL 库、Keil MDK 的 28 个外设实验工程。

## 工程列表

- `BASIC_TIM_1ms_test`
- `BASIC_TIM_test`
- `BSP_Drivers`
- `DMA_I2C_test`
- `DMA_SPI_test`
- `DMA_USART_test`
- `I2C_EEPROM_test`
- `InternalFlash_test`
- `INTTERUPT_GPIO_test`
- `INTTERUPT_I2C_test`
- `INTTERUPT_SPI_test`
- `INTTERUPT_USART_test`
- `IWDG_test`
- `KEY_LED_TEST`
- `LED_test`
- `RCC_test`
- `RTC_Data_test`
- `RTC_TIME_test`
- `SC_LED_test`
- `SPI_Flash_Demo_test`
- `SPI_Flash_test`
- `TIM_Breathing_Light_test`
- `TIM_FULL_ColorLED_test`
- `TIM_GENERAL_test`
- `TIM_IC_HC_SR04_test`
- `Usart_Redir_test`
- `Usart1_Echo_test`
- `Usart1_test`
- `WWDG_test`

## 关于 Drivers 目录

本仓库**不包含** `Drivers/`（HAL 库与 CMSIS）。原因：

- 每个工程都自带一份完整的 HAL + CMSIS，28 份重复约 **1.8 GB**
- Gitee 单仓库上限为 500 MB，无法直接提交

克隆后编译步骤：

1. 安装 STM32CubeMX，在 `Help -> Manage embedded software packages` 里安装 **STM32Cube MCU Package for STM32F1**
2. 用 CubeMX 打开目标工程目录下的 `工程名.ioc`
3. 在 `Project Manager` 页确认 Toolchain/IDE 为 **MDK-ARM V5**，点击 `GENERATE CODE`
4. 用 Keil 打开 `MDK-ARM/工程名.uvprojx`，编译下载

> CubeMX 生成时会重新创建 `Drivers/`。如果工程里手动改过 HAL 源码会被覆盖，建议不要修改 HAL 库本身。

## 目录结构

每个工程结构一致：

```
工程名/
├── Core/
│   ├── Inc/                头文件
│   └── Src/                源文件（main.c 等）
├── MDK-ARM/                Keil 工程文件
│   ├── 工程名.uvprojx
│   ├── 工程名.uvoptx
│   └── startup_stm32f103xe.s
├── Drivers/                （被 .gitignore 排除，需 CubeMX 重新生成）
└── 工程名.ioc              CubeMX 配置文件
```

编译产物（`Objects/`、`Listings/`、`*.hex`、`*.map`）与 IDE 本地配置（`.vscode/`）均已被 `.gitignore` 排除。

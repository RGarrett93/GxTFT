// e.g. https://www.aliexpress.com/item/IPS-3-97-inch-HD-TFT-LCD-Touch-Screen-Module-OTM8009A-Drive-IC-800-480/32676929794.html
// on   https://www.aliexpress.com/item/STM32F407ZGT6-Development-Board-ARM-M4-STM32F4-cortex-M4-core-Board-Compatibility-Multiple-Extension/32795142050.html
//
// this version is for use with Arduino package "STM32 Boards (select from submenu)" (STMicroelectronics), board "Generic STM32F4 series" part "BLACK F407ZG".
// preferences Additional Boards Manager URLs https://raw.githubusercontent.com/stm32duino/BoardManagerFiles/master/STM32/package_stm_index.json

#include "../GxIO/STM32MICRO/GxIO_STM32F407ZGM4_FSMC/GxIO_STM32F407ZGM4_FSMC.h"
#include "../GxCTRL/GxCTRL_OTM8009A/GxCTRL_OTM8009A.h" // 800x480
GxIO_Class io; // #define GxIO_Class is in the selected header file
GxCTRL_Class controller(io); // #define GxCTRL_Class is in the selected header file
TFT_Class tft(io, controller, 800, 480); // landscape 800x480
//TFT_Class tft(io, controller, 480, 800); // portrait 480x800

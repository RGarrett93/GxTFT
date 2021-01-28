//Using ZinggJM/GxTFT library and adding to GxIO for STM32 Nucleo 144
//The below doesn't work, currently experimenting with a Nucleo 144 & SSD1963 Display
//For now currently attempting to use the exposed pins on the board CN7/CN10 for Data and CN9 for Ctrl 

#include "Nucleo_144.h"


#define PA_USED_BITS 0x00E8
#define PB_USED_BITS 0x0300
#define PC_USED_BITS 0x0008
#define PD_USED_BITS 0xC000
#define PE_USED_BITS 0x2A00
#define PF_USED_BITS 0xF428
#define PG_USED_BITS 0x4200

#define PA_DATA_BITS 0x00E8
#define PB_DATA_BITS 0x0300
#define PC_DATA_BITS 0x0008
#define PD_DATA_BITS 0xC000
#define PE_DATA_BITS 0x2A00
#define PF_DATA_BITS 0xF428
#define PG_DATA_BITS 0x4200

#define PA_MODE_MASK 0x0000FCC0 // all used bits
#define PA_MODE_BITS 0x00005440 // 01 : general purpose output mode
#define PB_MODE_MASK 0x000F0000 // all used bits
#define PB_MODE_BITS 0x00050000 // 01 : general purpose output mode
#define PC_MODE_MASK 0x000000C3 // all used bits
#define PC_MODE_BITS 0x00000041 // 01 : general purpose output mode
#define PD_MODE_MASK 0xF0000000 // all used bits
#define PD_MODE_BITS 0x50000000 // 01 : general purpose output mode
#define PE_MODE_MASK 0x0CCC0000 // all used bits
#define PE_MODE_BITS 0x04440000 // 01 : general purpose output mode
#define PF_MODE_MASK 0xFF300CC0 // all used bits
#define PF_MODE_BITS 0x55100440 // 01 : general purpose output mode
#define PG_MODE_MASK 0x300C0000 // all used bits
#define PG_MODE_BITS 0x10040000 // 01 : general purpose output mode

#define PA_MODE_DATA 0x0000FC00 // all data bits
#define PA_MODE_OUTP 0x00005400 // 01 : general purpose output mode
#define PA_MODE_INP  0x00000000 // 00 : input floating mode
#define PB_MODE_DATA 0x000F0000 // all data bits
#define PB_MODE_OUTP 0x00050000 // 01 : general purpose output mode
#define PB_MODE_INP  0x00000000 // 00 : input floating mode
#define PD_MODE_DATA 0xF0000000 // all data bits
#define PD_MODE_OUTP 0x50000000 // 01 : general purpose output mode
#define PD_MODE_INP  0x00000000 // 00 : input floating mode
#define PE_MODE_DATA 0x0CCC0000 // all data bits
#define PE_MODE_OUTP 0x04440000 // 01 : general purpose output mode
#define PE_MODE_INP  0x00000000 // 00 : input floating mode
#define PF_MODE_DATA 0xFF000000 // all data bits
#define PF_MODE_OUTP 0x55000000 // 01 : general purpose output mode
#define PF_MODE_INP  0x00000000 // 00 : input floating mode
#define PG_MODE_DATA 0x300C0000 // all data bits
#define PG_MODE_OUTP 0x10040000 // 01 : general purpose output mode
#define PG_MODE_INP  0x00000000 // 00 : input floating mode

Nucleo_144::Nucleo_144(bool bl_active_high)
{
  _cs   = PA3;
  _rs   = PF3;
  _rst  = 0; // not available, driven from NRST
  _wr   = PC3;
  _rd   = PC0;
  _bl   = PB1;
  _bl_active_high = bl_active_high;
}

void Nucleo_144::reset()
{
  // _rst pin not available
}

void Nucleo_144::init()
{
  RCC->AHB1ENR |= 0x0000007F; // enable A, B, C, D, E, F, G clock speed
  READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN); // delay after an RCC peripheral clock enabling
  READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN); // delay after an RCC peripheral clock enabling
  READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN); // delay after an RCC peripheral clock enabling
  READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN); // delay after an RCC peripheral clock enabling
  READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOEEN); // delay after an RCC peripheral clock enabling
  READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOFEN); // delay after an RCC peripheral clock enabling
  READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOGEN); // delay after an RCC peripheral clock enabling

  GPIOA->BSRR |= PA_USED_BITS; // preset all output high
  GPIOA->MODER &= ~PA_MODE_MASK;
  GPIOA->MODER |= PA_MODE_BITS;
  GPIOA->OTYPER &= ~PA_USED_BITS; // 0 : output push-pull //PA_USED_BITS
  GPIOA->OSPEEDR &= ~0x00; // 0 : low speed //PA_MODE_MASK
  GPIOA->PUPDR &= ~PA_MODE_MASK; // 0 : no pull-up, no pull-down //PA_MODE_MASK

  GPIOB->BSRR |= PB_USED_BITS; // preset all output high
  GPIOB->MODER &= ~PB_MODE_MASK;
  GPIOB->MODER |= PB_MODE_BITS;
  GPIOB->OTYPER &= ~PB_USED_BITS; // 0 : output push-pull
  GPIOB->OSPEEDR &= ~0x00; // 0 : low speed
  GPIOB->PUPDR &= ~PB_MODE_MASK; // 0 : no pull-up, no pull-down

  GPIOC->BSRR |= PC_USED_BITS; // preset all output high
  GPIOC->MODER &= ~PC_MODE_MASK;
  GPIOC->MODER |= PC_MODE_BITS;
  GPIOC->OTYPER &= ~PC_USED_BITS; // 0 : output push-pull
  GPIOC->OSPEEDR &= ~0x00; // 0 : low speed
  GPIOC->PUPDR &= ~PC_MODE_MASK; // 0 : no pull-up, no pull-down

  GPIOD->BSRR |= PD_USED_BITS; // preset all output high
  GPIOD->MODER &= ~PD_MODE_MASK;
  GPIOD->MODER |= PD_MODE_BITS;
  GPIOD->OTYPER &= ~PD_USED_BITS; // 0 : output push-pull
  GPIOD->OSPEEDR &= ~0x00; // 0 : low speed
  GPIOD->PUPDR &= ~PD_MODE_MASK; // 0 : no pull-up, no pull-down

  GPIOE->BSRR |= PE_USED_BITS; // preset all output high
  GPIOE->MODER &= ~PE_MODE_MASK;
  GPIOE->MODER |= PE_MODE_BITS;
  GPIOE->OTYPER &= ~PE_USED_BITS; // 0 : output push-pull
  GPIOE->OSPEEDR &= ~0x00; // 0 : low speed
  GPIOE->PUPDR &= ~PE_MODE_MASK; // 0 : no pull-up, no pull-down

  GPIOF->BSRR |= PF_USED_BITS; // preset all output high
  GPIOF->MODER &= ~PF_MODE_MASK;
  GPIOF->MODER |= PF_MODE_BITS;
  GPIOF->OTYPER &= ~PF_USED_BITS; // 0 : output push-pull
  GPIOF->OSPEEDR &= ~0x00; // 0 : low speed
  GPIOF->PUPDR &= ~PF_MODE_MASK; // 0 : no pull-up, no pull-down

  GPIOG->BSRR |= PG_USED_BITS; // preset all output high
  GPIOG->MODER &= ~PG_MODE_MASK;
  GPIOG->MODER |= PG_MODE_BITS;
  GPIOG->OTYPER &= ~PG_USED_BITS; // 0 : output push-pull
  GPIOG->OSPEEDR &= ~0x00; // 0 : low speed
  GPIOG->PUPDR &= ~PG_MODE_MASK; // 0 : no pull-up, no pull-down

  
  pinMode(_cs, OUTPUT);
  digitalWrite(_cs, HIGH);
  pinMode(_rs, OUTPUT);
  digitalWrite(_rs, HIGH);
  pinMode(_bl, OUTPUT);
  digitalWrite(_bl, LOW);
  reset();
}

uint8_t Nucleo_144::readDataTransaction()
{
  return readData16Transaction();
}

uint16_t Nucleo_144::readData16Transaction()
{
  GPIOA->BSRR = (0x1 << (3 + 16));  // PA3 CS low
  uint16_t rv = readData16();
  GPIOA->BSRR = (0x1 << 3);  // PA3 CS high
  return rv;
}

uint8_t Nucleo_144::readData()
{
  return readData16();
}

uint16_t Nucleo_144::readData16()
{
  // Set direction input
  GPIOA->MODER &= ~PD_MODE_DATA;
  GPIOB->MODER &= ~PD_MODE_DATA;
  GPIOD->MODER &= ~PD_MODE_DATA;
  GPIOE->MODER &= ~PD_MODE_DATA;
  GPIOF->MODER &= ~PE_MODE_DATA;
  GPIOG->MODER &= ~PD_MODE_DATA;
//  GPIOD->BSRR = (0x1 << (4 + 16));  // PC0 RD low pulse
//  GPIOD->BSRR = (0x1 << 4); // PC0 RD high
  GPIOC->BSRR = (0x1 << (0 + 16));  // PC0 RD low read
  GPIOC->BSRR = (0x1 << (0 + 16));  // PC0 RD low read
  GPIOC->BSRR = (0x1 << (0 + 16));  // PC0 RD low read
  GPIOC->BSRR = (0x1 << (0 + 16));  // PC0 RD low read
  GPIOC->BSRR = (0x1 << (0 + 16));  // PC0 RD low read
  GPIOC->BSRR = (0x1 << (0 + 16));  // PC0 RD low read
  GPIOC->BSRR = (0x1 << (0 + 16));  // PC0 RD low read
  GPIOC->BSRR = (0x1 << (0 + 16));  // PC0 RD low read
/*GPIOC->BSRR = 0b0000000000000001 << 16;
GPIOC->BSRR = 0b0000000000000001 << 16;
GPIOC->BSRR = 0b0000000000000001 << 16;
GPIOC->BSRR = 0b0000000000000001 << 16;
GPIOC->BSRR = 0b0000000000000001 << 16;
GPIOC->BSRR = 0b0000000000000001 << 16;
GPIOC->BSRR = 0b0000000000000001 << 16;*/
  uint16_t rv = 0;
  // The compiler efficiently codes this  so it is quite quick.
  rv |= (GPIOB->IDR & (0x1 << 8)) << (15 - 8); // PB8
  rv |= (GPIOB->IDR & (0x1 << 9)) << (14 - 9); // PB9
  rv |= (GPIOA->IDR & (0x1 << 5)) << (13 - 5); // PA5
  rv |= (GPIOA->IDR & (0x1 << 6)) << (12 - 6); // PA6
  rv |= (GPIOA->IDR & (0x1 << 7)) << (11 - 7); // PA7
  rv |= (GPIOD->IDR & (0x1 << 14)) >> -(10 - 14); // PD14
  rv |= (GPIOD->IDR & (0x1 << 15)) >> -(9 - 15); // PD15
  rv |= (GPIOF->IDR & (0x1 << 12)) >> -(8 - 12); // PF12
  rv |= (GPIOF->IDR & (0x1 << 13)) >> -(7 - 13); // PF13
  rv |= (GPIOE->IDR & (0x1 << 9)) >> -(6 - 9); // PE9
  rv |= (GPIOE->IDR & (0x1 << 11)) >> -(5 - 11); // PE11
  rv |= (GPIOF->IDR & (0x1 << 14)) >> -(4 - 14); // PF14
  rv |= (GPIOE->IDR & (0x1 << 13)) >> -(3 - 13); // PE13
  rv |= (GPIOF->IDR & (0x1 << 15)) >> -(2 - 15); // PF15
  rv |= (GPIOG->IDR & (0x1 << 14)) >> -(1 - 14); // PG14
  rv |= (GPIOG->IDR & (0x1 << 9)) >> -(0 - 9); // PG9
  GPIOC->BSRR = (0x1 << 0); // PC0 RD high
  // Set direction output again
  GPIOA->MODER &= ~PA_MODE_DATA;
  GPIOA->MODER |= PA_MODE_OUTP;
  GPIOB->MODER &= ~PB_MODE_DATA;
  GPIOB->MODER |= PB_MODE_OUTP;
  GPIOD->MODER &= ~PD_MODE_DATA;
  GPIOD->MODER |= PD_MODE_OUTP;
  GPIOE->MODER &= ~PE_MODE_DATA;
  GPIOE->MODER |= PE_MODE_OUTP;
  GPIOF->MODER &= ~PF_MODE_DATA;
  GPIOF->MODER |= PF_MODE_OUTP;
  GPIOG->MODER &= ~PG_MODE_DATA;
  GPIOG->MODER |= PG_MODE_OUTP;
  return rv;
}
/*
uint32_t Nucleo_144::readRawData32(uint8_t part)
{
  // Set direction input
  GPIOD->MODER &= ~PD_MODE_DATA;
  GPIOE->MODER &= ~PE_MODE_DATA;
  GPIOC->BSRR = (0x1 << (0 + 16));  // PC0 RD low pulse
  GPIOC->BSRR = (0x1 << 0); // PC0 RD high
  GPIOC->BSRR = (0x1 << (0 + 16));  // PC0 RD low read
  uint32_t rv = 0;
  if (part == 0) rv = GPIOD->IDR & PD_DATA_BITS;
  if (part == 1) rv = GPIOE->IDR & PE_DATA_BITS;
  GPIOD->BSRR = (0x1 << 4); // PC0 RD high
  // Set direction output again
  GPIOD->MODER &= ~PD_MODE_DATA;
  GPIOD->MODER |= PD_MODE_OUTP;
  GPIOE->MODER &= ~PD_MODE_DATA;
  GPIOE->MODER |= PE_MODE_OUTP;
  return rv;
}
*/
void Nucleo_144::writeCommandTransaction(uint8_t c)
{
  GPIOA->BSRR = (0x1 << (3 + 16));  // PA3 CS low
  GPIOF->BSRR = (0x1 << (3 + 16));  // PF3 RS low
  writeData16(c);
  GPIOF->BSRR = (0x1 << 3);  // PF3 RS high
  GPIOA->BSRR = (0x1 << 3);  // PA3 CS high
}

void Nucleo_144::writeCommand16Transaction(uint16_t c)
{
  GPIOA->BSRR = (0x1 << (3 + 16));  // PA3 CS low
  GPIOF->BSRR = (0x1 << (3 + 16));  // PF3 RS low
  writeData16(c);
  GPIOF->BSRR = (0x1 << 3);  // PF3 RS high
  GPIOA->BSRR = (0x1 << 3);  // PA3 CS high
}

void Nucleo_144::writeDataTransaction(uint8_t d)
{
  GPIOA->BSRR = (0x1 << (3 + 16));  // PA3 CS low
  writeData16(d);
  GPIOA->BSRR = (0x1 << 3);  // PA3 CS high
}

void Nucleo_144::writeData16Transaction(uint16_t d, uint32_t num)
{
 // GPIOA->BSRR = (0x1 << (3 + 16));  // PA3 CS low
 digitalWrite(PA3, LOW);
  writeData16(d, num);
  digitalWrite(PA3, HIGH); 
  //GPIOA->BSRR = (0x1 << 3);  // PA3 CS high
}

void Nucleo_144::writeCommand(uint8_t c)
{
  GPIOF->BSRR = (0x1 << (3 + 16));  // PF3 RS low
  writeData16(c);
  GPIOF->BSRR = (0x1 << 3);  // PF3 RS high
}

void Nucleo_144::writeCommand16(uint16_t c)
{
  GPIOF->BSRR = (0x1 << (3 + 16));  // PF3 RS low
  writeData16(c);
  GPIOF->BSRR = (0x1 << 3);  // PF3 RS high
}

void Nucleo_144::writeData(uint8_t d)
{
  writeData16(d);
}

void Nucleo_144::writeData(uint8_t* d, uint32_t num)
{
  while (num > 0)
  {
    writeData16(*d);
    d++;
    num--;
  }
}

void Nucleo_144::writeData16(uint16_t d, uint32_t num)
{
  GPIOA->BSRR = PA_DATA_BITS << 16; // clear bits
  GPIOB->BSRR = PB_DATA_BITS << 16; // clear bits
  //GPIOC->BSRR = PC_DATA_BITS << 16; // clear bits
  GPIOD->BSRR = PD_DATA_BITS << 16; // clear bits
  GPIOE->BSRR = PE_DATA_BITS << 16; // clear bits
  GPIOF->BSRR = PF_DATA_BITS << 16; // clear bits
  GPIOG->BSRR = PG_DATA_BITS << 16; // clear bits

  // TFT connector to Nucleo 144 pins
  // D0   D1   D2   D3   D4   D5   D6  D7   D8   D9  D10  D11 D12 D13 D14 D15
  // PG9 PG14 PF15 PE13 PF14 PE11 PE9 PF13 PF12 PD15 PD14 PA7 PA6 PA5 PB9 PB8

  // The compiler efficiently codes this  so it is quite quick.
 
  if (d & 0x8000) GPIOB->BSRR = 0x1 << 8; // PB8
  if (d & 0x4000) GPIOB->BSRR = 0x1 << 9; // PB9
  if (d & 0x2000) GPIOA->BSRR = 0x1 << 5; // PA5
  if (d & 0x1000) GPIOA->BSRR = 0x1 << 6; // PA6
  if (d & 0x0800) GPIOA->BSRR = 0x1 << 7; // PA7
  if (d & 0x0400) GPIOD->BSRR = 0x1 << 14; // PD14
  if (d & 0x0200) GPIOD->BSRR = 0x1 << 15; // PD15
  if (d & 0x0100) GPIOF->BSRR = 0x1 << 12; // PF12

  if (d & 0x0080) GPIOF->BSRR = 0x1 << 13; // PF13
  if (d & 0x0040) GPIOE->BSRR = 0x1 << 9; // PE9
  if (d & 0x0020) GPIOE->BSRR = 0x1 << 11; // PE11
  if (d & 0x0010) GPIOF->BSRR = 0x1 << 14; // PF14
  if (d & 0x0008) GPIOE->BSRR = 0x1 << 13; // PE13
  if (d & 0x0004) GPIOF->BSRR = 0x1 << 15; // PF15
  if (d & 0x0002) GPIOG->BSRR = 0x1 << 14; // PG14
  if (d & 0x0001) GPIOG->BSRR = 0x1 << 9; // PG9
///////////////////////////////////////////////////////////
/*
  if (d & 0x8000) GPIOG->BSRR = 0x1 << 9; // PB8
  if (d & 0x4000) GPIOG->BSRR = 0x1 << 14; // PB9
  if (d & 0x2000) GPIOF->BSRR = 0x1 << 15;// PA5
  if (d & 0x1000) GPIOE->BSRR = 0x1 << 13;// PA6
  if (d & 0x0800) GPIOF->BSRR = 0x1 << 14; // PA7
  if (d & 0x0400) GPIOE->BSRR = 0x1 << 11; // PD14
  if (d & 0x0200) GPIOE->BSRR = 0x1 << 9; // PD15
  if (d & 0x0100) GPIOF->BSRR = 0x1 << 13; // PF12

  if (d & 0x0080) GPIOF->BSRR = 0x1 << 12; // PF13
  if (d & 0x0040) GPIOD->BSRR = 0x1 << 15; // PE9
  if (d & 0x0020) GPIOD->BSRR = 0x1 << 14; // PE11
  if (d & 0x0010) GPIOA->BSRR = 0x1 << 7; // PF14
  if (d & 0x0008) GPIOA->BSRR = 0x1 << 6; // PE13
  if (d & 0x0004) GPIOA->BSRR = 0x1 << 5; // PF15
  if (d & 0x0002) GPIOB->BSRR = 0x1 << 9; // PG14
  if (d & 0x0001) GPIOB->BSRR = 0x1 << 8; // PG9
*/
  while (num)
  {
    /*
    GPIOC->BSRR = (0x1 << (3 + 16));  // PC3 WR low
    GPIOC->BSRR = (0x1 << (3 + 16));  // PC3 WR low
    GPIOC->BSRR = (0x1 << (3 + 16));  // PC3 WR low
    GPIOC->BSRR = (0x1 << (3 + 16));  // PC3 WR low
    GPIOC->BSRR = (0x1 << 3); // PC3 WR high
    */
    digitalWrite(_wr, LOW);
    digitalWrite(_wr, LOW);
    digitalWrite(_wr, LOW);
    digitalWrite(_wr, LOW);
    digitalWrite(_wr, HIGH);
    num--;
  }
}

void Nucleo_144::writeAddrMSBfirst(uint16_t d)
{
  writeData16(d >> 8);
  writeData16(d & 0xFF);
}

void Nucleo_144::startTransaction()
{
  GPIOA->BSRR = (0x1 << (3 + 16));  // PA3 CS low
}

void Nucleo_144::endTransaction()
{
  GPIOA->BSRR = (0x1 << 3);  // PA3 CS high
}

void Nucleo_144::selectRegister(bool rs_low)
{
  digitalWrite(_rs, (rs_low ? LOW : HIGH));
}

void Nucleo_144::setBackLight(bool lit)
{
  digitalWrite(_bl, (lit == _bl_active_high));
}


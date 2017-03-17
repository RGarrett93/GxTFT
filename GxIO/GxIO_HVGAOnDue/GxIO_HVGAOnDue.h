// created by Jean-Marc Zingg to be the GxIO_HVGAOnMEGA io class for the GxTFT library
// code extracts taken from https://github.com/Bodmer/TFT_HX8357_Due
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE

#ifndef _GxIO_HVGAOnDue_H_
#define _GxIO_HVGAOnDue_H_

#include "../GxIO.h"

#if defined(ARDUINO_ARCH_SAM)

class GxIO_HVGAOnDue : public GxIO
{
  public:
    GxIO_HVGAOnDue();
    const char* name = "GxIO_HVGAOnDue";
    void reset();
    void init();
    //uint8_t transferTransaction(uint8_t d);
    //uint16_t transfer16Transaction(uint16_t d);
    //uint8_t readDataTransaction();
    //uint16_t readData16Transaction();
    void writeCommandTransaction(uint8_t c);
    void writeDataTransaction(uint8_t d);
    void writeData16Transaction(uint16_t d, uint32_t num = 1);
    void writeCommand(uint8_t c);
    void writeData(uint8_t d);
    void writeData(uint8_t* d, uint32_t num);
    void writeData16(uint16_t d, uint32_t num = 1);
    void writeAddrMSBfirst(uint16_t d);
    void startTransaction();
    void endTransaction();
    void setBackLight(bool lit);
  private:
    void strobes(uint32_t num);
    int8_t _cs, _rs, _rst, _wr, _rd, _bl; // Control lines
};

#define GxIO_Class GxIO_HVGAOnDue

#endif

#endif

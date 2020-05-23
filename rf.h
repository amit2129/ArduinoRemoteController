#ifndef RF_H
#define RF_H
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "UniversalRemote.h"
#define CS_PIN 7
#define DC_PIN 8
//RF24 nRF24(CS_PIN, DC_PIN);
class UniversalRemote;
class RF{
  private:
    byte state;
    const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };
    static UniversalRemote *remote;
  public:
    //RF();
    void init();
    void get_control_options();
    bool get_link_state();
};





#endif

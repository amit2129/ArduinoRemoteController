#include "rf.h"
RF24 nRF24(CS_PIN, DC_PIN);

void RF::init(){
  nRF24.begin();
  nRF24.setAutoAck(1);                    // Ensure autoACK is enabled
  nRF24.enableAckPayload();               // Allow optional ack payloads
  nRF24.setRetries(0, 15);                // Smallest time between retries, max no. of retries
  nRF24.setPayloadSize(32);                // Here we are sending 1-byte payloads to test the call-response speed
  nRF24.setPALevel(RF24_PA_MAX);
  nRF24.setCRCLength(RF24_CRC_8);
  nRF24.openWritingPipe(pipes[0]);        // Both radios listen on the same pipes by default, and switch when writing
  nRF24.openReadingPipe(1, pipes[1]);
  nRF24.startListening();                 // Start listening
  nRF24.printDetails();                   // Dump the configuration of the rf unit for debugging
}
void RF::get_control_options(){}

bool RF::get_link_state(){
  
}

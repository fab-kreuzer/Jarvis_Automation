#ifndef JARVIS_PINOUTS_H
#define JARVIS_PINOUTS_H
#include <Arduino.h>

// You need to define pins for 4 to 6 GPIOs to connect to your Jarvis RJ-45 cable.
// 4 pins go to the handset buttons. These encode the Up, Down, Memory and Presets (1-4) buttons.
// 2 pins go to the serial TX/RX pins between the handset and the desk controller.
//   * Since we only receive on these pins, I call them "desk transmit (DTX)" and handset transmit (HTX)
//   * HTX is optional; I only use this for experimenting with the handset serial commands.
//   * HS3 is optional; it is not needed for controlling the Jarvis Desk, afaict.
//
// RJ-45   Signal   Description
//  pin     ID
//   1      HS3     Handset control line 3
//   2      DTX     Serial control messages from controller to handset
//   3      GND
//   4      HTX     Serial control messages from handset to controller
//   5      VCC 5v
//   6      HS2     Handset control line 2
//   7      HS1     Handset control line 1
//   8      HS0     Handset control line 0

// -----------------------------------------------------------------------------

#define NC -1   // Use NC for pins that are not connected or not used

int HS3 = 33;
int DTX = 12; 
int HTX = 14;
int HS2 = 27;
int HS1 = 25;
int HS0 = 26;

inline bool is_pin_connected(int pin) { return pin != NC; }

#endif
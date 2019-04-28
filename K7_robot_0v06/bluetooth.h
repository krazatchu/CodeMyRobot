#ifndef _BLUETOOTH_H
#define _BLUETOOTH_H

// Header File for Serial Bluetooth, will be added by default into Arduino
#include "BluetoothSerial.h"

// Bluetooth setup.
void BTsetup(void);

// Bluetooth read character. This goes in the loop().
bool BTloop(char *value);

#endif

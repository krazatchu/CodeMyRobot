#include "bluetooth.h"

BluetoothSerial ESP_BT; //Object for Bluetooth

void BTsetup(void) {
  Serial.begin(115200);
  ESP_BT.begin("ESP32_Bluetooth_Control"); //Name of your Bluetooth Signal
  Serial.println("Bluetooth Device is Ready to Pair");
}

bool BTloop(char *value) {
  if (ESP_BT.available()) //Check if we receive anything from Bluetooth
  {
    value[0] = ESP_BT.read(); //Read what we recevive
    // Ignore chars below 'space'.
    if ((int)value[0] < 32) {
      return false;
    }

    Serial.print("Bluetooth Received: ");
    Serial.println((int)value[0]);

    // Reply to sender.
    ESP_BT.print("Received: ");
    ESP_BT.println((int)value[0]);

    return true;
  }

  return false;
}

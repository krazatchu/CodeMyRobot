BluetoothSerial ESP_BT; //Object for Bluetooth

int incoming;
//int LED_BUILTIN = 4;

#include  "bluetooth.h"

void BTsetup() {
  Serial.begin(9600); //Start  Serial monitor in 9600
  ESP_BT.begin("ESP32_LED_Control"); //Name of your Bluetooth Signal
  Serial.println("Bluetooth Device is Ready to Pair");

  //pinMode (LED_BUILTIN, OUTPUT);//Specify that LED pin is output
}

void BTloop() {
  
  if (ESP_BT.available()) //Check if we receive anything from Bluetooth
  {
    incoming = ESP_BT.read(); //Read what we recevive 
    Serial.print("Received:"); Serial.println(incoming);

    if (incoming == 48) //0
        {
       // digitalWrite(LED_BUILTIN, HIGH);
        ESP_BT.println("FACE 0 ON");
        }
        
    if (incoming == 49) //1
        {
       // digitalWrite(LED_BUILTIN, LOW);
        ESP_BT.println("FACE 1 ON");
        }  
        
    if (incoming == 50) //2
        {
       // digitalWrite(LED_BUILTIN, LOW);
        ESP_BT.println("FACE 2 ON");
        }
    if (incoming == 51) //3
        {
       // digitalWrite(LED_BUILTIN, HIGH);
        ESP_BT.println("FACE 3 ON");
        }
        
    if (incoming == 52) //4
        {
       // digitalWrite(LED_BUILTIN, LOW);
        ESP_BT.println("FACE 4 ON");
        }  
        
    if (incoming == 53) //5
        {
       // digitalWrite(LED_BUILTIN, LOW);
        ESP_BT.println("FACE 5 ON");
        }
    if (incoming == 54) //6
        {
       // digitalWrite(LED_BUILTIN, LOW);
        ESP_BT.println("FACE 6 ON");
        }  
        
    if (incoming == 55) //7
        {
       // digitalWrite(LED_BUILTIN, LOW);
        ESP_BT.println("FACE 7 ON");
        }          
  }
  delay(20);
}

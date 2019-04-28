#include <IRremote.h>
#include "pins.h"
IRrecv irrecv(irReceivePin);
decode_results results;


#define forwardCode 0xFFFFFF
#define backwardCode 0xFFFF11
#define leftCode 0xFFFF22
#define rightCode 0xFFFF33
#define enterCode 0xEEEEEE

#define oneCode 0x111111
#define twoCode 0x222222
#define threeCode 0x333333
#define fourCode 0x444444
#define fiveCode 0x555555
#define sixCode 0x666666
#define sevenCode 0x777777
#define eightCode 0x888888
#define nineCode 0x999999


void enableIR()
{
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");
}

void controlIRReceive()
{
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    
    if (results.value == forwardCode)
    {
      //goForward();
    }
    else if (results.value == backwardCode)
    {
      //goBackward();
    }
    else if (results.value == leftCode)
    {
      //goLeft();
    }
    else if (results.value == rightCode)
    {
      //goRight();
    }
    else if (results.value == enterCode)
    {
      //spin?
    }
    else if (results.value == oneCode)
    {
      //change faces on different the numbers?
    }
    else if (results.value == twoCode)
    {
      
    }
    else if (results.value == threeCode)
    {
      
    }
    else if (results.value == fourCode)
    {
      
    }
    else if (results.value == fiveCode)
    {
      
    }
    else if (results.value == sixCode)
    {
      
    }
    else if (results.value == sevenCode)
    {
      
    }
    else if (results.value == eightCode)
    {
      
    }
    else if (results.value == nineCode)
    {
      
    }
    else
    {
      //goStop();
    }
    irrecv.resume(); // Receive the next value
  }
}

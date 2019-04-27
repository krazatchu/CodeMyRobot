# How to Set up on Linux (Tested on Ubuntu 18.04, Fedora 2.9, Mint)

1. Install the ESP32 board files: https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md 
2. Choose the board ESP32 Dev Module
3. Install pyserial globally. (Using the --user tag doesn't work) --> sudo -H pip install pyserial 
4. Give permission to the port by using chmod 666 dev/ttyUSB\*
5. Change the upload speed to 1152000 



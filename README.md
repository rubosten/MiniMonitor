# MiniMonitor
An ESP32 powered 3d printer monitor


There are two parts of making this device work; the klipper software mod and the device itself.

# Installation
Download the "minimonitorklipper" folder from the github and upload it to your device running klipper. The folder should be placed in your home directory. Cd into the folder and run "npm install". After this copy the bash script file into your home directory. Once completed, type in "chmod +x minimonitor.sh" to make it executable. If you want the service to run at startup, enter the following command to your terminal: "crontab -e". In the edit file add this line: "@reboot sh /home/pi/minimonitor.sh". This part is now done.

# Device build
Connect all four pins of your display to corresponding ESP32 pins. 
GND->GND
VCC->3V3
SCL->22
SDA->21

Once this is completed, download the firmware folder and open in platformio. Edit the main.cpp file in the src directory and fill out your WiFi credentials. Then, flash the firmware and everything should be working.
![IMG_3379](https://github.com/rubosten/MiniMonitor/assets/143548699/dd9426f7-20ec-4e40-9a92-5096a54af056)

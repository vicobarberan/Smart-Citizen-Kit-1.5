#include <Wire.h>
#include <SPI.h>
#include <SD.h>


#include "Constants.h"
#include "SCKDrivers.h"
#include "SCKUrban.h"
#include "SCKBase.h"

SCKDriver driver;
SCKUrban urban;

#define USBEnabled      true 
#define sensorEnabled   true
#define debuggEnabled   false

uint32_t timetransmit = 0;  
uint32_t TimeUpdate   = 10;  //Variable temporal de tiempo entre actualizacion y actualizacion de los sensensores
uint32_t NumUpdates   = 0;  //Numero de actualizaciones antes de postear

File myFile;

float SENSORvalue[20];

void setup() {
  urban.begin();
  driver.ESPoff();
  #if debuggEnabled
    SerialUSB.print(F("Initializing SD card..."));
  #endif 
    if (!SD.begin(CS_MMC)) {
  #if debuggEnabled
      SerialUSB.println(F("initialization failed!"));
  #endif 
      return;
    }
  #if debuggEnabled
    SerialUSB.println(F("initialization done."));   
  #endif 
    if (!SD.exists("post.csv")) {
      #if debuggEnabled
        SerialUSB.println(F("Creating post.csv..."));
      #endif 
      SD.open("post.csv", FILE_WRITE);
      myFile.close();
      txHeader();
      
    } else{
      #if debuggEnabled
        SerialUSB.println(F("post.csv exists ..."));
      #endif 
    }
    timetransmit = millis();
}

void loop() {  
#if sensorEnabled  
  if ((millis()-timetransmit) >= (unsigned long)TimeUpdate*1000)
  {  
    SerialUSB.println("*** loop ***");
    timetransmit = millis();
    updateSensorsSD();
    txSD();
    #if USBEnabled
        txDebugSD();
    #endif
  }
#endif  
  urban.management();
}

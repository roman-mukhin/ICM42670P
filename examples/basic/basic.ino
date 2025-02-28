#include <Wire.h>
#include "ICM42670P.h"

#define SENS_VCC_CTRL PIN_PD2
#define PWR_DELAY 100  // [ms]

ICM42670P IMU;

// Software reset
void softwareRST() {
  Serial.flush();
  // - software reset MCU via WDT with min timeout
  _PROTECTED_WRITE(WDT.CTRLA, WDT_PERIOD_8CLK_gc);
  delay(PWR_DELAY);
}

void setup() {

  // - setup UART for debuging
  Serial.begin(115200);
  Serial.println(F("START"));

  // - setup control pins
  pinMode(SENS_VCC_CTRL, OUTPUT);
  digitalWrite(SENS_VCC_CTRL, HIGH);
  delay(PWR_DELAY);

  // setup IMU  
  if (!IMU.begin()) softwareRST();
  delay(PWR_DELAY);
}

void loop() {
  // -- read data
  int16_t G_x = 0, G_y = 0, G_z = 0, w_x = 0, w_y = 0, w_z = 0;
  IMU.getRawData(G_x , G_y , G_z, w_x, w_y , w_z );
 
  Serial.print(G_x);
  Serial.print(",");
  Serial.print(G_y);
  Serial.print(",");
  Serial.print(G_z);
  Serial.print(",");
  Serial.print(w_x);
  Serial.print(",");
  Serial.print(w_y);
  Serial.print(",");
  Serial.println(w_z);

  delay(250);
}

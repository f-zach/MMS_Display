#include <Arduino.h>
#include <MMS_Display.h>

U8G2_ST7920_128X64_F_8080 lcd(U8G2_R0, 5, 6, 7, 8, 9, 10, 11, 12, /*enable=*/4 /* A4 */, /*cs=*/U8X8_PIN_NONE, /*dc/rs=*/2 /* A3 */, /*reset=*/3 /* A1 */); // Remember to set R/W to 0
MMS_Display display(lcd);


void setup() {
  delay(2000);
  Serial.begin(9600);
  display.begin(TILES_2T_1B,1);
}

void loop() {
  for(int i = 0; i <= 6; i++)
  {
    display.begin(i,1);
    delay(3000);
  }  
}
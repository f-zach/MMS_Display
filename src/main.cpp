#include <Arduino.h>
#include <MMS_Display.h>
#include <Encoder.h>

U8G2_ST7920_128X64_F_8080 lcd(U8G2_R0, 4, 5, 6, 7, 8, 9, 10, 11, /*enable=*/A1 /* A4 */, /*cs=*/U8X8_PIN_NONE, /*dc/rs=*/A2 /* A3 */, /*reset=*/12 /* A1 */); // Remember to set R/W to 0
MMS_Display display(lcd);

int encoderButton = A4, controllerOnPin = 13;

Encoder encoder(3,2);
long lastVal = 1;
long value = 0;

int status = 0;
bool controllerOn = 0, controllerOnLast = 0;

bool button = true, buttonLast = true;
long tStart=0;

String message= "";

 char buffer[20] = {};

void setup() {
  delay(2000);
  Serial.begin(115200);
  display.begin(TILES_2T_1B,1);
  pinMode(controllerOnPin,INPUT);
  pinMode(encoderButton,INPUT);
  }

void loop() {

  controllerOn = digitalRead(13);

  if(controllerOn && !controllerOnLast)
  {
    display.setDrawColor(0);
    display.drawBox(0,16,8,86,48);
    display.setDrawColor(1);
    display.drawFrame(0,16,8,86,48);
    display.setFont(u8g2_font_profont10_mr);
    display.drawButtonUTF8(0, 64, 46, U8G2_BTN_SHADOW1|U8G2_BTN_INV|U8G2_BTN_HCENTER|U8G2_BTN_BW1, 30, 2,2,"OK");
    display.setFontMode(0);

    encoder.write(0);
    
    int tLastUpdate = millis();
    long setValue = 0;

    display.setFont(u8g2_font_profont15_mr);
    do
    {  
      controllerOn = digitalRead(controllerOnPin);

      if(millis()-tLastUpdate>=100)
      {
        display.setDrawColor(1);

        setValue = encoder.read();
        if(setValue < 0)
        {
          encoder.write(0);
          setValue = 0;
        }
        else
        {
          setValue *= 125;
        }
        display.drawRightAlignedStr(0,16+10,8 + 20, "       ");
        display.printRightAligned(0, 16+10, 8 + 20, setValue);
        display.sendBuffer();

        tLastUpdate = millis();
      }
    }while(digitalRead(encoderButton)&&controllerOn);

    if(!controllerOn)
    {

    }
    else if(controllerOn)
    {
    display.setFont(u8g2_font_profont10_mr);
    display.setDrawColor(0);
    display.drawBox(0,17,33,66, 22);
    display.setDrawColor(1);
    display.drawButtonUTF8(0, 66, 48, U8G2_BTN_INV|U8G2_BTN_HCENTER|U8G2_BTN_BW1, 30, 2,2,"OK");
    display.setFontMode(0);
    display.sendBuffer();
    delay(250);

    Serial.println(setValue);
    while(Serial.available() <= 0)
    {

    }
    if(Serial.available() > 0)
    {
      message = Serial.readStringUntil('\n');
      message.toCharArray(buffer,20);
    }

    display.setDrawColor(0);
    display.drawBox(0,16,8,86,48);
    display.setDrawColor(1);
    display.drawFrame(0,16,8,86,48);
    display.setFont(u8g2_font_profont15_mr);
    display.drawStr(0, 16 + 5, 8 + 5 + 9, "Set");
    display.drawStr(0, 16 + 5, 8 + 5 + 18 + 4, buffer);

    display.sendBuffer();

    delay(1000);
    }
    
    display.clearDisplay();
  }
  controllerOnLast = controllerOn;
  
  value = encoder.read()/4;

  if(value != lastVal)
  {
    
    display.setDrawColor(1);
    display.setFont(u8g2_font_profont15_mr);
    display.drawStr(1,3,16,"       ");
    
    display.setCursor(1, 3, 16);
    display.print(value);
    lastVal = value;
  }
  
  button = digitalRead(A4);


  if((button != buttonLast) && !button)
  {
    display.setDrawColor(2);

    display.drawBox(2,1,20,8,8);

    Serial.println(value);

    encoder.write(0);

    display.setDrawColor(1);   
  }
  buttonLast = button;

  if(Serial.available() > 0)
  {
    while (Serial.available() > 0)
    {
      message = Serial.readStringUntil('\n');
      message.toCharArray(buffer, 20);
      display.drawStr(2,3,16,buffer);
    }
    
    do{
      Serial.read();
    }while(Serial.available());
  }

  if(millis()-tStart >=100)
  {
  display.setDrawColor(1);
  display.drawTiles();
  display.sendBuffer(); 
  tStart = millis();
  } 
}
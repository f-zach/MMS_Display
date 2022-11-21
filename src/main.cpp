#include <Arduino.h>
#include <MMS_Display.h>
#include <Encoder.h>
#include <Button.h>

U8G2_ST7920_128X64_F_8080 lcd(U8G2_R0, 4, 5, 6, 7, 8, 9, 10, 11, /*enable=*/A1 /* A4 */, /*cs=*/U8X8_PIN_NONE, /*dc/rs=*/A2 /* A3 */, /*reset=*/12 /* A1 */); // Remember to set R/W to 0
MMS_Display display(lcd);
Button encoderButton(A4);

void tile1();
void tile2();
void tile3();
void tile4();
void dialogueWindow();

int controllerOnPin = 13;

Encoder encoder(3, 2);
long encoderValueLast = 1;
long encoderValue = 0, brakeCounter = 0;
long lastUpdate = 0;

int mode = 0, state = 0;
bool controllerOn = 0, controllerOnLast = 0, confirm = false, blinkToggle = true;

bool button = true, buttonLast = true, dialogueChoice = false;
long tStart = 0, loopTime = 0, blinkTime = 0;

char incomingChar;

float brakeSetting;
float throttleSetting;
float power;
unsigned long rpm;
unsigned long setRPM;
int potValue;
int setPotValue;
int windowMessage;

void setup()
{
  Serial.begin(115200);
  display.begin(TILES_2L_2R, 1);
  encoderButton.begin();
  pinMode(A4, INPUT);
  pinMode(controllerOnPin, INPUT);
}

void loop()
{
  tStart = millis();

  controllerOn = digitalRead(controllerOnPin);

  encoderValue = encoder.read();

  if (Serial.available() > 0)
  {
    delay(1);

    do
    {
      incomingChar = Serial.read();
      // incomingString += incomingChar;
    } while (incomingChar != '*');

    do
    {
      incomingChar = Serial.read();
      switch (incomingChar)
      {
      case 'b':
        brakeSetting = Serial.readStringUntil(';').toFloat();
        break;
      case 't':
        throttleSetting = Serial.readStringUntil(';').toFloat();
        break;
      case 'p':
        power = Serial.readStringUntil(';').toFloat();
        break;
      case 'r':
        rpm = Serial.readStringUntil(';').toFloat();
        break;
      case 's':
        setRPM = Serial.readStringUntil(';').toFloat();
        break;
      case 'm':
        mode = Serial.readStringUntil(';').toFloat();
        break;
      case 'v':
        potValue = Serial.readStringUntil(';').toInt();
        break;
      case 'n':
        windowMessage = Serial.readStringUntil(';').toInt();
        break;
      case 'a':
        setPotValue = Serial.readStringUntil(';').toInt();
        break;
      case 'o':
        brakeCounter = Serial.readStringUntil(';').toInt();
        break;

      default:
        break;
      }

      if (encoderButton.pressed())
      {
        switch (mode)
        {
        case 0:
          break;

        case 1:
          Serial.print('*');
          Serial.print(encoderValue * 250);

          break;

        case 2:
          if (confirm)
          {
            Serial.print('#');
          }
          else if (!confirm)
          {
            Serial.print('-');
          }
          break;

        case 3:

          Serial.print('+');

          break;

        default:
          break;
        }

        encoder.write(0);
      }

      // incomingString += incomingChar;
    } while (incomingChar != '#');

    // incomingString.toCharArray(buffer,50);
  }

  if (millis() - lastUpdate >= 100)
  {
    noInterrupts();
    display.setDrawColor(0);
    display.drawBox(0, 0, 0, 128, 64);
    display.setDrawColor(1);

    tile1();
    tile2();
    tile3();
    tile4();

    display.drawTiles();

    dialogueWindow();

    display.sendBuffer();
    interrupts();

    lastUpdate = millis();
  }

  encoderValueLast = encoderValue;
  controllerOnLast = controllerOn;

  if (millis() - blinkTime >= 500)
  {
    blinkTime = millis();
    blinkToggle = !blinkToggle;
  }
}

void tile1()
{

  display.clearTile(1);
  display.setFont(u8g2_font_profont12_mr);
  display.setCursor(1, 3, 11);
  display.print(F("Power"));
  display.setFont(u8g2_font_profont17_mn);
  if (power >= 10.0)
  {
    display.setCursor(1, 3, 26);
  }
  else if (power < 10)
  {
    display.setCursor(1, 12, 26);
  }
  display.print(power);
  display.setFont(u8g2_font_profont12_mr);
  display.setCursor(1, 3 + 45 + 1, 26);
  display.print(F("kW"));
}

void tile2()
{
  display.clearTile(2);

  display.setFont(u8g2_font_profont12_mr);
  display.setCursor(2, 3, 11);
  display.print(F("RPM"));

  display.setFont(u8g2_font_profont17_mn);
  display.setCursor(2, 3, 26);
  display.print(rpm);
}

void tile3()
{
  display.clearTile(3);

  switch (mode)
  {
  case 0:
    display.setFont(u8g2_font_profont17_mr);
    display.setCursor(3, 5, 21);
    display.print(F("MANUAL"));
    break;

  case 1:
    display.setFont(u8g2_font_profont17_mr);
    display.setCursor(3, 18, 21);
    display.print(F("SET"));
    break;

  case 2:
    display.setFont(u8g2_font_profont17_mr);
    display.setCursor(3, 18, 21);
    display.print(F("SET"));
    break;

  case 3:
    display.setFont(u8g2_font_profont17_mr);
    display.setCursor(3, 14, 21);
    display.print(F("AUTO"));
    break;

  case 4:
    display.setFont(u8g2_font_profont17_mr);

    if (blinkToggle)
    {
      switch (windowMessage)
      {
      case 0:
        display.setCursor(3, 18, 13);
        display.print(F("THR"));
        display.setCursor(3, 22, 28);
        display.print(F("UP"));
        break;

      case 1:
        display.setCursor(3, 18, 13);
        display.print(F("THR"));
        display.setCursor(3, 13, 28);
        display.print(F("DOWN"));
        break;

      case 2:
        display.setCursor(3, 18, 13);
        display.print(F("BRK"));
        display.setCursor(3, 22, 28);
        display.print(F("UP"));
        break;

      case 3:
        display.setCursor(3, 18, 13);
        display.print(F("BRK"));
        display.setCursor(3, 13, 28);
        display.print(F("DOWN"));
        break;

      default:
        break;
      }
    }
    else if (!blinkToggle)
    {
      display.clearTile(3);
    }
    break;

  case 5:
    if(blinkToggle)
    {
    display.setDrawColor(0);
    display.drawBox(3, 0, 0, 62, 30);
    display.setDrawColor(1);
    display.setFont(u8g2_font_profont17_mr);
    display.setCursor(3, 8, 21);
    display.print(F("WAIT!"));
    }
    else if(!blinkToggle)
    {
      display.clearTile(3);
    }
    break;

  case 6:
    display.setFont(u8g2_font_profont17_mr);

    switch (windowMessage)
    {
    case 0:
      display.setCursor(3, 2, 13);
      display.print(F("MAX"));
      display.setCursor(3, 35, 13);
      display.print(F("THR"));
      display.setCursor(3, 18, 28);
      display.print(F("SET"));
      break;

    case 1:
      display.setCursor(3, 2, 13);
      display.print(F("MIN"));
      display.setCursor(3, 35, 13);
      display.print(F("THR"));
      display.setCursor(3, 22, 28);
      display.print(F("SET"));
      break;

    case 2:
      display.setCursor(3, 2, 13);
      display.print(F("MAX"));
      display.setCursor(3, 35, 13);
      display.print(F("BRK"));
      display.setCursor(3, 22, 28);
      display.print(F("SET"));
      break;

    case 3:
      display.setCursor(3, 2, 13);
      display.print(F("MIN"));
      display.setCursor(3, 35, 13);
      display.print(F("BRK"));
      display.setCursor(3, 22, 28);
      display.print(F("SET"));
      break;

    default:
      break;
    }
    break;

  default:
    break;
  }
}

void tile4()
{
  switch (mode)
  {
  case 3:
    display.clearTile(4);
    display.setFont(u8g2_font_profont12_mr);
    display.setCursor(4, 3, 12);
    display.print(F("Set RPM:"));
    display.setCursor(4, 3, 24);
    display.print(setRPM);
    break;

    // case 4:
    //   display.clearTile(4);
    //   if (blinkToggle)
    //   {
    //     display.lcd.drawGlyph(32+16+8,64+24, 0x23f6)
    //   }
    //   break;

  default:
    throttleSetting *= 100;
    brakeSetting *= 100;

    display.drawBar(4, -1, -1, 64, 16, throttleSetting, 100, 2, 0);
    display.drawBar(4, -1, 15, 64, 16, brakeSetting, 100, 2, 0);

    display.setFont(u8g2_font_profont11_mr);
    display.setDrawColor(2);
    display.setFontMode(1);
    display.setCursor(4, 2, 11);
    display.print(F("THR:"));
    display.setCursor(4, 2, 27);
    //display.print(F("B:"));
    display.setCursor(4, 54, 11);
    display.print(F("%"));
    // display.setCursor(4, 54, 27);
    // display.print(F("%"));
    display.setCursor(4, 26, 11);
    if(throttleSetting >= 100)
    {
      display.print(100);
    }
    else
    {
      display.lcd.print(throttleSetting,1);
    }
    
    if(brakeCounter < 10)
    {
      display.setCursor(4, 55, 27);
    }
    else if(brakeCounter < 100)
    {
      display.setCursor(4, 55-6, 27);
    }
    else if(brakeCounter < 1000)
    {
      display.setCursor(4, 55-2*6, 27);
    }
    else if(brakeCounter >= 1000)
    {
      display.setCursor(4, 55-3*6, 27);
    }
    display.lcd.print(brakeCounter);

    display.setCursor(4,2,27);
    display.lcd.print(brakeSetting,1);
    display.setCursor(4,31,27);
    display.print(F("/"));


    

    display.setDrawColor(1);
    display.setFontMode(0);
    break;
  }
}

void dialogueWindow()
{
  switch (mode)
  {
  case 1:
    display.setDrawColor(0);
    display.drawBox(0, 16, 8, 96, 48);
    display.setDrawColor(1);
    display.drawFrame(0, 16, 8, 96, 48);
    display.setFont(u8g2_font_profont12_mr);
    display.setCursor(0, 20, 20);
    display.print(F("Set RPM to:"));
    display.setFont(u8g2_font_profont17_mn);
    display.setCursor(0, 30, 35);
    
    if (encoderValue < 0)
    {
      encoderValue = 0;
      encoder.write(0);
    }

    display.print(encoderValue * 250);
    break;

  case 2:
    display.setDrawColor(0);
    display.drawBox(0, 16, 8, 96, 48);
    display.setDrawColor(1);
    display.drawFrame(0, 16, 8, 96, 48);
    display.setFont(u8g2_font_profont12_mr);
    display.setCursor(0, 20, 20);
    display.print(F("Confirm RPM:"));
    display.setFont(u8g2_font_profont17_mn);
    display.setCursor(0, 30, 35);
    display.print(setRPM);

    if (encoderValue < 0)
    {
      confirm = true;
      encoder.write(0);
    }
    else if (encoderValue > 0)
    {
      confirm = false;
      encoder.write(0);
    }

    display.setFont(u8g2_font_m2icon_9_tf);
    display.setDrawColor(confirm);
    display.drawBox(0, 47, 44, 12, 9);
    display.setDrawColor(!confirm);
    display.lcd.drawGlyph(48, 52, 68);
    display.setDrawColor(!confirm);
    display.drawBox(0, 69, 44, 12, 9);
    display.setDrawColor(confirm);
    display.lcd.drawGlyph(72, 52, 67);
    display.setDrawColor(1);
    break;

  default:
    break;
  }
}
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
long encoderValue = 0;
long lastUpdate = 0;

int mode = 0, state = 0;
bool controllerOn = 0, controllerOnLast = 0, confirm = false;

bool button = true, buttonLast = true, dialogueChoice = false;
long tStart = 0, loopTime = 0;

char incomingChar;

float brakeSetting;
float throttleSetting;
float power;
unsigned long rpm;
unsigned long setRPM;

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

  if (millis() - lastUpdate >= 200)
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
}

void tile1()
{

  display.clearTile(1);
  display.setFont(u8g2_font_profont12_mr);
  display.setCursor(1, 3, 11);
  display.print(F("Power"));
  display.setFont(u8g2_font_profont17_mn);
  display.setCursor(1, 3, 26);
  display.print(power);
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
    display.drawRFrame(3, 21, 11, 20, 8, 3);
    break;

  case 1:
    display.drawRFrame(3, 21, 11, 20, 8, 3);
    break;

  case 2:
    display.drawRFrame(3, 21, 11, 20, 8, 3);
    break;

  case 3:
    display.drawRBox(3, 21, 11, 20, 8, 3);
    break;

  default:
    break;
  }
}

void tile4()
{
  if (mode < 3)
  {
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
    display.print(F("BRK:"));
    display.setCursor(4, 54, 11);
    display.print(F("%"));
    display.setCursor(4, 54, 27);
    display.print(F("%"));
    display.setCursor(4, 35, 11);
    display.print(int(throttleSetting));
    display.setCursor(4, 35, 27);
    display.print(int(brakeSetting));

    display.setDrawColor(1);
    display.setFontMode(0);
  }
  else if (mode == 3)
  {
    display.clearTile(4);
    display.setFont(u8g2_font_profont12_mr);
    display.setCursor(4, 3, 12);
    display.print(F("Set RPM:"));
    display.setCursor(4, 3, 24);
    display.print(setRPM);
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
    display.drawBox(0,47,44,12,9);
    display.setDrawColor(!confirm);
    display.lcd.drawGlyph(48, 52, 68);
    display.setDrawColor(!confirm);
    display.drawBox(0,69,44,12,9);
    display.setDrawColor(confirm);
    display.lcd.drawGlyph(72, 52, 67);
    display.setDrawColor(1);
    break;

  default:
    break;
  }
}
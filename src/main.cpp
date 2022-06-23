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

int mode = 0;
bool controllerOn = 0, controllerOnLast = 0;

bool button = true, buttonLast = true, dialogueChoice = false;
long tStart = 0, loopTime = 0;

String message = "";
String incomingString;

char buffer[6] = {};
char incomingChar;

String brakeSettingIn;
String throttleSettingIn;
String powerIn;
String rpmIn;
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
          Serial.print(encoderValue*125);
          
          break;

        case 2:
          Serial.print('#');
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
  

  controllerOnLast = controllerOn;
}

void tile1()
{
  display.setFont(u8g2_font_profont12_mr);
  display.setCursor(1, 3, 18);
  display.print(mode);
}

void tile2()
{
  display.setFont(u8g2_font_profont12_mr);
  display.setCursor(2, 3, 18);
  display.print(millis() - tStart);
}

void tile3()
{
  display.setFont(u8g2_font_profont12_mr);
  display.setCursor(3, 3, 18);
  display.print(power);
}

void tile4()
{
  if (!digitalRead(controllerOnPin))
  {
    throttleSetting *= 100;
    brakeSetting *= 100;

    display.drawBar(4, -1, -1, 64, 16, throttleSetting, 100, 2, 0);
    display.drawBar(4, -1, 15, 64, 16, brakeSetting, 100, 2, 0);

    display.setFont(u8g2_font_t0_11_tr);
    display.setDrawColor(2);
    display.setFontMode(1);
    display.drawStr(4, 2, 11, "THR:");
    display.drawStr(4, 2, 27, "BRK:");
    display.drawStr(4, 54, 11, "%");
    display.drawStr(4, 54, 27, "%");
    display.setCursor(4, 35, 11);
    display.print(int(throttleSetting));
    display.setCursor(4, 35, 27);
    display.print(int(brakeSetting));

    display.setDrawColor(1);
    display.setFontMode(0);
  }
  else if (digitalRead(controllerOnPin))
  {
    display.clearTile(4);
    display.setFont(u8g2_font_profont12_mr);
    display.drawStr(4, 3, 12, "Set RPM:");
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
    display.setCursor(0, 19, 20);
    display.print(encoderValue * 125);
    break;

  case 2:
    display.setDrawColor(0);
    display.drawBox(0, 16, 8, 96, 48);
    display.setDrawColor(1);
    display.drawFrame(0, 16, 8, 96, 48);
    display.setFont(u8g2_font_profont12_mr);
    display.setCursor(0, 19, 30);
    display.print(setRPM);
    break;

  default:
    break;
  }
}
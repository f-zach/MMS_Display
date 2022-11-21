#include <MMS_Display.h>

MMS_Display::MMS_Display(U8G2 &lcdObj)
{
    lcd = lcdObj;
}

void MMS_Display::begin(uint8_t layout, uint8_t frameThickness)
{
    lcd.begin();

    createTiles(layout, frameThickness);
    drawTiles();

    lcd.sendBuffer();
}

void MMS_Display::createTiles(uint8_t layout, uint8_t frameThickness)
{
    _frameThickness = frameThickness;
    _displayWidth = lcd.getDisplayWidth();
    _displayHeight = lcd.getDisplayHeight();

    tilesX[0] = 0;
    tilesY[0] = 0;
    _tileWidth[0] = _displayWidth;

    switch (layout)
    {
    case 0:
        // 2 Left, 2 Right
        tilesX[1] = 0 + frameThickness;
        tilesY[1] = 0 + frameThickness;
        tilesX[2] = _displayWidth / 2 + frameThickness;
        tilesY[2] = 0 + frameThickness;
        tilesX[3] = 0 + frameThickness;
        tilesY[3] = _displayHeight / 2 + frameThickness;
        tilesX[4] = _displayWidth / 2 + frameThickness;
        tilesY[4] = _displayHeight / 2 + frameThickness;

        _tileWidth[1] = tilesX[4] - tilesX[1] - 1;
        _tileWidth[2] = _displayWidth - tilesX[2] - 1;
        _tileWidth[3] = tilesX[4] - tilesX[3] - 1;
        _tileWidth[4] = _displayWidth - tilesX[4] - 1;

        _framelineH[0] = 0;
        _framelineH[1] = _displayHeight / 2;
        _framelineH[2] = _displayWidth;
        _framelineV[0] = _displayWidth / 2;
        _framelineV[1] = 0;
        _framelineV[2] = _displayHeight;

        break;

    case 1:
        // 2 Left, 1 Right
        tilesX[1] = 0 + frameThickness;
        tilesY[1] = 0 + frameThickness;
        tilesX[2] = 0 + frameThickness;
        tilesY[2] = _displayHeight / 2 + frameThickness;
        tilesX[3] = _displayWidth / 2 + frameThickness;
        tilesY[3] = 0 + frameThickness;

        _tileWidth[1] = tilesX[3] - tilesX[1] - 1;
        _tileWidth[2] = tilesX[3] - tilesX[2] - 1;
        _tileWidth[3] = _displayWidth - tilesX[2] - 1;

        _framelineH[0] = 0;
        _framelineH[1] = _displayHeight / 2;
        _framelineH[2] = _displayWidth / 2;
        _framelineV[0] = _displayWidth / 2;
        _framelineV[1] = 0;
        _framelineV[2] = _displayHeight;
        break;

    case 2:
        // 1 Left, 2 Right
        tilesX[1] = 0 + frameThickness;
        tilesY[1] = 0 + frameThickness;
        tilesX[2] = _displayWidth / 2 + frameThickness;
        tilesY[2] = 0 + frameThickness;
        tilesX[3] = _displayWidth / 2 + frameThickness;
        tilesY[3] = _displayHeight / 2 + frameThickness;

        _tileWidth[1] = tilesX[3] - tilesX[1] - 1;
        _tileWidth[2] = _displayWidth - tilesX[2] - 1;
        _tileWidth[3] = _displayWidth - tilesX[2] - 1;

        _framelineH[0] = _displayWidth / 2;
        _framelineH[1] = _displayHeight / 2;
        _framelineH[2] = _displayWidth / 2;
        _framelineV[0] = _displayWidth / 2;
        _framelineV[1] = 0;
        _framelineV[2] = _displayHeight;
        break;

    case 3:
        // 1 Left, 1 Right
        tilesX[1] = 0 + frameThickness;
        tilesY[1] = 0 + frameThickness;
        tilesX[2] = _displayWidth / 2 + frameThickness;
        tilesY[2] = 0 + frameThickness;

        _tileWidth[1] = tilesX[2] - tilesX[1] - 1;
        _tileWidth[2] = _displayWidth - tilesX[2] - 1;

        _framelineH[0] = 0;
        _framelineH[1] = 0;
        _framelineH[2] = 0;
        _framelineV[0] = _displayWidth / 2;
        _framelineV[1] = 0;
        _framelineV[2] = _displayHeight;
        break;

    case 4:
        // 2 Top, 1 Bottom
        tilesX[1] = 0 + frameThickness;
        tilesY[1] = 0 + frameThickness;
        tilesX[2] = _displayWidth / 2 + frameThickness;
        tilesY[2] = 0 + frameThickness;
        tilesX[3] = 0 + frameThickness;
        tilesY[3] = _displayHeight / 2 + frameThickness;

        _tileWidth[1] = tilesX[2] - tilesX[1] - 1;
        _tileWidth[2] = _displayWidth - tilesX[2] - 1;
        _tileWidth[3] = _displayWidth - tilesX[3] - 1;

        _framelineH[0] = 0;
        _framelineH[1] = _displayHeight / 2;
        _framelineH[2] = _displayWidth;
        _framelineV[0] = _displayWidth / 2;
        _framelineV[1] = 0;
        _framelineV[2] = _displayHeight / 2;
        break;

    case 5:
        // 1 Top, 2 Bottom
        tilesX[1] = 0 + frameThickness;
        tilesY[1] = 0 + frameThickness;
        tilesX[2] = 0 + frameThickness;
        tilesY[2] = _displayHeight / 2 + frameThickness;
        tilesX[3] = _displayWidth / 2 + frameThickness;
        tilesY[3] = _displayHeight / 2 + frameThickness;

        _tileWidth[1] = _displayWidth - tilesX[1] - 1;
        _tileWidth[2] = tilesX[3] - tilesX[2] - 1;
        _tileWidth[3] = _displayWidth - tilesX[2] - 1;

        _framelineH[0] = 0;
        _framelineH[1] = _displayHeight / 2;
        _framelineH[2] = _displayWidth;
        _framelineV[0] = _displayWidth / 2;
        _framelineV[1] = _displayHeight / 2;
        _framelineV[2] = _displayHeight / 2;
        break;

    case 6:
        // 1 Top, 1 Bottom
        tilesX[1] = 0 + frameThickness;
        tilesY[1] = 0 + frameThickness;
        tilesX[2] = 0 + frameThickness;
        tilesY[2] = _displayHeight / 2 + frameThickness;

        _tileWidth[1] = _displayWidth - tilesX[1] - 1;
        _tileWidth[2] = _displayWidth - tilesX[2] - 1;

        _framelineH[0] = 0;
        _framelineH[1] = _displayHeight / 2;
        _framelineH[2] = _displayWidth;
        _framelineV[0] = 0;
        _framelineV[1] = 0;
        _framelineV[2] = 0;
        break;

    case 7:
        // 3 Horizontal
        tilesX[0] = 0 + frameThickness;
        tilesY[0] = 0 + frameThickness;
        tilesX[1] = _displayWidth / 2 + frameThickness;
        tilesY[1] = 0 + frameThickness;
        tilesX[2] = 0 + frameThickness;
        tilesY[2] = 0 + frameThickness;
        break;

    case 8:
        // 4 Horizontal

        break;

    case 9:
        // Just outer Frame
        tilesX[1] = 0 + frameThickness;
        tilesY[1] = 0 + frameThickness;

        _tileWidth[1] = _displayWidth - 2;

        _framelineH[0] = 0;
        _framelineH[1] = 0;
        _framelineH[2] = 0;
        _framelineV[0] = 0;
        _framelineV[1] = 0;
        _framelineV[2] = 0;
        break;

    case 10:
        // No Frames
        _frameThickness = 0;
        tilesX[1] = 0;
        tilesY[1] = 0;

        _tileWidth[1] = _displayWidth;

        _framelineH[0] = 0;
        _framelineH[1] = 0;
        _framelineH[2] = 0;
        _framelineV[0] = 0;
        _framelineV[1] = 0;
        _framelineV[2] = 0;
        break;

    default:
        break;
    }
}

void MMS_Display::drawTiles()
{
    for (int i = 0; i < _frameThickness; i++)
    {
        lcd.drawFrame(i, i, _displayWidth - 2 * i, _displayHeight - 2 * i);
        lcd.drawHLine(_framelineH[0] - i, _framelineH[1] - i, _framelineH[2]);
        lcd.drawVLine(_framelineV[0] - i, _framelineV[1] - i, _framelineV[2]);
    }
}

void MMS_Display::clearTile(uint8_t targetTile)
{
    setDrawColor(0);
    drawBox(targetTile, 0, 0, _tileWidth[targetTile], 30);
    setDrawColor(1);
}


void MMS_Display::sendBuffer()
{
    lcd.sendBuffer();
}

void MMS_Display::clearDisplay()
{
    lcd.clearDisplay();
}

void MMS_Display::drawStr(uint8_t targetTile, uint8_t x, uint8_t y, const char *str)
{
    lcd.drawStr(tilesX[targetTile] + x, tilesY[targetTile] + y, str);
}

void MMS_Display::drawBox(uint8_t targetTile, uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
    lcd.drawBox(tilesX[targetTile] + x, tilesY[targetTile] + y, w, h);
}

void MMS_Display::drawFrame(uint8_t targetTile, uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
    lcd.drawFrame(tilesX[targetTile] + x, tilesY[targetTile] + y, w, h);
}

void MMS_Display::drawRBox(uint8_t targetTile, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r)
{
    lcd.drawRBox(tilesX[targetTile] + x, tilesY[targetTile] + y, w, h, r);
}

void MMS_Display::drawRFrame(uint8_t targetTile, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r)
{
    lcd.drawRFrame(tilesX[targetTile] + x, tilesY[targetTile] + y, w, h, r);
}

void MMS_Display::drawDisc(uint8_t targetTile, uint8_t x, uint8_t y, uint8_t r)
{
    lcd.drawDisc(tilesX[targetTile] + x, tilesY[targetTile] + y, r);
}

void MMS_Display::drawCircle(uint8_t targetTile, uint8_t x, uint8_t y, uint8_t r)
{
    lcd.drawCircle(tilesX[targetTile] + x, tilesY[targetTile] + y, r);
}

void MMS_Display::drawTriangle(uint8_t targetTile, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    lcd.drawTriangle(tilesX[targetTile] + x0, tilesY[targetTile] + y0, tilesX[targetTile] + x1, tilesY[targetTile] + y1, tilesX[targetTile] + x2, tilesY[targetTile] + y2);
}

void MMS_Display::drawPixel(uint8_t targetTile, uint8_t x, uint8_t y)
{
    lcd.drawPixel(tilesX[targetTile] + x, tilesY[targetTile] + y);
}

void MMS_Display::drawButtonUTF8(uint8_t targetTile, uint8_t x, uint8_t y, uint8_t flags, uint8_t width, uint8_t padding_h, uint8_t padding_v, const char* text)
{
    lcd.drawButtonUTF8(tilesX[targetTile] + x, tilesY[targetTile] + y, flags, width, padding_h, padding_v, text);
}

void MMS_Display::setCursor(uint8_t targetTile, uint8_t x, uint8_t y)
{
    lcd.setCursor(tilesX[targetTile] + x, tilesY[targetTile] + y);
}

void MMS_Display::setFont(const uint8_t *font)
{
    lcd.setFont(font);
}

void MMS_Display::setFontMode(uint8_t is_transparent)
{
    lcd.setFontMode(is_transparent);
}

void MMS_Display::setDrawColor(uint8_t color)
{
    lcd.setDrawColor(color);
}


void MMS_Display::drawRightAlignedStr(uint8_t targetTile, uint8_t xOffset, uint8_t y, const char *str)
{
    uint8_t x = tilesX[targetTile] + _tileWidth[targetTile] - xOffset - 1;

    drawStr(targetTile, x - lcd.getStrWidth(str), y, str);
}

void MMS_Display::printRightAligned(uint8_t targetTile, uint8_t xOffset, uint8_t y, int val)
{
    char buffer[20];
    itoa(val, buffer, 10);
    drawRightAlignedStr(targetTile, xOffset, y, buffer);
}

void MMS_Display::printRightAligned(uint8_t targetTile, uint8_t xOffset, uint8_t y, long val)
{
    char buffer[20];
    ltoa(val, buffer, 10);
    drawRightAlignedStr(targetTile, xOffset, y, buffer);
}

void MMS_Display::printRightAligned(uint8_t targetTile, uint8_t xOffset, uint8_t y, float val)
{
    char buffer[20];
    dtostrf(val, 3, 2, buffer);
    drawRightAlignedStr(targetTile, xOffset, y, buffer);
}

void MMS_Display::printRightAligned(uint8_t targetTile, uint8_t xOffset, uint8_t y, double val)
{
    char buffer[20];
    dtostrf(val, 3, 2, buffer);
    drawRightAlignedStr(targetTile, xOffset, y, buffer);
}

void MMS_Display::drawBar(uint8_t targetTile, uint8_t x, uint8_t y, uint8_t w, uint8_t h, double val, double valMax, uint8_t borderType, uint8_t shading)
{
    setDrawColor(0);
    drawBox(targetTile, x, y, w, h);
    setDrawColor(1);

    if (borderType > 0)
    {
        drawFrame(targetTile, x, y, w, h);
    }

    uint8_t barWidth = round(float(w-2*borderType) * val / valMax);
    

    if (borderType == 0)
    {
        if (!shading)
        {
            drawBox(targetTile, x, y, barWidth, h);
        }
        else if (shading)
        {
            for (int i = 0; i <= barWidth; i++)
            {
                for (int j = 0; j <= h; j++)
                {
                    if (!(i % 2 == j % 2))
                    {
                        drawPixel(targetTile, i + x, j + y);
                    }
                }
            }
        }
    }
    else if (borderType == 1)
    {
        if (!shading)
        {
            drawBox(targetTile, x + 1, y + 1, barWidth, h - 2);
        }
        else if (shading)
        {
            for (int i = 0; i <= barWidth; i++)
            {
                for (int j = 0; j <= h - 2; j++)
                {
                    if (!(i % 2 == j % 2))
                    {
                        drawPixel(targetTile, i + x + 1, j + y + 1);
                    }
                }
            }
        }
    }
    else if (borderType == 2)
    {
        if (!shading)
        {
            drawBox(targetTile, x + 2, y + 2, barWidth, h - 4);
        }
        else if (shading)
        {
            for (int i = 0; i <= barWidth - 1; i++)
            {
                for (int j = 0; j <= h - 5; j++)
                {
                    if (!(i % 2 == j % 2))
                    {
                        drawPixel(targetTile, i + x + 2, j + y + 2);
                    }
                }
            }
        }
    }
}

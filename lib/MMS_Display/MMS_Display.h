#include <Arduino.h>
#include <u8g2lib.h>

#define TILES_2L_2R 0
#define TILES_2L_1R 1
#define TILES_1L_2R 2
#define TILES_1L_1R 3
#define TILES_2T_1B 4
#define TILES_1T_2B 5
#define TILES_1T_1B 6
#define TILES_3HOR  7
#define TILES_4HOR  8
#define TILES_FRAME 9
#define TILES_NONE 10



class MMS_Display
{
private:
    U8G2 lcd;
    uint8_t _displayWidth = 0;
    uint8_t _displayHeight = 0;
    uint8_t _tileWidth[4] = {};
    uint8_t _frameThickness = 0;
    uint8_t _layout = 0;
    uint8_t _framelineH[3] = {};
    uint8_t _framelineV[3] = {};
    
public:
    MMS_Display(U8G2& lcdObj);
    void begin(uint8_t layout, uint8_t frameThickness);
    void createTiles(uint8_t layout, uint8_t frameThickness);
    void drawTiles();

    void drawString(uint8_t targetTile,uint8_t x, uint8_t y, const char* str);
    void drawBox(uint8_t targetTile,uint8_t x, uint8_t y, uint8_t w, uint8_t h);
    void drawFrame(uint8_t targetTile,uint8_t x, uint8_t y, uint8_t w, uint8_t h);
    void drawRBox(uint8_t targetTile,uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r);
    void drawRFrame(uint8_t targetTile,uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r);
    void drawDisc(uint8_t targetTile,uint8_t x, uint8_t y,uint8_t r);
    void drawCircle(uint8_t targetTile,uint8_t x, uint8_t y, uint8_t r);
    void setCursor(uint8_t targetTile, uint8_t x, uint8_t y);
    void setFont(const uint8_t * font);
    void setDrawColor(uint8_t color);
    void drawRightAlignedStr(uint8_t targetTile, uint8_t xOffset, uint8_t y, const char* str);    

    template <typename T>
    void print(T val);

    template <typename T>
    void printRightAligned(uint8_t targetTile, uint8_t xOffset, uint8_t y, T val);


    uint8_t tilesX[4] = {};
    uint8_t tilesY[4] = {};
    U8G2 _lcd;

};

MMS_Display::MMS_Display(U8G2& lcdObj)
{
    lcd = lcdObj;
}

void MMS_Display::begin(uint8_t layout, uint8_t frameThickness)
{
    lcd.begin();
    createTiles(layout,frameThickness);
    drawTiles();
    

    lcd.setFontDirection(0);
    lcd.setFont(u8g2_font_profont12_mr);

    setCursor(0,3,11);
    print(111);
    drawBox(0,0,0,3,3);

    setCursor(1,3,11);
    print(222);
    drawBox(1,0,0,3,3);

    setCursor(2,3,11);
    print(333);
    drawBox(2,0,0,3,3);

    lcd.sendBuffer();


}

void MMS_Display::createTiles(uint8_t layout, uint8_t frameThickness)
{
    _frameThickness = frameThickness;
    _displayWidth = lcd.getDisplayWidth();
    _displayHeight = lcd.getDisplayHeight();

    switch (layout)
    {
    case 0:
    // 2 Left, 2 Right
        tilesX[0] = 0 + frameThickness; tilesY[0] = 0 + frameThickness;
        tilesX[1] = _displayWidth/2 + frameThickness; tilesY[1] = 0 + frameThickness;
        tilesX[2] = 0 + frameThickness; tilesY[2] = _displayHeight/2 + frameThickness;
        tilesX[3] = _displayWidth/2 + frameThickness; tilesY[3] = _displayHeight/2 + frameThickness;

        _tileWidth[0] = tilesX[1] - tilesX[0] - 2;
        _tileWidth[1] = _displayWidth - tilesX[1] - 2;
        _tileWidth[2] = tilesX[3] - tilesX[2] - 2;
        _tileWidth[3] = _displayWidth - tilesX[3] - 2;

        _framelineH[0] = 0; _framelineH[1] = _displayHeight/2; _framelineH[2] = _displayWidth;
        _framelineV[0] = _displayWidth/2; _framelineV[1] = 0; _framelineV[2] = _displayHeight;

        break;

    case 1:
    // 2 Left, 1 Right
        tilesX[0] = 0 + frameThickness; tilesY[0] = 0 + frameThickness;
        tilesX[1] = 0 + frameThickness; tilesY[1] = _displayHeight/2 + frameThickness;
        tilesX[2] = _displayWidth/2 + frameThickness; tilesY[2] = 0 + frameThickness;

        _tileWidth[0] = tilesX[2] - tilesX[0] - 2;
        _tileWidth[1] = tilesX[2] - tilesX[1] - 2;
        _tileWidth[2] = _displayWidth - tilesX[1] - 2;


        _framelineH[0] = 0; _framelineH[1] = _displayHeight/2; _framelineH[2] = _displayWidth/2;
        _framelineV[0] = _displayWidth/2; _framelineV[1] = 0; _framelineV[2] = _displayHeight;
        break;

    case 2:
    // 1 Left, 2 Right
        tilesX[0] = 0 + frameThickness; tilesY[0] = 0 + frameThickness;
        tilesX[1] = _displayWidth/2 + frameThickness; tilesY[1] = 0 + frameThickness;
        tilesX[2] = _displayWidth/2 + frameThickness; tilesY[2] = _displayHeight/2 + frameThickness;

        _tileWidth[0] = tilesX[1] - tilesX[0] - 2;
        _tileWidth[1] = _displayWidth - tilesX[1] - 2;
        _tileWidth[2] = _displayWidth - tilesX[1] - 2;

        _framelineH[0] = _displayWidth/2; _framelineH[1] = _displayHeight/2; _framelineH[2] = _displayWidth/2;
        _framelineV[0] = _displayWidth/2; _framelineV[1] = 0; _framelineV[2] = _displayHeight;
        break;

    case 3:
    // 1 Left, 1 Right
        tilesX[0] = 0 + frameThickness; tilesY[0] = 0 + frameThickness;
        tilesX[1] = _displayWidth/2 + frameThickness; tilesY[1] = 0 + frameThickness;

        _tileWidth[0] = tilesX[1] - tilesX[0] - 2;
        _tileWidth[1] = _displayWidth - tilesX[1] - 2;

        _framelineH[0] = 0; _framelineH[1] = 0; _framelineH[2] = 0;
        _framelineV[0] = _displayWidth/2; _framelineV[1] = 0; _framelineV[2] = _displayHeight;
        break;

    case 4:
    // 2 Top, 1 Bottom
        tilesX[0] = 0 + frameThickness; tilesY[0] = 0 + frameThickness;
        tilesX[1] = _displayWidth/2 + frameThickness; tilesY[1] = 0 + frameThickness;
        tilesX[2] = 0 + frameThickness; tilesY[2] = _displayHeight/2 + frameThickness;

        _tileWidth[0] = tilesX[1] - tilesX[0] - 2;
        _tileWidth[1] = _displayWidth - tilesX[1] - 2;
        _tileWidth[2] = _displayWidth - tilesX[0] - 2;
  

        _framelineH[0] = 0; _framelineH[1] = _displayHeight/2; _framelineH[2] = _displayWidth;
        _framelineV[0] = _displayWidth/2; _framelineV[1] = 0; _framelineV[2] = _displayHeight/2;
        break; 

    case 5:
    // 1 Top, 2 Bottom
        tilesX[0] = 0 + frameThickness; tilesY[0] = 0 + frameThickness;
        tilesX[1] = 0 + frameThickness; tilesY[1] = _displayHeight/2 + frameThickness;
        tilesX[2] = _displayWidth/2 + frameThickness; tilesY[2] = _displayHeight/2 + frameThickness;

        _tileWidth[0] = _displayWidth - tilesX[0] - 2;
        _tileWidth[1] = tilesX[2] - tilesX[1] - 2;
        _tileWidth[2] = _displayWidth - tilesX[1] - 2;

        _framelineH[0] = 0; _framelineH[1] = _displayHeight/2; _framelineH[2] = _displayWidth;
        _framelineV[0] = _displayWidth/2; _framelineV[1] = _displayHeight/2; _framelineV[2] = _displayHeight/2;
        break;

    case 6:
    // 1 Top, 1 Bottom
        tilesX[0] = 0 + frameThickness; tilesY[0] = 0 + frameThickness;
        tilesX[1] = 0 + frameThickness; tilesY[1] = _displayHeight/2 + frameThickness;

        _tileWidth[0] = _displayWidth - tilesX[0] - 2;
        _tileWidth[1] = _displayWidth - tilesX[1] - 2;

        _framelineH[0] = 0; _framelineH[1] = _displayHeight/2; _framelineH[2] = _displayWidth;
        _framelineV[0] = 0; _framelineV[1] = 0; _framelineV[2] = 0;
        break;

    case 7:
    // 3 Horizontal
        tilesX[0] = 0 + frameThickness; tilesY[0] = 0 + frameThickness;
        tilesX[1] = _displayWidth/2 + frameThickness; tilesY[1] = 0 + frameThickness;
        tilesX[2] = 0 + frameThickness; tilesY[2] = 0 + frameThickness;
        break;

    case 8:
    // 4 Horizontal

        break;

    case 9:
    // Just outer Frame
        tilesX[0] = 0 + frameThickness; tilesY[0] = 0 + frameThickness;

        _tileWidth[0] = _displayWidth - 2;

        _framelineH[0] = 0; _framelineH[1] = 0; _framelineH[2] = 0;
        _framelineV[0] = 0; _framelineV[1] = 0; _framelineV[2] = 0;
        break;
     
    case 10:
    // No Frames
        _frameThickness = 0;
        tilesX[0] = 0; tilesY[0] = 0;

        _tileWidth[0] = _displayWidth;

        _framelineH[0] = 0; _framelineH[1] = 0; _framelineH[2] = 0;
        _framelineV[0] = 0; _framelineV[1] = 0; _framelineV[2] = 0;
        break;               
    
    default:
        break;
    }
}

void MMS_Display::drawTiles()
{
    for(int i = 0; i < _frameThickness; i++)
    {
       lcd.drawFrame(i,i,_displayWidth-2*i, _displayHeight-2*i); 
       lcd.drawHLine(_framelineH[0] - i, _framelineH[1] - i, _framelineH[2]);
       lcd.drawVLine(_framelineV[0] - i, _framelineV[1] - i, _framelineV[2]);
    }   

}

void MMS_Display::drawString(uint8_t targetTile,uint8_t x, uint8_t y, const char* str)
{
    lcd.drawStr(tilesX[targetTile]+x,tilesY[targetTile]+y, str);
}

void MMS_Display::drawBox(uint8_t targetTile,uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
    lcd.drawBox(tilesX[targetTile]+x,tilesY[targetTile]+y, w, h);
}

void MMS_Display::drawFrame(uint8_t targetTile,uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
    lcd.drawFrame(tilesX[targetTile]+x,tilesY[targetTile]+y, w, h);

}

void MMS_Display::drawRBox(uint8_t targetTile,uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r)
{
    lcd.drawRBox(tilesX[targetTile]+x,tilesY[targetTile]+y, w, h, r);
}

void MMS_Display::drawRFrame(uint8_t targetTile,uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r)
{
    lcd.drawRFrame(tilesX[targetTile]+x,tilesY[targetTile]+y, w, h, r);
}

void MMS_Display::drawDisc(uint8_t targetTile,uint8_t x, uint8_t y, uint8_t r)
{
    lcd.drawDisc(tilesX[targetTile]+x,tilesY[targetTile]+y, r);
}

void MMS_Display::drawCircle(uint8_t targetTile,uint8_t x, uint8_t y,uint8_t r)
{
    lcd.drawCircle(tilesX[targetTile]+x,tilesY[targetTile]+y, r);
}

void MMS_Display::setCursor(uint8_t targetTile, uint8_t x, uint8_t y)
{
    lcd.setCursor(tilesX[targetTile] + x, tilesY[targetTile] + y);
}

void MMS_Display::setFont(const uint8_t * font)
{
    lcd.setFont(font);
}

void MMS_Display::setDrawColor(uint8_t color)
{
    lcd.setDrawColor(color);
}

template <typename T>
void MMS_Display::print(T val)
{
    lcd.print(val);
}

template <typename V>
void printRightAligned(uint8_t targetTile, uint8_t xOffset, uint8_t y, V val)
{
    uint8_t x = tilesX[targetTile] + _displayWidth/2 - xOffset - 1;
}
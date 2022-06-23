#include <Arduino.h>
#include <u8g2lib.h>

#define TILES_2L_2R 0
#define TILES_2L_1R 1
#define TILES_1L_2R 2
#define TILES_1L_1R 3
#define TILES_2T_1B 4
#define TILES_1T_2B 5
#define TILES_1T_1B 6
#define TILES_3HOR 7
#define TILES_4HOR 8
#define TILES_FRAME 9
#define TILES_NONE 10

class MMS_Display
{
private:
    uint8_t _displayWidth = 0;
    uint8_t _displayHeight = 0;
    uint8_t _tileWidth[5] = {};
    uint8_t _frameThickness = 0;
    uint8_t _layout = 0;
    uint8_t _framelineH[3] = {};
    uint8_t _framelineV[3] = {};

    char *_valToCharArray(int val);
    char *_valToCharArray(long val);
    char *_valToCharArray(float val);
    char *_valToCharArray(double val);

public:
    MMS_Display(U8G2 &lcdObj);
    void begin(uint8_t layout, uint8_t frameThickness);
    void createTiles(uint8_t layout, uint8_t frameThickness);
    void drawTiles();
    void clearTile(uint8_t targetTile);

    void sendBuffer();
    void clearDisplay();
    void drawStr(uint8_t targetTile, uint8_t x, uint8_t y, const char *str);
    void drawBox(uint8_t targetTile, uint8_t x, uint8_t y, uint8_t w, uint8_t h);
    void drawFrame(uint8_t targetTile, uint8_t x, uint8_t y, uint8_t w, uint8_t h);
    void drawRBox(uint8_t targetTile, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r);
    void drawRFrame(uint8_t targetTile, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r);
    void drawDisc(uint8_t targetTile, uint8_t x, uint8_t y, uint8_t r);
    void drawCircle(uint8_t targetTile, uint8_t x, uint8_t y, uint8_t r);
    void drawPixel(uint8_t targetTile, uint8_t x, uint8_t y);
    void drawButtonUTF8(uint8_t targetTile, uint8_t x, uint8_t y, uint8_t flags, uint8_t width, uint8_t padding_h, uint8_t padding_v, const char* text);
    void setCursor(uint8_t targetTile, uint8_t x, uint8_t y);
    void setFont(const uint8_t *font);
    void setFontMode(uint8_t is_transparent);
    void setDrawColor(uint8_t color);
    void drawRightAlignedStr(uint8_t targetTile, uint8_t xOffset, uint8_t y, const char *str);
    void printRightAligned(uint8_t targetTile, uint8_t xOffset, uint8_t y, int val);
    void printRightAligned(uint8_t targetTile, uint8_t xOffset, uint8_t y, long val);
    void printRightAligned(uint8_t targetTile, uint8_t xOffset, uint8_t y, float val);
    void printRightAligned(uint8_t targetTile, uint8_t xOffset, uint8_t y, double val);

    void drawBar(uint8_t targetTile, uint8_t x, uint8_t y, uint8_t w, uint8_t h, double val, double valMax, uint8_t borderType, uint8_t shading);

    template <typename T>
    void print(T val);

    U8G2 lcd;

    uint8_t tilesX[5] = {};
    uint8_t tilesY[5] = {};
};

template <typename T>
void MMS_Display::print(T val)
{
    lcd.print(val);
}
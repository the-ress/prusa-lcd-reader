#pragma once

#include "const.h"

// https://github.com/fsalomon/HD44780-decoder/blob/main/sniff.ino

#define NUM_CHARS 80
#define LINE_LENGTH 20
#define MAX_UTF8_CHAR_LENGTH 7 // 4 bytes for character, 3 bytes for variation selector

#define LCD_STR_BEDTEMP      '\x00'
#define LCD_STR_DEGREE       '\x01'
#define LCD_STR_THERMOMETER  '\x02'
#define LCD_STR_UPLEVEL      '\x03'
#define LCD_STR_REFRESH      '\x04'
#define LCD_STR_FOLDER       '\x05'
#define LCD_STR_FEEDRATE     '\x06'
#define LCD_STR_ARROW_2_DOWN '\x06'
#define LCD_STR_CLOCK        '\x07'
#define LCD_STR_CONFIRM      '\x07'
#define LCD_STR_ARROW_RIGHT  '\x7E'
#define LCD_STR_SOLID_BLOCK  '\xFF'

// https://github.com/prusa3d/Prusa-Firmware/commit/1c26875e0e729ce6bc339f1e746c0a9f767e2096
#define LCD_STR_ARROW_2_DOWN_OLD '\x01'
#define LCD_STR_CONFIRM_OLD '\x02'

namespace esphome {
namespace prusa_lcd {

class PrusaLcdRenderer {
  public:
    std::array<uint8_t, DDRAM_SIZE> DDRAM = { 32 };
    std::array<uint8_t, CGRAM_SIZE> CGRAM = {};

    char lineBuffer0[LINE_LENGTH * MAX_UTF8_CHAR_LENGTH + 1] = { 0 };
    char lineBuffer1[LINE_LENGTH * MAX_UTF8_CHAR_LENGTH + 1] = { 0 };
    char lineBuffer2[LINE_LENGTH * MAX_UTF8_CHAR_LENGTH + 1] = { 0 };
    char lineBuffer3[LINE_LENGTH * MAX_UTF8_CHAR_LENGTH + 1] = { 0 };

    PrusaLcdRenderer() {
      DDRAM.fill(32);
      CGRAM.fill(0);
    }

    bool update();

  private:
    long changeTime_ = 0;

    void writeLineBufferCharacter_(char *buffer, int i, int *j);
    inline void writeLineBufferByte(char *buffer, uint8_t c, int *j);
    inline bool isBitmapInCgram_(uint8_t c, const uint8_t *bitmap);
};

}  // namespace prusa_lcd
}  // namespace esphome

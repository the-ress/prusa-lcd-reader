#include "prusa_lcd.h"

#include <binary.h>
#include "esphome/core/log.h"

namespace esphome {
namespace prusa_lcd {

static const char *const TAG = "prusa_lcd";

const uint8_t lcd_chardata_arr2down[8] = {
	B00000,
	B00000,
	B10001,
	B01010,
	B00100,
	B10001,
	B01010,
	B00100,
};

const uint8_t lcd_chardata_confirm[8] = {
	B00000,
	B00001,
	B00011,
	B10110,
	B11100,
	B01000,
	B00000,
};

inline bool PrusaLcdRenderer::isBitmapInCgram_(uint8_t c, const uint8_t *bitmap) {
  for (int i = 0; i < 8; i++) {
    if ((CGRAM[c * 8 + i] & 0x1F) != bitmap[i]) {
      return false;
    }
  }
  return true;
}

inline void PrusaLcdRenderer::writeLineBufferByte(char *buffer, uint8_t c, int *j) {
  if (c != buffer[*j]) {
    changeTime_ = (unsigned long)(esp_timer_get_time() / 1000ULL);
    buffer[*j] = (char)c;
  }
  (*j)++;
}

void PrusaLcdRenderer::writeLineBufferCharacter_(char *buffer, int i, int *j) {
  uint8_t c = DDRAM[i];

  if (c == LCD_STR_ARROW_2_DOWN || c == LCD_STR_ARROW_2_DOWN_OLD) {
    if (isBitmapInCgram_(c, lcd_chardata_arr2down)) {
      // Downwards Paired Arrows
      writeLineBufferByte(buffer, 226, j);
      writeLineBufferByte(buffer, 135, j);
      writeLineBufferByte(buffer, 138, j);

      // Downwards Triangle-Headed Paired Arrows
      // Missing on iOS
      // writeLineBufferByte(buffer, 226, j);
      // writeLineBufferByte(buffer, 174, j);
      // writeLineBufferByte(buffer, 135, j);
      return;
    }
  }

  if (c == LCD_STR_CONFIRM || c == LCD_STR_CONFIRM_OLD) {
    if (isBitmapInCgram_(c, lcd_chardata_confirm)) {
      // Check Mark
      writeLineBufferByte(buffer, 226, j);
      writeLineBufferByte(buffer, 156, j);
      writeLineBufferByte(buffer, 147, j);
      return;
    }
  }

  switch(c) {
    case 32:
      // No-Break Space
      writeLineBufferByte(buffer, 194, j);
      writeLineBufferByte(buffer, 160, j);
      return;
    case LCD_STR_BEDTEMP:
      // Negative Squared Latin Capital Letter H
      writeLineBufferByte(buffer, 240, j);
      writeLineBufferByte(buffer, 159, j);
      writeLineBufferByte(buffer, 133, j);
      writeLineBufferByte(buffer, 183, j);

      // Squared Latin Capital Letter H
      // writeLineBufferByte(buffer, 240, j);
      // writeLineBufferByte(buffer, 159, j);
      // writeLineBufferByte(buffer, 132, j);
      // writeLineBufferByte(buffer, 183, j);

      // Square with Orthogonal Crosshatch Fill
      // writeLineBufferByte(buffer, 226, j);
      // writeLineBufferByte(buffer, 150, j);
      // writeLineBufferByte(buffer, 166, j);
      return;
    case LCD_STR_DEGREE:
      // Degree Sign
      writeLineBufferByte(buffer, 194, j);
      writeLineBufferByte(buffer, 176, j);
      return;
    case LCD_STR_THERMOMETER:
      // Thermometer
      writeLineBufferByte(buffer, 240, j);
      writeLineBufferByte(buffer, 159, j);
      writeLineBufferByte(buffer, 140, j);
      writeLineBufferByte(buffer, 161, j);
      // Variation Selector-15 (text)
      writeLineBufferByte(buffer, 239, j);
      writeLineBufferByte(buffer, 184, j);
      writeLineBufferByte(buffer, 142, j);
      return;
    case LCD_STR_UPLEVEL:
      // Rightwards Arrow with Tip Upwards
      writeLineBufferByte(buffer, 226, j);
      writeLineBufferByte(buffer, 172, j);
      writeLineBufferByte(buffer, 143, j);

      // Rightwards Triangle-Headed Arrow with Long Tip Upwards
      // Missing on iOS
      // writeLineBufferByte(buffer, 226, j);
      // writeLineBufferByte(buffer, 174, j);
      // writeLineBufferByte(buffer, 165, j);
      return;
    case LCD_STR_REFRESH:
      // Clockwise Gapped Circle Arrow
      writeLineBufferByte(buffer, 226, j);
      writeLineBufferByte(buffer, 159, j);
      writeLineBufferByte(buffer, 179, j);

      // Clockwise Right and Left Semicircle Arrows
      // Missing on iOS
      // writeLineBufferByte(buffer, 240, j);
      // writeLineBufferByte(buffer, 159, j);
      // writeLineBufferByte(buffer, 151, j);
      // writeLineBufferByte(buffer, 152, j);
      return;
    case LCD_STR_FOLDER:
      // File Folder Emoji
      writeLineBufferByte(buffer, 240, j);
      writeLineBufferByte(buffer, 159, j);
      writeLineBufferByte(buffer, 147, j);
      writeLineBufferByte(buffer, 129, j);
      // Variation Selector-15 (text)
      writeLineBufferByte(buffer, 239, j);
      writeLineBufferByte(buffer, 184, j);
      writeLineBufferByte(buffer, 142, j);

      // Folder
      // Missing on iOS
      // writeLineBufferByte(buffer, 240, j);
      // writeLineBufferByte(buffer, 159, j);
      // writeLineBufferByte(buffer, 151, j);
      // writeLineBufferByte(buffer, 128, j);
      return;
    case LCD_STR_FEEDRATE:
      // Open-Outlined Rightwards Arrow
      writeLineBufferByte(buffer, 226, j);
      writeLineBufferByte(buffer, 158, j);
      writeLineBufferByte(buffer, 190, j);

      // Rightwards Triangle-Headed Paired Arrows
      // Missing on iOS
      // writeLineBufferByte(buffer, 226, j);
      // writeLineBufferByte(buffer, 174, j);
      // writeLineBufferByte(buffer, 134, j);
      return;
    case LCD_STR_CLOCK:
      // Clock Face Two Oclock
      writeLineBufferByte(buffer, 240, j);
      writeLineBufferByte(buffer, 159, j);
      writeLineBufferByte(buffer, 149, j);
      writeLineBufferByte(buffer, 145, j);
      // Variation Selector-15 (text)
      writeLineBufferByte(buffer, 239, j);
      writeLineBufferByte(buffer, 184, j);
      writeLineBufferByte(buffer, 142, j);
      return;
    case LCD_STR_ARROW_RIGHT:
      // Rightwards Arrow
      writeLineBufferByte(buffer, 226, j);
      writeLineBufferByte(buffer, 134, j);
      writeLineBufferByte(buffer, 146, j);

      // Rightwards Triangle-Headed Arrow
      // Replaced for consistency with Rightwards Arrow with Tip Upwards
      // writeLineBufferByte(buffer, 226, j);
      // writeLineBufferByte(buffer, 173, j);
      // writeLineBufferByte(buffer, 162, j);
      return;
    case LCD_STR_SOLID_BLOCK:
      // Full Block
      writeLineBufferByte(buffer, 226, j);
      writeLineBufferByte(buffer, 150, j);
      writeLineBufferByte(buffer, 136, j);
      return;
    default:
      writeLineBufferByte(buffer, c, j);
      return;
  }
}

bool PrusaLcdRenderer::update() {
  int j = 0;
  for (int i = 0; i < 20; i++) {
    writeLineBufferCharacter_(lineBuffer0, i, &j);
  }
  lineBuffer0[j] = 0;
  
  j = 0;
  for (int i = 64; i < 84; i++) {
    writeLineBufferCharacter_(lineBuffer1, i, &j);
  }
  lineBuffer1[j] = 0;

  j = 0;
  for (int i = 20; i < 40; i++) {
    writeLineBufferCharacter_(lineBuffer2, i, &j);
  }
  lineBuffer2[j] = 0;

  j = 0;
  for (int i = 84; i < 104; i++) {
    writeLineBufferCharacter_(lineBuffer3, i, &j);
  }
  lineBuffer3[j] = 0;

  // if (changeTime_ > 0 && changeTime_ < millis() - 500) {
  if (changeTime_ > 0) {
    changeTime_ = 0;
    return true;
  }

  return false;
}

}  // namespace prusa_lcd
}  // namespace esphome

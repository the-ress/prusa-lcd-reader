#include "prusa_lcd_decoder.h"

#include <freertos/task.h>
#include "esphome/core/log.h"

namespace esphome {
namespace prusa_lcd {

static const char *const TAG = "prusa_lcd";

void PrusaLcdDecoder::setup() {
  interruptQueue_ = xQueueCreate(64, sizeof(int));
  xTaskCreate(decodeTask_, "DecodeTask", 2048, this, 1, NULL);

	gpio_set_intr_type(enable_pin_, GPIO_INTR_NEGEDGE);
	gpio_intr_enable(enable_pin_);

  gpio_install_isr_service(0);
  gpio_isr_handler_add(enable_pin_, gpioInterruptHandler_, this);
}

void PrusaLcdDecoder::gpioInterruptHandler_(void *arg) {
  PrusaLcdDecoder *obj = (PrusaLcdDecoder*)arg;

  uint8_t rs = gpio_get_level(obj->rs_pin_);
  uint8_t d7 = gpio_get_level(obj->d7_pin_);
  uint8_t d6 = gpio_get_level(obj->d6_pin_);
  uint8_t d5 = gpio_get_level(obj->d5_pin_);
  uint8_t d4 = gpio_get_level(obj->d4_pin_);

  uint8_t data = (rs << 7) + (d7 << 3) + (d6 << 2) + (d5 << 1) + d4;
  xQueueSendFromISR(obj->interruptQueue_, &data, NULL);
}

void PrusaLcdDecoder::decodeTask_(void *arg) {
  PrusaLcdDecoder *obj = (PrusaLcdDecoder*)arg;

  uint8_t data;
  while (true) {
    if (xQueueReceive(obj->interruptQueue_, &data, portMAX_DELAY)) {
      obj->decode_(data & 0xF, data >> 7);
    }
  }
}

void PrusaLcdDecoder::decode_(uint8_t data, bool rs) {
  if (gotUpperNibble_) {
    data = (upperNibble_ << 4 | data);
    gotUpperNibble_ = false;
  } else {
    upperNibble_ = data;
    gotUpperNibble_ = true;
    return;
  }
  if (rs == 1) {
    // store received byte
    if (receivingCgram_) {
      CGRAM[cgramIndex_] = data;
      cgramIndex_ = (cgramIndex_ + 1) % CGRAM_SIZE;
    } else if (receivingDdram_) {
      DDRAM[ddramIndex_] = data;
      ddramIndex_ = (ddramIndex_ + 1) % DDRAM_SIZE;
    }
  } else {
    // command received
    evalCommand_(data);
  }
}

const uint8_t LCD_CLEAR =  0x01;
const uint8_t LCD_CLEAR_MASK = 0xFF;

const uint8_t LCD_RETURN_HOME =  0x02;
const uint8_t LCD_RETURN_HOME_MASK = 0xFE;

const uint8_t LCD_SETDDRAMADDR = 0x80;
const uint8_t LCD_SETDDRAMADDR_MASK = 0x80;

const uint8_t LCD_SETCGRAMADDR = 0x40;
const uint8_t LCD_SETCGRAMADDR_MASK = 0xC0;

const uint8_t LCD_SETFUNCTION_4BIT = 0x20;
const uint8_t LCD_SETFUNCTION_8BIT = 0x30;
const uint8_t LCD_SETFUNCTION_MASK = 0xF0;

void PrusaLcdDecoder::evalCommand_(uint8_t command) {
  receivingCgram_ = false;
  receivingDdram_ = false;

  if ((command & LCD_CLEAR_MASK) == LCD_CLEAR) {
    DDRAM.fill(32);
    ddramIndex_ = 0;
    return;
  }
  if ((command & LCD_RETURN_HOME_MASK) == LCD_RETURN_HOME) {
    ddramIndex_ = 0;
    return;
  }
  if ((command & LCD_SETCGRAMADDR_MASK) == LCD_SETCGRAMADDR) {
    cgramIndex_ = (command & 0x3f) % CGRAM_SIZE;
    receivingCgram_ = true;
    return;
  }
  if ((command & LCD_SETDDRAMADDR_MASK) == LCD_SETDDRAMADDR) {
    ddramIndex_ = (command & 0x7f) % DDRAM_SIZE;
    receivingDdram_ = true;
    return;
  }

  // unknown command
}

void PrusaLcdDecoder::set_data_pins(std::array<uint8_t, 4> pins) {
  this->d4_pin_ = (gpio_num_t)pins[0];
  this->d5_pin_ = (gpio_num_t)pins[1];
  this->d6_pin_ = (gpio_num_t)pins[2];
  this->d7_pin_ = (gpio_num_t)pins[3];
}
void PrusaLcdDecoder::set_enable_pin(uint8_t enable_pin) { this->enable_pin_ = (gpio_num_t)enable_pin; }
void PrusaLcdDecoder::set_rs_pin(uint8_t rs_pin) { this->rs_pin_ = (gpio_num_t)rs_pin; }

void PrusaLcdDecoder::dump_config() {
  ESP_LOGCONFIG(TAG, "  Enable Pin: %d", enable_pin_);
  ESP_LOGCONFIG(TAG, "  RS Pin: %d", rs_pin_);
  ESP_LOGCONFIG(TAG, "  Data Pins:");
  ESP_LOGCONFIG(TAG, "    D4: %d", d4_pin_);
  ESP_LOGCONFIG(TAG, "    D5: %d", d5_pin_);
  ESP_LOGCONFIG(TAG, "    D6: %d", d6_pin_);
  ESP_LOGCONFIG(TAG, "    D7: %d", d7_pin_);
}

void PrusaLcdDecoder::dump_ddram() {
  ESP_LOGD(TAG, "DDRAM:");
  for (int i = 0; i < DDRAM_SIZE; i++) {
    ESP_LOGD(TAG, "  %d = %d (%c)", i, DDRAM[i], DDRAM[i]);
  }
}

void PrusaLcdDecoder::dump_cgram() {
  ESP_LOGD(TAG, "CGRAM:");
  for (int a = 0; a < 8; a++) {
    ESP_LOGD(TAG, "  %d:", a);
    for (int i = 0; i < 8; i++) {
      char m = 16;
      char x = CGRAM[a * 8 + i];
      char line[6] = { 0 };
      for (int j = 0; j < 5; j++) {
        if (x & m) {
          line[j] = 'x';
        } else {
          line[j] = ' ';
        }
        m >>= 1;
      }

      ESP_LOGD(TAG, "    %s", line);
    }
  }
}

}  // namespace prusa_lcd
}  // namespace esphome

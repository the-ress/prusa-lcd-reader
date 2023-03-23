#pragma once

#include <array>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#include "const.h"

namespace esphome {
namespace prusa_lcd {

class PrusaLcdDecoder {
  public:
    PrusaLcdDecoder() {
      DDRAM.fill(32);
      CGRAM.fill(0);
    }

    std::array<uint8_t, DDRAM_SIZE> DDRAM = { 32 };
    std::array<uint8_t, CGRAM_SIZE> CGRAM = {};

    void set_data_pins(std::array<uint8_t, 4> pins);
    void set_enable_pin(uint8_t enable_pin);
    void set_rs_pin(uint8_t rs_pin);

    void setup();
    void dump_config();
    void dump_ddram();
    void dump_cgram();

  private:
    gpio_num_t d4_pin_ = GPIO_NUM_NC;
    gpio_num_t d5_pin_ = GPIO_NUM_NC;
    gpio_num_t d6_pin_ = GPIO_NUM_NC;
    gpio_num_t d7_pin_ = GPIO_NUM_NC;
    gpio_num_t enable_pin_ = GPIO_NUM_NC;
    gpio_num_t rs_pin_ = GPIO_NUM_NC;

    xQueueHandle interruptQueue_;

    bool gotUpperNibble_ = false;
    uint8_t upperNibble_;

    uint8_t ddramIndex_ = 0;
    uint8_t cgramIndex_ = 0;
    bool receivingDdram_ = false;
    bool receivingCgram_ = false;

    void decode_(uint8_t data, bool rs);
    void evalCommand_(uint8_t command);

    static void gpioInterruptHandler_(void *args);
    static void decodeTask_(void *args);
};

}  // namespace prusa_lcd
}  // namespace esphome

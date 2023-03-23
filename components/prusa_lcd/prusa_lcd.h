#pragma once

#include <array>
#include "esphome/core/component.h"
#include "esphome/components/text_sensor/text_sensor.h"

#include "prusa_lcd_decoder.h"
#include "prusa_lcd_renderer.h"

// https://github.com/fsalomon/HD44780-decoder/blob/main/sniff.ino

namespace esphome {
namespace prusa_lcd {

class PrusaLcdComponent : public PollingComponent {
  public:
    PrusaLcdComponent();

    void set_data_pins(std::array<uint8_t, 4> pins) { decoder_.set_data_pins(pins); }
    void set_enable_pin(uint8_t enable_pin)  { decoder_.set_enable_pin(enable_pin); }
    void set_rs_pin(uint8_t rs_pin)  { decoder_.set_rs_pin(rs_pin); }

    void set_text_sensors(std::array<text_sensor::TextSensor*, 4> sensors);

    void setup() override;
    void update() override;
    void dump_config() override;

  private:
    PrusaLcdDecoder decoder_;
    PrusaLcdRenderer renderer_;
    long changeTime_ = 0;

    text_sensor::TextSensor *line0_sensor_ = nullptr;
    text_sensor::TextSensor *line1_sensor_ = nullptr;
    text_sensor::TextSensor *line2_sensor_ = nullptr;
    text_sensor::TextSensor *line3_sensor_ = nullptr;
};

}  // namespace prusa_lcd
}  // namespace esphome

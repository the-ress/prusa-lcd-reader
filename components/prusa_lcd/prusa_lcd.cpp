#include "prusa_lcd.h"

#include "esphome/core/log.h"
#include "const.h"

namespace esphome {
namespace prusa_lcd {

static const char *const TAG = "prusa_lcd";

PrusaLcdComponent::PrusaLcdComponent() : PollingComponent(100) { }

void PrusaLcdComponent::setup() {
  decoder_.setup();
}

void PrusaLcdComponent::update() {
  renderer_.DDRAM = decoder_.DDRAM;
  renderer_.CGRAM = decoder_.CGRAM;

  if (renderer_.update()) {
    line0_sensor_->publish_state(renderer_.lineBuffer0);
    line1_sensor_->publish_state(renderer_.lineBuffer1);
    line2_sensor_->publish_state(renderer_.lineBuffer2);
    line3_sensor_->publish_state(renderer_.lineBuffer3);

    // decoder_.dump_ddram();
    // decoder_.dump_cgram();
  }
}

void PrusaLcdComponent::set_text_sensors(std::array<text_sensor::TextSensor*, 4> sensors) {
  line0_sensor_ = sensors[0];
  line1_sensor_ = sensors[1];
  line2_sensor_ = sensors[2];
  line3_sensor_ = sensors[3];
}

void PrusaLcdComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Prusa LCD:");

  decoder_.dump_config();
  LOG_UPDATE_INTERVAL(this);

  LOG_TEXT_SENSOR("  ", "Line 0", this->line0_sensor_);
  LOG_TEXT_SENSOR("  ", "Line 1", this->line1_sensor_);
  LOG_TEXT_SENSOR("  ", "Line 2", this->line2_sensor_);
  LOG_TEXT_SENSOR("  ", "Line 3", this->line3_sensor_);
}

}  // namespace prusa_lcd
}  // namespace esphome

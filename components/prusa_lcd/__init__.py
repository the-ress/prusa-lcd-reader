import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import text_sensor
from esphome.const import (
    CONF_DATA_PINS,
    CONF_ENABLE_PIN,
    CONF_RS_PIN,
    CONF_TEXT_SENSORS,
    CONF_ID,
)

prusa_lcd_ns = cg.esphome_ns.namespace("prusa_lcd")
PrusaLcdComponent = prusa_lcd_ns.class_(
    "PrusaLcdComponent", cg.PollingComponent
)

# def validate_pin_length(value):
#     if len(value) != 4:
#         raise cv.Invalid(
#             f"Only 4-pin mode is supported, not {len(value)}-pin mode"
#         )
#     return value

# def validate_sensor_length(value):
#     if len(value) != 4:
#         raise cv.Invalid(
#             f"There should be 4 text sensors (one per LCD row)"
#         )
#     return value

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(PrusaLcdComponent),
            cv.Required(CONF_DATA_PINS): cv.All(
                [pins.internal_gpio_input_pin_number], cv.Length(min=4, max=4)
            ),
            cv.Required(CONF_ENABLE_PIN): pins.internal_gpio_input_pin_number,
            cv.Required(CONF_RS_PIN): pins.internal_gpio_input_pin_number,

            cv.Required(CONF_TEXT_SENSORS): cv.All(
                [text_sensor.text_sensor_schema()], cv.Length(min=4, max=4)
            ),
        }
    )
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    cg.add(var.set_data_pins(config[CONF_DATA_PINS]))
    cg.add(var.set_enable_pin(config[CONF_ENABLE_PIN]))
    cg.add(var.set_rs_pin(config[CONF_RS_PIN]))

    sensors_ = []
    for conf in config[CONF_TEXT_SENSORS]:
        sensors_.append(await text_sensor.new_text_sensor(conf))
    cg.add(var.set_text_sensors(sensors_))

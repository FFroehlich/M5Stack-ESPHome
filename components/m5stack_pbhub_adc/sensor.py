import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import adc, sensor
from esphome.components.adc.sensor import (
    ATTENUATION_MODES,
    ESP32_VARIANT_ADC1_PIN_TO_CHANNEL,
    ESP32_VARIANT_ADC2_PIN_TO_CHANNEL,
    SAMPLING_MODES,
    CONF_SAMPLES,
    CONF_SAMPLING_MODE,
    _attenuation, 
    _sampling_mode,
)
from esphome.const import (
    CONF_ATTENUATION,
    CONF_ID,
    CONF_NUMBER,
    CONF_PIN,
    CONF_RAW,
    CONF_WIFI,
    DEVICE_CLASS_VOLTAGE,
    STATE_CLASS_MEASUREMENT,
    UNIT_VOLT,
)

from esphome.components.m5stack_pbhub import mstack_pbhub_ns, M5StackPBHUBComponent

CONF_M5STACK_PBHUB_ID = "m5stack_pbhub_id"
CONF_CHANNEL = "channel"
CONF_PIN = "pin"

PbHubADCSensor = mstack_pbhub_ns.class_("PbHubADCSensor", sensor.Sensor, cg.PollingComponent)

def validate_config(config):
    adc.sensor.validate_config(config)
    return config

CONFIG_SCHEMA = CONFIG_SCHEMA = cv.All(
    sensor.sensor_schema(
        PbHubADCSensor,
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=2,
        device_class=DEVICE_CLASS_VOLTAGE,
        state_class=STATE_CLASS_MEASUREMENT,
    )
    .extend(
        {
            cv.Required(CONF_PIN): adc.validate_adc_pin,
            cv.GenerateID(): cv.declare_id(PbHubADCSensor),
            cv.Required(CONF_M5STACK_PBHUB_ID): cv.use_id(M5StackPBHUBComponent),
            cv.Required(CONF_CHANNEL): cv.int_range(min=0, max=5),
            cv.Required(CONF_PIN): cv.int_range(min=0, max=1),
            cv.Optional(CONF_RAW, default=False): cv.boolean,
            cv.SplitDefault(CONF_ATTENUATION, esp32="0db"): cv.All(
                cv.only_on_esp32, _attenuation
            ),
            cv.Optional(CONF_SAMPLES, default=1): cv.int_range(min=1, max=255),
            cv.Optional(CONF_SAMPLING_MODE, default="avg"): _sampling_mode,
        }
    )
    .extend(cv.polling_component_schema("60s")),
    validate_config,
)

async def to_code(config):
    parent = await cg.get_variable(config[CONF_M5STACK_PBHUB_ID])
    var = cg.new_Pvariable(config[CONF_ID], parent, config[CONF_CHANNEL], config[CONF_PIN])
    await sensor.register_sensor(var, config)
    await cg.register_component(var, config)

    cg.add(var.set_output_raw(config[CONF_RAW]))
    cg.add(var.set_sample_count(config[CONF_SAMPLES]))
    cg.add(var.set_sampling_mode(config[CONF_SAMPLING_MODE]))

    if attenuation := config.get(CONF_ATTENUATION):
        if attenuation == "auto":
            cg.add(var.set_autorange(cg.global_ns.true))
        else:
            cg.add(var.set_attenuation(attenuation))

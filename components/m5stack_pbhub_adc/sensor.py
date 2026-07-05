import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    CONF_RAW,
    DEVICE_CLASS_VOLTAGE,
    STATE_CLASS_MEASUREMENT,
    UNIT_VOLT,
)

from esphome.components.m5stack_pbhub import mstack_pbhub_ns, M5StackPBHUBComponent

CONF_M5STACK_PBHUB_ID = "m5stack_pbhub_id"
CONF_CHANNEL = "channel"
CONF_PIN = "pin"

PbHubADCSensor = mstack_pbhub_ns.class_(
    "PbHubADCSensor", sensor.Sensor, cg.PollingComponent
)

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        PbHubADCSensor,
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=2,
        device_class=DEVICE_CLASS_VOLTAGE,
        state_class=STATE_CLASS_MEASUREMENT,
    )
    .extend(
        {
            cv.Required(CONF_M5STACK_PBHUB_ID): cv.use_id(M5StackPBHUBComponent),
            cv.Required(CONF_CHANNEL): cv.int_range(min=0, max=5),
            cv.Required(CONF_PIN): cv.int_range(min=0, max=1),
            cv.Optional(CONF_RAW, default=False): cv.boolean,
        }
    )
    .extend(cv.polling_component_schema("60s"))
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_M5STACK_PBHUB_ID])
    var = cg.new_Pvariable(
        config[CONF_ID], parent, config[CONF_CHANNEL], config[CONF_PIN]
    )
    await sensor.register_sensor(var, config)
    await cg.register_component(var, config)

    cg.add(var.set_output_raw(config[CONF_RAW]))

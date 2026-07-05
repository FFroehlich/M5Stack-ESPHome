import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import i2c, sensor
from esphome.const import (
    CONF_ID,
    CONF_INPUT,
    CONF_NUMBER,
    CONF_MODE,
    CONF_INVERTED,
    CONF_OUTPUT,
    DEVICE_CLASS_VOLTAGE,
    STATE_CLASS_MEASUREMENT,
    UNIT_VOLT,
)

CONF_M5STACK_PBHUB_ID = "m5stack_pbhub_id"
CONF_CHANNEL = "channel"
CONF_PIN = "pin"

mstack_pbhub_ns = cg.esphome_ns.namespace("m5stack_pbhub")

M5StackPBHUBComponent = mstack_pbhub_ns.class_("M5StackPBHUBComponent", cg.Component)

PbHubADCSensor = mstack_pbhub_ns.class_(
    "PbHubADCSensor", sensor.Sensor, cg.PollingComponent
)

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        unit_of_measurement="V",
        icon="mdi:flash",
        accuracy_decimals=2,
    )
    .extend(
        {
            cv.GenerateID(): cv.declare_id(PbHubADCSensor),
            cv.Required(CONF_M5STACK_PBHUB_ID): cv.use_id(M5StackPBHUBComponent),
            cv.Required(CONF_CHANNEL): cv.int_range(min=0, max=5),
        }
    )
    .extend(cv.polling_component_schema("10s"))
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_M5STACK_PBHUB_ID])
    var = cg.new_Pvariable(config[CONF_ID], hub, config[CONF_CHANNEL])
    await sensor.register_sensor(var, config)
    await cg.register_component(var, config)

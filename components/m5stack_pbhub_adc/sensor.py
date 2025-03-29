import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import adc
from esphome.const import (
    CONF_ID,
    CONF_UNIT_OF_MEASUREMENT,
    CONF_ICON,
    CONF_ACCURACY_DECIMALS,
    CONF_DEVICE_CLASS,
    CONF_UPDATE_INTERVAL,
)

from esphome.componets.m5stack_pbhub import m5stack_pbhub_ns

CONF_M5STACK_PBHUB_ID = "m5stack_pbhub_id"
CONF_CHANNEL = "channel"
CONF_PIN = "pin"

PbHub = m5stack_pbhub_ns.class_("PortHub", cg.Component)
PbHubADCSensor = m5stack_pbhub_ns.class_("PbHubADCSensor", sensor.Sensor, cg.PollingComponent)

CONFIG_SCHEMA = adc.adc_sensor_schema(
    unit_of_measurement="V",
    icon="mdi:flash",
    accuracy_decimals=2,
).extend({
    cv.GenerateID(): cv.declare_id(PbHubADCSensor),
    cv.Required(CONF_M5STACK_PBHUB_ID): cv.use_id(PbHub),
    cv.Required(CONF_CHANNEL): cv.int_range(min=0, max=5),
    cv.Required(CONF_PIN): cv.int_range(min=0, max=1),
}).extend(cv.polling_component_schema("10s"))


async def to_code(config):
    parent = await cg.get_variable(config[CONF_M5STACK_PBHUB_ID])
    var = cg.new_Pvariable(config[CONF_ID], parent, config[CONF_CHANNEL], config[CONF_PIN])
    await sensor.register_sensor(var, config)
    await cg.register_component(var, config)

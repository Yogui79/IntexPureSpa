import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

DEPENDENCIES = ["uart"]
AUTO_LOAD = ["binary_sensor", "sensor", "switch", "number"]

spa_ns = cg.esphome_ns.namespace("spa")
Spa = spa_ns.class_("Spa", cg.Component, uart.UARTDevice)

CONF_NETWORK_ID = "network_id"
CONF_CHANNEL = "channel"
CONF_MODEL = "model"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(Spa),
        cv.Required(CONF_NETWORK_ID): cv.hex_uint16_t,
        cv.Required(CONF_CHANNEL): cv.hex_uint8_t,
        cv.Required(CONF_MODEL): cv.one_of(28458, 28442, int=True),
    }
).extend(uart.UART_DEVICE_SCHEMA).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    cg.add(var.set_network_id(config[CONF_NETWORK_ID]))
    cg.add(var.set_channel(config[CONF_CHANNEL]))
    cg.add(var.set_model(config[CONF_MODEL]))
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID
from esphome import automation

DEPENDENCIES = ["uart"]
AUTO_LOAD = ["binary_sensor", "sensor", "switch", "number"]

spa_ns = cg.esphome_ns.namespace("spa")
Spa = spa_ns.class_("Spa", cg.Component, uart.UARTDevice)

SpaSendCommandAction = spa_ns.class_("SpaSendCommandAction", automation.Action)
SpaSetTemperatureAction = spa_ns.class_("SpaSetTemperatureAction", automation.Action)
SpaSetFilterTimeAction = spa_ns.class_("SpaSetFilterTimeAction", automation.Action)
SpaSetSanitizerTimeAction = spa_ns.class_("SpaSetSanitizerTimeAction", automation.Action)

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

SpaSendCommandAction = spa_ns.class_("SpaSendCommandAction", automation.Action)

@automation.register_action("spa.send_command", SpaSendCommandAction, cv.Schema({
    cv.Required(CONF_ID): cv.use_id(Spa),
    cv.Required("command"): cv.templatable(cv.hex_uint16_t),
}))

async def spa_send_command_to_code(config, action_id, template_arg, args):
    var = cg.new_Pvariable(action_id, template_arg)
    parent = await cg.get_variable(config[CONF_ID])
    cg.add(var.set_parent(parent))
    template_ = await cg.templatable(config["command"], args, cg.uint16)
    cg.add(var.set_command(template_))
    return var

@automation.register_action("spa.set_temperature", SpaSetTemperatureAction, cv.Schema({
    cv.Required(CONF_ID): cv.use_id(Spa),
    cv.Required("temperature"): cv.templatable(cv.float_),
}))

async def spa_set_temperature_to_code(config, action_id, template_arg, args):
    var = cg.new_Pvariable(action_id, template_arg)
    parent = await cg.get_variable(config[CONF_ID])
    cg.add(var.set_parent(parent))
    template_ = await cg.templatable(config["temperature"], args, cg.float_)
    cg.add(var.set_temperature(template_))
    return var

@automation.register_action("spa.set_filter_time", SpaSetFilterTimeAction, cv.Schema({
    cv.Required(CONF_ID): cv.use_id(Spa),
    cv.Required("hours"): cv.templatable(cv.int_range(min=0, max=24)),
}))

async def spa_set_filter_time_to_code(config, action_id, template_arg, args):
    var = cg.new_Pvariable(action_id, template_arg)
    parent = await cg.get_variable(config[CONF_ID])
    cg.add(var.set_parent(parent))
    template_ = await cg.templatable(config["hours"], args, cg.uint8)
    cg.add(var.set_hours(template_))
    return var

@automation.register_action("spa.set_sanitizer_time", SpaSetSanitizerTimeAction, cv.Schema({
    cv.Required(CONF_ID): cv.use_id(Spa),
    cv.Required("hours"): cv.templatable(cv.int_range(min=0, max=24)),
}))

async def spa_set_sanitizer_time_to_code(config, action_id, template_arg, args):
    var = cg.new_Pvariable(action_id, template_arg)
    parent = await cg.get_variable(config[CONF_ID])
    cg.add(var.set_parent(parent))
    template_ = await cg.templatable(config["hours"], args, cg.uint8)
    cg.add(var.set_hours(template_))
    return var

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    cg.add(var.set_network_id(config[CONF_NETWORK_ID]))
    cg.add(var.set_channel(config[CONF_CHANNEL]))
    cg.add(var.set_model(config[CONF_MODEL]))
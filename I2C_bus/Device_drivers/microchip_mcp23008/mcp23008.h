#ifndef LIBI2C_MCP23008_H
#define LIBI2C_MCP23008_H

#include <rtems.h>
#include <dev/i2c/i2c.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MCP23008_ADDR 0x20

typedef enum
{
  MCP23008_CONF_INPUT,
  MCP23008_CONF_OUTPUT,
  MCP23008_INVERT_LOGIC,
  MCP23008_NORMAL_LOGIC,
  MCP23008_SET_PULLUP,
  MCP23008_UNSET_PULLUP,
  MCP23008_SET_OUTPUT,
  MCP23008_CLEAR_OUTPUT,
  MCP23008_GET_OUTPUT_VAL,
  MCP23008_READ_INPUT
} mcp23008_cmd;

int i2c_dev_register_mcp23008(
  const char *bus_path,
  const char *dev_path,
  uint16_t address
);

#ifdef __cplusplus
}
#endif

#endif /* LIBI2C_MCP23008_H */

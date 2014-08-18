#include <rtems.h>
#include <rtems/libi2c.h>

#include <rtems/libio.h>

#ifndef LIBI2C_MCP23008_H
#define LIBI2C_MCP23008_H

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

rtems_status_code i2c_mcp23008_init(rtems_device_major_number major, rtems_device_minor_number minor, void *arg);
rtems_status_code i2c_mcp23008_open(rtems_device_major_number major, rtems_device_minor_number minor, void *arg);
rtems_status_code i2c_mcp23008_close(rtems_device_major_number major, rtems_device_minor_number minor, void *arg);
rtems_status_code i2c_mcp23008_read_entry(rtems_device_major_number major, rtems_device_minor_number minor, void *arg);
rtems_status_code i2c_mcp23008_write_entry(rtems_device_major_number major, rtems_device_minor_number minor, void *arg);
rtems_status_code i2c_mcp23008_ioctl(rtems_device_major_number major, rtems_device_minor_number minor, void *arg);

extern rtems_driver_address_table i2c_mcp23008_ops;

extern rtems_libi2c_drv_t i2c_mcp23008_drv_t;

#ifdef __cplusplus
}
#endif

#endif /* LIBI2C_MCP23008_H */

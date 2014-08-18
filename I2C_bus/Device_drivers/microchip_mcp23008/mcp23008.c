#include <libchip/mcp23008.h>

#define MCP23008_REG_IODIR   0x00
#define MCP23008_REG_IPOL    0x01
#define MCP23008_REG_GPINTEN 0x02
#define MCP23008_REG_DEFVAL  0x03
#define MCP23008_REG_INTCON  0x04
#define MCP23008_REG_IOCON   0x05
#define MCP23008_REG_GPPU    0x06
#define MCP23008_REG_INTF    0x07
#define MCP23008_REG_INTCAP  0x08
#define MCP23008_REG_GPIO    0x09
#define MCP23008_REG_OLAT    0x0A

static rtems_libi2c_tfr_mode_t tfr_mode = 
{
  /* Set a baudrate of 100kHz */
  .baudrate = 100000
};

/* Wait ms miliseconds */
static rtems_status_code i2c_mcp23008_wait_ms(int ms)
{
  rtems_interval ticks_per_second;

  ticks_per_second = rtems_clock_get_ticks_per_second();

  return rtems_task_wake_after((ticks_per_second * ms / 1000));
}

static rtems_status_code i2c_mcp23008_write_register(rtems_device_minor_number minor, int reg, int val)
{
  rtems_status_code sc = RTEMS_SUCCESSFUL;

  unsigned char cmd_buf[2];
  int rv;

  /* Start the bus */
  sc = rtems_libi2c_send_start(minor);
   
  if ( sc != RTEMS_SUCCESSFUL )
    return sc;

  /* Set transfer mode */
  sc = rtems_libi2c_ioctl(minor, RTEMS_LIBI2C_IOCTL_SET_TFRMODE, &tfr_mode);

  if ( sc != RTEMS_SUCCESSFUL )
    return sc;
    
  /* Address device */
  sc = rtems_libi2c_send_addr(minor, FALSE);

  if ( sc != RTEMS_SUCCESSFUL )
    return sc;

  /* Wait 1 milisecond */
  sc = i2c_mcp23008_wait_ms(1);

  if ( sc != RTEMS_SUCCESSFUL )
    return sc;

  cmd_buf[0] = reg;

  cmd_buf[1] = val;
    
  rv = rtems_libi2c_write_bytes(minor, cmd_buf, 2);

  if ( rv < 0 ) 
    return RTEMS_IO_ERROR;
      
  /* Terminate transfer */
  sc = rtems_libi2c_send_stop(minor);

  if ( sc != RTEMS_SUCCESSFUL )
    return sc;

  /* Wait 1 milisecond */
  sc = i2c_mcp23008_wait_ms(1);

  if ( sc != RTEMS_SUCCESSFUL )
    return sc;

  return sc;
}

static rtems_status_code i2c_mcp23008_read_register(rtems_device_minor_number minor, int reg, uint8_t* reg_content)
{
  rtems_status_code sc = RTEMS_SUCCESSFUL;

  unsigned char cmd_buf[1];
  int rv;
   
  /* Start the bus */
  sc = rtems_libi2c_send_start(minor);

  if ( sc != RTEMS_SUCCESSFUL )
    return sc;

  /* Set transfer mode */
   sc = rtems_libi2c_ioctl(minor, RTEMS_LIBI2C_IOCTL_SET_TFRMODE, &tfr_mode);

  if ( sc != RTEMS_SUCCESSFUL )
      return sc;
    
  /* Address device */
  sc = rtems_libi2c_send_addr(minor, FALSE);

  if ( sc != RTEMS_SUCCESSFUL )
      return sc;
  
  /* Wait 1 milisecond */
  sc = i2c_mcp23008_wait_ms(1);

  if ( sc != RTEMS_SUCCESSFUL )
   return sc;

  cmd_buf[0] = reg;

  rv = rtems_libi2c_write_bytes(minor, cmd_buf, 1);

  if ( rv < 0 ) 
     return RTEMS_IO_ERROR;

  /* Address device */
  sc = rtems_libi2c_send_addr(minor, TRUE);

  if ( sc != RTEMS_SUCCESSFUL )
      return sc;
  
  /* Wait 1 milisecond */
  sc = i2c_mcp23008_wait_ms(1);

  if ( sc != RTEMS_SUCCESSFUL )
   return sc;

  /* Fetch data */
  rv = rtems_libi2c_read_bytes (minor, reg_content, 1);
   
  if ( rv < 0 ) 
      return RTEMS_IO_ERROR;
  
  /* Terminate transfer */
  sc = rtems_libi2c_send_stop(minor);
  
  if ( sc != RTEMS_SUCCESSFUL )
   return sc;

  /* Wait 1 milisecond */
  sc = i2c_mcp23008_wait_ms(1);

  if ( sc != RTEMS_SUCCESSFUL )
   return sc;

  return sc;
}

static rtems_status_code i2c_mcp23008_set_register(rtems_device_minor_number minor, int reg, int pin, int val)
{
  rtems_status_code sc;
  uint8_t reg_content;

  sc = i2c_mcp23008_read_register(minor, reg, &reg_content);

  if ( sc != RTEMS_SUCCESSFUL )
    return RTEMS_IO_ERROR;

  switch ( val )
  {
    case 1:

      reg_content |= (1 << pin);
      break;

    case 0:

      reg_content &= ~(1 << pin);
      break;

    default:
      return RTEMS_INVALID_NUMBER;
  }

  sc = i2c_mcp23008_write_register(minor, reg, reg_content);

  return sc;
}

rtems_status_code i2c_mcp23008_ioctl(rtems_device_major_number major, rtems_device_minor_number minor, void *arg)
{  
  rtems_libio_ioctl_args_t *args = arg;
  rtems_status_code sc = RTEMS_SUCCESSFUL;
  int pin, cmd;

  int rv = 0;
   
  uint8_t reg_content;

  cmd = (int)(args->command);
  pin = (int)(args->buffer);

  if ( pin < 0 || pin > 7 )
    return RTEMS_INVALID_NUMBER;

  switch ( cmd ) 
  {
    case MCP23008_CONF_INPUT:

      sc = i2c_mcp23008_set_register(minor, MCP23008_REG_IODIR, pin, 1);
      break;

    case MCP23008_CONF_OUTPUT:

      sc = i2c_mcp23008_set_register(minor, MCP23008_REG_IODIR, pin, 0);
      break;

    case MCP23008_INVERT_LOGIC:

      sc = i2c_mcp23008_set_register(minor, MCP23008_REG_IPOL, pin, 1);
      break;

    case MCP23008_NORMAL_LOGIC:

      sc = i2c_mcp23008_set_register(minor, MCP23008_REG_IPOL, pin, 0);
      break;
      
    case MCP23008_SET_PULLUP:

      sc = i2c_mcp23008_set_register(minor, MCP23008_REG_GPPU, pin, 1);
      break;

    case MCP23008_UNSET_PULLUP:

      sc = i2c_mcp23008_set_register(minor, MCP23008_REG_GPPU, pin, 0);
      break;
    
    case MCP23008_SET_OUTPUT:

      sc = i2c_mcp23008_set_register(minor, MCP23008_REG_OLAT, pin, 1);
      break;

    case MCP23008_CLEAR_OUTPUT:

      sc = i2c_mcp23008_set_register(minor, MCP23008_REG_OLAT, pin, 0);
      break;

    case MCP23008_GET_OUTPUT_VAL:

      sc = i2c_mcp23008_read_register(minor, MCP23008_REG_OLAT, &reg_content);

      if ( sc != RTEMS_SUCCESSFUL )
        return RTEMS_IO_ERROR;

      rv = ((reg_content & (1 << pin)) >> pin);
      
      break;

    case MCP23008_READ_INPUT:

      sc = i2c_mcp23008_read_register(minor, MCP23008_REG_GPIO, &reg_content);

      if ( sc != RTEMS_SUCCESSFUL )
        return RTEMS_IO_ERROR;

      rv = ((reg_content & (1 << pin)) >> pin);
      
      break;

  default:

    sc = RTEMS_INVALID_NUMBER;
      
  }

  args->ioctl_return = rv;

  return sc;
}

rtems_status_code i2c_mcp23008_init(rtems_device_major_number major, rtems_device_minor_number minor, void *arg)
{
  return RTEMS_SUCCESSFUL;
}

rtems_status_code i2c_mcp23008_open(rtems_device_major_number major, rtems_device_minor_number minor, void *arg)
{
  return RTEMS_SUCCESSFUL;
}

rtems_status_code i2c_mcp23008_close(rtems_device_major_number major, rtems_device_minor_number minor, void *arg)
{
  return RTEMS_SUCCESSFUL;
}

rtems_status_code i2c_mcp23008_read_entry(rtems_device_major_number major, rtems_device_minor_number minor, void *arg)
{
  return RTEMS_SUCCESSFUL;
}

rtems_status_code i2c_mcp23008_write_entry(rtems_device_major_number major, rtems_device_minor_number minor, void *arg)
{
  return RTEMS_SUCCESSFUL;
}

rtems_driver_address_table i2c_mcp23008_ops = 
{
  .initialization_entry = i2c_mcp23008_init,
  .open_entry           = i2c_mcp23008_open,
  .close_entry          = i2c_mcp23008_close,
  .read_entry           = i2c_mcp23008_read_entry,
  .write_entry          = i2c_mcp23008_write_entry,
  .control_entry        = i2c_mcp23008_ioctl
};

rtems_libi2c_drv_t i2c_mcp23008_drv_t =
{
    .ops  = &i2c_mcp23008_ops, 
    .size = sizeof (i2c_mcp23008_drv_t),
};

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

static int i2c_mcp23008_write_register(i2c_dev *dev, uint8_t reg, uint8_t val)
{
  uint8_t content[2];

  content[0] = reg;
  content[1] = val;

  i2c_msg msg = {
    .addr = dev->address,
    .flags = 0,
    .len = 2,
    .buf = content
  };

  return i2c_bus_transfer(dev->bus, &msg, 1);
}

static int i2c_mcp23008_read_register(
  i2c_dev *dev,
  uint8_t reg,
  uint8_t* reg_content
) {
  i2c_msg msg[2] = {
    {
    .addr = dev->address,
    .flags = 0,
    .len = 1,
    .buf = &reg
    },
    {
    .addr = dev->address,
    .flags = I2C_M_RD,
    .len = 1,
    .buf = reg_content
    }
  };

  return i2c_bus_transfer(dev->bus, msg, 2);
}

static int i2c_mcp23008_set_register(
  i2c_dev *dev,
  uint8_t reg,
  uint8_t pin,
  uint8_t val
) {
  uint8_t reg_content;
  int rv;

  rv = i2c_mcp23008_read_register(dev, reg, &reg_content);

  if ( rv < 0 ) {
    return rv;
  }

  switch ( val ) {
    case 0:
      reg_content &= ~(1 << pin);
      break;

    case 1:
      reg_content |= (1 << pin);
      break;

    default:
      return -1;
  }

  rv = i2c_mcp23008_write_register(dev, reg, reg_content);

  return rv;
}

static int i2c_mcp23008_linux_ioctl(
  i2c_dev *dev,
  ioctl_command_t command,
  void *arg
) {
  uint8_t reg_content;
  int rv = 0;
  int pin;

  pin = (int)(uintptr_t) arg;

  if ( pin < 0 || pin > 7 )
    return RTEMS_INVALID_NUMBER;

  switch ( command ) {
    case MCP23008_CONF_INPUT:
      rv = i2c_mcp23008_set_register(dev, MCP23008_REG_IODIR, pin, 1);
      break;

    case MCP23008_CONF_OUTPUT:
      rv = i2c_mcp23008_set_register(dev, MCP23008_REG_IODIR, pin, 0);
      break;

    case MCP23008_INVERT_LOGIC:
      rv = i2c_mcp23008_set_register(dev, MCP23008_REG_IPOL, pin, 1);
      break;

    case MCP23008_NORMAL_LOGIC:
      rv = i2c_mcp23008_set_register(dev, MCP23008_REG_IPOL, pin, 0);
      break;

    case MCP23008_SET_PULLUP:
      rv = i2c_mcp23008_set_register(dev, MCP23008_REG_GPPU, pin, 1);
      break;

    case MCP23008_UNSET_PULLUP:
      rv = i2c_mcp23008_set_register(dev, MCP23008_REG_GPPU, pin, 0);
      break;

    case MCP23008_SET_OUTPUT:
      rv = i2c_mcp23008_set_register(dev, MCP23008_REG_OLAT, pin, 1);
      break;

    case MCP23008_CLEAR_OUTPUT:
      rv = i2c_mcp23008_set_register(dev, MCP23008_REG_OLAT, pin, 0);
      break;

    case MCP23008_GET_OUTPUT_VAL:
      rv = i2c_mcp23008_read_register(dev, MCP23008_REG_OLAT, &reg_content);

      if ( rv < 0 ) {
        return rv;
      }

      rv = ((reg_content & (1 << pin)) >> pin);

      break;

    case MCP23008_READ_INPUT:
      rv = i2c_mcp23008_read_register(dev, MCP23008_REG_GPIO, &reg_content);

      if ( rv < 0 ) {
        return rv;
      }

      rv = ((reg_content & (1 << pin)) >> pin);

      break;

    default:
      rv = -1;
  }

  return rv;
}

int i2c_dev_register_mcp23008(
  const char *bus_path,
  const char *dev_path,
  uint16_t address
) {
  i2c_dev *dev;

  dev = i2c_dev_alloc_and_init(sizeof(*dev), bus_path, address);
  if (dev == NULL) {
    perror();

    return -1;
  }

  dev->ioctl = i2c_mcp23008_linux_ioctl;

  return i2c_dev_register(dev, dev_path);
}

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/test.h>

#include <bsp.h> /* for device driver prototypes */

#include <bsp/i2c.h>
#include <rtems/status-checks.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include <libchip/mcp23008.h>

/* forward declarations to avoid warnings */
rtems_task Init(rtems_task_argument argument);

const char rtems_test_name[] = "I2C_MCP23008_TEST";

rtems_task Init(rtems_task_argument ignored)
{
  int rv = 0;
  int val;
  int fd;

  rtems_test_begin ();

  rv = rpi_setup_i2c_bus();
  assert ( rv == 0 );

  rv = i2c_dev_register_mcp23008(
         "/dev/i2c",
         "/dev/i2c.mcp23008",
         MCP23008_ADDR
       );

  assert ( rv == 0 );

  /* Open the mcp23008 device file */
  fd = open("/dev/i2c.mcp23008", O_RDWR);
  RTEMS_CHECK_RV(rv, "Open /dev/i2c.mcp23008");

  /* Set the mcp23008 gpio pin 4 as an output */
  rv = ioctl(fd, MCP23008_CONF_OUTPUT, (void *)(uintptr_t) 4);
  RTEMS_CHECK_RV(rv, "mcp23008 gpio conf output");

  /* Set the LED connected to the mcp23008 gpio pin 4 off */
  rv = ioctl(fd, MCP23008_CLEAR_OUTPUT, (void *)(uintptr_t)4);
  RTEMS_CHECK_RV(rv, "mcp23008 gpio clear output");

  /* Set the mcp23008 gpio pin 2 as an input */
  rv = ioctl(fd, MCP23008_CONF_INPUT, (void *)(uintptr_t) 2);
  RTEMS_CHECK_RV(rv, "mcp23008 gpio conf input");

  /* Enable the mcp23008 gpio pin 2 pull up */
  rv = ioctl(fd, MCP23008_SET_PULLUP, (void *)(uintptr_t) 2);
  RTEMS_CHECK_RV(rv, "mcp23008 gpio set pullup");

  /* Polls forever the buton conected to the mcp23008 gpio pin 2,
   * and lights the LED connected to the mcp23008 gpio pin 4 when the button is pressed.
   */
  while ( 1 ) {
    val = ioctl(fd, MCP23008_READ_INPUT, (void *)(uintptr_t) 2);

    if ( val == 0 ) {
      rv = ioctl(fd, MCP23008_SET_OUTPUT, (void *)(uintptr_t) 4);
      RTEMS_CHECK_RV(rv, "mcp23008 gpio set output");
    }
    else {
      rv = ioctl(fd, MCP23008_CLEAR_OUTPUT, (void *)(uintptr_t) 4);
      RTEMS_CHECK_RV(rv, "mcp23008 gpio clear output");
    }
  }

  rv = close(fd);
  RTEMS_CHECK_RV(rv, "Close /dev/i2c.mcp23008");

  rtems_test_end();
  exit ( 0 );
}

#define CONFIGURE_MAXIMUM_DRIVERS 10
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM

#define CONFIGURE_MAXIMUM_SEMAPHORES 10

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS 30

#define CONFIGURE_MAXIMUM_TASKS 20

 #define CONFIGURE_INIT_TASK_STACK_SIZE (32 * 1024)

#define CONFIGURE_INITIAL_EXTENSIONS RTEMS_TEST_INITIAL_EXTENSION

#define CONFIGURE_INIT
#include <rtems/confdefs.h>

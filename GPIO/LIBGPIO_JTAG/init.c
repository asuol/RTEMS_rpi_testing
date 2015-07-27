#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/test.h>

#include <bsp.h> /* for device driver prototypes */
#include <bsp/gpio.h>
#include <bsp/rpi-gpio.h>

#include <assert.h>
#include <stdlib.h>

/* forward declarations to avoid warnings */
rtems_task Init(rtems_task_argument argument);

const char rtems_test_name[] = "LIBGPIO_JTAG";

rtems_task Init(
  rtems_task_argument ignored
)
{
  rtems_status_code sc;

  rtems_test_begin ();

  /* Initializes the GPIO API */
  rtems_gpio_initialize ();

  /* Setups the JTAG pin interface */
  sc = rpi_gpio_select_jtag ();
  assert(sc == RTEMS_SUCCESSFUL);

  rtems_test_end();
  exit(0);
}

#define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_MAXIMUM_TASKS            1
#define CONFIGURE_USE_DEVFS_AS_BASE_FILESYSTEM

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INITIAL_EXTENSIONS RTEMS_TEST_INITIAL_EXTENSION

#define CONFIGURE_INIT
#include <rtems/confdefs.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/test.h>

#include <bsp.h> /* for device driver prototypes */
#include <bsp/gpio.h>

#include <assert.h>
#include <stdlib.h>

#include "rpi-conf.c"

/* forward declarations to avoid warnings */
rtems_task Init(rtems_task_argument argument);

const char rtems_test_name[] = "LIBGPIO_MULTI_TEST";

rtems_task Init(
  rtems_task_argument ignored
)
{
  rtems_status_code sc;
  int val;

  rtems_test_begin();

  /* Initializes the GPIO API */
  sc = rtems_gpio_initialize();
  assert(sc == RTEMS_SUCCESSFUL);

  sc = rtems_gpio_multi_select(test, 4);
  assert(sc == RTEMS_SUCCESSFUL);

  /* Polls the two switches. */
  while ( 1 ) {
    val = rtems_gpio_get_value(sw1_pin);

     if ( val == 0 ) {
      sc = rtems_gpio_set(led2_pin);
      assert(sc == RTEMS_SUCCESSFUL);
    }

    else {
      sc = rtems_gpio_clear(led2_pin);
      assert(sc == RTEMS_SUCCESSFUL);
    }

     val = rtems_gpio_get_value(sw2_pin);

    if ( val == 0 ) {
      sc = rtems_gpio_set(led1_pin);
      assert(sc == RTEMS_SUCCESSFUL);
    }

    else {
      sc = rtems_gpio_clear(led1_pin);
      assert(sc == RTEMS_SUCCESSFUL);
    }
  }

  rtems_test_end();
  exit(0);
}

#define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_MAXIMUM_TASKS 1
#define CONFIGURE_USE_DEVFS_AS_BASE_FILESYSTEM

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INITIAL_EXTENSIONS RTEMS_TEST_INITIAL_EXTENSION

#define CONFIGURE_INIT
#include <rtems/confdefs.h>

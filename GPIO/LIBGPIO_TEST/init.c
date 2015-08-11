#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/test.h>

#include <bsp.h> /* for device driver prototypes */
#include <bsp/gpio.h>

#include <assert.h>
#include <stdlib.h>

/* forward declarations to avoid warnings */
rtems_task Init(rtems_task_argument argument);

const char rtems_test_name[] = "LIBGPIO_TEST";

rtems_task Init(
  rtems_task_argument ignored
)
{
  rtems_status_code sc;
  uint32_t led1, led2;
  uint32_t sw1, sw2;
  int val;

  led1 = 3;
  led2 = 18;
  sw1 = 7;
  sw2 = 2;

  rtems_test_begin ();

  /* Initializes the GPIO API */
  sc = rtems_gpio_initialize ();
  assert(sc == RTEMS_SUCCESSFUL);

  sc = rtems_gpio_request_pin(led1, DIGITAL_OUTPUT, false, false, NULL);
  assert(sc == RTEMS_SUCCESSFUL);

  sc = rtems_gpio_request_pin(led2, DIGITAL_OUTPUT, false, false, NULL);
  assert(sc == RTEMS_SUCCESSFUL);

  sc = rtems_gpio_request_pin(sw1, DIGITAL_INPUT, false, false, NULL);
  assert(sc == RTEMS_SUCCESSFUL);

  sc = rtems_gpio_request_pin(sw2, DIGITAL_INPUT, false, false, NULL);
  assert(sc == RTEMS_SUCCESSFUL);

  /* Enables the internal pull up resistor on the first switch, */
  sc = rtems_gpio_resistor_mode(sw1, PULL_UP);
  assert(sc == RTEMS_SUCCESSFUL);

  /* Enables the internal pull up resistor on the second switch. */
  sc = rtems_gpio_resistor_mode(sw2, PULL_UP);
  assert(sc == RTEMS_SUCCESSFUL);

  /* Polls the two switches. */
  while ( 1 ) {
    val = rtems_gpio_get_value(sw1);

     if ( val == 0 ) {
      sc = rtems_gpio_set(led2);
      assert(sc == RTEMS_SUCCESSFUL);
    }

    else {
      sc = rtems_gpio_clear(led2);
      assert(sc == RTEMS_SUCCESSFUL);
    }

    val = rtems_gpio_get_value(sw2);

    if ( val == 0 ) {
      sc = rtems_gpio_set(led1);
      assert(sc == RTEMS_SUCCESSFUL);
    }

    else {
      sc = rtems_gpio_clear(led1);
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/test.h>

#include <bsp.h> /* for device driver prototypes */
#include <bsp/gpio.h>

#include <assert.h>
#include <stdlib.h>

rtems_gpio_irq_state edge_test_1(void *arg);
rtems_gpio_irq_state edge_test_2(void *arg);

/*
 * Lits LEDs with a switch press.
 *
 * Requires two LEDs and two switches. The platform configuration must
 * define the following constants, indicating the GPIO number where each
 * element is connected:
 *
 * led1_pin
 * led2_pin
 * sw1_pin
 * sw2_pin
 *
 * The interrupt handlers used with this application must be threaded,
 * as the GPIO API functions perform locking.
 */

#include "rpi-conf.c"

rtems_gpio_irq_state edge_test_1(void *arg)
{
  rtems_status_code sc;
  int val;

  val = rtems_gpio_get_value(sw1_pin);

  if ( val == 0 ) {
    sc = rtems_gpio_set(led1_pin);
    assert(sc == RTEMS_SUCCESSFUL);
  }
  else {
    sc = rtems_gpio_clear(led1_pin);
    assert(sc == RTEMS_SUCCESSFUL);
  }

  return IRQ_HANDLED;
}

rtems_gpio_irq_state edge_test_2(void *arg)
{
  rtems_status_code sc;
  int val;

  val = rtems_gpio_get_value(sw2_pin);

  if ( val == 0 ) {
    sc = rtems_gpio_set(led2_pin);
    assert(sc == RTEMS_SUCCESSFUL);
  }
  else {
    sc = rtems_gpio_clear(led2_pin);
    assert(sc == RTEMS_SUCCESSFUL);
  }

  return IRQ_HANDLED;
}

/* forward declarations to avoid warnings */
rtems_task Init(rtems_task_argument argument);

const char rtems_test_name[] = "LIBGPIO_MULTI_TEST_IRQ";

rtems_task Init(
  rtems_task_argument ignored
)
{
  rtems_status_code sc;

  rtems_test_begin();

  /* Initializes the GPIO API */
  rtems_gpio_initialize();

  sc = rtems_gpio_multi_select(test, 4);
  assert(sc == RTEMS_SUCCESSFUL);

  /* Keeps the program running, so interrupts can be tested. */
  while (1);

  rtems_test_end();
  exit(0);
}

#define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_MAXIMUM_TASKS 5
#define CONFIGURE_EXTRA_TASK_STACKS (2 * RTEMS_MINIMUM_STACK_SIZE)
#define CONFIGURE_USE_DEVFS_AS_BASE_FILESYSTEM

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_INIT_TASK_PRIORITY 10
#define CONFIGURE_INIT_TASK_INITIAL_MODES RTEMS_DEFAULT_MODES

#define CONFIGURE_INITIAL_EXTENSIONS RTEMS_TEST_INITIAL_EXTENSION

#define CONFIGURE_INIT
#include <rtems/confdefs.h>

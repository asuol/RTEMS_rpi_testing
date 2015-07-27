#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/test.h>
#include <bsp.h> /* for device driver prototypes */
#include <bsp/gpio.h>

#include <assert.h>
#include <stdlib.h>

/* forward declarations to avoid warnings */
rtems_task Init(rtems_task_argument ignored);

rtems_gpio_irq_state edge_test_1(void * arg);
rtems_gpio_irq_state edge_test_2(void * arg);

uint32_t led1, led2;
uint32_t sw1, sw2;

const char rtems_test_name[] = "LIBGPIO_TEST_IRQ";

rtems_gpio_irq_state edge_test_1(void *arg)
{
  rtems_status_code sc;
  int pin_number;
  int val;

  pin_number = *((int*) arg);

  printk("\nisr: pin %d\n", pin_number);

  val = rtems_gpio_get_value(sw1);

  if ( val == 0 ) {
    sc = rtems_gpio_set(led1);
    assert(sc == RTEMS_SUCCESSFUL);
  }
  else {
    sc = rtems_gpio_clear(led1);
    assert(sc == RTEMS_SUCCESSFUL);
  }

  return IRQ_HANDLED;
}

rtems_gpio_irq_state edge_test_2(void *arg)
{
  rtems_status_code sc;
  int pin_number;
  int val;

  pin_number = *((int*) arg);

  printk("\nisr: pin %d\n", pin_number);

  val = rtems_gpio_get_value(sw2);

  if ( val == 0 ) {
    sc = rtems_gpio_set(led2);
    assert(sc == RTEMS_SUCCESSFUL);
  }
  else {
    sc = rtems_gpio_clear(led2);
    assert(sc == RTEMS_SUCCESSFUL);
  }

  return IRQ_HANDLED;
}

rtems_task Init(rtems_task_argument ignored)
{
  rtems_status_code sc;

  /* Pin numbers. */
  led1 = 3;
  led2 = 18;
  sw1 = 7;
  sw2 = 2;

  rtems_test_begin();

  /* Initializes the GPIO API */
  sc = rtems_gpio_initialize();
  assert(sc == RTEMS_SUCCESSFUL);

  sc = rtems_gpio_request_pin(led1, DIGITAL_OUTPUT, false, false, NULL);
  assert(sc == RTEMS_SUCCESSFUL);

  sc = rtems_gpio_request_pin(led2, DIGITAL_OUTPUT, false, false,  NULL);
  assert(sc == RTEMS_SUCCESSFUL);

  sc = rtems_gpio_request_pin(sw1, DIGITAL_INPUT, false, false, NULL);
  assert(sc == RTEMS_SUCCESSFUL);

  sc = rtems_gpio_request_pin(sw2, DIGITAL_INPUT, false, false, NULL);
  assert(sc == RTEMS_SUCCESSFUL);

  /* Enables the internal pull up resistor on the first switch. */
  sc = rtems_gpio_resistor_mode(sw1, PULL_UP);
  assert(sc == RTEMS_SUCCESSFUL);

  /* Enables the internal pull up resistor on the second switch. */
  sc = rtems_gpio_resistor_mode(sw2, PULL_UP);
  assert(sc == RTEMS_SUCCESSFUL);

  /* Enable interrupts, and assign handler functions */
  sc = rtems_gpio_enable_interrupt(sw1, BOTH_EDGES, UNIQUE_HANDLER, true, edge_test_1, &sw1);
  assert(sc == RTEMS_SUCCESSFUL);

  sc = rtems_gpio_enable_interrupt(sw2, BOTH_EDGES, UNIQUE_HANDLER, true, edge_test_2, &sw2);
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

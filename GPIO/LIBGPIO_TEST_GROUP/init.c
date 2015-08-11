#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/test.h>

#include <bsp.h> /* for device driver prototypes */
#include <bsp/gpio.h>

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "rpi-conf.c"

/* forward declarations to avoid warnings */
rtems_task Init(rtems_task_argument argument);

const char rtems_test_name[] = "LIBGPIO_TEST_GROUP";

rtems_task Init(
  rtems_task_argument ignored
)
{
  rtems_status_code sc;
  rtems_gpio_group *group;

  rtems_test_begin();
  
  /* Initializes the GPIO API */
  sc = rtems_gpio_initialize();
  assert(sc == RTEMS_SUCCESSFUL);

  group = rtems_gpio_create_pin_group();

  sc = rtems_gpio_define_pin_group(&test_group, group);
  assert(sc == RTEMS_SUCCESSFUL);

  /* 0x1 lits the first LED
   * 0x2 lits the second LED
   * 0x3 lits both LEDs */
  sc = rtems_gpio_write_group(0x3, group);
  assert(sc == RTEMS_SUCCESSFUL);

  sc = rtems_gpio_release_pin_group(group);
  assert(sc == RTEMS_SUCCESSFUL);
  
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

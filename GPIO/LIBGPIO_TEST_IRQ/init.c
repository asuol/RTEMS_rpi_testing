#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/test.h>

#include <bsp.h> /* for device driver prototypes */

#include <bsp/gpio.h> /* Calls the BSP gpio library */
#include <rtems/status-checks.h>

#include <stdio.h>
#include <stdlib.h>

/* forward declarations to avoid warnings */
rtems_task Init(rtems_task_argument argument);

void edge_test_1(void);
void edge_test_2(void);
void level_test(void);

const char rtems_test_name[] = "LIBGPIO_TEST_IRQ";

void edge_test_1(void)
{
  int rv;
  int val;
  
  val = gpio_get_val(2);

  if ( val == 0 )
  {
    rv = gpio_set(3);
    RTEMS_CHECK_RV(rv, "gpio_set");
  }
  else
  {
    rv = gpio_clear(3);
    RTEMS_CHECK_RV(rv, "gpio_clear");
  }
}

void edge_test_2(void)
{
  int rv;
  int val;
  
  val = gpio_get_val(17);

  if ( val == 0 )
  {
    rv = gpio_set(7);
    RTEMS_CHECK_RV(rv, "gpio_set");
  }
  else
  {
    rv = gpio_clear(7);
    RTEMS_CHECK_RV(rv, "gpio_clear");
  }
}

void level_test(void)
{
  printk("\nLED ON!\n");
}

rtems_task Init(
  rtems_task_argument ignored
)
{
  rtems_interval ticks = 0;
  int rv = 0;

  ticks = rtems_clock_get_ticks_per_second();

  rtems_test_begin ();

  /* Initializes the GPIO API */
  gpio_initialize ();
  
  rv = gpio_select_pin (3, DIGITAL_OUTPUT);
  RTEMS_CHECK_RV (rv, "gpio_select_pin output");
  
  rv = gpio_select_pin (7, DIGITAL_OUTPUT);
  RTEMS_CHECK_RV (rv, "gpio_select_pin output");
  
  rv = gpio_clear (3);
  RTEMS_CHECK_RV(rv, "gpio_clear");
  
  rv = gpio_clear (7);
  RTEMS_CHECK_RV(rv, "gpio_clear");
  
  rv = gpio_select_pin (2, DIGITAL_INPUT);
  RTEMS_CHECK_RV(rv, "gpio_select_pin input");
  
  rv = gpio_select_pin (17, DIGITAL_INPUT);
  RTEMS_CHECK_RV(rv, "gpio_select_pin input");

  /* Enables the internal pull up resistor on the GPIO 2 pin */
  rv = gpio_input_mode (2, PULL_UP);
  RTEMS_CHECK_RV(rv, "gpio_input_mode");
  
  /* Enables the internal pull up resistor on the GPIO 8 pin */
  rv = gpio_input_mode (17, PULL_UP);
  RTEMS_CHECK_RV(rv, "gpio_input_mode");
  
  /* Debouces gpio pin 2 switch by requiring ~50 miliseconds between interrupts */
  rv = gpio_debounce_switch (2, (ticks * 0.05));
  RTEMS_CHECK_RV(rv, "gpio_debounce_switch");

  /* Enable interrupts, and assign handler functions */ 
  rv = gpio_enable_interrupt (2, BOTH_EDGES, edge_test_1);
  RTEMS_CHECK_RV(rv, "gpio_enable_interrupt");
  
  rv = gpio_enable_interrupt (17, BOTH_EDGES, edge_test_2);
  RTEMS_CHECK_RV(rv, "gpio_enable_interrupt");
  
  rv = gpio_enable_interrupt (7, HIGH_LEVEL, level_test);
  RTEMS_CHECK_RV(rv, "gpio_enable_interrupt");
  
  /* Keeps the program running, so interrupts can be tested */
  while (1);
  
  rtems_test_end ();
  exit ( 0 );
}

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_MAXIMUM_TASKS            1
#define CONFIGURE_USE_DEVFS_AS_BASE_FILESYSTEM

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INITIAL_EXTENSIONS RTEMS_TEST_INITIAL_EXTENSION

#define CONFIGURE_INIT
#include <rtems/confdefs.h>

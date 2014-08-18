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

/* forward declarations to avoid warnings */
rtems_task Init(rtems_task_argument argument);

const char rtems_test_name[] = "SPI_23k256_TEST";

rtems_task Init(
  rtems_task_argument ignored
)
{
  int rv = 0;
  int fd;

  char buf[32];

  rtems_test_begin ();
  
  fd = open("/dev/spi.23k256", O_RDWR);
  RTEMS_CHECK_RV(rv, "Open /dev/spi.23k256");
  
  rv = write(fd, "This message confirms that RTEMS can send and receive data using the SPI bus to the 23k256 SRAM device!", 104);

  printf("\nWrote %d chars\n", rv);
  
  rv = read(fd, buf, 104);

  printf("\nRead %d chars\n", rv);
  
  printf("\nRead data -> %s\n\n", buf);
    
  rv = close(fd);
  RTEMS_CHECK_RV(rv, "Close /dev/spi.23k256");

  rtems_test_end ();
  exit ( 0 );
}

#define CONFIGURE_MAXIMUM_DRIVERS 10
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM

#define CONFIGURE_MAXIMUM_SEMAPHORES 3

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS 30

#define CONFIGURE_MAXIMUM_TASKS 20

 #define CONFIGURE_INIT_TASK_STACK_SIZE (32 * 1024)

#define CONFIGURE_INITIAL_EXTENSIONS RTEMS_TEST_INITIAL_EXTENSION

#define CONFIGURE_INIT
#include <rtems/confdefs.h>

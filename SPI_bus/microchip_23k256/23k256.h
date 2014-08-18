#include <rtems.h>
#include <rtems/libi2c.h>

#include <rtems/libio.h>

#ifndef LIBI2C_23k256_H
#define LIBI2C_23k256_H

#ifdef __cplusplus
extern "C" {
#endif

rtems_status_code spi_23k256_write(rtems_device_major_number major, rtems_device_minor_number minor, void *arg);
rtems_status_code spi_23k256_read(rtems_device_major_number major, rtems_device_minor_number minor, void *arg);

extern rtems_driver_address_table spi_23k256_rw_ops;

extern rtems_libi2c_drv_t spi_23k256_rw_drv_t;

#ifdef __cplusplus
}
#endif

#endif /* LIBI2C_23k256_H */

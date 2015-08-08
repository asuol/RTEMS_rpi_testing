#ifndef LIBI2C_23k256_H
#define LIBI2C_23k256_H

#include <rtems.h>
#include <rtems/libi2c.h>
#include <rtems/libio.h>

#define SPI_23k256_ADDR 0x00

#ifdef __cplusplus
extern "C" {
#endif

extern int spi_libi2c_register_23k256(unsigned spi_bus);

#ifdef __cplusplus
}
#endif

#endif /* LIBI2C_23k256_H */

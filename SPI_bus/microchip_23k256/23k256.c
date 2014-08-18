#include <libchip/23k256.h>

#define SPI_MEM_CMD_WRITE 0x02 /* Write */
#define SPI_MEM_CMD_READ  0x03 /* Read */
#define SPI_MEM_CMD_RDSR  0x05 /* Read status register */
#define SPI_MEM_CMD_WRSR  0x01 /* Write status register */
#define SPI_MEM_CMD_SEQ   0x40

static rtems_libi2c_tfr_mode_t tfr_mode = 
{
  .baudrate      = 20000000, 
  .bits_per_char = 8, 
  .lsb_first     = FALSE,    
  .clock_inv     = TRUE,    
  .clock_phs     = FALSE
};

/* Wait ms miliseconds */
static rtems_status_code spi_23k256_wait_ms(int ms)
{
  rtems_interval ticks_per_second;

  ticks_per_second = rtems_clock_get_ticks_per_second();

  return rtems_task_wake_after((ticks_per_second * ms / 1000));
}

rtems_status_code spi_23k256_write(rtems_device_major_number major, rtems_device_minor_number minor, void *arg)
{
  rtems_status_code     sc = RTEMS_SUCCESSFUL;
  rtems_libio_rw_args_t *rwargs = arg;
  int                   cnt = rwargs->count;
  unsigned char         *buf = (unsigned char *)rwargs->buffer;
  int                   bytes_sent = 0;
  int                   curr_cnt;
  unsigned char         cmdbuf[4];
  int                   ret_cnt = 0;
  int                   cmd_size;

  /* Using hard-coded offset instead of rwargs->offset for testing purposes */
  int off = 1;

  while ( cnt > bytes_sent ) 
  {
    curr_cnt = cnt - bytes_sent;

    /* Start the bus */
    sc = rtems_libi2c_send_start(minor);
   
    if ( sc != RTEMS_SUCCESSFUL )
      return sc;

    /* Set transfer mode */
    sc = rtems_libi2c_ioctl(minor, RTEMS_LIBI2C_IOCTL_SET_TFRMODE, &tfr_mode);

    if ( sc != RTEMS_SUCCESSFUL )
      return sc;
    
    /* Address device */
    sc = rtems_libi2c_send_addr(minor,TRUE);

    if ( sc != RTEMS_SUCCESSFUL )
      return sc;

    /* Wait 1 milisecond */
    sc = spi_23k256_wait_ms(1);

    if ( sc != RTEMS_SUCCESSFUL )
      return sc;

    /*
     * Enable Sequential mode
     */
    cmdbuf[0] = SPI_MEM_CMD_WRSR;
   
    cmdbuf[1] = SPI_MEM_CMD_SEQ;
    
    cmd_size  = 2;

    ret_cnt = rtems_libi2c_write_bytes(minor,cmdbuf,cmd_size);

    if ( ret_cnt < 0 ) 
      return RTEMS_IO_ERROR;
      
    /* Terminate transfer */
    sc = rtems_libi2c_send_stop(minor);

    if ( sc != RTEMS_SUCCESSFUL )
      return sc;

    /* Wait 1 milisecond */
    sc = spi_23k256_wait_ms(1);

    if ( sc != RTEMS_SUCCESSFUL )
      return sc;

    /* Start the bus */
    sc = rtems_libi2c_send_start(minor);

    if ( sc != RTEMS_SUCCESSFUL )
      return sc;

    /* Address device */
    sc = rtems_libi2c_send_addr(minor,TRUE);

    if ( sc != RTEMS_SUCCESSFUL )
      return sc;

    /* Wait 1 milisecond */
    sc = spi_23k256_wait_ms(1);

    if ( sc != RTEMS_SUCCESSFUL )
      return sc;

    /*
     * Send write instruction and address
     */
    cmdbuf[0] = SPI_MEM_CMD_WRITE;
      
    cmdbuf[1] = (off >> 8) & 0xff;
    cmdbuf[2] = (off >> 0) & 0xff;
      
    cmd_size  = 3;

    ret_cnt = rtems_libi2c_write_bytes(minor,cmdbuf,cmd_size);
 
    if ( ret_cnt < 0 ) 
      return RTEMS_IO_ERROR;
    
    /* Send data */
    ret_cnt = rtems_libi2c_write_bytes(minor,buf,curr_cnt);

    if ( ret_cnt < 0 ) 
      return RTEMS_IO_ERROR;
    
    /* Terminate transfer */
    sc = rtems_libi2c_send_stop(minor);

    if ( sc != RTEMS_SUCCESSFUL )
      return sc;
    
    /* Wait 1 milisecond */
    sc = spi_23k256_wait_ms(1);

    if ( sc != RTEMS_SUCCESSFUL )
      return sc;

    /*
     * adjust bytecount to be sent and pointers
     */
    bytes_sent += curr_cnt;
    off        += curr_cnt;
    buf        += curr_cnt;
  }

  rwargs->bytes_moved = bytes_sent;

  return sc;
}

rtems_status_code spi_23k256_read(rtems_device_major_number major, rtems_device_minor_number minor, void *arg)
{
  rtems_status_code     sc = RTEMS_SUCCESSFUL;
  rtems_libio_rw_args_t *rwargs = arg;
  int                   cnt = rwargs->count;
  unsigned char         *buf = (unsigned char *)rwargs->buffer;
  unsigned char         cmdbuf[4];
  int                   ret_cnt = 0;
  int                   cmd_size;
    
  /* Using hard-coded offset instead of rwargs->offset for testing purposes */
  int off = 1;

  /* Start the bus */
    sc = rtems_libi2c_send_start(minor);

    if ( sc != RTEMS_SUCCESSFUL )
      return sc;

  /* Set transfer mode */
  sc = rtems_libi2c_ioctl(minor, RTEMS_LIBI2C_IOCTL_SET_TFRMODE, &tfr_mode);
     
  if ( sc != RTEMS_SUCCESSFUL )
      return sc;

  /* Address device */
  sc = rtems_libi2c_send_addr(minor,TRUE);

  if ( sc != RTEMS_SUCCESSFUL )
      return sc;
  
  /* Wait 1 milisecond */
  sc = spi_23k256_wait_ms(1);

  if ( sc != RTEMS_SUCCESSFUL )
   return sc;

  /*
   * Send read command and address
   */

  cmdbuf[0] = SPI_MEM_CMD_READ;

  cmdbuf[1] = (off >> 8) & 0xff;
  cmdbuf[2] = (off >> 0) & 0xff;
     
  cmd_size  = 3;

  ret_cnt = rtems_libi2c_write_bytes(minor,cmdbuf,cmd_size);

  if ( ret_cnt < 0 ) 
     return RTEMS_IO_ERROR;
    
  /* Fetch data */
  ret_cnt = rtems_libi2c_read_bytes (minor,buf,cnt);
   
  if ( ret_cnt < 0 ) 
      return RTEMS_IO_ERROR;

  /* Terminate transfer */
  sc = rtems_libi2c_send_stop(minor);
  
  if ( sc != RTEMS_SUCCESSFUL )
   return sc;

  rwargs->bytes_moved = (sc == RTEMS_SUCCESSFUL) ? ret_cnt : 0;

  return sc;
}

rtems_driver_address_table spi_23k256_rw_ops = 
{
  .read_entry  = spi_23k256_read,
  .write_entry = spi_23k256_write
};

rtems_libi2c_drv_t spi_23k256_rw_drv_t =
{
    .ops  = &spi_23k256_rw_ops, 
    .size = sizeof (spi_23k256_rw_drv_t),
};

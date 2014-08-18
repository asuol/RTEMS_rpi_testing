
rtems_tests_PROGRAMS = SPI_23k256_TEST
SPI_23k256_TEST_SOURCES = init.c

#dist_rtems_tests_DATA = SPI_23k256_TEST.scn
#dist_rtems_tests_DATA += SPI_23k256_TEST.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am


LINK_OBJS = $(SPI_23k256_TEST_OBJECTS)
LINK_LIBS = $(SPI_23k256_TEST_LDLIBS)

SPI_23k256_TEST$(EXEEXT): $(SPI_23k256_TEST_OBJECTS) $(SPI_23k256_TEST_DEPENDENCIES)
	@rm -f SPI_23k256_TEST$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am

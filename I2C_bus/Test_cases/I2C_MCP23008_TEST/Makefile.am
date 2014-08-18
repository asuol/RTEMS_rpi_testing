
rtems_tests_PROGRAMS = I2C_MCP23008_TEST
I2C_MCP23008_TEST_SOURCES = init.c

#dist_rtems_tests_DATA = I2C_MCP23008_TEST.scn
#dist_rtems_tests_DATA += I2C_MCP23008_TEST.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am


LINK_OBJS = $(I2C_MCP23008_TEST_OBJECTS)
LINK_LIBS = $(I2C_MCP23008_TEST_LDLIBS)

I2C_MCP23008_TEST$(EXEEXT): $(I2C_MCP23008_TEST_OBJECTS) $(I2C_MCP23008_TEST_DEPENDENCIES)
	@rm -f I2C_MCP23008_TEST$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am

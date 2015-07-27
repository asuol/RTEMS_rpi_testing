
rtems_tests_PROGRAMS = LIBGPIO_JTAG
LIBGPIO_JTAG_SOURCES = init.c

#dist_rtems_tests_DATA = LIBGPIO_JTAG.scn
#dist_rtems_tests_DATA += LIBGPIO_JTAG.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am


LINK_OBJS = $(LIBGPIO_JTAG_OBJECTS)
LINK_LIBS = $(LIBGPIO_JTAG_LDLIBS)

LIBGPIO_JTAG$(EXEEXT): $(LIBGPIO_JTAG_OBJECTS) $(LIBGPIO_JTAG_DEPENDENCIES)
	@rm -f LIBGPIO_JTAG$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am

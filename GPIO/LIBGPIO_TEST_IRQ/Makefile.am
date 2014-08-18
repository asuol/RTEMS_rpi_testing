
rtems_tests_PROGRAMS = LIBGPIO_TEST_IRQ
LIBGPIO_TEST_IRQ_SOURCES = init.c

#dist_rtems_tests_DATA = LIBGPIO_TEST_IRQ.scn
#dist_rtems_tests_DATA += LIBGPIO_TEST_IRQ.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am


LINK_OBJS = $(LIBGPIO_TEST_IRQ_OBJECTS)
LINK_LIBS = $(LIBGPIO_TEST_IRQ_LDLIBS)

LIBGPIO_TEST_IRQ$(EXEEXT): $(LIBGPIO_TEST_IRQ_OBJECTS) $(LIBGPIO_TEST_IRQ_DEPENDENCIES)
	@rm -f LIBGPIO_TEST_IRQ$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am

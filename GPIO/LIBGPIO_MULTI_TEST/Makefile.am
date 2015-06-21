
rtems_tests_PROGRAMS = LIBGPIO_MULTI_TEST
LIBGPIO_MULTI_TEST_SOURCES = init.c

#dist_rtems_tests_DATA = LIBGPIO_MULTI_TEST.scn
#dist_rtems_tests_DATA += LIBGPIO_MULTI_TEST.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am


LINK_OBJS = $(LIBGPIO_MULTI_TEST_OBJECTS)
LINK_LIBS = $(LIBGPIO_MULTI_TEST_LDLIBS)

LIBGPIO_MULTI_TEST$(EXEEXT): $(LIBGPIO_MULTI_TEST_OBJECTS) $(LIBGPIO_MULTI_TEST_DEPENDENCIES)
	@rm -f LIBGPIO_MULTI_TEST$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am

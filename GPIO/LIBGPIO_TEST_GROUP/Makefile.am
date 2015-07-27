
rtems_tests_PROGRAMS = LIBGPIO_TEST_GROUP
LIBGPIO_TEST_GROUP_SOURCES = init.c

#dist_rtems_tests_DATA = LIBGPIO_TEST_GROUP.scn
#dist_rtems_tests_DATA += LIBGPIO_TEST_GROUP.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am


LINK_OBJS = $(LIBGPIO_TEST_GROUP_OBJECTS)
LINK_LIBS = $(LIBGPIO_TEST_GROUP_LDLIBS)

LIBGPIO_TEST_GROUP$(EXEEXT): $(LIBGPIO_TEST_GROUP_OBJECTS) $(LIBGPIO_TEST_GROUP_DEPENDENCIES)
	@rm -f LIBGPIO_TEST_GROUP$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am

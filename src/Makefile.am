## Process this file with automake to produce Makefile.in

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@PACKAGE_CFLAGS@

bin_PROGRAMS = parky

parky_SOURCES = \
	main.c \
	support.c support.h \
	interface.c interface.h \
	callbacks.c callbacks.h \
	citizen.c citizen.h \
	reservation.c reservation.h \
	parking.c parking.h \
	agent.c agent.h \
	service.c service.h \
	common.c common.h \
	admin.c admin.h

parky_LDADD = @PACKAGE_LIBS@ $(INTLLIBS)


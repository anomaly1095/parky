## Process this file with automake to produce Makefile.in

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@PACKAGE_CFLAGS@

bin_PROGRAMS = parky

parky_SOURCES = \
	main.c main.h\
	support.c support.h \
	interface.c interface.h \
	callbacks.c callbacks.h \
	citizen.c citizen.h \
	parking.c parking.h \
	agent.c agent.h \
	reservation.c reservation.h \
	service.c service.h \


parky_LDADD = @PACKAGE_LIBS@ $(INTLLIBS)


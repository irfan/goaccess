#AUTOMAKE_OPTIONS = foreign
bin_PROGRAMS = goaccess

confdir = $(sysconfdir)
dist_conf_DATA = config/goaccess.conf

goaccess_SOURCES = \
   src/browsers.c      \
   src/browsers.h      \
   src/commons.c       \
   src/commons.h       \
   src/csv.c           \
   src/csv.h           \
   src/error.c         \
   src/error.h         \
   src/gdashboard.c    \
   src/gdashboard.h    \
   src/gdns.c          \
   src/gdns.h          \
   src/gmenu.c         \
   src/gmenu.h         \
   src/goaccess.c      \
   src/goaccess.h      \
   src/json.c          \
   src/json.h          \
   src/opesys.c        \
   src/opesys.h        \
   src/options.c       \
   src/options.h       \
   src/output.c        \
   src/output.h        \
   src/parser.c        \
   src/parser.h        \
   src/sort.c          \
   src/sort.h          \
   src/settings.c      \
   src/settings.h      \
   src/ui.c            \
   src/ui.h            \
   src/util.c          \
   src/util.h          \
   src/xmalloc.c       \
   src/xmalloc.h

if TCB
goaccess_SOURCES += \
   src/tcabinet.c       \
   src/tcabinet.h
else
goaccess_SOURCES += \
   src/glibht.c         \
   src/glibht.h
endif

if GEOLOCATION
goaccess_SOURCES += \
   src/geolocation.c    \
   src/geolocation.h
endif

if DEBUG
AM_CFLAGS = -DDEBUG -O0 -g -DSYSCONFDIR=\"$(sysconfdir)\"
else
AM_CFLAGS = -O2 -DSYSCONFDIR=\"$(sysconfdir)\"
endif

AM_LDFLAGS = -rdynamic

AM_CFLAGS += @GLIB2_CFLAGS@
AM_CFLAGS += -Wno-long-long -Wall -W -Wnested-externs -Wformat=2
AM_CFLAGS += -Wmissing-prototypes -Wstrict-prototypes -Wmissing-declarations
AM_CFLAGS += -Wwrite-strings -Wshadow -Wpointer-arith -Wsign-compare
AM_CFLAGS += -Wredundant-decls -Wbad-function-cast -Winline -Wcast-align -Wextra
AM_CFLAGS += -Wdeclaration-after-statement -Wno-missing-field-initializers

goaccess_LDADD = -lm
dist_man_MANS = goaccess.1

AC_PREREQ([2.58])
dnl PLEASE remove the nano when doing a release
AC_INIT(schroedinger,0.9.0.1)

AS_NANO(SCHRO_CVS=no,SCHRO_CVS=yes)

AC_CANONICAL_HOST([])
AM_INIT_AUTOMAKE(1.6)

SCHRO_MAJORMINOR=0.9
AC_SUBST(SCHRO_MAJORMINOR)

AM_CONFIG_HEADER(config.h)

AC_DEFINE_UNQUOTED(DIRAC_COMPAT, 1, [Enable Dirac compatibility])

dnl CURRENT, REVISION, AGE
dnl - library source changed -> increment REVISION
dnl - interfaces added/removed/changed -> increment CURRENT, REVISION = 0
dnl - interfaces added -> increment AGE
dnl - interfaces removed -> AGE = 0
SCHRO_LIBVERSION="0:0:0"
AC_SUBST(SCHRO_LIBVERSION)
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL
AM_PROG_AS
AM_PROG_CC_C_O

dnl allow for different autotools
AS_AUTOTOOLS_ALTERNATE

AC_CONFIG_SRCDIR([schroedinger/schro.h])

ACLOCAL_AMFLAGS="-I m4 $ACLOCAL_AMFLAGS"
AC_SUBST(ACLOCAL_AMFLAGS)

dnl set up gettext
dnl the version check needs to stay here because autopoint greps for it
#AM_GNU_GETTEXT_VERSION([0.11.5])
#AM_GNU_GETTEXT([external])

dnl liboil is required
OIL_VER="0.3.12"
PKG_CHECK_MODULES(LIBOIL, liboil-0.3 >= $OIL_VER, HAVE_LIBOIL=yes, HAVE_LIBOIL=no)
if test "x${HAVE_LIBOIL}" != xyes ; then
  AC_ERROR([liboil-0.3 >= $OIL_VER is required])
fi

##################################################
# Check for gtk-doc.
##################################################

if test "x$cross_compiling" = "xyes" ; then
  enable_gtk_doc=no
fi
GTK_DOC_CHECK([1.0])

AX_CREATE_STDINT_H([schroedinger/schro-stdint.h])

AS_COMPILER_FLAG(-Wall, SCHRO_CFLAGS="$SCHRO_CFLAGS -Wall")
if test "x$SCHRO_CVS" = "xyes"
then
  AS_COMPILER_FLAG(-Werror, SCHRO_CFLAGS="$SCHRO_CFLAGS -Werror")
fi

# Enable severe pain
#COMPANY_CFLAGS="-Wextra -Wno-missing-field-initializers -Wno-unused-parameter -Wold-style-definition -Wdeclaration-after-statement -Wmissing-declarations -Wmissing-prototypes -Wredundant-decls -Wmissing-noreturn -Wshadow -Wpointer-arith -Wcast-align -Wwrite-strings -Winline -Wformat-nonliteral -Wformat-security -Wswitch-enum -Wswitch-default"
#SCHRO_CFLAGS="$SCHRO_CFLAGS $COMPANY_CFLAGS"

SCHRO_CFLAGS="$SCHRO_CFLAGS -I\$(top_srcdir)"
AC_SUBST(SCHRO_CFLAGS)

SCHRO_LIBS="\$(top_builddir)/schroedinger/libschroedinger-$SCHRO_MAJORMINOR.la"
# hack around ld bug
SCHRO_LIBS="$SCHRO_LIBS $LIBOIL_LIBS -lpthread"
#SCHRO_LIBS="$SCHRO_LIBS -lprofiler"
AC_SUBST(SCHRO_LIBS)

########################
# GStreamer stuff
########################

GST_MAJORMINOR=0.10

dnl *** check for arguments to configure ***

AC_ARG_ENABLE(gstreamer, AC_HELP_STRING([--disable-gstreamer],
              [disable gstreamer support (default=enabled)]))
if test "x$enable_gstreamer" != xno
then
  PKG_CHECK_MODULES(GST, gstreamer-0.10 >= 0.10.12 gstreamer-base-0.10 gstreamer-plugins-base-0.10,
      HAVE_GST=yes, HAVE_GST=no)
else
  HAVE_GST=no
fi

AM_CONDITIONAL(HAVE_GST, test "x$HAVE_GST" = "xyes")
AC_SUBST(GST_CFLAGS)
AC_SUBST(GST_LIBS)

plugindir="\$(libdir)/gstreamer-$GST_MAJORMINOR"
AC_SUBST(plugindir)

GST_ALL_LDFLAGS="-no-undefined"
AC_SUBST(GST_ALL_LDFLAGS)

GST_PLUGIN_LDFLAGS="-module -avoid-version -export-symbols-regex '^[_]*gst_plugin_desc\$\$' $GST_ALL_LDFLAGS"
AC_SUBST(GST_PLUGIN_LDFLAGS)

AC_DEFINE(GST_PACKAGE_ORIGIN, "unknown", [Package origin in GStreamer plugin])

########################
# Check for CUDA
########################
AC_ARG_WITH([cuda],
    AS_HELP_STRING([--with-cuda=PATH],[prefix where cuda is installed [default=no]]),
    [],
    [with_cuda=no])
if test "x$with_cuda" = xyes; then
  AC_PATH_PROG([NVCC], [nvcc], no, [$PATH:/usr/local/cuda/bin])
  if test "x$NVCC" = xno ; then
    AC_MSG_FAILURE(["--with-cuda was give, but nvcc was not found"])
  fi
  with_cuda=`echo $NVCC|sed 's,bin/nvcc,,'`
fi

if test "x$with_cuda" != xno; then
  CUDA_CFLAGS="-I$with_cuda/include"
  CUDA_LIBS="-L$with_cuda/lib -lcuda -lcudart"
  NVCC="$with_cuda/bin/nvcc"
  NVCCFLAGS="-O2 -use_fast_math"
  HAVE_CUDA=yes
  AC_DEFINE(HAVE_CUDA, 1, [Compile in CUDA rendering backend])
  AC_DEFINE(SCHRO_GPU, 1, [Compile in CUDA rendering backend])
else
  CUDA_CFLAGS=
  CUDA_LIBS=
  NVCC=
  NVCCFLAGS=
  HAVE_CUDA=no
fi
AC_SUBST(CUDA_CFLAGS)
AC_SUBST(CUDA_LIBS)
AC_SUBST(NVCC)
AC_SUBST(NVCCFLAGS)
AM_CONDITIONAL(HAVE_CUDA, test "x$HAVE_CUDA" = "xyes")

#
# Check for check
#
AM_PATH_CHECK(0.9.2, HAVE_CHECK=yes, HAVE_CHECK=no)
AM_CONDITIONAL(HAVE_CHECK, test "x$HAVE_CHECK" = "xyes")

AM_CONDITIONAL(BUILD_BROKEN_TESTS, false)

AS_HOST_DEFINES()
	
pkgconfigdir="\$(libdir)/pkgconfig"
AC_SUBST(pkgconfigdir)

AC_CONFIG_FILES([
Makefile 
doc/Makefile
schroedinger/Makefile
schroedinger/cuda/Makefile
gst/Makefile
testsuite/Makefile
testsuite/check/Makefile
testsuite/perf/Makefile
testsuite/coder/Makefile
testsuite/dirac/Makefile
testsuite/installation/Makefile
testsuite/manual/Makefile
testsuite/prototype/Makefile
testsuite/streams/Makefile
testsuite/tables2/Makefile
tools/Makefile
schroedinger-uninstalled.pc
schroedinger.pc
schroedinger.spec
])
AC_OUTPUT

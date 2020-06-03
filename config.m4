dnl $Id$
dnl config.m4 for extension donkeyid

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(donkeyid, for donkeyid support,
dnl Make sure that the comment is aligned:
[  --with-donkeyid             Include donkeyid support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(donkeyid, whether to enable donkeyid support,
dnl Make sure that the comment is aligned:
dnl [  --enable-donkeyid           Enable donkeyid support])

if test "$PHP_DONKEYID" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-donkeyid -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/donkeyid.h"  # you most likely want to change this
  dnl if test -r $PHP_DONKEYID/$SEARCH_FOR; then # path given as parameter
  dnl   DONKEYID_DIR=$PHP_DONKEYID
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for donkeyid files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       DONKEYID_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$DONKEYID_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the donkeyid distribution])
  dnl fi

  dnl # --with-donkeyid -> add include path
  dnl PHP_ADD_INCLUDE($DONKEYID_DIR/include)

  dnl # --with-donkeyid -> check for lib and symbol presence
  dnl LIBNAME=donkeyid # you may want to change this
  dnl LIBSYMBOL=donkeyid # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $DONKEYID_DIR/$PHP_LIBDIR, DONKEYID_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_DONKEYIDLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong donkeyid lib version or lib not found])
  dnl ],[
  dnl   -L$DONKEYID_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(DONKEYID_SHARED_LIBADD)

  donkeyid_source_file="donkeyid.c \
        src/donkeyid.c \
        src/shm.c \
        src/spinlock.c"
  PHP_NEW_EXTENSION(donkeyid, $donkeyid_source_file, $ext_shared)
fi

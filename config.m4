dnl config.m4 for extension yazstd

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(yazstd, for yazstd support,
dnl Make sure that the comment is aligned:
dnl [  --with-yazstd             Include yazstd support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(yazstd, whether to enable yazstd support,
dnl Make sure that the comment is aligned:
[  --enable-yazstd          Enable yazstd support], no)

PHP_ARG_WITH(libzstd, whether to use system zstd library,
[  --with-libzstd           Use system zstd library], no, no)


if test "$PHP_YAZSTD" != "no"; then

  if test "$PHP_LIBZSTD" != "no"; then
    AC_PATH_PROG(PKG_CONFIG, pkg-config, no)

    AC_MSG_CHECKING(for libzstd)
    if test -x "$PKG_CONFIG" && $PKG_CONFIG --exists libzstd; then
      if $PKG_CONFIG libzstd --atleast-version 1; then
        LIBZSTD_CFLAGS=`$PKG_CONFIG libzstd --cflags`
        LIBZSTD_LIBDIR=`$PKG_CONFIG libzstd --libs`
        LIBZSTD_VERSON=`$PKG_CONFIG libzstd --modversion`
        AC_MSG_RESULT(from pkgconfig: version $LIBZSTD_VERSON)
      else
        AC_MSG_ERROR(system libzstd is too old)
      fi
    else
      AC_MSG_ERROR(pkg-config not found)
    fi
    PHP_EVAL_LIBLINE($LIBZSTD_LIBDIR, ZSTD_SHARED_LIBADD)
    PHP_EVAL_INCLINE($LIBZSTD_CFLAGS)
  else
    ZSTD_COMMON_SOURCES="
      zstd/lib/common/debug.c
      zstd/lib/common/entropy_common.c
      zstd/lib/common/error_private.c
      zstd/lib/common/fse_decompress.c
      zstd/lib/common/pool.c
      zstd/lib/common/threading.c
      zstd/lib/common/xxhash.c
      zstd/lib/common/zstd_common.c
    "
    ZSTD_COMPRESS_SOURCES="
      zstd/lib/compress/fse_compress.c
      zstd/lib/compress/hist.c
      zstd/lib/compress/huf_compress.c
      zstd/lib/compress/zstd_compress.c
      zstd/lib/compress/zstd_double_fast.c
      zstd/lib/compress/zstd_fast.c
      zstd/lib/compress/zstd_lazy.c
      zstd/lib/compress/zstd_ldm.c
      zstd/lib/compress/zstd_opt.c
      zstd/lib/compress/zstdmt_compress.c
    "
    ZSTD_DECOMPRESS_SOURCES="
      zstd/lib/decompress/huf_decompress.c
      zstd/lib/decompress/zstd_ddict.c
      zstd/lib/decompress/zstd_decompress.c
      zstd/lib/decompress/zstd_decompress_block.c
    "

    PHP_ADD_INCLUDE(PHP_EXT_SRCDIR()/zstd/lib/common)
    PHP_ADD_INCLUDE(PHP_EXT_SRCDIR()/zstd/lib)
  fi  
  AC_DEFINE(HAVE_YAZSTD, 1, [ Have yazstd support ])

  PHP_NEW_EXTENSION(yazstd, yazstd.c $ZSTD_COMMON_SOURCES $ZSTD_COMPRESS_SOURCES $ZSTD_DECOMPRESS_SOURCES, $ext_shared)
  PHP_SUBST(ZSTD_SHARED_LIBADD)

  if test "$PHP_LIBZSTD" = "no"; then
    PHP_ADD_BUILD_DIR($ext_builddir/zstd/lib/common)
    PHP_ADD_BUILD_DIR($ext_builddir/zstd/lib/compress)
    PHP_ADD_BUILD_DIR($ext_builddir/zstd/lib/decompress)
  fi

fi

/* yazstd extension for PHP */

#ifndef PHP_YAZSTD_H
# define PHP_YAZSTD_H

extern zend_module_entry yazstd_module_entry;
# define phpext_yazstd_ptr &yazstd_module_entry

# define PHP_YAZSTD_VERSION "0.2.0"

# if defined(ZTS) && defined(COMPILE_DL_YAZSTD)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_YAZSTD_H */


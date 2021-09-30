/* yazstd extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_yazstd.h"

#include <zstd.h>

#ifndef ZEND_THIS
# define ZEND_THIS &EX(This)
#endif

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

static zend_object_handlers yazstd_decompress_object_handlers;

typedef struct _yazstd_decompress_data {
    char *bufout;
    size_t sizeout;
    ZSTD_DCtx* dctx;
	int test;
	zend_object std;
} yazstd_decompress_data;

#define Z_DECMPRESS_P(zv) \
    ((yazstd_decompress_data*)((char*)(Z_OBJ_P(zv)) - XtOffsetOf(yazstd_decompress_data, std)))
	
	
zend_object *yazstd_decompress_data_new(zend_class_entry *ce)
{
    yazstd_decompress_data *yazstd_ref = zend_object_alloc(sizeof(yazstd_decompress_data), ce);

    zend_object_std_init(&yazstd_ref->std, ce);
    yazstd_ref->std.handlers = &yazstd_decompress_object_handlers;
    return &yazstd_ref->std;
}


static void yazstd_decompress_free_storage(zend_object *object TSRMLS_DC)
{
	yazstd_decompress_data *yazstd_ref = (yazstd_decompress_data *) ((char *) object - XtOffsetOf(yazstd_decompress_data, std));
	ZSTD_freeDCtx(yazstd_ref->dctx);
	efree(yazstd_ref->bufout);
}
	

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(yazstd)
{
#if defined(ZTS) && defined(COMPILE_DL_YAZSTD)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(yazstd)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "yazstd support", "enabled");
	php_info_print_table_row(2, "Extension Version", PHP_YAZSTD_VERSION);
	php_info_print_table_row(2, "libzstd Version", ZSTD_VERSION_STRING);
	php_info_print_table_end();
}
/* }}} */

/* {{{ arginfo
 */

ZEND_BEGIN_ARG_INFO_EX(yazstd_decompress_decompress_arg, 0, 0, 1)
    ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

/* }}} */

/* {{{ yazstd_functions[]
 */

ZEND_METHOD( yazstd_decompress, __construct)
{
	yazstd_decompress_data *yazstd_objref = Z_DECMPRESS_P(ZEND_THIS);
	yazstd_objref->dctx = ZSTD_createDCtx();
	yazstd_objref->bufout = emalloc(yazstd_objref->sizeout = ZSTD_DStreamOutSize());
	
}


ZEND_METHOD( yazstd_decompress, decompress)
{
	zval *data;
	size_t result;
	yazstd_decompress_data *yazstd_objref = Z_DECMPRESS_P(ZEND_THIS);
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
                              "z", &data) == FAILURE) {
        RETURN_FALSE;
    }
	ZSTD_inBuffer input = { NULL, 0, 0 };
	input.src = Z_STRVAL_P(data);
    input.size = Z_STRLEN_P(data);
    input.pos = 0;
	
	ZSTD_outBuffer output = { NULL, 0, 0 };
	output.dst = yazstd_objref->bufout;
	output.size = yazstd_objref->sizeout;
	
	while (input.pos < input.size) {
            if (output.pos == output.size) {
                output.size += yazstd_objref->sizeout;
                yazstd_objref->bufout = erealloc(yazstd_objref->bufout, output.size);
                output.dst = yazstd_objref->bufout;
            }

            result = ZSTD_decompressStream(yazstd_objref->dctx, &output, &input);
            if (ZSTD_isError(result)) {
                //efree(output);
                //ZSTD_freeDStream(stream);
                zend_error(E_WARNING,
                           "zstd_uncompress: can not decompress stream");
                RETURN_FALSE;
            }

            if (result == 0) {
                break;
            }
    }
	
#if ZEND_MODULE_API_NO >= 20141001
        RETVAL_STRINGL(yazstd_objref->bufout, output.pos);
#else
        RETVAL_STRINGL(yazstd_objref->bufout, output.pos, 1);
#endif
}


zend_class_entry * yazstd_ce; 

static zend_function_entry yazstd_decompress_method [] = {
	ZEND_ME( yazstd_decompress, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	ZEND_ME( yazstd_decompress, decompress, yazstd_decompress_decompress_arg, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

PHP_MINIT_FUNCTION(yazstd)
{
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce,"yazstd_decompress",yazstd_decompress_method);
	yazstd_ce = zend_register_internal_class(&ce TSRMLS_CC);
	yazstd_ce->create_object = yazstd_decompress_data_new;
	memcpy(&yazstd_decompress_object_handlers, &std_object_handlers,
        sizeof(zend_object_handlers));
    yazstd_decompress_object_handlers.offset = XtOffsetOf(yazstd_decompress_data, std);
	yazstd_decompress_object_handlers.free_obj = yazstd_decompress_free_storage;
	return SUCCESS;
}


/* }}} */

/* {{{ yazstd_module_entry
 */
zend_module_entry yazstd_module_entry = {
	STANDARD_MODULE_HEADER,
	"yazstd",					/* Extension name */
	NULL,						/* zend_function_entry */
	ZEND_MINIT(yazstd),			/* PHP_MINIT - Module initialization */
	NULL,							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(yazstd),			/* PHP_RINIT - Request initialization */
	NULL,							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(yazstd),			/* PHP_MINFO - Module info */
	PHP_YAZSTD_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_YAZSTD
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(yazstd)
#endif


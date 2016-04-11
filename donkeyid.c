/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2015 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_donkeyid.h"
#include "donkey_id.h"
#include <stdint.h>
#include <inttypes.h>


static int64_t S64(const char *s) {
	  int64_t i;
	  char c ;
	  int scanned = sscanf(s, "%" SCNd64 "%c", &i, &c);
	  if (scanned == 1) return i;
	  if (scanned > 1) {
		// TBD about extra data found
		return i;
		}
	  // TBD failed to scan;
	  return 0;
}

/* If you declare any globals in php_donkeyid.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(donkeyid)
*/

/* True global resources - no need for thread safety here */
static int le_donkeyid;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("donkeyid.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_donkeyid_globals, donkeyid_globals)
    STD_PHP_INI_ENTRY("donkeyid.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_donkeyid_globals, donkeyid_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */
/* {{{ donkeyid_functions[]
 *
 * Every user visible function must have an entry in donkeyid_functions[].
 */
const zend_function_entry donkeyid_functions[] = {
	//PHP_ME(PHP_DONKEYID_CLASS_NAME, __construct, NULL,  ZEND_ACC_PUBLIC)
	PHP_ME(PHP_DONKEYID_CLASS_NAME, get_next_id, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(PHP_DONKEYID_CLASS_NAME, get_time_by_donkeyid, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(PHP_DONKEYID_CLASS_NAME, get_node_by_donkeyid, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(PHP_DONKEYID_CLASS_NAME, set_node_id, NULL, ZEND_ACC_PUBLIC)
	PHP_FE(confirm_donkeyid_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in donkeyid_functions[] */
};
/* }}} */

/* {{{ donkeyid_module_entry
 */
zend_module_entry donkeyid_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"donkeyid",
	donkeyid_functions,
	PHP_MINIT(donkeyid),
	PHP_MSHUTDOWN(donkeyid),
	PHP_RINIT(donkeyid),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(donkeyid),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(donkeyid),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_DONKEYID_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};

/* }}} */

#ifdef COMPILE_DL_DONKEYID
ZEND_GET_MODULE(donkeyid)
#endif


/* }}} */



/* {{{ php_donkeyid_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_donkeyid_init_globals(zend_donkeyid_globals *donkeyid_globals)
{
	donkeyid_globals->global_value = 0;
	donkeyid_globals->global_string = NULL;
}
*/
/* }}} */

//global class
zend_class_entry *donkeyid_ce;


/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(donkeyid)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	zend_class_entry donkeyid_class_entry;
	INIT_CLASS_ENTRY(donkeyid_class_entry,PHP_DONKEYID_CLASS_NAME,donkeyid_functions);
	donkeyid_ce  =  zend_register_internal_class(&donkeyid_class_entry TSRMLS_CC);

	//init error
	zend_declare_property_null(donkeyid_ce,ZEND_STRL("error"),ZEND_ACC_PRIVATE TSRMLS_CC);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(donkeyid)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(donkeyid)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(donkeyid)
{
	return SUCCESS;
}
/* }}} */




/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(donkeyid)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "donkeyid support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_donkeyid_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_donkeyid_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "donkeyid", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/



/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/
//PHP_METHOD(PHP_DONKEYID_CLASS_NAME,__construct)
//{
//	zval* self = getThis();
//	char *val = NULL;
//	int val_len;
//
//	//获取类方法的参数
//	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"s",&val,&val_len) == FAILURE){
//		return;
//	}
//}

PHP_METHOD(PHP_DONKEYID_CLASS_NAME,set_node_id)
{
		long nodeid;

		//获取类方法的参数
		if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"l",&nodeid) == FAILURE){
			return;
		}
		if(nodeid < 0 && nodeid > 255){
			RETURN_FALSE;
		}
		set_node_id(nodeid);
		RETURN_TRUE;
}

PHP_METHOD(PHP_DONKEYID_CLASS_NAME,get_next_id)
{
	zval *self = getThis();
	char  buffer[20];

	uint64_t donkeyid = get_donkey_id();
	sprintf(buffer,"%"PRIu64 ,donkeyid);
	RETVAL_STRING(buffer,1);
}
PHP_METHOD(PHP_DONKEYID_CLASS_NAME,get_time_by_donkeyid)
{
		char *val = NULL;
		int val_len;

		//获取类方法的参数
		if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"s",&val,&val_len) == FAILURE){
			return;
		}
		int64_t id = S64(val);
		if(id == 0){
			RETURN_FALSE;
		}
		int64_t time = GET_TIME_BY_DONKEYID(id);
		char buffer[20];
		sprintf(buffer,"%"PRIu64 ,time);
		RETVAL_STRING(buffer,1);
}
PHP_METHOD(PHP_DONKEYID_CLASS_NAME,get_node_by_donkeyid)
{
		char *val = NULL;
		int val_len;

		//获取类方法的参数
		if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"s",&val,&val_len) == FAILURE){
			return;
		}
		int64_t id = S64(val);
		if(id == 0){
			RETURN_FALSE;
		}
		int64_t time = GET_NODE_BY_DONKEYID(id);
		char buffer[20];
		sprintf(buffer,"%"PRIu64 ,time);
		RETVAL_STRING(buffer,1);
}


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

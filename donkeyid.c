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
#include "src/donkeyid.h"
#include <inttypes.h>
#include <main/SAPI.h>


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
        PHP_ME(PHP_DONKEYID_CLASS_NAME, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
        PHP_ME(PHP_DONKEYID_CLASS_NAME, __destruct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
        PHP_ME(PHP_DONKEYID_CLASS_NAME, getNextId, NULL, ZEND_ACC_PUBLIC)
        PHP_ME(PHP_DONKEYID_CLASS_NAME, parseTime, NULL, ZEND_ACC_PUBLIC)
        PHP_ME(PHP_DONKEYID_CLASS_NAME, parseNodeId, NULL, ZEND_ACC_PUBLIC)
        PHP_ME(PHP_DONKEYID_CLASS_NAME, setNodeId, NULL, ZEND_ACC_PUBLIC)
        PHP_ME(PHP_DONKEYID_CLASS_NAME, parseWorkerId, NULL, ZEND_ACC_PUBLIC)
        PHP_ME(PHP_DONKEYID_CLASS_NAME, parseSequence, NULL, ZEND_ACC_PUBLIC)
        PHP_FE_END    /* Must be the last line in donkeyid_functions[] */
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
        PHP_RINIT(donkeyid),        /* Replace with NULL if there's nothing to do at request start */
        PHP_RSHUTDOWN(donkeyid),    /* Replace with NULL if there's nothing to do at request end */
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
PHP_MINIT_FUNCTION (donkeyid) {
    /* If you have INI entries, uncomment these lines
    REGISTER_INI_ENTRIES();
    */
    zend_class_entry donkeyid_class_entry;
    INIT_CLASS_ENTRY(donkeyid_class_entry, PHP_DONKEYID_CLASS_NAME, donkeyid_functions);
    donkeyid_ce = zend_register_internal_class(&donkeyid_class_entry TSRMLS_CC);
    //init error
    zend_declare_property_null(donkeyid_ce, ZEND_STRL("error"), ZEND_ACC_PRIVATE TSRMLS_CC);
    //根据运行环境初始化不同的内存使用方式
    if (!strcasecmp(sapi_module.name,"cli")){
        donkeyid_init(0);
    }else{
        donkeyid_init(1);
    }
    atexit(donkeyid_atexit);
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION (donkeyid) {
    /* uncomment this line if you have INI entries
    UNREGISTER_INI_ENTRIES();
    */
    donkeyid_shutdown();
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION (donkeyid) {
    donkeyid_set_worker_id();
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION (donkeyid) {
    return SUCCESS;
}
/* }}} */




/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION (donkeyid) {
    php_info_print_table_start();
    php_info_print_table_header(2, "DonkeyId support", "enabled");
    php_info_print_table_row(2, "Version", PHP_DONKEYID_VERSION);
    php_info_print_table_row(2, "Author", "zhiming.liu[email: 187231450@qq.com]");
    php_info_print_table_end();

    /* Remove comments if you have entries in php.ini
    DISPLAY_INI_ENTRIES();
    */
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
PHP_METHOD(PHP_DONKEYID_CLASS_NAME,__construct)
{
	long type = 0;
    char *val = 0;
    int val_len;
	//获取类方法的参数
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"|ls",&type,&val,&val_len) == FAILURE){
        RETURN_FALSE;
	}
    //不是1就是0
    if(type<0 && type > 1){
        type = 0;
    }
    zend_update_property_long(donkeyid_ce, getThis(), ZEND_STRL("type"), type TSRMLS_CC);
    donkeyid_set_type((int)type);
    //设置纪元
    __time_t epoch = strtoul(val,NULL,10);
    if (epoch < 0 && epoch >= get_curr_timestamp()) {
        epoch = 0LLU;
    }
    donkeyid_set_epoch(epoch);
    zend_update_property_long(donkeyid_ce, getThis(), ZEND_STRL("epoch"), epoch TSRMLS_CC);
}
PHP_METHOD(PHP_DONKEYID_CLASS_NAME,__destruct)
{
    return;
}

PHP_METHOD (PHP_DONKEYID_CLASS_NAME, setNodeId) {

    long nodeid;
    //获取类方法的参数
    if (zend_parse_parameters(ZEND_NUM_ARGS()TSRMLS_CC, "l", &nodeid) == FAILURE) {
        return;
    }
    if (nodeid < 0 && nodeid > 255) {
        RETURN_FALSE;
    }
    donkeyid_set_node_id((int)nodeid);
    RETURN_TRUE;
}

PHP_METHOD (PHP_DONKEYID_CLASS_NAME, getNextId) {

    char buffer[64];
    int len;
    uint64_t donkeyid = donkeyid_next_id();
    len = sprintf(buffer, "%"PRIu64, donkeyid);

    RETURN_STRINGL(buffer,len, 1);
}

PHP_METHOD (PHP_DONKEYID_CLASS_NAME, parseTime) {
    char *val = NULL;
    int val_len;
    char buffer[64];
    int len;

    //获取类方法的参数
    if (zend_parse_parameters(ZEND_NUM_ARGS()TSRMLS_CC, "s", &val, &val_len) == FAILURE) {
        return;
    }
    uint64_t id = strtoul(val,NULL,10);
    if (id == 0) {
        RETURN_FALSE;
    }
    zval *ztype = zend_read_property(donkeyid_ce, getThis(), ZEND_STRL("type"), 0 TSRMLS_CC);
    zval *zepoch = zend_read_property(donkeyid_ce, getThis(), ZEND_STRL("epoch"), 0 TSRMLS_CC);
    uint64_t time = GET_TIMESTAMP_BY_DONKEY_ID(id,Z_LVAL_P(ztype),Z_LVAL_P(zepoch));
    len = sprintf(buffer, "%"PRIu64, Z_LVAL_P(ztype) == 0 ? time:time*1000);
    RETURN_STRINGL(buffer,len, 1);
}

PHP_METHOD (PHP_DONKEYID_CLASS_NAME, parseNodeId) {
    char *val = NULL;
    int val_len;

    //获取类方法的参数
    if (zend_parse_parameters(ZEND_NUM_ARGS()TSRMLS_CC, "s", &val, &val_len) == FAILURE) {
        return;
    }
    uint64_t id = strtoul(val,NULL,10);
    if (id == 0) {
        RETURN_FALSE;
    }
    zval *ztype = zend_read_property(donkeyid_ce, getThis(), ZEND_STRL("type"), 0 TSRMLS_CC);
    int nodeid = GET_NODE_ID_BY_DONKEY_ID(id,Z_LVAL_P(ztype));
    RETURN_LONG(nodeid);
}

PHP_METHOD (PHP_DONKEYID_CLASS_NAME, parseWorkerId) {
    char *val = NULL;
    int val_len;

    //获取类方法的参数
    if (zend_parse_parameters(ZEND_NUM_ARGS()TSRMLS_CC, "s", &val, &val_len) == FAILURE) {
        return;
    }
    uint64_t id = strtoul(val,NULL,10);
    if (id == 0) {
        RETURN_FALSE;
    }
    zval *ztype = zend_read_property(donkeyid_ce, getThis(), ZEND_STRL("type"), 0 TSRMLS_CC);
    int workerid = GET_WORKER_ID_BY_DONKEY_ID(id,Z_LVAL_P(ztype));
    RETURN_LONG(workerid);
}

PHP_METHOD (PHP_DONKEYID_CLASS_NAME, parseSequence) {
    char *val = NULL;
    int val_len;

    //获取类方法的参数
    if (zend_parse_parameters(ZEND_NUM_ARGS()TSRMLS_CC, "s", &val, &val_len) == FAILURE) {
        return;
    }
    uint64_t id = strtoul(val,NULL,10);
    if (id == 0) {
        RETURN_FALSE;
    }
    zval *ztype = zend_read_property(donkeyid_ce, getThis(), ZEND_STRL("type"), 0 TSRMLS_CC);
    int sequence = GET_SEQUENCE_BY_DONKEY_ID(id,Z_LVAL_P(ztype));
    RETURN_LONG(sequence);
}


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

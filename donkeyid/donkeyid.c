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
  | Author: liuzhiming 187231450@qq.com                                  |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_donkeyid.h"
#include "src/donkeyid.h"
#include "php_wrapper.h"


/* If you declare any globals in php_donkeyid.h uncomment this:
 *
 * */
ZEND_DECLARE_MODULE_GLOBALS(donkeyid)


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
/* {{{ PHP_INI
 */
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("donkeyid.node_id", "0", PHP_INI_ALL, OnUpdateLong,dk_node_id,zend_donkeyid_globals,donkeyid_globals)
    STD_PHP_INI_ENTRY("donkeyid.epoch", "0", PHP_INI_ALL, OnUpdateLong,dk_epoch,zend_donkeyid_globals,donkeyid_globals)
PHP_INI_END()
/* }}} */
//类方法参数定义
ZEND_BEGIN_ARG_INFO_EX(arginfo_donkeyid_getIdByTime, 0, 0, 0)
                ZEND_ARG_INFO(0, num)
                ZEND_ARG_INFO(0, time)
ZEND_END_ARG_INFO()
ZEND_BEGIN_ARG_INFO_EX(arginfo_donkeyid_parseId, 0, 0, 0)
                ZEND_ARG_INFO(0, id)
ZEND_END_ARG_INFO()

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */
/* {{{ donkeyid_functions[]
 *
 * Every user visible function must have an entry in donkeyid_functions[].
 */
const zend_function_entry donkeyid_methods[] = {
        PHP_FE(dk_get_next_id, NULL)
        PHP_FE(dk_get_next_ids, arginfo_donkeyid_getIdByTime)
        PHP_FE(dk_parse_id, arginfo_donkeyid_parseId)
        PHP_FE(dk_get_ts_id, NULL)
        PHP_FE(dk_get_ts_ids, arginfo_donkeyid_getIdByTime)
        PHP_FE(dk_parse_ts_id, arginfo_donkeyid_parseId)
        PHP_FE(dk_get_dt_id, NULL)
        PHP_FE_END    /* Must be the last line in donkeyid_functions[] */
};
/* }}} */
/* {{{ donkeyid_module_entry
 */
zend_module_entry donkeyid_module_entry = {
        STANDARD_MODULE_HEADER,
        "donkeyid",
        donkeyid_methods,
        PHP_MINIT(donkeyid),
        PHP_MSHUTDOWN(donkeyid),
        PHP_RINIT(donkeyid),        /* Replace with NULL if there's nothing to do at request start */
        PHP_RSHUTDOWN(donkeyid),    /* Replace with NULL if there's nothing to do at request end */
        PHP_MINFO(donkeyid),
        PHP_DONKEYID_VERSION,
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


/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION (donkeyid) {
    /* If you have INI entries, uncomment these lines
     * */
    REGISTER_INI_ENTRIES();
    if (donkeyid_init() == -1){
        return FAILURE;
    }
    atexit(donkeyid_shutdown);
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION (donkeyid) {
    /* uncomment this line if you have INI entries
     * */
    UNREGISTER_INI_ENTRIES();

    donkeyid_shutdown();
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION (donkeyid) {
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
PHP_FUNCTION(dk_get_next_id)
{
    char buffer[64];
    int len;
    long node_id =  DONKEYID_G(dk_node_id);
    time_t epoch = DONKEYID_G(dk_epoch);
    uint64_t donkeyid = donkeyid_next_id(node_id,epoch);
    len = sprintf(buffer, "%"PRIu64, donkeyid);
    DK_RETURN_STRINGL(buffer, len, 1);
}
PHP_FUNCTION(dk_get_ts_id)
{
    char buffer[64];
    int len;
    long node_id =  DONKEYID_G(dk_node_id);
    time_t epoch = DONKEYID_G(dk_epoch);
    uint64_t donkeyid = donkeyid_ts_id(node_id,epoch);
    len = sprintf(buffer, "%"PRIu64, donkeyid);
    DK_RETURN_STRINGL(buffer, len, 1);
}

PHP_FUNCTION(dk_get_dt_id)
{
    char buffer[64];
    int len;
    long node_id =  DONKEYID_G(dk_node_id);
    len = donkeyid_dt_id(node_id,buffer);
    DK_RETURN_STRINGL(buffer, len, 1);
}

PHP_FUNCTION(dk_get_next_ids)
{
    char buffer[64];
    uint len;
    char *val = 0;
    zend_size_t val_len;
    long num;
    int n;
    //获取类方法的参数
    if (zend_parse_parameters(ZEND_NUM_ARGS()TSRMLS_CC, "l|s",&num, &val, &val_len) == FAILURE) {
        return;
    }
    if(num >= MAX_BATCH_ID_LEN){
        num = MAX_BATCH_ID_LEN;
    } else if(num <= 0){
        num = 1;
    }
    uint64_t *idlist = (uint64_t *)malloc(sizeof(uint64_t)*num);

    long node_id =  DONKEYID_G(dk_node_id);
    time_t epoch = DONKEYID_G(dk_epoch);
    if (val == 0) {
        for (n = 0; n < num ; n++) {
            *(idlist+n) = donkeyid_next_id(node_id,epoch);
        }
    } else{
        uint64_t time = strtoul(val, NULL, 10);
        if (time == 0){
            RETURN_FALSE;
        }
        if (donkeyid_get_next_ids(idlist,time,num,node_id,epoch) != 0){
            RETURN_FALSE;
        }
    }
    array_init(return_value);
    for (n = 0; n < num ; n++) {
        len = (uint)sprintf(buffer, "%"PRIu64, *(idlist+n));
        dk_add_next_index_stringl(return_value,buffer,len,1);
    }
    free(idlist);
}

PHP_FUNCTION(dk_get_ts_ids)
{
    char buffer[64];
    uint len;
    char *val = NULL;
    zend_size_t val_len;
    long num;
    int n;
    //获取类方法的参数
    if (zend_parse_parameters(ZEND_NUM_ARGS()TSRMLS_CC, "l|s",&num, &val, &val_len) == FAILURE) {
        return;
    }
    if(num >= TYPE_1_SEQUENCE_MASK){
        num = TYPE_1_SEQUENCE_MASK;
    } else if(num <= 0){
        num = 1;
    }
    uint64_t *idlist = (uint64_t *)malloc(sizeof(uint64_t)*num);
    long node_id =  DONKEYID_G(dk_node_id);
    time_t epoch = DONKEYID_G(dk_epoch);


    if (val == 0) {
        for (n = 0; n < num ; n++) {
            *(idlist+n) = donkeyid_ts_id(node_id,epoch);
        }
    } else{
        uint64_t time = strtoul(val, NULL, 10);
        if (time == 0){
            RETURN_FALSE;
        }
        if (donkeyid_get_ts_ids(idlist,time,num,node_id,epoch) != 0){
            RETURN_FALSE;
        }
    }
    array_init(return_value);
    for (n = 0; n < num ; n++) {
        len = (uint)sprintf(buffer, "%"PRIu64, *(idlist+n));
        dk_add_next_index_stringl(return_value,buffer,len,1);
    }
    free(idlist);
}
PHP_FUNCTION(dk_parse_ts_id)
{
    char *val = NULL;
    zend_size_t val_len;
    char buffer[64];
    int len;
    //获取类方法的参数
    if (zend_parse_parameters(ZEND_NUM_ARGS()TSRMLS_CC, "s", &val, &val_len) == FAILURE) {
        return;
    }
    if (val_len > 20){
        RETURN_FALSE;
    }
    uint64_t id = strtoul(val, NULL, 10);
    if (id == 0) {
        RETURN_FALSE;
    }
    array_init(return_value);
    uint64_t time = GET_TIMESTAMP_BY_DONKEY_ID(id, TSTYPE, DONKEYID_G(dk_epoch));
    len = sprintf(buffer, "%"PRIu64, time);
    int sequence = GET_SEQUENCE_BY_DONKEY_ID(id, TSTYPE);
    int nodeid = GET_NODE_ID_BY_DONKEY_ID(id, TSTYPE);

    dk_add_assoc_stringl_ex(return_value,"time",5,buffer,(uint)len,1);
    add_assoc_long_ex(return_value,"node_id",8,nodeid);
    add_assoc_long_ex(return_value,"sequence",9,sequence);
}

PHP_FUNCTION(dk_parse_id)
{
    char *val = NULL;
    zend_size_t val_len;
    char buffer[64];
    int len;
    //获取类方法的参数
    if (zend_parse_parameters(ZEND_NUM_ARGS()TSRMLS_CC, "s", &val, &val_len) == FAILURE) {
        return;
    }
    if (val_len > 20){
        RETURN_FALSE;
    }
    uint64_t id = strtoul(val, NULL, 10);
    if (id == 0) {
        RETURN_FALSE;
    }
    array_init(return_value);
    uint64_t time = GET_TIMESTAMP_BY_DONKEY_ID(id, NEXTTYPE, DONKEYID_G(dk_epoch));
    len = sprintf(buffer, "%"PRIu64, time);
    int sequence = GET_SEQUENCE_BY_DONKEY_ID(id, NEXTTYPE);
    int nodeid = GET_NODE_ID_BY_DONKEY_ID(id, NEXTTYPE);

    dk_add_assoc_stringl_ex(return_value,"time",5,buffer,(uint)len,1);
    add_assoc_long_ex(return_value,"node_id",8,nodeid);
    add_assoc_long_ex(return_value,"sequence",9,sequence);

}


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

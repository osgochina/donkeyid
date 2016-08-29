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
  | Author: liuzhiming 187231450@qq.com                                                      |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_DONKEYID_H
#define PHP_DONKEYID_H

extern zend_module_entry donkeyid_module_entry;
#define phpext_donkeyid_ptr &donkeyid_module_entry

#define PHP_DONKEYID_VERSION "1.0.1" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_DONKEYID_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_DONKEYID_API __attribute__ ((visibility("default")))
#else
#	define PHP_DONKEYID_API
#endif

#ifdef ZTS

#include "TSRM.h"

#endif

PHP_MINIT_FUNCTION (donkeyid);

PHP_MSHUTDOWN_FUNCTION (donkeyid);

PHP_RINIT_FUNCTION (donkeyid);

PHP_RSHUTDOWN_FUNCTION (donkeyid);

PHP_MINFO_FUNCTION (donkeyid);

PHP_FUNCTION(dk_get_next_id);//获取基于Snowflake算法的id
PHP_FUNCTION(dk_get_next_ids);//获取基于Snowflake算法的id列表
PHP_FUNCTION(dk_parse_id);//解析基于Snowflake算法的id元数据

PHP_FUNCTION(dk_get_ts_id);//获取10进制的时间戳类型的id
PHP_FUNCTION(dk_get_ts_ids);//获取10进制的时间戳类型的id列表
PHP_FUNCTION(dk_parse_ts_id);//解析id元数据

PHP_FUNCTION(dk_get_dt_id);//获取字符串类型的id，显式包含日期时间属性
/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     
*/
ZEND_BEGIN_MODULE_GLOBALS(donkeyid)
	long  dk_node_id;//节点id
	long  dk_epoch; //起始计算时间
ZEND_END_MODULE_GLOBALS(donkeyid)


/* In every utility function you add that needs to use variables 
   in php_donkeyid_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as DONKEYID_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define DONKEYID_G(v) TSRMG(donkeyid_globals_id, zend_donkeyid_globals *, v)
#else
#define DONKEYID_G(v) (donkeyid_globals.v)
#endif

#endif	/* PHP_DONKEYID_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

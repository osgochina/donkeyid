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

#ifndef PHP_DONKEYID_H
#define PHP_DONKEYID_H

extern zend_module_entry donkeyid_module_entry;
#define phpext_donkeyid_ptr &donkeyid_module_entry

#define PHP_DONKEYID_VERSION "0.1.0" /* Replace with version number for your extension */
#define PHP_DONKEYID_CLASS_NAME "DonkeyId"

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

PHP_MINIT_FUNCTION(donkeyid);
PHP_MSHUTDOWN_FUNCTION(donkeyid);
PHP_RINIT_FUNCTION(donkeyid);
PHP_RSHUTDOWN_FUNCTION(donkeyid);
PHP_MINFO_FUNCTION(donkeyid);


//class method
PHP_METHOD(PHP_DONKEYID_CLASS_NAME,set_node_id);
PHP_METHOD(PHP_DONKEYID_CLASS_NAME,get_next_id);
PHP_METHOD(PHP_DONKEYID_CLASS_NAME,get_time_by_donkeyid);
PHP_METHOD(PHP_DONKEYID_CLASS_NAME,get_node_by_donkeyid);


PHP_FUNCTION(confirm_donkeyid_compiled);	/* For testing, remove later. */
/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(donkeyid)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(donkeyid)
*/

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

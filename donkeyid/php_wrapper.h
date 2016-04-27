//
// Created by vic on 16-4-18.
//

#ifndef DONKEYID_PHP_WRAPPER_H
#define DONKEYID_PHP_WRAPPER_H

#include <Zend/zend_types.h>


#if PHP_MAJOR_VERSION < 7

#define dk_zend_read_property zend_read_property
#define DK_RETURN_STRINGL RETURN_STRINGL
typedef int zend_size_t;

#define dk_add_next_index_stringl(a,s,l,d) add_next_index_stringl(a,s,l,d)

#else

#define DK_RETURN_STRINGL(s,l,dup) RETURN_STRINGL(s,l)
#define dk_add_next_index_stringl(a,s,l,d)  add_next_index_stringl(a,s,l)

static inline zval* dk_zend_read_property(zend_class_entry *scope, zval *object, const char *name, size_t name_length, zend_bool silent){
    zval rv;
    return zend_read_property(scope,object,name,name_length,silent,&rv);
}
typedef size_t zend_size_t;

#endif




#endif //DONKEYID_PHP_WRAPPER_H

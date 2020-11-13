//
// Created by vic on 16-4-18.
//

#ifndef DONKEYID_PHP_WRAPPER_H
#define DONKEYID_PHP_WRAPPER_H

#include <Zend/zend_types.h>


#if PHP_MAJOR_VERSION < 7

#define DK_RETURN_STRINGL RETURN_STRINGL
typedef int zend_size_t;

#define dk_add_next_index_stringl(a,s,l,d) add_next_index_stringl(a,s,l,d)
#define dk_add_assoc_stringl_ex(a,ks,kl,s,l,d) add_assoc_stringl_ex(a,ks,kl,s,l,d)

#else

#define DK_RETURN_STRINGL(s,l,dup) RETURN_STRINGL(s,l)
#define dk_add_next_index_stringl(a,s,l,d)  add_next_index_stringl(a,s,l)
#define dk_add_assoc_stringl_ex(a,ks,kl,s,l,d) add_assoc_stringl_ex(a,ks,kl,s,l)

typedef size_t zend_size_t;

#endif



#if PHP_VERSION_ID >= 80000
#define TSRMLS_C
#define TSRMLS_CC
#define TSRMLS_D
#define TSRMLS_DC
#define TSRMLS_FETCH_FROM_CTX(ctx)
#define TSRMLS_SET_CTX(ctx)
#endif


#endif //DONKEYID_PHP_WRAPPER_H

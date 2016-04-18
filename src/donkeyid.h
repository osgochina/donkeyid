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

#ifndef DONKEYID_DONKEYID_H
#define DONKEYID_DONKEYID_H

#include <bits/types.h>

#define TIMESTAMP_BITS 42   //时间所占bit位数
#define NODE_ID_BITS 8      //节点所占bit位数
#define WORKER_ID_BITS 5    //进程worker所占bit位数
#define SEQUENCE_BITS 9     //毫秒内自增

#define TIMESTAMP_MASK (-1LL^(-1LL<<TIMESTAMP_BITS))
#define NODE_ID_MASK (-1^(-1<<NODE_ID_BITS))
#define WORKER_ID_MASK (-1^(-1<<WORKER_ID_BITS))
#define SEQUENCE_MASK (-1^(-1<<SEQUENCE_BITS))

#define TIMESTAMP_LEFT_SHIFT (NODE_ID_BITS+WORKER_ID_BITS+SEQUENCE_BITS)
#define NODE_ID_LEFT_SHIFT (WORKER_ID_BITS+SEQUENCE_BITS)
#define WORKER_ID_LEFT_SHIFT (SEQUENCE_BITS)

#define TYPE_1_TIMESTAMP 1000000000
#define TYPE_1_NODE_ID 1000000

#define GET_TIMESTAMP_BY_DONKEY_ID(id,type,epoch) (type==0)?((__uint64_t)(id>>TIMESTAMP_LEFT_SHIFT)+(epoch*1000)):(__uint64_t)((id+epoch*TYPE_1_TIMESTAMP)/TYPE_1_TIMESTAMP)
#define GET_NODE_ID_BY_DONKEY_ID(id,type)  (type==0)?(int)((id>>NODE_ID_LEFT_SHIFT)&NODE_ID_MASK):(int)((id-((id/TYPE_1_TIMESTAMP)*TYPE_1_TIMESTAMP))/TYPE_1_NODE_ID)
#define GET_WORKER_ID_BY_DONKEY_ID(id,type) (type==0)?(int)((id>>WORKER_ID_LEFT_SHIFT)&WORKER_ID_MASK):0
#define GET_SEQUENCE_BY_DONKEY_ID(id,type) (type==0)?(int)(id&SEQUENCE_MASK):(int)((((id-((id/TYPE_1_TIMESTAMP)*TYPE_1_TIMESTAMP))-(GET_NODE_ID_BY_DONKEY_ID(id,type)*TYPE_1_NODE_ID)))/10)


typedef struct {
    __uint64_t epoch; //自定义起始时间
    __uint64_t last_timestamp; //最后使用毫秒数
    int node_id;                //节点ID
    int sequence;               //单服务器毫秒内的自增值
} donkeyid_context_t;

int donkeyid_init(int);

void donkeyid_set_type(int);

void donkeyid_shutdown();

void donkeyid_atexit();

void donkeyid_set_epoch(__time_t);
__uint64_t get_curr_timestamp();

void donkeyid_set_node_id(int);

void donkeyid_set_worker_id();

__uint64_t donkeyid_next_id();


#endif //DONKEYID_DONKEYID_H

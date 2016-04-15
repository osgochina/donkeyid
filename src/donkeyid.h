//
// Created by liuzhiming on 16-4-14.
//

#ifndef DONKEYID_DONKEYID_H
#define DONKEYID_DONKEYID_H

#include <bits/types.h>

#define TIMESTAMP_BITS 42   //时间所占bit位数
#define NODE_ID_BITS 7      //节点所占bit位数
#define WORKER_ID_BITS 5    //进程worker所占bit位数
#define SEQUENCE_BITS 8     //毫秒内自增

#define TIMESTAMP_MASK (-1LL^(-1LL<<TIMESTAMP_BITS))
#define NODE_ID_MASK (-1^(-1<<NODE_ID_BITS))
#define WORKER_ID_MASK (-1^(-1<<WORKER_ID_BITS))
#define SEQUENCE_MASK (-1^(-1<<SEQUENCE_BITS))

#define TIMESTAMP_LEFT_SHIFT (NODE_ID_BITS+WORKER_ID_BITS+SEQUENCE_BITS)
#define NODE_ID_LEFT_SHIFT (WORKER_ID_BITS+SEQUENCE_BITS)
#define WORKER_ID_LEFT_SHIFT (SEQUENCE_BITS)

#define GET_TIMESTAMP_BY_DONKEY_ID(id) (__uint64_t)(id>>TIMESTAMP_LEFT_SHIFT)
#define GET_NODE_ID_BY_DONKEY_ID(id) (int)((id>>NODE_ID_LEFT_SHIFT)&NODE_ID_MASK)
#define GET_WORKER_ID_BY_DONKEY_ID(id) (int)((id>>WORKER_ID_LEFT_SHIFT)&WORKER_ID_MASK)
#define GET_SEQUENCE_BY_DONKEY_ID(id) (int)(id&SEQUENCE_MASK)



typedef struct {
    __uint64_t epoch; //自定义起始时间
    __uint64_t last_timestamp; //最后使用毫秒数
    int node_id;                //节点ID
    int sequence;               //单服务器毫秒内的自增值
} donkeyid_context_t;

int donkeyid_init(int isshm, int type);
void donkeyid_shutdown(int isshm);
void donkeyid_atexit();
void donkeyid_set_epoch(__time_t timestamp);
void donkeyid_set_node_id(int node_id);
__uint64_t donkeyid_next_id();



#endif //DONKEYID_DONKEYID_H

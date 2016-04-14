//
// Created by liuzhiming on 16-4-14.
//

#ifndef DONKEYID_DONKEYID_H
#define DONKEYID_DONKEYID_H

#include <bits/types.h>

typedef volatile unsigned int atomic_t;

typedef struct {
    __uint64_t epoch; //自定义起始时间
    __uint64_t last_timestamp; //最后使用毫秒数
    int node_id;                //节点ID
    int sequence;               //单服务器毫秒内的自增值
} donkeyid_context_t;


#endif //DONKEYID_DONKEYID_H

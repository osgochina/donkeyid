#ifndef DONKEYID_H_INCLUDED
#define DONKEYID_H_INCLUDED
#include <stdint.h>

#define GET_TIME_BY_DONKEYID(id)   (id/10)>>17
#define GET_NODE_BY_DONKEYID(id)   ((id/10)>>8L)&0XFF


//设置节点id
void set_node_id(int);
//获取id
uint64_t get_donkey_id();


#endif // DONKEYID_H_INCLUDED

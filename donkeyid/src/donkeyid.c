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

#include <malloc.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "donkeyid.h"
#include "shm.h"
#include "spinlock.h"

int ncpu;
static struct shm shmctx;
static char *ctxaddr;
static int pid = -1;
static int worker_id = 0;
static int isshm = 0;

typedef struct {
    atomic_t lock;
    donkeyid_context_t donkeyid_context;
} mlocks;
static mlocks *lock;
static mlocks dtypes[2] = {0};
static int dtype = 0;

/**
 *  初始化
 */
int donkeyid_init(int _isshm) {

    isshm = _isshm;
    //是否使用共享内存
    if (isshm <= 0) {
        ctxaddr = malloc(sizeof(dtypes));
        if (!ctxaddr) {
            return -1;
        }
        bzero(ctxaddr, sizeof(dtypes));
        lock = (mlocks *) ctxaddr;
    } else {
        shmctx.size = sizeof(dtypes);
        if (shm_alloc(&shmctx) == -1) {
            return -1;
        }
        bzero(shmctx.addr, sizeof(dtypes));
        lock = (mlocks *) shmctx.addr;
    }

    //获取cpu核心数量
    ncpu = (int) sysconf(_SC_NPROCESSORS_ONLN);
    if (ncpu <= 0) {
        ncpu = 1;
    }

    return 0;
}

/**
 * 正常结束结束释放内存
 */
void donkeyid_shutdown() {
    if (isshm <= 0 ) {
        if (ctxaddr != NULL){
            free(ctxaddr);
            ctxaddr = NULL;
        }
    } else {
        if (shmctx.size){
            int i;
            for (i = 0; i <= dtype ; i++) {
                if((lock+i)->lock == pid){
                    spin_unlock(&((lock+i)->lock),pid);
                }
            }
            shm_free(&shmctx);
            shmctx.size = 0;
        }

    }
}


/**
 * 设置id生成类型
 */
void donkeyid_set_type(int type) {
    dtype = type;
}

/**
 * 设置时间戳计算时间
 */
void donkeyid_set_epoch(__time_t timestamp) {
    if (timestamp <= 0) {
        timestamp = 0;
    }
    spin_lock(&((lock+dtype)->lock),pid);
    (lock+dtype)->donkeyid_context.epoch = (__uint64_t) ((timestamp & 0xFFFFFFFF) * 1000);
    spin_unlock(&((lock+dtype)->lock),pid);
}

/**
 * 设置节点id
 */
void donkeyid_set_node_id(int node_id) {
    spin_lock(&((lock+dtype)->lock),pid);
    if (dtype == 0){
        (lock+dtype)->donkeyid_context.node_id = node_id & NODE_ID_MASK;
    } else{
        (lock+dtype)->donkeyid_context.node_id = node_id & TYPE_1_NODE_ID_MASK;
    }

    spin_unlock(&((lock+dtype)->lock),pid);
}

/**
 * 设置worker_id
 */
void donkeyid_set_worker_id() {
    if (pid == -1)
        pid = (int) getpid();
    if (worker_id == 0)
        worker_id = pid & WORKER_ID_MASK;
}

/**
 * 获取当前毫秒数
 */
static __uint64_t get_curr_timestamp_ms() {
    struct timeval tv;
    if (gettimeofday(&tv, NULL) == -1) {
        return 0ULL;
    }
    __uint64_t ms_t = (__uint64_t) tv.tv_sec * 1000ULL + (__uint64_t) tv.tv_usec / 1000ULL;
    return ms_t;
}
/**
 * 获取当前秒数
 */
__uint64_t get_curr_timestamp(){
    struct timeval tv;
    if (gettimeofday(&tv, NULL) == -1) {
        return 0ULL;
    }
    return  (__uint64_t) tv.tv_sec;
}

/**
 * 等待1毫秒
 */
static __uint64_t wait_next_ms() {
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 1000;
    select(0, NULL, NULL, NULL, &tv);
    return get_curr_timestamp_ms();
}
static __uint64_t wait_next_stamp(__uint64_t now) {
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 1000;
    __uint64_t  cur = 0;
    do{
        cur = get_curr_timestamp();
        select(0, NULL, NULL, NULL, &tv);
    }while (cur <= now);
    return cur;
}


/**
 * 获取唯一id
 */
__uint64_t donkeyid_next_id() {
    __uint64_t id;
    spin_lock(&((lock+dtype)->lock), pid);
    //根据不同的类型，生成不同类型的id
    switch (dtype) {
        //生成10进制基地的自增id
        case 1: {
            __uint64_t now = get_curr_timestamp();
            if (now == 0ULL) {
                id =  0ULL;
                goto unlock_end;
            }
            if (now < (lock+dtype)->donkeyid_context.last_timestamp) {
                (lock+dtype)->donkeyid_context.last_timestamp = now;
            }
            if (now == (lock+dtype)->donkeyid_context.last_timestamp) {
                (lock+dtype)->donkeyid_context.sequence = (lock+dtype)->donkeyid_context.sequence + 1 & TYPE_1_SEQUENCE_MASK;
                if ((lock+dtype)->donkeyid_context.sequence == 0) {
                    now = wait_next_stamp(now);
                }
            } else {
                //使得生成的id尾号均匀
                srand(now);
                (lock+dtype)->donkeyid_context.sequence = rand() % 10;
            }
            (lock+dtype)->donkeyid_context.last_timestamp = now;
            id = ((__uint64_t) (((now - ((lock+dtype)->donkeyid_context.epoch != 0?(lock+dtype)->donkeyid_context.epoch/1000:0))) & TYPE_1_TIMESTAMP_MASK) * TYPE_1_TIMESTAMP)
                 +((__uint64_t)((lock+dtype)->donkeyid_context.node_id & TYPE_1_NODE_ID_MASK) * TYPE_1_NODE_ID)
                 +((__uint64_t)(lock+dtype)->donkeyid_context.sequence * 10);
            break;
        }
        //默认类型
        case 0:
        default: {
            __uint64_t now = get_curr_timestamp_ms();
            if (now == 0ULL) {
                id =  0ULL;
                goto unlock_end;
            }
            if (now < (lock+dtype)->donkeyid_context.last_timestamp) {
                (lock+dtype)->donkeyid_context.last_timestamp = now;
            }
            if (now == (lock+dtype)->donkeyid_context.last_timestamp) {
                (lock+dtype)->donkeyid_context.sequence = lock->donkeyid_context.sequence + 1 & SEQUENCE_MASK;
                if ((lock+dtype)->donkeyid_context.sequence == 0) {
                    now = wait_next_ms();
                }
            } else {
                //使得生成的id尾号均匀
                srand(now);
                (lock+dtype)->donkeyid_context.sequence = rand() % 2;
            }
            (lock+dtype)->donkeyid_context.last_timestamp = now;
            id = ((__uint64_t) ((now - (lock+dtype)->donkeyid_context.epoch) & TIMESTAMP_MASK) << TIMESTAMP_LEFT_SHIFT)
                 | ((__uint64_t) ((lock+dtype)->donkeyid_context.node_id & NODE_ID_MASK) << NODE_ID_LEFT_SHIFT)
                 | ((__uint64_t) worker_id << WORKER_ID_LEFT_SHIFT)
                 | ((__uint64_t) (lock+dtype)->donkeyid_context.sequence);
            break;
        }
    }
    unlock_end:
    spin_unlock(&((lock+dtype)->lock), pid);
    return id;
}

/**
 * 批量获取1秒内的id
 */
int donkeyid_get_id_by_time(__uint64_t  *list,__time_t time,int sum)
{
    //时间不能小于0，不能小于起始纪元
    if (time < 0 || (time*1000) < (lock+dtype)->donkeyid_context.epoch){
        return -1;
    }

    int msec,sequence;
    int n=0;
    switch (dtype) {
        //生成10进制基地的自增id
        case 1: {
            //单次获取数量不能超过上限
            if (sum > TYPE_1_SEQUENCE_MASK){
                return -1;
            }
            int max_sequence = TYPE_1_SEQUENCE_MASK;
            for (sequence = 0;sequence<max_sequence;sequence++){
                if (n >= sum){ break;}
                *(list+n) = ((__uint64_t) ((((time)  - ((lock+dtype)->donkeyid_context.epoch != 0?(lock+dtype)->donkeyid_context.epoch/1000:0))) & TYPE_1_TIMESTAMP_MASK) * TYPE_1_TIMESTAMP)
                            +((__uint64_t)((lock+dtype)->donkeyid_context.node_id & TYPE_1_NODE_ID_MASK) * TYPE_1_NODE_ID)
                            +((__uint64_t)sequence * 10);
                n++;
            }
            break;
        }
        //默认类型
        case 0:
        default: {
            //单次获取数量不能超过上限
            if (sum >= MAX_BATCH_ID_LEN){
                return -1;
            }
            int max_sequence = MAX_BATCH_ID_LEN/1000;
            for (msec = 0; msec < 1000; msec++) {
                for (sequence = 0;sequence<max_sequence;sequence++){
                    if (n >= sum){ break;}
                    *(list+n) = ((__uint64_t) (((time*1000+msec) - (lock+dtype)->donkeyid_context.epoch) & TIMESTAMP_MASK) << TIMESTAMP_LEFT_SHIFT)
                                | ((__uint64_t) ((lock+dtype)->donkeyid_context.node_id & NODE_ID_MASK) << NODE_ID_LEFT_SHIFT)
                                | ((__uint64_t) worker_id << WORKER_ID_LEFT_SHIFT)
                                | ((__uint64_t) sequence);
                    n++;
                }
            }
            break;
        }
    }
    return 0;

}

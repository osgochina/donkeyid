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
static pid_t pid = -1;
typedef struct {
    atomic_t lock;
    donkeyid_context_t donkeyid_context;
} mlocks;

static mlocks *mlock;
static mlocks dtypes[MAX_DONKEYID_TYPE] = {0};

/**
 *  初始化
 */
int donkeyid_init() {

    shmctx.size = sizeof(dtypes);
    if (shm_alloc(&shmctx) == -1) {
        return -1;
    }
    bzero(shmctx.addr, sizeof(dtypes));
    mlock = (mlocks *) shmctx.addr;
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
    if (shmctx.size){
        int i;
        for (i = 0; i <= MAX_DONKEYID_TYPE ; i++) {
            if((mlock+i)->lock == pid){
                spin_unlock(&((mlock+i)->lock),pid);
            }
        }
        shm_free(&shmctx);
        shmctx.size = 0;
    }
}


/**
 * 设置pid
 */
static void init_pid() {
    if (pid == -1)
        pid = (int) getpid();
}


/**
 * 获取当前毫秒数
 */
static uint64_t get_curr_timestamp_ms() {
    struct timeval tv;
    if (gettimeofday(&tv, NULL) == -1) {
        return 0ULL;
    }
    uint64_t ms_t = (uint64_t) tv.tv_sec * 1000ULL + (uint64_t) tv.tv_usec / 1000ULL;
    return ms_t;
}
/**
 * 获取当前秒数
 */
uint64_t get_curr_timestamp(){
    struct timeval tv;
    if (gettimeofday(&tv, NULL) == -1) {
        return 0ULL;
    }
    return  (uint64_t) tv.tv_sec;
}

/**
 * 等待1毫秒
 */
static uint64_t wait_next_ms() {
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 1000;
    select(0, NULL, NULL, NULL, &tv);
    return get_curr_timestamp_ms();
}

/**
 * 等待一秒
 */
static uint64_t wait_next_stamp(uint64_t now) {
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 1000;
    uint64_t  cur = 0;
    do{
        cur = get_curr_timestamp();
        select(0, NULL, NULL, NULL, &tv);
    }while (cur <= now);
    return cur;
}


/**
 * 获取唯一id
 */
uint64_t donkeyid_next_id(dk_p_t pt) {
    //初始化pid
    init_pid();

    uint64_t id;
    spin_lock(&((mlock+pt.dtype)->lock), pid);
    //根据不同的类型，生成不同类型的id
    switch (pt.dtype) {
        //生成10进制基地的自增id
        case 1: {
            uint64_t now = get_curr_timestamp();
            if (now == 0ULL) {
                id =  0ULL;
                goto unlock_end;
            }
            if (now < (mlock+pt.dtype)->donkeyid_context.last_timestamp) {
                (mlock+pt.dtype)->donkeyid_context.last_timestamp = now;
            }
            if (now == (mlock+pt.dtype)->donkeyid_context.last_timestamp) {
                (mlock+pt.dtype)->donkeyid_context.sequence = (mlock+pt.dtype)->donkeyid_context.sequence + 1 & TYPE_1_SEQUENCE_MASK;
                if ((mlock+pt.dtype)->donkeyid_context.sequence == 0) {
                    now = wait_next_stamp(now);
                }
            } else {
                //使得生成的id尾号均匀
                srand((unsigned int)now);
                (mlock+pt.dtype)->donkeyid_context.sequence = rand() % 10;
            }
            (mlock+pt.dtype)->donkeyid_context.last_timestamp = now;
            id = ((uint64_t) (((now - (pt.epoch != 0?pt.epoch:0))) & TYPE_1_TIMESTAMP_MASK) * TYPE_1_TIMESTAMP)
                 +((uint64_t)(pt.node_id) * TYPE_1_NODE_ID)
                 +((uint64_t)(mlock+pt.dtype)->donkeyid_context.sequence * 10);
            break;
        }
        //默认类型
        case 0:
        default: {
            uint64_t now = get_curr_timestamp_ms();
            if (now == 0ULL) {
                id =  0ULL;
                goto unlock_end;
            }
            if (now < (mlock+pt.dtype)->donkeyid_context.last_timestamp) {
                (mlock+pt.dtype)->donkeyid_context.last_timestamp = now;
            }
            if (now == (mlock+pt.dtype)->donkeyid_context.last_timestamp) {
                (mlock+pt.dtype)->donkeyid_context.sequence = mlock->donkeyid_context.sequence + 1 & SEQUENCE_MASK;
                if ((mlock+pt.dtype)->donkeyid_context.sequence == 0) {
                    now = wait_next_ms();
                }
            } else {
                //使得生成的id尾号均匀
                srand((unsigned int)now);
                (mlock+pt.dtype)->donkeyid_context.sequence = rand() % 2;
            }
            (mlock+pt.dtype)->donkeyid_context.last_timestamp = now;
            id = ((uint64_t) ((now - pt.epoch*1000) & TIMESTAMP_MASK) << TIMESTAMP_LEFT_SHIFT)
                 | ((uint64_t) (pt.node_id & NODE_ID_MASK) << NODE_ID_LEFT_SHIFT)
                 | ((uint64_t) (mlock+pt.dtype)->donkeyid_context.sequence);
            break;
        }
    }
    unlock_end:
    spin_unlock(&((mlock+pt.dtype)->lock), pid);
    return id;
}

/**
 * 批量获取1秒内的id
 */
int donkeyid_get_id_by_time(uint64_t  *list,time_t time,int sum,dk_p_t pt)
{
    //初始化pid
    init_pid();

    //时间不能小于0，不能小于起始纪元
    if (time < 0 || (time*1000) < pt.epoch){
        return -1;
    }

    int msec,sequence;
    int n=0;
    switch (pt.dtype) {
        //生成10进制基地的自增id
        case 1: {
            //单次获取数量不能超过上限
            if (sum > TYPE_1_SEQUENCE_MASK){
                return -1;
            }
            int max_sequence = TYPE_1_SEQUENCE_MASK;
            for (sequence = 0;sequence<max_sequence;sequence++){
                if (n >= sum){ break;}
                *(list+n) = ((uint64_t) ((((time)  - (pt.epoch != 0?pt.epoch:0))) & TYPE_1_TIMESTAMP_MASK) * TYPE_1_TIMESTAMP)
                            +((uint64_t)(pt.node_id) * TYPE_1_NODE_ID)
                            +((uint64_t)sequence * 10);
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
                    *(list+n) = ((uint64_t) (((time*1000+msec) - pt.epoch*1000) & TIMESTAMP_MASK) << TIMESTAMP_LEFT_SHIFT)
                                | ((uint64_t) (pt.node_id & NODE_ID_MASK) << NODE_ID_LEFT_SHIFT)
                                | ((uint64_t) sequence);
                    n++;
                }
            }
            break;
        }
    }
    return 0;

}

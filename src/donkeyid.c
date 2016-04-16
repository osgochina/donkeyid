//
// Created by liuzhiming on 16-4-14.
//

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
        memset(ctxaddr, 0, sizeof(dtypes));
        lock = (mlocks *) ctxaddr;
    } else {
        shmctx.size = sizeof(dtypes);
        if (shm_alloc(&shmctx) == -1) {
            return -1;
        }
        memset(shmctx.addr, 0, sizeof(dtypes));
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
    if (isshm <= 0) {
        free(ctxaddr);
        ctxaddr = NULL;
    } else {
        shm_free(&shmctx);
    }
}

/**
 * 出错结束
 */
void donkeyid_atexit() {
    if(lock->lock == pid){
        spin_unlock(&(lock->lock),pid);
    }
    donkeyid_shutdown();
}

/**
 * 设置id生成类型
 */
void donkeyid_set_type(int type) {
    dtype = type;
    lock += dtype;
}

/**
 * 设置时间戳计算时间
 */
void donkeyid_set_epoch(__time_t timestamp) {
    if (timestamp <= 0) {
        timestamp = 0;
    }
    spin_lock(&(lock->lock),pid);
    lock->donkeyid_context.epoch = (__uint64_t) ((timestamp & 0xFFFFFFFF) * 1000);
    spin_unlock(&(lock->lock),pid);
}

/**
 * 设置节点id
 */
void donkeyid_set_node_id(int node_id) {
    spin_lock(&(lock->lock),pid);
    lock->donkeyid_context.node_id = node_id & NODE_ID_MASK;
    spin_unlock(&(lock->lock),pid);
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
static __uint64_t get_curr_timestamp(){
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
    spin_lock(&(lock->lock), pid);
    //根据不同的类型，生成不同类型的id
    switch (dtype) {
        //生成10进制基地的自增id
        case 1: {
            __uint64_t now = get_curr_timestamp();
            if (now == 0ULL) {
                id =  0ULL;
                goto unlock_end;
            }
            if (now < lock->donkeyid_context.last_timestamp) {
                lock->donkeyid_context.last_timestamp = now;
            }
            if (now == lock->donkeyid_context.last_timestamp) {
                lock->donkeyid_context.sequence = lock->donkeyid_context.sequence + 1 & 0xFFFF;
                if (lock->donkeyid_context.sequence == 0) {
                    now = wait_next_stamp(now);
                }
            } else {
                //使得生成的id尾号均匀
                srand(now);
                lock->donkeyid_context.sequence = rand() % 10;
            }
            lock->donkeyid_context.last_timestamp = now;
            id = ((__uint64_t) (((now - (lock->donkeyid_context.epoch != 0?lock->donkeyid_context.epoch/1000:0))) & 0xFFFFFFFF) * TYPE_1_TIMESTAMP)
                 +((__uint64_t)(lock->donkeyid_context.node_id & 0xFF) * 1000000)
                 +((__uint64_t)lock->donkeyid_context.sequence * 10);
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
            if (now < lock->donkeyid_context.last_timestamp) {
                lock->donkeyid_context.last_timestamp = now;
            }
            if (now == lock->donkeyid_context.last_timestamp) {
                lock->donkeyid_context.sequence = lock->donkeyid_context.sequence + 1 & SEQUENCE_MASK;
                if (lock->donkeyid_context.sequence == 0) {
                    now = wait_next_ms();
                }
            } else {
                //使得生成的id尾号均匀
                srand(now);
                lock->donkeyid_context.sequence = rand() % 2;
            }
            lock->donkeyid_context.last_timestamp = now;
            id = ((__uint64_t) ((now - lock->donkeyid_context.epoch) & TIMESTAMP_MASK) << TIMESTAMP_LEFT_SHIFT)
                 | ((__uint64_t) (lock->donkeyid_context.node_id & NODE_ID_MASK) << NODE_ID_LEFT_SHIFT)
                 | ((__uint64_t) worker_id << WORKER_ID_LEFT_SHIFT)
                 | ((__uint64_t) lock->donkeyid_context.sequence);
            break;
        }
    }
    unlock_end:
    spin_unlock(&(lock->lock), pid);
    return id;
}


//int main() {
//    int type = 1;
//    long epoch = 1460710318;
//    donkeyid_init(0);
//    donkeyid_set_worker_id();
//    donkeyid_set_type(type);
//    donkeyid_set_node_id(99);
//    donkeyid_set_epoch(epoch);
//
//    int i = 0;
//    for (i = 0; i < 1; ++i) {
//        __uint64_t donkeyid = donkeyid_next_id();
//        printf("%"PRIu64"\n", donkeyid);
//        printf("%"PRIu64"\n",GET_TIMESTAMP_BY_DONKEY_ID(donkeyid,type,epoch));
//        printf("%d\n",GET_NODE_ID_BY_DONKEY_ID(donkeyid,type));
//        printf("%d\n",GET_WORKER_ID_BY_DONKEY_ID(donkeyid,type));
//        printf("%d\n",GET_SEQUENCE_BY_DONKEY_ID(donkeyid,type));
//    }
//    donkeyid_shutdown(1);
//}


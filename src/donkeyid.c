//
// Created by liuzhiming on 16-4-14.
//

#include <malloc.h>
#include "donkeyid.h"
#include "shm.h"

static donkeyid_context_t *donkeyid_context;
static atomic_t *lock;
static struct shm shmctx;
static char *ctxaddr;
static int pid = -1;

/**
 *  初始化
 */
int donkeyid_init(int isshm){

    //是否使用共享内存
    if (isshm <=0){
        ctxaddr = malloc(sizeof(atomic_t)+ sizeof(donkeyid_context_t));
        if (!ctxaddr){
            return -1;
        }
        lock = (atomic_t *)ctxaddr;
        donkeyid_context = (donkeyid_context_t *)(ctxaddr + sizeof(atomic_t));
    } else {
        shmctx.size = sizeof(atomic_t) + sizeof(donkeyid_context_t);
        if (shm_alloc(&shmctx) == -1){
            return -1;
        }
        lock = (atomic_t *)shmctx.addr;
        donkeyid_context = (donkeyid_context_t *)((char *)shmctx.addr + sizeof(atomic_t));
    }
    *lock = 0; //把锁值置零
    return 0;
}

/**
 * 正常结束结束释放内存
 */
void donkeyid_shutdown(int isshm){
    if (isshm <= 0){
        free(ctxaddr);
    }else{
        shm_free(&shmctx);
    }
}

/**
 * 出错结束
 */
void donkeyid_atexit(){

}

int main(){
    donkeyid_init(1);
    donkeyid_shutdown(1);
}


#include <sys/time.h>
#include <stdlib.h>
#include "donkey_id.h"

#define TOTALBITS            64L
#define CUSTOMBITS       (TOTALBITS-5L)
#define  TIMEBITS             (CUSTOMBITS-42L)
#define  NODEIDBITS       (TIMEBITS-9L)
#define  INDEXMASK         (-1L^(-1L << 8L))

#define  GETTIMEBITS(tn)             tn<<TIMEBITS
#define  GETNODEBITS(nid)        ( (nid&(-1L^(-1L<<9L)))<<NODEIDBITS)
#define  GETINDEXBITS(num)     num
#define  GETCUSTOMBITS(id)      id*10

static struct donkey_info{
    int node_id;
    int atomic_num;
    uint64_t last_time;
} d_info;
//获取当前时间
static uint64_t  get_curr_ms();
//原子自增
static int atomic_incr(int );
//等待下一个毫秒
static uint64_t wait_next_ms(uint64_t );

/**
    设置节点id
**/
void set_node_id(int node_id){
    d_info.node_id = node_id;
}

/**
    原子自增
**/
static int atomic_incr(int num){
    __sync_add_and_fetch(&num,1);
    return num;
}

/**
    等待下一个毫秒
**/
static uint64_t wait_next_ms(uint64_t lastTime){
    uint64_t cur = 0;
    do{
        cur = get_curr_ms();
    }while(cur <= lastTime);
    return cur;
}

/**
    获取当前毫秒数
**/
static uint64_t  get_curr_ms(){
    struct timeval tv;
    gettimeofday(&tv,NULL);
    uint64_t ms_t = tv.tv_sec*1000+tv.tv_usec/1000;
    return ms_t;
}

/**
    获取id
**/
uint64_t get_donkey_id(){

    uint64_t donkeyid = 0;
    uint64_t time_now = get_curr_ms();
    if(time_now < d_info.last_time){
    	time_now = d_info.last_time;
    }
    if(time_now == d_info.last_time){
        d_info.atomic_num = atomic_incr(d_info.atomic_num)  &  INDEXMASK;
        if(d_info.atomic_num == 0){
            time_now = wait_next_ms(d_info.last_time);
            d_info.last_time = time_now;
        }
    }else{
        d_info.atomic_num= 0;
        d_info.last_time = time_now;
    }
     //时间
    donkeyid =  GETTIMEBITS(time_now) ;
    //节点
    donkeyid |=  GETNODEBITS(d_info.node_id);
    //毫秒自增
    donkeyid |=  GETINDEXBITS(d_info.atomic_num);
    //个位置零
    donkeyid = GETCUSTOMBITS(donkeyid);
    return donkeyid;
}


//
// Created by vic on 16-4-20.
//
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include "donkeyid.h"
int main() {
    int type = 1;
    long epoch = 0;
    donkeyid_init();
    dk_p_t pt = {dtype:0,node_id:1,epoch:0};
    int i = 0;
    for (i = 0; i < 100; ++i) {
        __uint64_t donkeyid = donkeyid_next_id(pt);
        printf("%"PRIu64"\n", donkeyid);
//        printf("%"PRIu64"\n",GET_TIMESTAMP_BY_DONKEY_ID(donkeyid,type,epoch));
//        printf("%d\n",GET_NODE_ID_BY_DONKEY_ID(donkeyid,type));
//        printf("%d\n",GET_WORKER_ID_BY_DONKEY_ID(donkeyid,type));
//        printf("%d\n",GET_SEQUENCE_BY_DONKEY_ID(donkeyid,type));
    }
    donkeyid_shutdown(1);
}

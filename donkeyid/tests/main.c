//
// Created by vic on 16-4-20.
//
#include <stdio.h>
#include <inttypes.h>
#include "donkeyid.h"
int main() {
    int type = 1;
    long epoch = 1460710318;
    donkeyid_init(0);
    donkeyid_set_worker_id();
    donkeyid_set_type(type);
    donkeyid_set_node_id(99);
    donkeyid_set_epoch(epoch);

    int i = 0;
    for (i = 0; i < 1; ++i) {
        __uint64_t donkeyid = donkeyid_next_id();
        printf("%"PRIu64"\n", donkeyid);
        printf("%"PRIu64"\n",GET_TIMESTAMP_BY_DONKEY_ID(donkeyid,type,epoch));
        printf("%d\n",GET_NODE_ID_BY_DONKEY_ID(donkeyid,type));
        printf("%d\n",GET_WORKER_ID_BY_DONKEY_ID(donkeyid,type));
        printf("%d\n",GET_SEQUENCE_BY_DONKEY_ID(donkeyid,type));
    }
    donkeyid_shutdown(1);
}

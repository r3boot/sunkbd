
#include <stdint.h>

#include "debug/print.h"

#define MAX_QUEUE_SIZE  64
#define NUM_QUEUES      2

typedef struct {
    uint8_t queue[MAX_QUEUE_SIZE];
    uint8_t size;
    uint8_t items;
    uint8_t head;
    uint8_t tail;
} Queue;

volatile Queue queues[NUM_QUEUES];

void init_queue(uint8_t idx)
{
    queues[idx].size = MAX_QUEUE_SIZE;
    queues[idx].items = 0;
    queues[idx].head = 0;
    queues[idx].tail = 0;
}

uint8_t q_full(uint8_t idx)
{
    return queues[idx].items == queues[idx].size;
}

uint8_t q_empty(uint8_t idx)
{
    return queues[idx].items == 0;
}

uint8_t q_count(uint8_t idx)
{
    return queues[idx].items;
}

uint8_t q_put(uint8_t idx, uint8_t scancode)
{
    if (q_full(idx)) {
        print("rx_buffer overflow!\n");
        return 0;
    }

    queues[idx].queue[queues[idx].tail++] = scancode;
    queues[idx].items++;
    return 1;

}

uint8_t q_get(uint8_t idx)
{
    if (q_empty(idx)) {
        return 0;
    }

    uint8_t item = 0;
    item = queues[idx].queue[queues[idx].head++];
    queues[idx].items--;
    if (queues[idx].head == queues[idx].size)
        queues[idx].head = 0;
    return item;
}

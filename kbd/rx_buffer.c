
#include <stdint.h>

#define MAX_QUEUE_SIZE  128


// Queue management
volatile uint8_t size = MAX_QUEUE_SIZE;
volatile uint8_t items = 0;
volatile uint8_t head = 0;
volatile uint8_t tail = 0;
volatile uint8_t ptr = NULL;

// The queue in which to store bytes
volatile uint8_t rx_buffer[MAX_QUEUE_SIZE];

uint8_t rx_buffer_full()
{
    return items == size;
}

uint8_t rx_buffer_empty()
{
    return items == 0;
}

uint8_t rx_buffer_count()
{
    return items;
}

uint8_t rx_buffer_put(uint8_t scancode)
{
    if (rx_buffer_full()) {
        print("rx_buffer overflow!\n");
        return 0;
    }

    rx_buffer[tail++] = scancode;
    items++;
    return 1;

}

uint8_t rx_buffer_get()
{
    if (rx_buffer_empty()) {
        print("rx_buffer empty\n");
        return 0;
    }

    uint8_t item = 0;
    item = rx_buffer[head++];
    items--;
    if (head == size) head = 0;
    return item;
}

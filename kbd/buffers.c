
#include <stdint.h>

#include "usb/print.h"

#define MAX_RX_BUFFER_SIZE   64
#define MAX_TX_BUFFER_SIZE   40

// Queue management
volatile uint8_t rx_buffer_size = MAX_RX_BUFFER_SIZE;
volatile uint8_t rx_buffer_items = 0;
volatile uint8_t rx_buffer_head = 0;
volatile uint8_t rx_buffer_tail = 0;

volatile uint8_t tx_buffer_size = MAX_TX_BUFFER_SIZE;
volatile uint8_t tx_buffer_items = 0;
volatile uint8_t tx_buffer_head = 0;
volatile uint8_t tx_buffer_tail = 0;

// The queue in which to store bytes
volatile uint8_t rx_buffer[MAX_RX_BUFFER_SIZE];
volatile uint8_t tx_buffer[MAX_TX_BUFFER_SIZE];

uint8_t rx_buffer_full(void)
{
    return rx_buffer_items == rx_buffer_size;
}

uint8_t rx_buffer_empty(void)
{
    return rx_buffer_items == 0;
}

uint8_t rx_buffer_count(void)
{
    return rx_buffer_items;
}

uint8_t rx_buffer_put(uint8_t scancode)
{
    if (rx_buffer_full()) {
        print("rx_buffer overflow!\n");
        return 0;
    }

    rx_buffer[rx_buffer_tail++] = scancode;
    rx_buffer_items++;
    return 1;

}

uint8_t rx_buffer_get(void)
{
    if (rx_buffer_empty()) {
        return 0;
    }

    uint8_t item = 0;
    item = rx_buffer[rx_buffer_head++];
    rx_buffer_items--;
    if (rx_buffer_head == rx_buffer_size)
        rx_buffer_head = 0;
    return item;
}

uint8_t tx_buffer_full(void)
{
    return tx_buffer_items == tx_buffer_size;
}

uint8_t tx_buffer_empty(void)
{
    return tx_buffer_items == 0;
}

uint8_t tx_buffer_count(void)
{
    return tx_buffer_items;
}

uint8_t tx_buffer_put(uint8_t scancode)
{
    if (tx_buffer_full()) {
        print("tx_buffer overflow!\n");
        return 0;
    }

    tx_buffer[tx_buffer_tail++] = scancode;
    tx_buffer_items++;
    return 1;

}

uint8_t tx_buffer_get(void)
{
    if (tx_buffer_empty()) {
        return 0;
    }

    uint8_t item = 0;
    item = tx_buffer[tx_buffer_head++];
    tx_buffer_items--;
    if (tx_buffer_head == tx_buffer_size)
        tx_buffer_head = 0;
    return item;
}


#define RX_QUEUE    0
#define TX_QUEUE    1

void init_queue(uint8_t idx);

uint8_t q_full(uint8_t idx);
uint8_t q_empty(uint8_t idx);
uint8_t q_count(uint8_t idx);
uint8_t q_put(uint8_t idx, uint8_t scancode);
uint8_t q_get(uint8_t idx);

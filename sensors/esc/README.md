# T-motor ESC Flame/Thunder drivers

## ESC Flame usage

ESC Flame driver parses UART feedback from [T-motor flame series](https://uav-en.tmotor.com/html/UAV/Multirotor/ESC/flame/).

```c++
static uint8_t dma_buffer[100];
static DmaUartHandler_t dma_handler = {
    .buf = dma_buffer,
    .size = 100
};
static EscFlame_t esc_flame_status;

void process_esc_flame() {
    // dma_handler.buf should be associated with DMA buffer
    size_t last_recv_index; // is the latest received index in DMA buffer

    if (escFlameParseDma(last_recv_index, &dma_handler, &esc_flame_status)) {
        // do something with data
    }
}

```

## ESC Thunder usage

ESC Thunder driver parses UART feedback from [T-motor 300A 24S ESC Thunder](https://store.tmotor.com/goods-975-Thunder+300A+24S.html).

Protocol description: https://m.xcopter.com/download/Thunder%20300A%20ESC%20Manual.pdf

Usage:

```c++
// Usage example:
EscThunderFeedback esc_thunder;
thunderInit(&esc_thunder);

uint8_t recv_byte;
if (ESC_THUNDER_SPD == thunderNextByte(&esc_thunder, recv_byte)) {
    // process esc_thunder.spd
}

```
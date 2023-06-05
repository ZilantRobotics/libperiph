# ESC Flame driver

ESC Flame driver parses UART feedback from [T-motor flame series](https://uav-en.tmotor.com/html/UAV/Multirotor/ESC/flame/).

## Usage

```c++
static UartDmaParser_t dma_parser = {0};
static EscFlameStatus_t esc_flame_status;

void process_esc_flame() {
    // dma_parser.buf should be associated with DMA buffer
    size_t last_recv_index; // is the latest received index in DMA buffer

    if (escFlameParseDma(last_recv_index, &dma_parser, &esc_flame_status)) {
        // do something with data
    }
}

```

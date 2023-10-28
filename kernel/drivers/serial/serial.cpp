#include "serial.h"
#include "../../io/io.h"

// Initializes communication according to the spec given
Serial::Serial(SerialConfig config) {
    port = config.port;
    // Disable interrupts
    outb(config.port + 1, 0x00);

    // Enable DLAB
    outb(config.port + 3, 0x80);

    // set the baudrate
    outb(config.port + 0, 0x03);
    outb(config.port + 1, 0x00);

    // configure for 8bits, no parity, one stop bit
    outb(config.port + 3, 0x03);

    // Enable FIFO, clear them, with 14-byte threshhold
    outb(config.port + 2, 0xC7);
    // Enable IRQ's, RTS/DSR set
    outb(config.port + 4, 0x0B );
    // Set in loopback mode, test the serial chip.
    outb(config.port + 4, 0x1E);

    // TEST
    outb(config.port + 0 , 0xAE);

    if(inb(config.port + 0) != 0xAE)
        return ; // FAIL

    outb(config.port + 4, 0x0F);
    return ;
}

void Serial::write(void* data, int len) {
    while (is_transmit_empty() == 0); // Wait for transmit queue to be free
    for (int i = 0; i < len ; i++){
        outb(port, ((uint8_t*)data)[i]);
    }
}

char Serial::read() {
    return inb(port);
}

int Serial::is_transmit_empty() {
    return inb(port + 5) & 0x20;
}



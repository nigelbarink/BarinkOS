#pragma once
// For now these are the standard
// serial ports we can talk to
enum SERIALPORT {
    COM1 = 0x3F8,
    COM2 = 0x2F8,
    COM3 = 0x3E8,
    COM4 = 0x2E8
};

struct SerialConfig {
    SERIALPORT port;
    char baud_rate_lo ;
    char baud_rate_hi;
};


class Serial   {

    public: 
        Serial (SerialConfig config );

        char read();
        void write(void* data, int length);

    private:
        SERIALPORT port;
        int is_transmit_empty();
};

void test_serial(){
        /** Serial test **/
        kterm_writestring("Writing to COM1 serial port:");
        init_serial();
        write_serial('A');
        write_serial('B');
        write_serial('C');
        write_serial('D');
        write_serial('E');

        char Character_received = read_serial();
        kterm_writestring("\n");
        kterm_writestring("received from COM 1: \n");
        kterm_put(Character_received);

        kterm_writestring("\n");
}
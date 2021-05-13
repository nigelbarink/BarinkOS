#include "idt.h"

static void itoa (char *buf, int base, int d) {
    char *p = buf;
    char *p1, *p2;
    unsigned long ud = d;
    int divisor = 10;
    if ( base == 'd' && d < 0){
        *p++ = '-';
        buf++;
        ud = -d;
    } else if (base == 'x'){
        divisor = 16;
    }

    do { 
        int remainder = ud % divisor;

         *p++ = (remainder < 10 ) ? remainder + '0' : remainder + 'a' -10;
    } while(ud /= divisor);

    /*terminate buf*/
    *p =0;
    p1 = buf;
    p2 = p -1;

    while (p1 < p2)
    {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;

    }

}

void printf ( const char *format, ...) {
    
    AS_KERNEL();
    char **arg = (char **)&format;
    int c;
    char buf[20];

    arg++;
    
    while ((c = *format++) != 0){
        if( c != '%')
            kterm_put(c);
        else{
            char *p, *p2;
            int pad0 = 0, pad = 0;

            c = *format++;
            if(c =='0'){
                pad0 = 1;
                c = *format++;
            }

            if ( c >= '0' && c <= '9'){
                pad = c - '0';
                c = *format++;
            }

            switch (c)
            {
            case 'd':

            case 'u':
            case 'x':
                itoa(buf, c, *((int *) arg++));

                p = buf;
                goto string;
                break;
            
            case 's':
                p = *arg++;
                if(!p)
                    p = "(null)";
                
                string: 
                    for (p2 = p; *p2; p2++);
                    for (; p2 < p + pad; p2++)
                        kterm_put(pad0 ? '0': ' ');
                    while (*p)
                        kterm_put(*p++);
                    break;


            default:
                kterm_put(*((int *)arg++));
                break;
            }
        }
    }
}


IDT_entry idt_table[256];
IDT_ptr idt_ptr;

void set_id_entry (uint8_t num , uint32_t base, uint16_t sel,  uint8_t flags){
    idt_table[num].offset_1 = base & 0xFFFF;
    idt_table[num].selector = sel;
    idt_table[num].zero = 0;
    idt_table[num].type_attr = flags;
    idt_table[num].offset_2 = (base >> 16) & 0xFFFF;

};


void irq_handler (registers regs) {
        kterm_writestring("received interrupt!\n");


        printf(" Interrupt number: %d \n", regs.int_no);

        if( regs.int_no == 13){
            printf(" Error code: %d \n", regs.err_code);

        }

}


void init_idt(){
    // Initialise the IDT pointer
    idt_ptr.length = sizeof(IDT_entry) * 255;
    idt_ptr.base = (uint32_t)&idt_table;


    // TODO: Set everything to zero first

    set_id_entry(0,  (uint32_t) irq0 , 0x08, 0x8E);
    set_id_entry(1,  (uint32_t) irq1 , 0x08, 0x8E);
    set_id_entry(2,  (uint32_t) irq2 , 0x08, 0x8E);
    set_id_entry(3,  (uint32_t) irq3 , 0x08, 0x8E);
    set_id_entry(4,  (uint32_t) irq4 , 0x08, 0x8E);
    set_id_entry(5,  (uint32_t) irq5 , 0x08, 0x8E);
    set_id_entry(6,  (uint32_t) irq6 , 0x08, 0x8E);
    set_id_entry(7,  (uint32_t) irq7 , 0x08, 0x8E);
    set_id_entry(8,  (uint32_t) irq8 , 0x08, 0x8E);
    set_id_entry(9,  (uint32_t) irq9 , 0x08, 0x8E);
    set_id_entry(10, (uint32_t) irq10 , 0x08, 0x8E);
    set_id_entry(11, (uint32_t) irq11 , 0x08, 0x8E);
    set_id_entry(12, (uint32_t) irq12 , 0x08, 0x8E);
    set_id_entry(13, (uint32_t) irq13 , 0x08, 0x8E);
    set_id_entry(14, (uint32_t) irq14 , 0x08, 0x8E);
    set_id_entry(15, (uint32_t) irq15 , 0x08, 0x8E);
    set_id_entry(16, (uint32_t) irq16 , 0x08, 0x8E);
    set_id_entry(17, (uint32_t) irq17 , 0x08, 0x8E);
    set_id_entry(18, (uint32_t) irq18 , 0x08, 0x8E);
    set_id_entry(19, (uint32_t) irq19 , 0x08, 0x8E); 
    set_id_entry(20, (uint32_t) irq20 , 0x08, 0x8E);
    set_id_entry(21, (uint32_t) irq21 , 0x08, 0x8E);
    set_id_entry(22, (uint32_t) irq22 , 0x08, 0x8E);
    set_id_entry(23, (uint32_t) irq23 , 0x08, 0x8E);
    set_id_entry(24, (uint32_t) irq24 , 0x08, 0x8E);
    set_id_entry(25, (uint32_t) irq25 , 0x08, 0x8E);
    set_id_entry(26, (uint32_t) irq26 , 0x08, 0x8E);
    set_id_entry(27, (uint32_t) irq27 , 0x08, 0x8E); 
    set_id_entry(28, (uint32_t) irq28 , 0x08, 0x8E);
    set_id_entry(29, (uint32_t) irq29 , 0x08, 0x8E);
    set_id_entry(30, (uint32_t) irq30 , 0x08, 0x8E);
    set_id_entry(31, (uint32_t) irq31 , 0x08, 0x8E);

/*
    // pic IRQ Table
    set_id_entry(32, (uint32_t)irs0, 0x08, 0x8E);
    set_id_entry(33, (uint32_t)irs1, 0x08, 0x8E);
    set_id_entry(34, (uint32_t)irs2, 0x08, 0x8E);
    set_id_entry(35, (uint32_t)irs3, 0x08, 0x8E);
    set_id_entry(36, (uint32_t)irs4, 0x08, 0x8E);
    set_id_entry(37, (uint32_t)irs5, 0x08, 0x8E);
    set_id_entry(38, (uint32_t)irs6, 0x08, 0x8E);
    set_id_entry(39, (uint32_t)irs7, 0x08, 0x8E);
    set_id_entry(40, (uint32_t)irs8, 0x08, 0x8E);
    set_id_entry(41, (uint32_t)irs9, 0x08, 0x8E);
    set_id_entry(42, (uint32_t)irs10, 0x08, 0x8E);
    set_id_entry(43, (uint32_t)irs11, 0x08, 0x8E);
    set_id_entry(44, (uint32_t)irs12, 0x08, 0x8E);
    set_id_entry(45, (uint32_t)irs13, 0x08, 0x8E);
    set_id_entry(46, (uint32_t)irs14, 0x08, 0x8E);
    set_id_entry(47, (uint32_t)irs15, 0x08, 0x8E);*/

    
    idt_flush((uint32_t)&idt_ptr);
}

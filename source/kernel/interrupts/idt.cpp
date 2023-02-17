#include "idt.h"
#include "../drivers/pit/pit.h"
#include "../drivers/ps-2/keyboard.h"
#include "../i386/processor.h"
#include "../memory/VirtualMemoryManager.h"
IDT_entry idt_table[256];
IDT_ptr idt_ptr;


void set_id_entry (uint8_t num , uint32_t base, uint16_t sel,  uint8_t flags){
    idt_table[num].offset_1 = base & 0xFFFF;
    idt_table[num].selector = sel;
    idt_table[num].zero = 0;
    idt_table[num].type_attr = flags;
    idt_table[num].offset_2 = (base >> 16) & 0xFFFF;

};

void irs_handler (registers* regs) {
     uint32_t FaultingAddress;
        //printf("(IRS) Interrupt number: %d \r", regs.int_no);
        switch (regs->int_no)
        {
        case 0:
            // Divide Error #DE
            printf("#DE\n");
            printf("EIP: 0x%x\n", regs->eip);
            printf("EAX: 0x%x\n", regs->eax);
            printf("EBP: 0x%x\n", regs->ebp);
        break;
        
        case 1:
            // Debug Exception #DB
            printf("#DB\n");
            printf("EIP: 0x%x\n", regs->eip);
            printf("EAX: 0x%x\n", regs->eax);
            printf("EBP: 0x%x\n", regs->ebp);
        break;

        case 2:
            // NMI Interrupt 
            printf("#NMI\n");
        break;
        
        case 3:
            // Breakpoint Exception #BP
            printf("#BP\n");
            printf("EIP: 0x%x\n", regs->eip);
            printf("EAX: 0x%x\n", regs->eax);
            printf("EBP: 0x%x\n", regs->ebp);
        break;
        
        case 4:
            // Overflow Exception #OF
            printf("#OF\n");
            printf("EIP: 0x%x\n", regs->eip);
            printf("EAX: 0x%x\n", regs->eax);
            printf("EBP: 0x%x\n", regs->ebp);
        break;
        
        case 5:
            // BOUND Range Exceeded Exception #BR
            printf("#BR\n");
            printf("EIP: 0x%x\n", regs->eip);
            printf("EAX: 0x%x\n", regs->eax);
            printf("EBP: 0x%x\n", regs->ebp);
        break;
        
        case 6:
            // Invalid OpCode Exception #UD 
            printf("#UD\n");
            printf("EIP: 0x%x\n", regs->eip);
            printf("EAX: 0x%x\n", regs->eax);
            printf("EBP: 0x%x\n", regs->ebp);
        break;
        
        case 7:
            // Device Not Available Exception  #NM
            printf("#NM\n");
            printf("EIP: 0x%x\n", regs->eip);
            printf("EAX: 0x%x\n", regs->eax);
            printf("EBP: 0x%x\n", regs->ebp);
        break;
        
        case 8:
            // Double Fault Exception #DF
            printf("#DF\n");
            printf("EIP: 0x%x\n", regs->eip);
            printf("EAX: 0x%x\n", regs->eax);
            printf("EBP: 0x%x\n", regs->ebp);
            while(true);
        break;

        case 9:
            // Coprocessor Segment Overrun
            printf("Coprocessor Segment overrun!\n");
        break;
        
        case 10:
            // Invalid TSS Exception #TS
            printf("#TS\n");
            printf("EIP: 0x%x\n", regs->eip);
            printf("EAX: 0x%x\n", regs->eax);
            printf("EBP: 0x%x\n", regs->ebp);
            __asm__("cli;" "1: hlt;" "jmp 1b;");
        break;

        case 11:
            // Segment Not Present #NP
            printf("#NP\n");
            printf("EIP: 0x%x\n", regs->eip);
            printf("EAX: 0x%x\n", regs->eax);
            printf("EBP: 0x%x\n", regs->ebp);
        break;
        
        case 12:
            // Stack Fault Exception #SS
            printf("#SS\n");
            printf("EIP: 0x%x\n", regs->eip);
            printf("EAX: 0x%x\n", regs->eax);
            printf("EBP: 0x%x\n", regs->ebp);
        break;
        
        case 13:{
            // General Protection Exception #GP
            printf("#GP\n");

            printf("Accessing memory caused a general protection exception.\n");
            printf("Faulting instruction at addres: 0x%x\n", regs->eip );
            printf("Error code: 0x%x\n", regs->err_code);

            if (regs->err_code != 0){
                printf("Fault due to entry at index: 0x%x (%d)\n", (regs->err_code >> 3 & 0xFFF ) , regs->err_code);


                uint8_t table = regs->err_code >> 1 & 0x3 ;
                
                if(table == 0   ){
                    printf("* Index references GDT\n");
                }
                if(table == 1  ){
                    printf("* Index references IDT\n");
                }

                if(table  == 2   ){
                    printf("* Index references LDT\n");
                }

                if(table  == 3 ){
                    printf("* Index references IDT\n");
                }

                if( regs->err_code & 0x1)
                {
                    printf("* Originated externally!\n");
                }
            }

            __asm__("cli;" "1: hlt;" "jmp 1b;");

        }
        break;
        
        case 14:
            // Page Fault Exception #PF
            printf("#PF\n");
            #define ALIGN(addr, align) (((addr) & ~((align) - 1)) + (align))
            FaultingAddress = processor::GetCR2();
            
            printf("Accessing the linear address 0x%x resulted in a page fault!\n\n", FaultingAddress);
        
            // Error code of 32 bits are on the stack
            // CR2 register contains the 32-bit linear virtual address that generated the exception
            // See Intel Software Developers manual Volume 3A Part 1 page 236 for more info
            #define PF_ERR_PRESENT_BIT 0x1
            #define PF_ERR_WRITE_BIT 0x2
            #define PF_ERR_USER_BIT 0x3
            #define PF_ERR_RESERVERD_WRITE_BIT 0x4
            #define PF_ERR_INSTRUCTION_FETCH_BIT 0x5
            #define PF_ERR_PROTECTION_KEY_BIT 0x6
            #define PF_ERR_SHADOW_STACK_BIT 0x7
            #define PF_ERR_SOFTWARE_GUARD_EXTENSION_BIT 0xE

            printf("REASON: \n\n");

         
            if (regs->err_code & PF_ERR_PRESENT_BIT ){
                printf("* Page protection violation!\n");
            } else{
                printf("* Page not-present!\n");
                Immediate_Map(FaultingAddress, FaultingAddress - 0xC0000000);
             
            }

            if(regs->err_code & PF_ERR_WRITE_BIT){
                printf("* Write access violation!\n");
            } else{
                printf("* Read access violation!\n");
            }

            if(regs->err_code & PF_ERR_USER_BIT){
                printf("* Violation from user-space (CPL=3)\n");
            }

            if(regs->err_code & PF_ERR_INSTRUCTION_FETCH_BIT){
                printf("* Caused by an instruction fetch. \n");
            }


            __asm__("cli;" "1: hlt;" "jmp 1b;");

        break;
        
        case 16:
            // x87 FPU Floating-point Error #MF
            printf("#MF\n");
            printf("EIP: 0x%x\n", regs->eip);
            printf("EAX: 0x%x\n", regs->eax);
            printf("EBP: 0x%x\n", regs->ebp);
        break;
        
        case 17:
            // Alignment Check Exception #AC
            printf("#AC\n");
            printf("EIP: 0x%x\n", regs->eip);
            printf("EAX: 0x%x\n", regs->eax);
            printf("EBP: 0x%x\n", regs->ebp); 
        break;
        
        case 18:
            // Machine-Check Exception #MC
            printf("#MC\n");
            printf("EIP: 0x%x\n", regs->eip);
            printf("EAX: 0x%x\n", regs->eax);
            printf("EBP: 0x%x\n", regs->ebp); 
        break;
        
        case 19:
            // SIMD Floating-point Exception #XM
            printf("#XM\n");
            printf("EIP: 0x%x\n", regs->eip);
            printf("EAX: 0x%x\n", regs->eax);
            printf("EBP: 0x%x\n", regs->ebp); 
        break;
        
        case 20:
             // Virtualization Exception #VE
            printf("#VE\n");
            printf("EIP: 0x%x\n", regs->eip);
            printf("EAX: 0x%x\n", regs->eax);
            printf("EBP: 0x%x\n", regs->ebp); 
        break;
        
        case 21:
             // Control Protection Exception #CP
            printf("#CP\n");
            printf("EIP: 0x%x\n", regs->eip);
            printf("EAX: 0x%x\n", regs->eax);
            printf("EBP: 0x%x\n", regs->ebp); 
        break;

        default:
            // PANIC!!!
            break;
        }
        
      
        
}

void irq_handler (registers regs) {

    

    switch (regs.int_no) {
    case 0:
            pit_tick++;
        break;
    case 1:
        // Keyboard interrupt !!

        int scan;
        int i;/*register*/

        // Read scancode 
        scan = inb(0x60);
        
        // Send ack message!
        i = inb(0x61);
        outb(0x61, i|0x80);
        outb(0x61, i);

        // NOTE: check for special scan codes 
        // e.g. modifiers etc..
        if( scan < 0x37){
            //printf("Read from IO: 0x%x\n", scan);
            keyPress.ScanCode = scan ;
            //printf( "[From Interrupt] Scancode: %x\n", keyPress.ScanCode);

        }

        
        break;    
    case 12:
        // PS2 Mouse interrupt 
        printf("Mouse event triggered!");
        //int event = inb(0x60);
        break;

    default:
        printf("Interrupt happened!");
        printf("Received INT: 0x%x\n", regs.int_no);
        break;
    }        

    outb(0x20, 0x20); // send end of interrupt to master

    if ( regs.int_no > 8 && regs.int_no <= 15) {
        outb(0xA0, 0x20); // send end of interrupt to slave 
    }
    
    
    if( regs.int_no == 13){
        printf(" Error code: %d \n", regs.err_code);

    }

}

void initidt(){
    // Initialise the IDT pointer
    idt_ptr.length = sizeof(IDT_entry) * 255;
    idt_ptr.base = (uint32_t)&idt_table;

#ifdef __VERBOSE__
    printf("Init IDT\n");
#endif

    // TODO: Set everything to zero first

    set_id_entry(0,  (uint32_t) irs0 , 0x08, 0x8F);
    set_id_entry(1,  (uint32_t) irs1 , 0x08, 0x8E);
    set_id_entry(2,  (uint32_t) irs2 , 0x08, 0x8E);
    set_id_entry(3,  (uint32_t) irs3 , 0x08, 0x8E);
    set_id_entry(4,  (uint32_t) irs4 , 0x08, 0x8E);
    set_id_entry(5,  (uint32_t) irs5 , 0x08, 0x8E);
    set_id_entry(6,  (uint32_t) irs6 , 0x08, 0x8E);
    set_id_entry(7,  (uint32_t) irs7 , 0x08, 0x8E);
    set_id_entry(8,  (uint32_t) irs8 , 0x08, 0x8E);
    set_id_entry(9,  (uint32_t) irs9 , 0x08, 0x8E);
    set_id_entry(10, (uint32_t) irs10 , 0x08, 0x8E);
    set_id_entry(11, (uint32_t) irs11 , 0x08, 0x8E);
    set_id_entry(12, (uint32_t) irs12 , 0x08, 0x8E);
    set_id_entry(13, (uint32_t) irs13 , 0x08, 0x8E);
    set_id_entry(14, (uint32_t) irs14 , 0x08, 0x8E);
    set_id_entry(15, (uint32_t) irs15 , 0x08, 0x8E);
    set_id_entry(16, (uint32_t) irs16 , 0x08, 0x8E);
    set_id_entry(17, (uint32_t) irs17 , 0x08, 0x8E);
    set_id_entry(18, (uint32_t) irs18 , 0x08, 0x8E);
    set_id_entry(19, (uint32_t) irs19 , 0x08, 0x8E); 
    set_id_entry(20, (uint32_t) irs20 , 0x08, 0x8E);
    set_id_entry(21, (uint32_t) irs21 , 0x08, 0x8E);
    set_id_entry(22, (uint32_t) irs22 , 0x08, 0x8E);
    set_id_entry(23, (uint32_t) irs23 , 0x08, 0x8E);
    set_id_entry(24, (uint32_t) irs24 , 0x08, 0x8E);
    set_id_entry(25, (uint32_t) irs25 , 0x08, 0x8E);
    set_id_entry(26, (uint32_t) irs26 , 0x08, 0x8E);
    set_id_entry(27, (uint32_t) irs27 , 0x08, 0x8E); 
    set_id_entry(28, (uint32_t) irs28 , 0x08, 0x8E);
    set_id_entry(29, (uint32_t) irs29 , 0x08, 0x8E);
    set_id_entry(30, (uint32_t) irs30 , 0x08, 0x8E);
    set_id_entry(31, (uint32_t) irs31 , 0x08, 0x8E);


    //print_serial("Remapping PIC\n");
    PIC_remap(0x20, 0x28);

    // clear mask for IRQ 12
    uint8_t value = inb(0x21) & ~(1<< 12);
    outb(0x21, value);



    // pic IRQ Table
    set_id_entry(32, (uint32_t)irq0, 0x08, 0x8E);
    set_id_entry(33, (uint32_t)irq1, 0x08, 0x8E); // PS2 Keyboard
    set_id_entry(34, (uint32_t)irq2, 0x08, 0x8E);
    set_id_entry(35, (uint32_t)irq3, 0x08, 0x8E);
    set_id_entry(36, (uint32_t)irq4, 0x08, 0x8E);
    set_id_entry(37, (uint32_t)irq5, 0x08, 0x8E);
    set_id_entry(38, (uint32_t)irq6, 0x08, 0x8E);
    set_id_entry(39, (uint32_t)irq7, 0x08, 0x8E);
    set_id_entry(40, (uint32_t)irq8, 0x08, 0x8E);
    set_id_entry(41, (uint32_t)irq9, 0x08, 0x8E);
    set_id_entry(42, (uint32_t)irq10, 0x08, 0x8E);
    set_id_entry(43, (uint32_t)irq11, 0x08, 0x8E);
    set_id_entry(44, (uint32_t)irq12, 0x08, 0x8E); // PS2 Mouse
    set_id_entry(45, (uint32_t)irq13, 0x08, 0x8E);
    set_id_entry(46, (uint32_t)irq14, 0x08, 0x8E);
    set_id_entry(47, (uint32_t)irq15, 0x08, 0x8E);

    
    idt_flush((uint32_t)&idt_ptr);
}

#include "pci.h"

#define PCI_BUS_ADDR_SHIFT 16
#define PCI_DEVICE_ADDR_SHIFT  11
#define PCI_FUNCTION_ADDR_SHIFT 8
#define PCI_ENABLE_ADDR_SHIFT 31

const char* GetClassCodeName (uint64_t ClassCode ) {
 
    switch (ClassCode)
    {
        case 0x0 :
            return "Unclassified";
        break;

        case 0x1:                 
            return "Mass Storage Controller";
        break;

        case 0x2: 
            return "Network Controller";
        break;

        case 0x3:          
            return "Display Controller"; 
        break;

        case 0x4:          
            return "Multimedia Controller";
        break;

        case 0x5:      
            return "Memory Controller";
        break;
    
        case 0x6:            
            return "Bridge";
        break;

        case 0x7 :                    
            return "Simple Communication Controller";
        break;

        case 0x8:
            return "Base System Peripheral"; 
        break;

        case 0x9:          
            return "Input Device Controller";
        break;  

        case 0xA:
            return "Docking station";
        break;  
        case 0xB:                
            return "Processor";
        break;   
        
        case 0xC:                 
            return "Serial Bus Controller";
        break; 
        
        case 0xD:                         
            return "Wireless Controller";
        break;  
        
        case 0xE:                            
            return "Intelligent Controller";
        break;   
        
        case 0xF:         
            return "Satellite Communication Controller";
        break;

        case 0x10:
            return "Encryption Controller";
        break;  
        
        case 0x11:       
            return "Signal Processing Controller";
        break;

        case 0x12:
            return "Processing Accelerator";
        break;

        case 0x13:   
            return "Non-Essential Instrumentation"; 
        break;

        default:
            return "Unknown";
            break;
        }
    
}

const char* getVendor( uint64_t VendorID){
    switch (VendorID)
    {
    case 0x8086:
        return "Intel Corporation";
        break;
    
    default:
        return "Vendor Unkown";
        break;
    }
}



uint32_t ConfigReadWord ( PCIBusAddress& PCIDeviceAddress , uint8_t offset){
    outl(CONFIG_ADDRESS , PCIDeviceAddress.getAddress() | offset );
    return inl(CONFIG_DATA);
}

uint32_t ConfigReadWord (uint8_t bus, uint8_t device, uint8_t func, uint8_t offset){
    uint32_t address;

    address = (uint32_t) (
        ((uint32_t) 1 << PCI_ENABLE_ADDR_SHIFT) |
        ((uint32_t)bus << PCI_BUS_ADDR_SHIFT) |
        ((uint32_t)device << PCI_DEVICE_ADDR_SHIFT) |
        ((uint32_t)func << PCI_FUNCTION_ADDR_SHIFT) | 
        offset );

    outl(CONFIG_ADDRESS, address);

    
    return inl(CONFIG_DATA);
}



uint8_t GetHeaderType( PCIBusAddress& PCIDeviceAddress ){
    uint32_t header_information = ConfigReadWord(PCIDeviceAddress , 0xC);
    return (uint8_t) (
        (header_information >> 16) //Get higher half
        & 0x00FF ); // Select the last two bytes 
}

uint16_t GetClassCodes( PCIBusAddress& PCIDeviceAddress ){
     uint32_t classcodes = ConfigReadWord(PCIDeviceAddress, 0x8);
                   return (uint16_t)((uint32_t)classcodes >> 16); 
                  
}

void PCI_Enumerate(){
            int devicesFound = 0;
        // loop through all possible busses, devices and their functions;
        for( int bus = 0 ; bus < 256 ; bus++)
        {
            
            for(int device = 0; device < 32 ; device ++)
            {
                 
                
                int function = 0;

                //uint64_t DeviceIdentify = ConfigReadWord(bus, device, function,0x0);
                uint32_t VendorID  = GetDevice(bus, device, function) & 0xFFFF;
                uint32_t DeviceID = GetDevice(bus, device, function) >> 16;

                

                if( DeviceID != 0xFFFF){
                    printf("Device found!\n");
                    printf("Bus: %d, Device: %d, function: %d \n", bus, device, function);
                    printf("DeviceID: 0x%x, VendorID: %s\n", DeviceID, getVendor(VendorID)  );

                    // iterate over the functions if it is a multi function device!
                    if( false ){
                        for ( function ++ ; function < 8; function++)
                        {
                        
                        }
                    }

                    PCIBusAddress busAddress =
                        PCIBusAddress{bus, device, function };

                    uint8_t header_type = GetHeaderType(busAddress);
                    printf( "Header type: 0x%x\n", header_type);

                    uint16_t deviceClasses = GetClassCodes(busAddress);
                    printf(" class: %s, subClass: %d\n\n",
                     (deviceClasses >>8) > 0x13 ? "Unknown": GetClassCodeName((deviceClasses >>8)), deviceClasses & 0xFF);

                    devicesFound++;            
                }
            }
            
        }

        printf("Found %d PCI devices!\n", devicesFound);
}

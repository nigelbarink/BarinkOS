#include "pci.h"

void PCI::Scan(){
     
        int devicesFound = 0;

        printf("Start finding devices, Found: %d devices");
        // loop through all possible busses, devices and their functions;
        for( int bus = 0 ; bus < 256 ; bus++)
        {
            
            for(int device = 0; device < 32 ; device ++)
            {
                int function = 0;

                uint64_t DeviceIdentify = PCI::ConfigReadWord(bus, device, function,0x0);
                uint32_t DeviceID = GetDevice(bus, device, function) >> 16;

                if( DeviceID != 0xFFFF){
                    PCIBusAddress busAddress =
                    PCIBusAddress{bus, device, function };

                    PrintPCIDevice(busAddress);

                    // iterate over the functions if it is a multi function device!
                    if( PCI::IsMultiFunctionDevice(busAddress) ){
                        printf("Multi function device! \n");
                        printf("Check remaining Functions\n");
                        for ( function = 1  ; function < 8; function++)
                        {
                            uint32_t DeviceID = GetDevice(bus, device, function) >> 16;

                            if( DeviceID != 0xFFFF){
                                PCIBusAddress busAddress2 = PCIBusAddress{bus, device, function};
                                PrintPCIDevice(busAddress2);
                                devicesFound++;
                            }
                        }
                    
                    }

                    
                    devicesFound++;            
                }
            }
            
        }

        printf("Found %d PCI devices!\n", devicesFound);
}

const char* PCI::getClassName (uint8_t ClassCode){
    bool isKnown = (ClassCode < PCI::KnownClassCodes);
    return  isKnown ? PCI::ClassCodeNames[ClassCode].name : "Unknown ClassCode";
}

const char* PCI::getVendor( uint32_t VendorID){
    switch (VendorID)
    {
        case 0x8086:
            return "Intel Corporation";
            break;

        case 0x10DE:
            return "NVIDIA Corporation";
            break;

        case 0x1022:
            return "Advanced Micro Devices, Inc.[AMD]";
            break;

        case 0x1002:
            return "Advanced Micor Devices, Inc.[AMD/ATI]";
            break;

        case 0xbeef:
            return "VirtualBox Graphics Adapter";
            break;

        case 0xcafe:
            return "VirtualBox Guest Service";
            break;

        default:
            return "Vendor Unkown";
            break;
    }

}

uint64_t PCI::GetDevice (int bus, int device, int function ){
    return PCI::ConfigReadWord(bus, device, function,0x0);
}

bool PCI::IsMultiFunctionDevice(PCIBusAddress& PCIDeviceAddress)
{
    uint32_t header_information = ConfigReadWord(PCIDeviceAddress, 0xC);
    return (((header_information>>16)
             & 0x80)
            >> 7  );
}

uint16_t PCI::GetClassCodes( PCIBusAddress& PCIDeviceAddress ){
    return (uint16_t)(ConfigReadWord(PCIDeviceAddress, 0x8) >> 16);
}

uint8_t PCI::GetHeaderType( PCIBusAddress& PCIDeviceAddress ){
    uint32_t header_information = ConfigReadWord(PCIDeviceAddress , 0xC);
    return (uint8_t) (
            ((header_information >> 16) //Get higher half
             & 0x00FF) // Select the last two bytes
            & 0x7F ); // Mask bit 7 as it indicates if the device is a mulit function device!
}

uint32_t PCI::ConfigReadWord (uint8_t bus, uint8_t device, uint8_t func, uint8_t offset){
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

uint8_t PCI::GetProgIF (PCIBusAddress& PCIDeviceAddress){
    uint32_t data = ConfigReadWord(PCIDeviceAddress, 0x8);
    return ((data >> 8) & 0xFF);
}

uint32_t PCI::ConfigReadWord ( PCIBusAddress& PCIDeviceAddress , uint8_t offset){
    outl(CONFIG_ADDRESS , PCIDeviceAddress.getAddress() | offset );
    return inl(CONFIG_DATA);
}


uint32_t PCI::ReadBAR ( PCIBusAddress& PCIDeviceAddress, int bar_number){
    int offsetToBar = 0x10 + (bar_number* 0x4);
    return ConfigReadWord(PCIDeviceAddress, offsetToBar);
}

void PCI::PrintPCIDevice (PCIBusAddress& PCIDeviceAddress)
{
    uint32_t DeviceID =  (PCI::GetDevice(PCIDeviceAddress.bus, PCIDeviceAddress.device, PCIDeviceAddress.function) >> 16);
    uint32_t VendorID  = PCI::GetDevice(PCIDeviceAddress.bus, PCIDeviceAddress.device, PCIDeviceAddress.function) & 0xFFFF;
    printf("Device found!\n");
    printf("Bus: %d, Device: %d, function: %d \n", PCIDeviceAddress.bus, PCIDeviceAddress.device, PCIDeviceAddress.function);
    printf("DeviceID: 0x%x, Vendor: %s\n",
           DeviceID
            , PCI::getVendor(VendorID)  );

    uint8_t header_type = PCI::GetHeaderType(PCIDeviceAddress);
    printf( "Header type: 0x%x\n", header_type);

    uint16_t deviceClasses = PCI::GetClassCodes(PCIDeviceAddress);

    printf("class: %s, subClass: %d\n\n", PCI::getClassName((deviceClasses >> 8)), deviceClasses & 0xFF);

}
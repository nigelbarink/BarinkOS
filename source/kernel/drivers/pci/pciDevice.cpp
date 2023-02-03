 #include "pciDevice.h"

// NOTE: we would really like to return a pointer 
// to the newly created PCIBusAddress struct; 
 PCIBusAddress const PCIDevice::PCIAddress(){
     return PCIBusAddress{bus ,device, function};
 }
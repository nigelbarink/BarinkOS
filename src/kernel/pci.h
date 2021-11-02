#pragma once
#include <stdint.h>
#include "io.h"
// Configuration Space Access Mechanism #1
#define CONFIG_ADDRESS 0xCF8 // Configuration adress that is to be accessed
#define CONFIG_DATA 0xCFC // Will do the actual configuration operation

/*
CONFIG_ADDRESS 

32 bit register 

bit 31      Enable bit      (Should CONFIG_DATA be translatedc to configuration cycles)      
bit 30 - 24 Reserved    
bit 23 - 16 Bus Number      (Choose a specific PCI BUS)
bit 15 - 11 Device Number   (Selects specific device one the pci bus)
bit 10 - 8  Function Number (Selects a specific function in a device)
bit 7 -  0  Register Offset (Offset in the configuration space of 256 Bytes ) NOTE: lowest two bits will always be zero 

*/


/* 
PCI Device structure

Register    offset  bits 31-24  bits 23-16  bits 15-8   bits 7-0
00          00      Device ID   <----       Vendor  ID  <-------
01          04      Status      <----       Command     <-------
02          08      Class code  Sub class   Prog IF     Revision ID
03          0C      BIST        Header Type Ltncy Timer Cache line Size
04          10      Base address #0 (BAR0) 
05          14      Base address #1 (BAR1) 
06          18      Base address #2 (BAR2) 
07          1C      Base address #3 (BAR3)
08          20      Base address #4 (BAR4)
09          24      Base address #5 (BAR5)
0A          28      Cardbus CIS Pointer
0B          2C      Subsystem ID <------   Subsystem Vendor ID  <-------
0C          30      Expansion ROM base address
0D          34      Reserved     <-------   Capabilities Pointer <------
0E          38      Reserved    <-------    <--------   <--------
0F          3C      Max ltncy   Min Grant   Interrupt PIN   Interrupt Line

*/


/*
The idea for now is to support the minimal things necessary to find ATA supported drives
 */


// Lets write some boiler plate configuration code

uint16_t ConfigReadWord (uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);

uint16_t CheckVendor (uint8_t bus, uint8_t slot);

void checkDevice (uint8_t bus, uint8_t device );
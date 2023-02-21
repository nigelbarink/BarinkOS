#pragma once
#include <stdint-gcc.h>
#include "../../../CoreLib/Path.h"
#include "StorageTypes.h"
 class VirtualFileSystem
 {
 public:
     static void initialize();
     static void Mount(PFS fs, unsigned int DeviceID);
     static void Unmount(unsigned int DeviceID);
     static FILE OpenFile(const char* path);
     static void RegisterPartition(PTR_PARTITION partition);

 private:
     static const unsigned int DEVICE_MAX = 26;
     static const unsigned int PARTITION_MAX = 4 * DEVICE_MAX;
     static PFS _filesystems[DEVICE_MAX];
     static unsigned int num_partitions;
     static PTR_PARTITION _partitions [PARTITION_MAX];
 };
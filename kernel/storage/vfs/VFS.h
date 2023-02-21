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
     static void ResolvePath(Path& path);

 private:
     static const unsigned int DEVICE_MAX = 26;
     static PFS _filesystems[DEVICE_MAX];

 };
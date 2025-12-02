# toolchain-i686-elf.cmake
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR i686)

# Point to your custom cross tools
set(CMAKE_C_COMPILER /opt/cross/bin/i686-elf-gcc)
set(CMAKE_CXX_COMPILER /opt/cross/bin/i686-elf-g++)
set(CMAKE_ASM_COMPILER /opt/cross/bin/i686-elf-as)
set(CMAKE_AR /opt/cross/bin/i686-elf-ar)
set(CMAKE_RANLIB /opt/cross/bin/i686-elf-ranlib)
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

# Flags for freestanding OS development
set(CMAKE_C_FLAGS "-ffreestanding -Og -ggdb -Wall -Wextra")
set(CMAKE_CXX_FLAGS "-ffreestanding -Og -ggdb -Wall -Wextra")


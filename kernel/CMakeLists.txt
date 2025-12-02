
project(Kernel LANGUAGES C CXX ASM)

set(SOURCES
	main.cpp
	boot.s
)

add_executable(kernel.elf ${SOURCES})
target_link_libraries(kernel.elf PRIVATE CoreLib)
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/link.ld)
target_link_options(kernel.elf PRIVATE "-T${LINKER_SCRIPT}")


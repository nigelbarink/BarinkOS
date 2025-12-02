project(CoreLib LANGUAGES CXX)

set(SOURCES
	ctype.cpp
	Memory.cpp
	Path.cpp
	Stack.cpp
	String.cpp
	StringView.cpp
)

add_library(CoreLib STATIC ${SOURCES})

target_include_directories(CoreLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

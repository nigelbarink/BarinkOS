CPP = /opt/cross/bin/i686-elf-g++
CFLAGS = -ffreestanding -Og -ggdb -Wall -Wextra

BUILD_DIR = ../build/CoreLib
OBJ_FOLDER = ../bin/CoreLib
OUTPUTFILE = $(BUILD_DIR)/libCoreLib.a

OFILES = $(OBJ_FOLDER)/ctype.o $(OBJ_FOLDER)/memory.o $(OBJ_FOLDER)/path.o $(OBJ_FOLDER)/stack.o $(OBJ_FOLDER)/string.o $(OBJ_FOLDER)/stringview.o

.phony: all
all: $(OUTPUTFILE)
	cp *.h $(BUILD_DIR)/include/CoreLib

$(OUTPUTFILE): $(OFILES)
	pwd
	ar -rc $(OUTPUTFILE) $(OFILES)



$(OBJ_FOLDER)/ctype.o: ctype.cpp
	$(CPP) -c ctype.cpp -o $(OBJ_FOLDER)/ctype.o $(CFLAGS)

$(OBJ_FOLDER)/memory.o: Memory.cpp
	$(CPP) -c Memory.cpp -o $(OBJ_FOLDER)/memory.o $(CFLAGS)

$(OBJ_FOLDER)/path.o: Path.cpp
	$(CPP) -c Path.cpp  -o $(OBJ_FOLDER)/path.o $(CFLAGS)

$(OBJ_FOLDER)/stack.o: Stack.cpp
	$(CPP) -c Stack.cpp  -o $(OBJ_FOLDER)/stack.o $(CFLAGS)

$(OBJ_FOLDER)/string.o: String.cpp
	$(CPP) -c String.cpp -o $(OBJ_FOLDER)/string.o $(CFLAGS)

$(OBJ_FOLDER)/stringview.o: StringView.cpp
	$(CPP) -c StringView.cpp  -o $(OBJ_FOLDER)/stringview.o $(CFLAGS)


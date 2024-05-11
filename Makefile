CC = g++
CFLAGS = -std=c++20
SRC_DIR = src
UTILS_DIR = src/utils
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(UTILS_DIR)/handlers/*.cpp)
HEADER_FILES = $(wildcard $(UTILS_DIR)/structs/*.hpp)
TARGET = club_manager

all: $(TARGET)

$(TARGET): $(SRC_FILES) $(HEADER_FILES)
	$(CC) $(CFLAGS) $(SRC_FILES) -o $(TARGET)

clean:
	del $(TARGET).exe
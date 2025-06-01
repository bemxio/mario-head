# constants
PREFIX = i686-w64-mingw32

CXX = $(PREFIX)-g++
CXXFLAGS = -std=c++98 -static -O2
CXXLIBS = -lntdll -lstrmiids -lole32 -luser32

WINDRES = $(PREFIX)-windres

SRC_DIR = src
ASSETS_DIR = assets
BUILD_DIR = build

EXECUTABLE = mario_head.exe

# targets
all: $(BUILD_DIR)/$(EXECUTABLE)

clean:
	$(RM) -r $(BUILD_DIR)

# rules
$(BUILD_DIR)/$(EXECUTABLE): $(SRC_DIR)/main.cpp $(BUILD_DIR)/resources.o | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(CXXLIBS)

$(BUILD_DIR)/resources.o: $(SRC_DIR)/resources.rc $(wildcard $(ASSETS_DIR)/*) | $(BUILD_DIR)
	$(WINDRES) -i $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
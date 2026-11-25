CXX = g++
CC  = gcc
CXXFLAGS = -std=c++17 -Wall -Wextra -Isrc -Ithird_party -Ithird_party/cJSON -MMD -MP
CFLAGS   = -Wall -Wextra -Ithird_party/cJSON -MMD -MP
LDFLAGS  = -static
LDLIBS   = -lraylib -l:libglfw3.dll.a -lgdi32 -luser32 -ladvapi32 -lcomdlg32 -lwinmm -lopengl32 -lm -lole32

TARGET      = game.exe
TEST_TARGET = tests.exe
BUILD       = build
SRC_DIR     = src
TEST_DIR    = tests
CJSON_DIR   = third_party/cJSON

# Source discovery (recursive)
HDRS       = $(shell find $(SRC_DIR) -name '*.h')
LIB_SRCS   = $(shell find $(SRC_DIR) -name '*.cpp' ! -name main.cpp)
MAIN_SRC   = $(SRC_DIR)/main.cpp
TEST_SRCS  = $(shell find $(TEST_DIR) -name '*.cpp')
CJSON_SRCS = $(wildcard $(CJSON_DIR)/*.c)

# Object files, mirroring the source folders
LIB_OBJS   = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD)/%.o,$(LIB_SRCS))
MAIN_OBJ   = $(BUILD)/main.o
TEST_OBJS  = $(patsubst $(TEST_DIR)/%.cpp,$(BUILD)/tests/%.o,$(TEST_SRCS))
CJSON_OBJS = $(patsubst $(CJSON_DIR)/%.c,$(BUILD)/cjson/%.o,$(CJSON_SRCS))
DEPS       = $(LIB_OBJS:.o=.d) $(MAIN_OBJ:.o=.d) $(TEST_OBJS:.o=.d) $(CJSON_OBJS:.o=.d)

.PHONY: all tests clean

all: $(TARGET)

tests: $(TEST_TARGET)

$(TARGET): $(LIB_OBJS) $(MAIN_OBJ) $(CJSON_OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS) $(LDLIBS)

$(TEST_TARGET): $(LIB_OBJS) $(TEST_OBJS) $(CJSON_OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS) $(LDLIBS)

$(BUILD)/%.o: $(SRC_DIR)/%.cpp $(HDRS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/tests/%.o: $(TEST_DIR)/%.cpp $(HDRS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/cjson/%.o: $(CJSON_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -rf $(BUILD) $(TARGET) $(TEST_TARGET)
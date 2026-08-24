CXX = g++

SRC_DIR = src
INC_DIR = include 
BUILD_DIR = build
BIN_DIR = bin

TARGET = $(BIN_DIR)/app

CFLAGS = -Wall -g -I$(INC_DIR) -MMD -MP -std=c++17 

SRCS := $(shell find $(SRC_DIR) -name "*.cpp")
VPATH := $(sort $(dir $(SRCS)))
OBJS := $(addprefix $(BUILD_DIR)/,$(notdir $(SRCS:.cpp=.o)))
DEPS = $(OBJS:.o=.d)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $^ -o $@

$(BUILD_DIR)/%.o: %.cpp

	$(CXX) $(CFLAGS) -c $< -o $@
-include $(DEPS)

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)

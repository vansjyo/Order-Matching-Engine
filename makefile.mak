# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 	-Wall -Wextra

# Directories
SRC_DIR := src
INC_DIR := headers
OBJ_DIR := obj
BIN_DIR := bin

# Source files
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))
EXECUTABLE := $(BIN_DIR)/program

# Helper files
HELPER_DIR := helpers
HELPER_SOURCES := $(wildcard $(HELPER_DIR)/*.cpp)
HELPER_OBJECTS := $(patsubst $(HELPER_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(HELPER_SOURCES))


# Include directories
INCLUDES := -I$(INC_DIR)

# Build targets
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(HELPER_OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@


$(OBJ_DIR)/%.o: $(HELPER_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@


clean:
	$(RM) -r $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean




# main: main.o helper.o output.o
# 	g++ -std=c++17 main.o helper.o limit.o order.o orderbook.o globalVar.o -o main

# main.o: main.cpp
# 	g++ -std=c++17 -c main.cpp

# helper.o: helper.cpp
# 	g++ -std=c++17 -c helper.cpp

# limit.o: limit.cpp
# 	g++ -std=c++17 -c ./Classes/limit.cpp

# order.o: order.cpp
# 	g++ -std=c++17 -c ./Classes/order.cpp

# orderbook.o: orderbook.cpp
# 	g++ -std=c++17 -c ./Classes/orderbook.cpp

# globalVar.o: globalVar.cpp
# 	g++ -std=c++17 -c globalVar.cpp

# clean: 
# 	rm *.o main
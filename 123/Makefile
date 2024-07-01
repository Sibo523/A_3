CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -pg
INCLUDES = -I.

# Define the source, object, and executable files
SRC = kosaraju.cpp main.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = kosaraju

TEST_SRC = test.cpp kosaraju.cpp
# Exclude main.cpp from the test object files to avoid multiple definitions of main
TEST_OBJ = $(filter-out main.o, $(TEST_SRC:.cpp=.o))
TEST_TARGET = test_kosaraju

# Default target
all: $(TARGET)

# Main program target
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ -lpthread

# Test target
test: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

# To obtain object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@ 

clean:
	rm -f $(TARGET) $(TEST_TARGET) $(OBJ) $(TEST_OBJ) gmon.out

.PHONY: all clean test
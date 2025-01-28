# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall

# Source files
SRCS = main.cpp process.cpp MLFQ.cpp scheduler.cpp

# Object files (replace .cpp with .o)
OBJS = $(SRCS:.cpp=.o)

# Output executable name
OUT = scheduler

# Default target to build the program
all: $(OUT)

# Rule to link object files and create the executable
$(OUT): $(OBJS)
	$(CXX) $(OBJS) -o $(OUT)

# Rule to compile .cpp files into .o object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up compiled object files and the executable
clean:
	rm -f $(OBJS) $(OUT)

# Phony targets (to avoid conflicts with files named 'all' or 'clean')
.PHONY: all clean

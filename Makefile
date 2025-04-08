# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Source files
SRCS = main.cpp user_manager.cpp crypto_util.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = user_system

# Filesystem library (needed for std::filesystem)
LDFLAGS = -lstdc++fs

# Default target
all: $(EXEC)

# Link the executable
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(EXEC)

# Run the program
run: $(EXEC)
	./$(EXEC)

# Phony targets (don't create files with these names)
.PHONY: all clean run
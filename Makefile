CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O2
TARGET = terminal
SRC = main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	@echo "Compiling $(TARGET)..."
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)
	@echo "Build completed successfully."

run: $(TARGET)
	@echo "Running terminal..."
	./$(TARGET)

clean:
	@echo "Cleaning build files..."
	rm -f $(TARGET)
	@echo "Clean completed."

.PHONY: all clean run

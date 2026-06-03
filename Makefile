CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O2
TARGET = terminal
SRC = main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	@echo "Компиляция $(TARGET)..."
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)
	@echo "Сборка завершена успешно."

run: $(TARGET)
	@echo "Запуск терминала..."
	./$(TARGET)

clean:
	@echo "Очистка..."
	rm -f $(TARGET)
	@echo "Очистка завершена."

.PHONY: all clean run
# שם הקובץ ההרצה
TARGET = kosaraju

# קובץ המקור
SRC = kosaraju.cpp

# הדגלים עבור הקומפיילר
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2

# כלל ברירת מחדל לבנייה
all: $(TARGET)

# כלל לבנייה
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# כלל לניקוי קבצים
clean:
	rm -f $(TARGET)

# כלל להרצת התוכנית עם קלט
run: $(TARGET)
	./$(TARGET) < input.txt

.PHONY: all clean run

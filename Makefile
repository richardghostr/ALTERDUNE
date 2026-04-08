CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
TARGET   = alterdune

SRC = src/main.cpp \
      src/Entity.cpp \
      src/Player.cpp \
      src/Monster.cpp \
      src/Normal.cpp \
      src/MiniBoss.cpp \
      src/Boss.cpp \
      src/Item.cpp \
      src/Inventory.cpp \
      src/ActAction.cpp \
      src/Bestiary.cpp \
      src/FileLoader.cpp \
      src/Game.cpp

OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
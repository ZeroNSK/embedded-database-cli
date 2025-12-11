TARGET = program
CXX = clang++
CXXFLAGS = -std=c++17 
SRC = main.cpp \
      database.cpp \
      set.cpp \
      stack.cpp \
      queue.cpp \
      hash.cpp \
      tree.cpp

OBJ = $(SRC:.cpp=.o)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

rebuild: clean $(TARGET)



LIB_PATH = ../lib
SRC_PATH = ../src
BIN_PATH = ../bin
TEST_PATH = ../test
CXX = g++
CXXFLAGS = -std=c++11 
CXXBOOST += -lboost_filesystem -lboost_system
LDPATH += 
SOURCES = $(notdir $(wildcard ../src/[^main]*.cpp))
#HEADERS = $(wildcard *.h*)
OBJECTS = $(SOURCES:.cpp=.o)
EXEC = run
TEST_EXEC = test_run

all: $(TEST_EXEC)
	@echo "Complete"

# Main target
$(TEST_EXEC):  $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(addprefix $(LIB_PATH)/,$(OBJECTS:.cpp=.o)) test_main.cpp -o $(BIN_PATH)/$(TEST_EXEC) $(CXXBOOST)

# To obtain object files
#%.o: %.cpp
#	$(CXX) $(CXXFLAGS) -c $(SRC_PATH)/$< -o $(LIB_PATH)/$@ $(CXXBOOST)

# To obtain object files
$(OBJECTS) : 
	$(CXX) $(CXXFLAGS) -c $(SRC_PATH)/$(@:.o=.cpp) -o $(LIB_PATH)/$@ $(CXXBOOST)


# To remove generated files
clean:
	rm -f $(BIN_PATH)/$(EXEC) $(LIB_PATH)/$(OBJECTS)

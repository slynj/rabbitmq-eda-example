CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

INCLUDES = -I/opt/homebrew/include
LIBS = -L/opt/homebrew/lib -lSimpleAmqpClient -lboost_system

TARGET = eda
SRCS = producer.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET) clean_obj

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LIBS) $(INCLUDES)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
	
clean_obj:
	rm -f $(OBJS)

.PHONY: all clean


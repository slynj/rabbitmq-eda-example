CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

INCLUDES = -I/opt/homebrew/include
LIBS = -L/opt/homebrew/lib -lSimpleAmqpClient -lboost_system

TARGETS = publisher direct
SRCS = publisher.cpp direct.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGETS) clean_obj

publisher: publisher.cpp
	$(CXX) $(CXXFLAGS) publisher.cpp -o publisher $(LIBS) $(INCLUDES)

direct: direct.cpp
	$(CXX) $(CXXFLAGS) direct.cpp -o direct $(LIBS) $(INCLUDES)

clean:
	rm -f $(OBJS) $(TARGEST)
	
clean_obj:
	rm -f $(OBJS)

.PHONY: all clean

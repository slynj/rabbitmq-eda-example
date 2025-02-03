CXX = g++
CXXFLAGS = -std=c++17 -Wall -g

INCLUDES = -I/usr/local/include -I/opt/homebrew/include
LIBS = -L/usr/local/lib -L/opt/homebrew/lib -lSimpleAmqpClient -lrabbitmq -lboost_system

TARGETS = producer direct timestamp
SRCS = producer.cpp consumer.cpp timestamp.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGETS) clean_obj

producer: producer.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) producer.cpp -o producer $(LIBS)

direct: consumer.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) consumer.cpp -o direct $(LIBS)

timestamp: timestamp.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) timestamp.cpp -o timestamp $(LIBS)

clean_obj:
	rm -f $(OBJS)

clean:
	rm -f producer consumer timestamp

.PHONY: all clean clean_obj
CXX = g++
CXXFLAGS = -std=c++17 -Wall -g

INCLUDES = -I/opt/homebrew/include -I/usr/local/include
LIBS = -L/opt/homebrew/lib -L/usr/local/lib -lamqpcpp -lssl -lcrypto -lboost_system -lboost_thread

TARGETS = producer direct
SRCS = producer.cpp directConsumer.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGETS) clean_obj

producer: producer.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) producer.cpp -o producer $(LIBS) 

direct: directConsumer.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) directConsumer.cpp -o direct $(LIBS) 

clean_obj:
	rm -f $(OBJS)

clean:
	rm -f producer direct

.PHONY: all clean clean_obj
#include <amqpcpp.h>
#include <iostream>
#include "MyConnectionHandler.h"

void messageReceived(const AMQP::Message &message, uint64_t deliveryTag, bool redeliverd) {
    std::string body(message.body(), message.bodySize());
    std::cout << "Received message: " << body << std::endl;
}

int main() {
    MyConnectionHandler handler;
    
    AMQP::Connection connection(&handler, AMQP::Address("amqp://guest:guest@localhost:15672"));
    AMQP::Channel channel(&connection);

    std::string queue_name = "queue_lyn";
    
    channel.declareQueue(queue_name);
    channel.bindQueue("exchange_lyn", queue_name, "key_lyn");
    channel.consume(queue_name).onReceived(messageReceived);

    std::cout << "Waiting for messages ..." << std::endl;
}
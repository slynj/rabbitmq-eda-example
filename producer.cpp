#include <amqpcpp.h>
#include <iostream>
#include "MyConnectionHandler.h"

int main() {
    // boost::asio::io_context io_context;

    // AMQP::LibBoostAsioHandler handler(io_context);
    MyConnectionHandler handler;
    
    AMQP::Connection connection(&handler, AMQP::Address("amqp://guest:guest@localhost:15672"));
    AMQP::Channel channel(&connection);
    // AMQP::TcpConnection connection(&handler, );
    // AMQP::TcpChannel channel(&connection);

    channel.declareExchange("exchange_lyn", AMQP::direct);

    std::string message = "Hello World!";
    std::string routing_key = "key_lyn";

    channel.publish("exchange_lyn", routing_key, message);
    std::cout << "Message sent: " << message << std::endl;
}

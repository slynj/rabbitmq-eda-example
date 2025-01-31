#include <amqpcpp.h>
#include <amqpcpp.h/libboostasio.h>
#include <boost/asio.hpp>
#include <iostream>

int main() {
    boost::asio::io_context io_context;

    AMQP::LibBoostAsioHandler handler(io_context);
    AMQP::TcpConnection connection(&handler, AMQP::Address("amqp://guest:guest@localhost/"));
    AMQP::TcpChannel channel(&connection);

    channel.declareExchange("my_exchange", AMQP::direct);

    std::string message = "Hello World!";
    std::string routing_key = "my_routing_key";

    channel.publish("my_exchange", routing_key, message);
    std::cout << "Message sent: " << message << std::endl;
}

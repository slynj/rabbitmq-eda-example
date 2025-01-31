#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <iostream>

int main() {
    auto channel = AmqpClient::Channel::Create("localhost");

    channel->DeclareExchange("exchange_lyn", AmqpClient::Channel::EXCHANGE_TYPE_DIRECT);

    std::string message = "Hello World~!";
    channel->BasicPublish("exchange_lyn", "routing_key_lyn", AmqpClient::BasicMessage::Create(message));

    std::cout << "Message sent: " << message << std::endl;
}
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <iostream>

int main() {
    auto channel = AmqpClient::Channel::Create("localhost");

    std::string queue_name = "queue_lyn";
    channel->DeclareQueue(queue_name, false, false, false);
    channel->BindQueue(queue_name, "exchange_lyn", "routing_key_lyn");

    std::cout << "Waiting for messages..." << std::endl;
    AmqpClient::Envelope::ptr_t envelope;
    std::string consumer_tag = channel->BasicConsume(queue_name);

    while (true) {
        bool success = channel->BasicConsumeMessage(consumer_tag, envelope);

        if (success) {
            std::cout << "Received message: " << envelope->Message()->Body() << std::endl;
        }
    }
}
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <iostream>
#include <string>

// docker run -d --name rabbitmq -p 5672:5672 -p 15672:15672 rabbitmq:management

int main() {
    AmqpClient::Channel::ptr_t channel = AmqpClient::Channel::Create("localhost");

    channel->DeclareExchange("order_status_exchange", AmqpClient::Channel::EXCHANGE_TYPE_DIRECT);
    channel->DeclareExchange("order_notification_exchange", AmqpClient::Channel::EXCHANGE_TYPE_FANOUT);
    channel->DeclareExchange("payment_status_exchange", AmqpClient::Channel::EXCHANGE_TYPE_TOPIC);

    std::string message_type, message;

    while (true) {
        std::cout << "Enter message type (direct/fanout/topic): ";
        std::cin >> message_type;
        std::cin.ignore();
        std::cout << "Enter message: ";
        std::getline(std::cin, message);

        if (message_type == "direct") {
            std::string status;
            std::cout << "Enter order status (success/fail)";
            std::cin >> status;

            channel->BasicPublish("order_status_exchange", status, AmqpClient::BasicMessage::Create(message));

            std::cout << "[Direct] Sent message: " << message << " with status: " << status << std::endl;
        } else if (message_type == "fanout") {
            channel->BasicPublish("order_notification_exchange", "", AmqpClient::BasicMessage::Create(message));

            std::cout << "[Fanout] Broadcast message: " << message << std::endl;
        } else if (message_type == "topic") {
            std::string routing_key;
            std::cout << "Enter routing key (payment.success/payment.fail): ";
            std::cin >> routing_key;

            channel->BasicPublish("payment_status_exchange", routing_key, AmqpClient::BasicMessage::Create(message));
            
            std::cout << "[Topic] Sent message: " << message << " with key: " << routing_key << std::endl;
        } else {
            std::cout << "Invalid message type!" << std::endl;
        }
    }
}
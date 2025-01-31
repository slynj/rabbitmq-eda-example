#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <iostream>
#include <string>

int main() {
    AmqpClient::Channel::ptr_t channel = AmqpClient::Channel::Create("localhost");

    std::string ORDER = "order_status_exchange";
    std::string NOTIFY = "order_notification_exchange";
    std::string PAYMENT = "payment_status_exchange";

    channel->DeclareExchange(ORDER, AmqpClient::Channel::EXCHANGE_TYPE_DIRECT);
    channel->DeclareExchange(NOTIFY, AmqpClient::Channel::EXCHANGE_TYPE_FANOUT);
    channel->DeclareExchange(PAYMENT, AmqpClient::Channel::EXCHANGE_TYPE_TOPIC);

    while (true) {
        std::string message_type, message;
        std::cout << "\nEnter message type (direct[d]/fanout[f]/topic[t]/exit): ";
        std::cin >> message_type;
        if (message_type == "exit") break;

        std::cin.ignore();
        std::cout << "Enter message: ";
        std::getline(std::cin, message);

        if (message_type == "d") {
            std::string status;
            std::cout << "Enter order status (success/fail): ";
            std::cin >> status;
            channel->BasicPublish(ORDER, status, AmqpClient::BasicMessage::Create(message));
            std::cout << "[Direct] Sent message: " << message << " with status: " << status << std::endl;
        } else if (message_type == "f") {
            channel->BasicPublish(NOTIFY, "", AmqpClient::BasicMessage::Create(message));
            std::cout << "[Fanout] t message: " << message << std::endl;
        } else if (message_type == "topic") {
            std::string routing_key;
            std::cout << "Enter routing key (e.g., payment.success, payment.failed): ";
            std::cin >> routing_key;
            channel->BasicPublish(PAYMENT, routing_key, AmqpClient::BasicMessage::Create(message));
            std::cout << "[Topic] Sent message: " << message << " with key: " << routing_key << std::endl;
        } else {
            std::cout << "Invalid message type!" << std::endl;
        }
    }
}
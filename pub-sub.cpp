#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <iostream>
#include <string>

// docker run -d --name rabbitmq -p 5672:5672 -p 15672:15672 rabbitmq:management

const std::string ORDER = "order_status_exchange";
const std::string NOTI = "order_notification_exchange";
const std::string PAYMENT = "payment_status_exchange";

void producer(AmqpClient::Channel::ptr_t channel) {
    

    std::string message_type, message;

    while (true) {
        std::cout << "Enter message type (d/f/t/exit): ";
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
            channel->BasicPublish(NOTI, "", AmqpClient::BasicMessage::Create(message));

            std::cout << "[Fanout] Broadcast message: " << message << std::endl;
        } else if (message_type == "t") {
            std::string routing_key;
            std::cout << "Enter routing key (payment.success/payment.fail): ";
            std::cin >> routing_key;

            channel->BasicPublish(PAYMENT, routing_key, AmqpClient::BasicMessage::Create(message));
            
            std::cout << "[Topic] Sent message: " << message << " with key: " << routing_key << std::endl;
        } else {
            std::cout << "Invalid message type!" << std::endl;
        }
    }
}


void direct_consumer(AmqpClient::Channel::ptr_t channel) {
    std::string qname_s = "success_queue";
    std::string qname_f = "fail_queue";

    channel->DeclareQueue(qname_s);
    channel->BindQueue(qname_s, ORDER, "success");

    channel->DeclareQueue(qname_f);
    channel->BindQueue(qname_f, ORDER, "fail");

    std::cout << "[Direct Consumer] Waiting for success messages..." << std::endl;
    
    while (true) {
        AmqpClient::Envelope::ptr_t envelope_s;
        AmqpClient::Envelope::ptr_t envelope_f;

        bool received_s = channel->BasicConsumeMessage(qname_s, envelope_s, 1000);
        bool received_f = channel->BasicConsumeMessage(qname_f, envelope_f, 1000);

        if (received_s) {
            std::cout << "[Direct Consumer] Received: " << envelope_s->Message()->Body() << std::endl;
        } else if (received_f) {
            std::cout << "[Direct Consumer] Received: " << envelope_f->Message()->Body() << std::endl;
        }
    }
}


int main() {
    AmqpClient::Channel::ptr_t channel = AmqpClient::Channel::Create("localhost");

    channel->DeclareExchange(ORDER, AmqpClient::Channel::EXCHANGE_TYPE_DIRECT);
    channel->DeclareExchange(NOTI, AmqpClient::Channel::EXCHANGE_TYPE_FANOUT);
    channel->DeclareExchange(PAYMENT, AmqpClient::Channel::EXCHANGE_TYPE_TOPIC);

    std::string role;
    std::cout << "Select role (producer(p)/direct_consumer(d)/fanout_consumer(f)/topic_consumer(t)): ";
    std::cin >> role;

    if (role == "p") {
        producer(channel);
    } else if (role == "d") {
        direct_consumer(channel);
    } 
    // else if (role == "fanout_consumer") {
    //     fanout_consumer(channel);
    // } 
    // else if (role == "topic_consumer") {
    //     topic_consumer(channel);
    // } 
    // else {
    //     std::cout << "Invalid role!" << std::endl;
    // }
}
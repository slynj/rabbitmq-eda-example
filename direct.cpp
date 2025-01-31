#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <iostream>

int main() {
    AmqpClient::Channel::ptr_t channel = AmqpClient::Channel::Create("localhost");

    std::string ORDER = "order_status_exchange";
    std::string qname_s = "success_queue";
    std::string qname_f = "fail_queue";

    channel->DeclareQueue(qname_s);
    channel->BindQueue(qname_s, ORDER, "success");

    channel->DeclareQueue(qname_f);
    channel->BindQueue(qname_f, ORDER, "fail");

    std::cout << "[Direct Consumer] Waiting for success and fail messages..." << std::endl;

    while (true) {
        AmqpClient::Envelope::ptr_t envelope_s;
        AmqpClient::Envelope::ptr_t envelope_f;

        channel->BasicConsume(qname_s, "direct");
        channel->BasicConsume(qname_f, "direct");

        bool received_s = channel->BasicConsumeMessage("direct", envelope_s, 1000);
        bool received_f = channel->BasicConsumeMessage("direct", envelope_f, 1000);

        if (received_s) {
            std::cout << "[Direct Consumer] Received (Success): " << envelope_s->Message()->Body() << std::endl;
        }

        if (received_f) {
            std::cout << "[Direct Consumer] Received (Fail): " << envelope_f->Message()->Body() << std::endl;
        }
    }
}
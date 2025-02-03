#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <iostream>

int main() {
    auto channel = AmqpClient::Channel::Create("localhost");

    std::string exchangeName = "Timestamp";

    std::string sentQ = "sent";
    std::string receivedQ = "received";
    std::string latencyQ = "latency";

    channel->DeclareQueue(sentQ, false, false, false);
    channel->BindQueue(sentQ, exchangeName, sentQ);

    channel->DeclareQueue(receivedQ, false, false, false);
    channel->BindQueue(receivedQ, exchangeName, receivedQ);

    channel->DeclareQueue(latencyQ, false, false, false);
    channel->BindQueue(latencyQ, exchangeName, latencyQ);

    std::cout << "Waiting for messages ..." << std::endl;

    AmqpClient::Envelope::ptr_t sentEnv;
    AmqpClient::Envelope::ptr_t receivedEnv;
    AmqpClient::Envelope::ptr_t latencyEnv;


    std::string sentTag = channel->BasicConsume(sentQ);
    std::string receivedTag = channel->BasicConsume(receivedQ);
    std::string latencyTag = channel->BasicConsume(latencyQ);

    while (true) {
        bool sentSuccess = channel->BasicConsumeMessage(sentTag, sentEnv, 10);
        bool receivedSuccess = channel->BasicConsumeMessage(receivedTag, receivedEnv, 10);
        bool latencySuccess = channel->BasicConsumeMessage(latencyTag, latencyEnv, 10);

        if (sentSuccess) {
            std::cout << "[Sent] " << sentEnv->Message()->Body() << std::endl;
        }
        
        if (receivedSuccess) {
            std::cout << "[Recived] " << receivedEnv->Message()->Body() << std::endl;
        }
        
        if (latencySuccess) {
            std::cout << "[Latency] " << latencyEnv->Message()->Body() << std::endl;
        }
    }
}
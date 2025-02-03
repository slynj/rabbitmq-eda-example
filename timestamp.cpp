#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <iostream>
#include <chrono>

std::string toString(std::chrono::high_resolution_clock::time_point& time) {
    auto time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()).count();
    return std::to_string(time_ms);
}

template <typename Duration>
std::string durationToString(const Duration& duration) {
    return std::to_string(duration.count());
}

std::chrono::high_resolution_clock::time_point sendRequest(AmqpClient::Channel::ptr_t ch) {
    auto start = std::chrono::high_resolution_clock::now();
    std::string startStr = toString(start);

    ch->BasicPublish("Timestamp", "sent", AmqpClient::BasicMessage::Create(startStr));
    std::cout << "[Request Sent] " << startStr << std::endl;

    return start;
}

std::chrono::high_resolution_clock::time_point recievedRequest(AmqpClient::Channel::ptr_t ch) {
    auto end = std::chrono::high_resolution_clock::now();
    std::string endStr = toString(end);

    ch->BasicPublish("Timestamp", "received", AmqpClient::BasicMessage::Create(endStr));
    std::cout << "[Recieved Request] " << endStr << std::endl;

    return end;
}

int main() {
    AmqpClient::Channel::ptr_t channel = AmqpClient::Channel::Create("localhost");
    channel->DeclareExchange("Timestamp", AmqpClient::Channel::EXCHANGE_TYPE_DIRECT);

    auto start = sendRequest(channel);
    std::string input;

    while (true) {
        std::cout << "Enter: ";
        std::cin >> input;

        if (input == "send") {
            auto end = recievedRequest(channel);
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            std::string durationStr = durationToString(duration);

            channel->BasicPublish("Timestamp", "latency", AmqpClient::BasicMessage::Create(durationStr));
            std::cout << "[Message sent] " << durationStr << std::endl;
            break;
        } else {
            std::cout << "wrong input";
        }
    }
}

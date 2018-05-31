#include <iostream>
#include <string>
#include "TcpServer.cpp"
#include "MessageClient.h"

int main() {
    TcpServer<MessageClient> server;
    server.Listen(3390);

    ev::default_loop loop;
    loop.run(0);

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
//
// Created by 欧阳亚坤 on 2018/5/22.
//

#include "TcpServer.h"
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "MessageClient.h"

template <typename T>
TcpServer<T>::TcpServer() {
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd < 0) {
        throw SocketException("init");
    }
}

template <typename T>
TcpServer<T>::~TcpServer() {
    close(sock_fd);
}

template <typename T>
void TcpServer<T>::Listen(int port) {
    struct sockaddr_in my_addr = { 0, AF_INET, htons(port), inet_addr("0.0.0.0"), { 0 } };
    bzero(&(my_addr.sin_zero), 8);

    if(bind(sock_fd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) < 0) {
        throw SocketException("bind");
    }

    fcntl(sock_fd, F_SETFL, fcntl(sock_fd, F_GETFL, 0) | O_NONBLOCK);

    if(listen(sock_fd, SOMAXCONN) < 0) {
        throw SocketException("listen");
    }

    printf("server started at port %d\n", port);

    ev_io.set<TcpServer, &TcpServer::acceptCallback>(this);
    ev_io.start(sock_fd, ev::READ);

    signal_io.set<TcpServer, &TcpServer::signalCallback>(this);
    signal_io.start(SIGINT);
}

template <typename T>
void TcpServer<T>::acceptCallback(ev::io &watcher, int revents) {
    if (EV_ERROR & revents) {
        perror("got invalid event");
        return;
    }

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    int client_sd = accept(watcher.fd, (struct sockaddr *)&client_addr, &client_len);

    if (client_sd < 0) {
        perror("accept error");
        return;
    }

    printf("accepted from %d\n", client_addr.sin_addr);

    clients.push_back(new T(client_sd));
    OnClientArrived(*clients.back());
}

template <typename T>
void TcpServer<T>::signalCallback(ev::sig &signal, int revents) {
    signal.loop.break_loop();
}

template <typename T>
void TcpServer<T>::OnClientArrived(client_type &client) {
    client.SendCommand(string("Hello from server!"), [](const string &buf) {
        printf("returns: %s", buf.begin());
    });
}
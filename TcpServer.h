//
// Created by 欧阳亚坤 on 2018/5/22.
//

#ifndef FIGHT_LANDLORD_SERVER_TCPSERVER_H
#define FIGHT_LANDLORD_SERVER_TCPSERVER_H

#include "TcpClient.h"
#include <exception>
#include <string>
#include <list>
#include <ev++.h>
#include <memory>

template<typename TClient>
class TcpServer {
public:
    typedef TClient client_type;

    TcpServer();
    ~TcpServer();
    void Listen(int port);

    virtual void OnClientArrived(client_type &client);

private:
    int sock_fd;
    ev::io ev_io;
    ev::sig signal_io;
    std::list<client_type *> clients;

    void acceptCallback(ev::io &watcher, int revents);
    void signalCallback(ev::sig &signal, int revents);

};

class SocketException: std::exception {
public:
    explicit SocketException(const char* reason = "") {
        message = "socket error: ";
        message.append(reason);
    }

    const char *what() const throw() {
        return message.c_str();
    }

private:
    std::string message;
};

#endif //FIGHT_LANDLORD_SERVER_TCPSERVER_H

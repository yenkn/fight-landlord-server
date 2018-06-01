//
// Created by 欧阳亚坤 on 2018/5/23.
//

#ifndef FIGHT_LANDLORD_SERVER_TCPCLIENT_H
#define FIGHT_LANDLORD_SERVER_TCPCLIENT_H

#include <ev++.h>
#include <list>
#include <string>

using std::string;

class TcpClient {
public:
    explicit TcpClient(int fd);
    TcpClient(const TcpClient &client) : TcpClient(client.client_fd) {}

    ~TcpClient();

    void Send(const string &);

protected:
    virtual void onDataRecived(const string &);

private:
    int client_fd;
    ev::io ev_io;
    std::list<string> writeQueue;

    void callback(ev::io &watcher, int revents);
    void readCallback(ev::io &watcher);
    void writeCallback(ev::io &watcher);
};


#endif //FIGHT_LANDLORD_SERVER_TCPCLIENT_H

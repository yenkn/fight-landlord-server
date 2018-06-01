//
// Created by 欧阳亚坤 on 2018/5/23.
//

#include "TcpClient.h"
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>

TcpClient::TcpClient(int fd) : client_fd(fd) {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);

    ev_io.set<TcpClient, &TcpClient::callback>(this);
    ev_io.start(fd, ev::READ);
}

TcpClient::~TcpClient() {
    ev_io.stop();
    close(client_fd);
}

void TcpClient::Send(const string &data) {
    writeQueue.push_back(data);
    ev_io.set(ev::READ|ev::WRITE);
}

void TcpClient::callback(ev::io &watcher, int revents) {
    if (EV_ERROR & revents) {
        perror("got invalid event");
        return;
    }

    if (revents & EV_READ)
        readCallback(watcher);

    if (revents & EV_WRITE)
        writeCallback(watcher);

    if (writeQueue.empty()) {
        ev_io.set(ev::READ);
    } else {
        ev_io.set(ev::READ|ev::WRITE);
    }
}

void TcpClient::readCallback(ev::io &watcher) {
    char buf[1024];

    ssize_t nread = recv(watcher.fd, buf, sizeof(buf), 0);

    if (nread < 0) {
        perror("read error");
        return;
    }

    if (nread == 0) {
        // connection closed by peer
        return;
    } else {
        onDataRecived(string(buf, nread));
    }
}

void TcpClient::writeCallback(ev::io &watcher) {
    if (writeQueue.empty()) {
        ev_io.set(ev::READ);
        return;
    }

    auto buffer = writeQueue.front();
    writeQueue.pop_front();

    ssize_t written = write(watcher.fd, buffer.data(), buffer.size());
    if (written < 0) {
        perror("read error");
        return;
    }

    buffer = buffer.substr(written);
    if (!buffer.empty()) {
        writeQueue.push_front(std::move(buffer));
    }
}

void TcpClient::onDataRecived(const string &data) {
    printf("recv: %s", data.c_str());
}
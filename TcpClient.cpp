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

void TcpClient::Send(const char *data, size_t len) {
    write_queue.push_back(new Buffer(data, len));
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

    if (write_queue.empty()) {
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
        onDataRecived(buf, nread);
    }
}

void TcpClient::writeCallback(ev::io &watcher) {
    if (write_queue.empty()) {
        ev_io.set(ev::READ);
        return;
    }

    Buffer* buffer = write_queue.front();

    ssize_t written = write(watcher.fd, buffer->seek(), buffer->size());
    if (written < 0) {
        perror("read error");
        return;
    }

    buffer->offset += written;
    if (buffer->empty()) {
        write_queue.pop_front();
        delete buffer;
    }
}

void TcpClient::onDataRecived(const char *data, ssize_t len) {
    printf("recv: %s", data);
}
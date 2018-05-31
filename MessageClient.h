//
// Created by 欧阳亚坤 on 2018/5/23.
//

#ifndef FIGHT_LANDLORD_SERVER_MESSAGECLIENT_H
#define FIGHT_LANDLORD_SERVER_MESSAGECLIENT_H

#include <memory>
#include <map>
#include <functional>
#include "TcpClient.h"
#include "DynamicBuffer.h"

using std::unique_ptr;
using std::map;
using std::function;

class MessageClient: public TcpClient {
public:
    typedef function<void(const Buffer &)> command_callback;

    explicit MessageClient(int fd);
    MessageClient(const MessageClient &client): TcpClient(client) {}
    ~MessageClient();

    int SendMessage(const Buffer &);

    static Buffer escape(const Buffer &data);
    static Buffer unescape(const Buffer &data);

    void SendCommand(const Buffer &data, command_callback cb);

protected:
    static const char ESCAPE_CHAR = '\\';
    static const char DIVIDER[];

    void onDataRecived(const char *data, ssize_t len);

private:
    map<int, command_callback> commandCallbacks;
    int currentPackageId = 0;
    DynamicBuffer reciveBuffer;
};


#endif //FIGHT_LANDLORD_SERVER_MESSAGECLIENT_H

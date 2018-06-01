//
// Created by 欧阳亚坤 on 2018/5/23.
//

#ifndef FIGHT_LANDLORD_SERVER_MESSAGECLIENT_H
#define FIGHT_LANDLORD_SERVER_MESSAGECLIENT_H

#include <memory>
#include <map>
#include <functional>
#include <string>
#include "TcpClient.h"

using std::unique_ptr;
using std::map;
using std::function;
using std::string;

class MessageClient: public TcpClient {
public:
    typedef function<void(const string &)> command_callback;

    explicit MessageClient(int fd);
    MessageClient(const MessageClient &client): TcpClient(client) {}

    int SendMessage(const string &);

    static string escape(const string &data);
    static string unescape(const string &data);

    void SendCommand(const string &data, command_callback cb);

protected:
    static const char ESCAPE_CHAR = '\\';
    static const char DIVIDER[];

    void onDataRecived(const string &);

private:
    map<int, command_callback> commandCallbacks;
    int currentPackageId = 0;
    string reciveBuffer;
};


#endif //FIGHT_LANDLORD_SERVER_MESSAGECLIENT_H

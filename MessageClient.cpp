//
// Created by 欧阳亚坤 on 2018/5/23.
//

#include <vector>
#include "MessageClient.h"

const char MessageClient::DIVIDER[] { '!', 's' };

MessageClient::MessageClient(int fd) : TcpClient(fd) {
}

MessageClient::~MessageClient() {
    //ev_async.stop();
}

int MessageClient::SendMessage(const Buffer &data) {
    int packageId = currentPackageId++;
    Buffer divider(DIVIDER, 2);
    DynamicBuffer buffer(divider);
    buffer.Append(reinterpret_cast<char *>(&packageId), 4);
    buffer.Append(escape(data));
    buffer.Append(DIVIDER[1]);
    buffer.Append(DIVIDER[0]);
    Send(buffer.cbegin(), buffer.size());
    return packageId;
}

void MessageClient::SendCommand(const Buffer &data, command_callback cb) {
    int packageId = SendMessage(data);
    commandCallbacks[packageId] = std::move(cb);
}

Buffer MessageClient::escape(const Buffer &data) {
    DynamicBuffer buffer(0);
    for (char iter : data) {
        if(iter == DIVIDER[0] || iter == DIVIDER[1] || iter == ESCAPE_CHAR) {
            buffer.Append(ESCAPE_CHAR);
        }
        buffer.Append(iter);
    }
    return buffer;
}

Buffer MessageClient::unescape(const Buffer &data) {
    DynamicBuffer buffer(data.size());
    for(auto iter = data.begin(); iter != data.end() - 1; iter++) {
        if(*iter == ESCAPE_CHAR &&
                (*(iter+1) == DIVIDER[0] ||
                 *(iter+1) == DIVIDER[1] ||
                 *(iter+1) == ESCAPE_CHAR)) {
            continue;
        }
        buffer.Append(*iter);
    }
    return buffer;
}

void MessageClient::onDataRecived(const char *data, ssize_t len) {
    static char lastChar = 0;
    static bool inPackage = false;
    auto iter = data;
    do {
        if(lastChar == DIVIDER[0] && *iter == DIVIDER[1]) {
            // package start
            reciveBuffer.Clear();
            inPackage = true;
        } else if(lastChar == DIVIDER[1] && *iter == DIVIDER[0]) {
            // package end
            int packageId = *(int *)reciveBuffer.cbegin();
            auto result = commandCallbacks.find(packageId);
            if(result != commandCallbacks.end()) {
                result->second(Buffer(reciveBuffer.begin() + 4, reciveBuffer.size() - 5));
            }
            inPackage = false;
        } else if(inPackage) {
            reciveBuffer.Append(*iter);
        }

        lastChar = *iter;
    } while(iter++ < data + len);

}
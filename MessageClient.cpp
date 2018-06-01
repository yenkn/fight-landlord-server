//
// Created by 欧阳亚坤 on 2018/5/23.
//

#include <vector>
#include "MessageClient.h"

const char MessageClient::DIVIDER[] { '!', 's' };

MessageClient::MessageClient(int fd) : TcpClient(fd) {
}

int MessageClient::SendMessage(const string &data) {
    int packageId = currentPackageId++;
    string divider(DIVIDER, 2);
    string buffer(divider);
    buffer.append(reinterpret_cast<char *>(&packageId), 4);
    buffer.append(escape(data));
    buffer.append(divider.rbegin(), divider.rend());
    Send(buffer);
    return packageId;
}

void MessageClient::SendCommand(const string &data, command_callback cb) {
    int packageId = SendMessage(data);
    commandCallbacks[packageId] = std::move(cb);
}

string MessageClient::escape(const string &data) {
    string buffer;
    for (char iter : data) {
        if(iter == DIVIDER[0] || iter == DIVIDER[1] || iter == ESCAPE_CHAR) {
            buffer.push_back(ESCAPE_CHAR);
        }
        buffer.push_back(iter);
    }
    return buffer;
}

string MessageClient::unescape(const string &data) {
    string buffer;
    buffer.reserve(data.size());
    for(auto iter = data.begin(); iter != data.end() - 1; iter++) {
        if(*iter == ESCAPE_CHAR &&
                (*(iter+1) == DIVIDER[0] ||
                 *(iter+1) == DIVIDER[1] ||
                 *(iter+1) == ESCAPE_CHAR)) {
            continue;
        }
        buffer.push_back(*iter);
    }
    return buffer;
}

void MessageClient::onDataRecived(const string &data) {
    static char lastChar = 0;
    static bool inPackage = false;
    auto iter = data.begin();
    do {
        if(lastChar == DIVIDER[0] && *iter == DIVIDER[1]) {
            // package start
            reciveBuffer.clear();
            inPackage = true;
        } else if(lastChar == DIVIDER[1] && *iter == DIVIDER[0]) {
            // package end
            int packageId = *(int *)reciveBuffer.data();
            auto result = commandCallbacks.find(packageId);
            if(result != commandCallbacks.end()) {
                result->second(unescape(string(reciveBuffer.data() + 4, reciveBuffer.size() - 5)));
            }
            inPackage = false;
        } else if(inPackage) {
            reciveBuffer.push_back(*iter);
        }

        lastChar = *iter;
    } while(iter++ < data.end());

}
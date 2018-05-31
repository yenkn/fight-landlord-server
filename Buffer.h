//
// Created by 欧阳亚坤 on 2018/5/23.
//

#ifndef FIGHT_LANDLORD_SERVER_BUFFER_H
#define FIGHT_LANDLORD_SERVER_BUFFER_H

#include <sys/socket.h>
#include <memory.h>
#include <iterator>

using std::reverse_iterator;

class Buffer {

public:
    typedef char * iterator;
    typedef const char * const_iterator;
    typedef reverse_iterator<iterator> reverse_iterator;

    size_t offset = 0;

    inline size_t size() const { return end_pos - elements - offset; }
    inline iterator begin() { return elements; }
    inline iterator end() { return end_pos; }
    inline const_iterator begin() const { return elements; }
    inline const_iterator end() const { return end_pos; }
    inline const_iterator cbegin() { return elements; }
    inline const_iterator cend() { return end_pos; }
    reverse_iterator rbegin() { return reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    inline bool empty() const { return size() == 0; }

    inline char *seek() { return elements + offset; }

    Buffer(const char *bytes, size_t len) {
        elements = new char[len];
        end_pos = elements + len;
        memcpy(elements, bytes, len);
    }

    template <size_t N>
    Buffer(const char (&data)[N]) : Buffer(data, N) {}

    Buffer(const Buffer &buf) : Buffer(buf.begin(), buf.size()) {
    }
    Buffer(Buffer &&buf) noexcept: elements(buf.elements), end_pos(buf.end_pos) {
        buf.elements = buf.end_pos = nullptr;
    }

    Buffer &operator=(const Buffer &buf) {
        auto dest = new char[buf.size()];
        memcpy(dest, buf.begin(), buf.size());
        delete[] elements;
        elements = dest;
        end_pos = elements + buf.size();
        return *this;
    }

    virtual ~Buffer() {
        free();
    }

protected:
    char *elements = nullptr;
    char *end_pos = nullptr;

    virtual void free() {
        delete[] elements;
    }
};

#endif //FIGHT_LANDLORD_SERVER_BUFFER_H

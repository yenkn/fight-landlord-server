//
// Created by 欧阳亚坤 on 2018/5/23.
//

#ifndef FIGHT_LANDLORD_SERVER_DYNAMICBUFFER_H
#define FIGHT_LANDLORD_SERVER_DYNAMICBUFFER_H

#include <memory>
#include "Buffer.h"

class DynamicBuffer: public Buffer {
public:
    explicit DynamicBuffer(size_t capacity = 1): Buffer(nullptr, 0) {
        reallocate(capacity);
    }
    DynamicBuffer(const char * data, size_t len) : Buffer(data, len) {
        cap = end_pos;
    }
    explicit DynamicBuffer(const Buffer &buf) : Buffer(buf) {
        cap = end_pos;
    }

    DynamicBuffer(DynamicBuffer &&buf) noexcept: Buffer(std::move(buf)), cap(buf.cap) {
        buf.cap = nullptr;
    }

    void Append(char c);
    void Append(const char *data, size_t len);
    template <class Iter>
    void Append(Iter start, Iter end);
    void Append(const Buffer &buf);

    void Clear(size_t capacity = 0);

    inline size_t capacity() const { return cap - elements; }

private:
    char *cap = nullptr;

    void free();
    void checkCapacity(size_t len);
    void reallocate(size_t capacity);
};


#endif //FIGHT_LANDLORD_SERVER_DYNAMICBUFFER_H

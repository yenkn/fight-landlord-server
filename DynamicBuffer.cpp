//
// Created by 欧阳亚坤 on 2018/5/23.
//

#include "DynamicBuffer.h"

void DynamicBuffer::Append(char c) {
    checkCapacity(1);
    *(end_pos++) = c;
}

void DynamicBuffer::Append(const char *data, size_t len) {
    checkCapacity(len);
    memcpy(end_pos, data, len);
    end_pos += len;
}

template <class Iter>
void DynamicBuffer::Append(Iter start, Iter end) {
    size_t len = end - start;
    Append(start, len);
}

void DynamicBuffer::Append(const Buffer &buf) {
    Append(buf.begin(), buf.end());
}

void DynamicBuffer::Clear(size_t capacity) {
    free();
    reallocate(capacity);
}

void DynamicBuffer::free() {
    delete[] elements;
    elements = end_pos = cap = nullptr;
}

void DynamicBuffer::checkCapacity(size_t len) {
    size_t space_size = capacity();
    if(size() + len > space_size) {
        space_size = space_size > 0 ? space_size : 1;
        while(space_size < size() + len) {
            space_size *= 2;
        }
        reallocate(space_size);
    }
}

void DynamicBuffer::reallocate(size_t capacity) {
    auto space = new char[capacity];
    auto len = size();
    memcpy(space, elements, len);
    free();
    elements = space;
    end_pos = elements + len;
    cap = elements + capacity;
}

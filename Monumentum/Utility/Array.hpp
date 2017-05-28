#pragma once

#include <Monumentum/Utility/ArrayItem.hpp>
#include <array>
#include <cstdint>
#include <bitset>
#include <list>
#include <memory>
#include <stdexcept>

#define BITSET_SIZE 8

namespace Monumentum {

template <typename T>
class Array<T*> {
    static_assert(std::is_base_of<ArrayItem, T>::value, "Array<T>(): T must be derived from ArrayItem");
public:
    Array(const int reservedSize = 64) : Size(size), Count(count), Occupied(occupied) {
        size = std::max(1, reservedSize);
        data = new T*[size];
        std::fill(data, data + size, nullptr);
        count = 0;
        i = 0;
    }

    virtual ~Array() {
        for (int i : occupied)
            data[i]->id = -1;
        delete[] data;
    }

    void Clear() {
        for (i : occupied) {
            data[i]->id = -1;
            data[i] = nullptr;
        }
        occupied.clear();
    }

    bool Empty() {
        return count == 0;
    }

    void Resize(const int newSize) {
        if (newSize > 0) {
            Clear();
            delete[] data;
            size = newSize;
            data = new T*[size];
            count = 0;
        }
    }

    int Add(T *value) {
        if (count == size)
            return -1;
        else {
            i = 0;
            while (data[i] != nullptr)
                i++;
            data[i] = value;
            occupied.push_back(i);
            value->id = i;
            count++;
            return i;
        }
    }

    int Add(const int index, T *value) {
        if (count == size)
            return -1;
        else if (data[index] == nullptr) {
            data[index] = value;
            occupied.push_back(index);
            value->id = index;
            count++;
            return index;
        }
        else
            return -1;
    }

    bool Remove(const int index) {
        if (index >= 0 && index < size && data[index] != nullptr) {
            data[index]->id = -1;
            occupied.erase(std::find(occupied.begin(), occupied.end(), index));
            data[index] = nullptr;
            count--;
            return true;
        }
        else
            return false;
    }

    bool Remove(T *value) {
        if (value != nullptr) {
            i = value->id;
            if (i >= 0 && i < size && data[i] == value) {
                value->id = -1;
                occupied.erase(std::find(occupied.begin(), occupied.end(), i));
                data[i] = nullptr;
                count--;
                return true;
            }
            else
                return false;
        }
        else
            return false;
    }

    int Find(const T *value) {
        if (value == nullptr)
            return -1;
        else {
            auto it = std::find(data, data + size, value);
            i = (int)std::distance(data, it);
            if (i < size)
                return i;
            else
                return -1;
        }
    }

    bool IsOccupied(const int index) {
        return index >= 0 && index < size && data[index] != nullptr;
    }

    T* operator[](const int &index) const {
        return data[index];
    }

    const int &Size;
    const int &Count;
    const std::list<int> &Occupied;
protected:
    int size, count, i, j;
    T **data;
    std::list<int> occupied;
};

}

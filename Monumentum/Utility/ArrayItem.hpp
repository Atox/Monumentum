#pragma once

namespace Monumentum {

template <typename T>
class Array;

class ArrayItem {
    template <typename T>
    friend class Array;
public:
    ArrayItem();
    virtual ~ArrayItem();
    const int &Id;
private:
    int id;
};

}

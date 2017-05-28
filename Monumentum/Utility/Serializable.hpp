#pragma once

#include <Monumentum/Utility/ByteBuffer.hpp>

namespace Monumentum {

class Serializable
{
public:
    Serializable();
    virtual ~Serializable();

    virtual void Serialize(ByteBuffer &out) = 0;
    virtual void Deserialize(ByteBuffer &in) = 0;
};

}

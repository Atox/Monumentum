#pragma once

#include <Monumentum/Utility/ByteBuffer.hpp>

namespace Monumentum {

class Command {
public:
    Command();
    virtual ~Command();
    virtual void Execute(Monumentum::ByteBuffer &arg) = 0;
};

}

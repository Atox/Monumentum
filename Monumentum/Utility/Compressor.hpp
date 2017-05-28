#pragma once

#include <Monumentum/Utility/ByteBuffer.hpp>
#include <cstdint>

namespace Monumentum {

class Compressor {
public:
    Compressor(const int reservedLength = 8192);
    virtual ~Compressor();

    uint8_t* const GetData();
    void Resize(int reservedLength);

    const int &ReservedLength;
    const int &Length;

    virtual void Compress(const uint8_t *data, const int uncompressedLength) = 0;
    virtual void Compress(ByteBuffer &buffer, const int uncompressedLength) = 0;
    virtual void Decompress(const uint8_t *data, const int compressedLength) = 0;
    virtual void Decompress(ByteBuffer &buffer, const int compressedLength) = 0;
protected:
    int length, reservedLength;
    uint8_t *data;
};
    
}

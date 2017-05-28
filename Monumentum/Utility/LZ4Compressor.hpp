#pragma once

#include <Monumentum/Utility/Compressor.hpp>

namespace Monumentum {
    
class LZ4Compressor: public Compressor {
public:
    LZ4Compressor(const int reservedSize = 8192);
    ~LZ4Compressor();
    virtual void Compress(const uint8_t *data, const int uncompressedSize);
    virtual void Compress(ByteBuffer &buffer, const int uncompressedSize);
    virtual void Decompress(const uint8_t *data, const int compressedSize);
    virtual void Decompress(ByteBuffer &buffer, const int compressedSize);
};

}

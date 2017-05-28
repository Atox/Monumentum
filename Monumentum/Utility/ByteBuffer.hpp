#pragma once

#include <inttypes.h>
#include <cstddef>
#include <cstdint>
#include <algorithm>

namespace Monumentum {

/*
Type sizes:
                       UNIX  Windows
Type           ILP64   LP64   LLP64
char              8      8       8
short            16     16      16
int              64     32      32
long             64     64      32
long long        64     64      64
pointer          64     64      64
*/

template< class T >
static T ReverseBytes(T _value)
{
    const size_t length = sizeof(T);
    if (length != 1) {
        std::reverse(reinterpret_cast< uint8_t* >(&_value),
            reinterpret_cast< uint8_t* >(&_value) + length);
    }
    return _value;
}

class Compressor;
class Serializable;

class ByteBuffer {
public:
    enum Endianness {
        LITTLE,
        BIG
    };
    ByteBuffer(const Endianness endianness, const int length = 0);
    ByteBuffer(const Endianness endianness, uint8_t *data, const int length);
    virtual ~ByteBuffer();
    uint8_t* const GetData();
    bool Open(const char *fileName, const bool resizeIfLess = true);
    bool Save(const char *fileName);
    bool SaveCompressed(const char *fileName, Compressor &compressor);
    void Load(uint8_t *data, const int length);
    void Resize(const int length);
    void Reset();
    void SetIndex(const int index);
    const int &Length;
    const int &Index;
    bool Ok;
    // Read
    uint8_t *ReadData(const int length);
    int8_t ReadInt8();
    uint8_t ReadUInt8();
    int16_t ReadInt16();
    uint16_t ReadUInt16();
    int32_t ReadInt32();
    uint32_t ReadUInt32();
    int64_t ReadInt64();
    uint64_t ReadUInt64();
    float ReadFloat();
    double ReadDouble();
    // Write
    void WriteObject(Serializable &object);
    void WriteData(const uint8_t *dataToWrite, const int length);
    void WriteInt8(const int8_t value);
    void WriteUInt8(const uint8_t value);
    void WriteInt16(const int16_t value);
    void WriteUInt16(const uint16_t value);
    void WriteInt32(const int32_t value);
    void WriteUInt32(const uint32_t value);
    void WriteInt64(const int64_t value);
    void WriteUInt64(const uint64_t value);
    void WriteFloat(const float value);
    void WriteDouble(const double value);
protected:
    Endianness endianness, platformEndianness;
    uint8_t *data;
    int length, index;
    bool allocated;
    void checkEndianness();
};

}

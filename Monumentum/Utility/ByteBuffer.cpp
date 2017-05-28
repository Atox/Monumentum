#include <Monumentum/Utility/ByteBuffer.hpp>
#include <Monumentum/Utility/Compressor.hpp>
#include <Monumentum/Utility/Serializable.hpp>
#include <fstream>
#include <cstring>

namespace Monumentum {

ByteBuffer::ByteBuffer(Endianness desiredEndianness, const int dataLength): Length(length), Index(index) {
    endianness = desiredEndianness;
    checkEndianness();
    if (dataLength > 0) {
        data = new uint8_t[dataLength];
        allocated = true;
        index = 0;
        length = dataLength;
    }
    else {
        allocated = false;
        index = 0;
        length = 0;
    }
    Ok = length > 0;
}

ByteBuffer::ByteBuffer(Endianness desiredEndianness, uint8_t *newData, const int dataLength) : Length(length), Index(index) {
    endianness = desiredEndianness;
    checkEndianness();
    data = newData;
    index = 0;
    length = dataLength;
    allocated = false;
    Ok = length > 0;
}

ByteBuffer::~ByteBuffer() {
    if (allocated)
        delete[] data;
}

void ByteBuffer::checkEndianness() {
    union {
        uint32_t i;
        char c[4];
    } bint = { 0x01020304 };
    if (bint.c[0] == 1)
        platformEndianness = Endianness::BIG;
    else
        platformEndianness = Endianness::LITTLE;
}

uint8_t* const ByteBuffer::GetData() {
    return data;
}

bool ByteBuffer::Open(const char *fileName, const bool resizeIfLess) {
    std::ifstream stream;
    stream.open(fileName, std::ifstream::in | std::ifstream::binary);
    if (stream.is_open()) {
        stream.seekg(0, std::ios_base::end);
        int streamLength = (int)stream.tellg();
        stream.seekg(0, std::ios_base::beg);
        if (length < streamLength || (length > streamLength && resizeIfLess))
            Resize(streamLength);
        else
            index = 0;
        stream.read((char*)data, length);
        stream.close();
        return true;
    }
    else
        return false;
}

bool ByteBuffer::Save(const char *fileName) {
    if (index > 0) {
        std::ofstream stream;
        stream.open(fileName, std::ofstream::out | std::ifstream::binary);
        if (stream.is_open()) {
            stream.write((char*)data, index);
            stream.close();
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

bool ByteBuffer::SaveCompressed(const char *fileName, Compressor &compressor) {
    if (index > 0) {
        std::ofstream stream;
        stream.open(fileName, std::ofstream::out | std::ifstream::binary);
        if (stream.is_open()) {
            compressor.Compress(data, Index);
            stream.write((char*)compressor.GetData(), compressor.Length);
            stream.close();
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

void ByteBuffer::Load(uint8_t *newData, const int dataLength) {
    if (allocated)
        delete[] data;
    data = newData;
    allocated = false;
    index = 0;
    length = dataLength;
    Ok = length > 0;
}

void ByteBuffer::Resize(const int dataLength) {
    if (allocated)
        delete[] data;
    allocated = true;
    data = new uint8_t[dataLength];
    index = 0;
    length = dataLength;
    Ok = length > 0;
}

void ByteBuffer::Reset() {
    index = 0;
    Ok = length > 0;
}

void ByteBuffer::SetIndex(const int newindex) {
    if (newindex >= 0 && newindex < length)
        index = newindex;
}

uint8_t* ByteBuffer::ReadData(const int dataLength) {
    if (index + dataLength <= length) {
        int dataindex = index;
        index += dataLength;
        return data + dataindex;
    }
    else {
        Ok = false;
        return nullptr;
    }
}

int8_t ByteBuffer::ReadInt8() {
    if (index < length)
        if (endianness == platformEndianness)
            return data[index++];
        else
            return ReverseBytes(data[index++]);
    else {
        Ok = false;
        return 0;
    }
}

uint8_t ByteBuffer::ReadUInt8() {
    if (index < length) {
        if (endianness == platformEndianness)
            return data[index++];
        else
            return ReverseBytes(data[index++]);
    }
    else {
        Ok = false;
        return 0;
    }
}

int16_t ByteBuffer::ReadInt16() {
    if (index + sizeof(int16_t) <= length) {
        int16_t value;
        memcpy(&value, data + index, sizeof(int16_t));
        index += sizeof(int16_t);
        if (endianness == platformEndianness)
            return value;
        else
            return ReverseBytes(value);
    }
    else {
        Ok = false;
        return 0;
    }
}

uint16_t ByteBuffer::ReadUInt16() {
    if (index + sizeof(uint16_t) <= length) {
        uint16_t value;
        memcpy(&value, data + index, sizeof(uint16_t));
        index += sizeof(uint16_t);
        if (endianness == platformEndianness)
            return value;
        else
            return ReverseBytes(value);
    }
    else {
        Ok = false;
        return 0;
    }
}

int32_t ByteBuffer::ReadInt32() {
    if (index + sizeof(int32_t) <= length) {
        int32_t value;
        memcpy(&value, data + index, sizeof(int32_t));
        index += sizeof(int32_t);
        if (endianness == platformEndianness)
            return value;
        else
            return ReverseBytes(value);
    }
    else {
        Ok = false;
        return 0;
    }
}

uint32_t ByteBuffer::ReadUInt32() {
    if (index + sizeof(uint32_t) <= length) {
        uint32_t value;
        memcpy(&value, data + index, sizeof(uint32_t));
        index += sizeof(uint32_t);
        if (endianness == platformEndianness)
            return value;
        else
            return ReverseBytes(value);
    }
    else {
        Ok = false;
        return 0;
    }
}

int64_t ByteBuffer::ReadInt64() {
    if (index + sizeof(int64_t) <= length) {
        int64_t value;
        memcpy(&value, data + index, sizeof(int64_t));
        index += sizeof(int64_t);
        if (endianness == platformEndianness)
            return value;
        else
            return ReverseBytes(value);
    }
    else {
        Ok = false;
        return 0;
    }
}

uint64_t ByteBuffer::ReadUInt64() {
    if (index + sizeof(uint64_t) <= length) {
        uint64_t value;
        memcpy(&value, data + index, sizeof(uint64_t));
        index += sizeof(uint64_t);
        if (endianness == platformEndianness)
            return value;
        else
            return ReverseBytes(value);
    }
    else {
        Ok = false;
        return 0;
    }
}

float ByteBuffer::ReadFloat() {
    if (index + sizeof(float) <= length) {
        float value;
        memcpy(&value, data + index, sizeof(float));
        index += sizeof(float);
        if (endianness == platformEndianness)
            return value;
        else
            return ReverseBytes(value);
    }
    else {
        Ok = false;
        return 0.0f;
    }
}

double ByteBuffer::ReadDouble() {
    if (index + sizeof(double) <= length) {
        double value;
        memcpy(&value, data + index, sizeof(double));
        index += sizeof(double);
        if (endianness == platformEndianness)
            return value;
        else
            return ReverseBytes(value);
    }
    else {
        Ok = false;
        return 0.0;
    }
}

void ByteBuffer::WriteObject(Serializable &object) {
    object.Serialize(*this);
}

void ByteBuffer::WriteData(const uint8_t *dataToWrite, const int dataLength) {
    if (index + dataLength <= length) {
        memcpy(data + index, dataToWrite, dataLength);
        index += dataLength;
    }
    else
        Ok = false;
}

void ByteBuffer::WriteInt8(int8_t value) {
    if (index < length) { // index + 1 <= length
        if (endianness != platformEndianness)
            value = ReverseBytes(value);
        data[index] = value;
        index++;
    }
    else
        Ok = false;
}

void ByteBuffer::WriteUInt8(uint8_t value) {
    if (index < length) { // index + 1 <= length
        if (endianness != platformEndianness)
            value = ReverseBytes(value);
        data[index] = value;
        index++;
    }
    else
        Ok = false;
}

void ByteBuffer::WriteInt16(int16_t value) {
    if (index + sizeof(int16_t) <= length) {
        if (endianness != platformEndianness)
            value = ReverseBytes(value);
        memcpy(data + index, &value, sizeof(int16_t));
        index += sizeof(int16_t);
    }
    else
        Ok = false;
}

void ByteBuffer::WriteUInt16(uint16_t value) {
    if (index + sizeof(uint16_t) <= length) {
        if (endianness != platformEndianness)
            value = ReverseBytes(value);
        memcpy(data + index, &value, sizeof(uint16_t));
        index += sizeof(uint16_t);
    }
    else
        Ok = false;
}

void ByteBuffer::WriteInt32(int32_t value) {
    if (index + sizeof(int32_t) <= length) {
        if (endianness != platformEndianness)
            value = ReverseBytes(value);
        memcpy(data + index, &value, sizeof(int32_t));
        index += sizeof(int32_t);
    }
    else
        Ok = false;
}

void ByteBuffer::WriteUInt32(uint32_t value) {
    if (index + sizeof(uint32_t) <= length) {
        if (endianness != platformEndianness)
            value = ReverseBytes(value);
        memcpy(data + index, &value, sizeof(uint32_t));
        index += sizeof(uint32_t);
    }
    else
        Ok = false;
}

void ByteBuffer::WriteInt64(int64_t value) {
    if (index + sizeof(int64_t) <= length) {
        if (endianness != platformEndianness)
            value = ReverseBytes(value);
        memcpy(data + index, &value, sizeof(int64_t));
        index += sizeof(int64_t);
    }
    else
        Ok = false;
}

void ByteBuffer::WriteUInt64(uint64_t value) {
    if (index + sizeof(uint64_t) <= length) {
        if (endianness != platformEndianness)
            value = ReverseBytes(value);
        memcpy(data + index, &value, sizeof(uint64_t));
        index += sizeof(uint64_t);
    }
    else
        Ok = false;
}

void ByteBuffer::WriteFloat(float value) {
    if (index + sizeof(float) <= length) {
        if (endianness != platformEndianness)
            value = ReverseBytes(value);
        memcpy(data + index, &value, sizeof(float));
        index += sizeof(float);
    }
    else
        Ok = false;
}

void ByteBuffer::WriteDouble(double value) {
    if (index + sizeof(double) <= length) {
        if (endianness != platformEndianness)
            value = ReverseBytes(value);
        memcpy(data + index, &value, sizeof(double));
        index += sizeof(double);
    }
    else
        Ok = false;
}

}

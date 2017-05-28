#include <Monumentum/Utility/Compressor.hpp>

namespace Monumentum {

Compressor::Compressor(const int reservedMaximumLength) : ReservedLength(reservedLength),
    Length(length) {
    if (reservedMaximumLength <= 0)
        reservedLength = 8192;
    else
        reservedLength = reservedMaximumLength;
    length = 0;
    data = new uint8_t[reservedLength];
}

Compressor::~Compressor() {
    delete[] data;
}

uint8_t* const Compressor::GetData() {
    return data;
}

void Compressor::Resize(const int reservedMaximumLength) {
    if (reservedMaximumLength > 0) {
        reservedLength = reservedMaximumLength;
        delete[] data;
        length = 0;
        data = new uint8_t[reservedLength];
    }
}
    
}


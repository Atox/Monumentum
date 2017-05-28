#include <Monumentum/Utility/LZ4Compressor.hpp>
#include <lz4.h>

namespace Monumentum {

LZ4Compressor::LZ4Compressor(int reservedSize) : Compressor(reservedSize) {

}

LZ4Compressor::~LZ4Compressor() {
    
}

void LZ4Compressor::Compress(const uint8_t *dataToCompress, const int uncompressedSize) {
    length = LZ4_compress_default((char*)dataToCompress, (char*)data, (int)uncompressedSize, reservedLength);
}

void LZ4Compressor::Compress(ByteBuffer &buffer, const int uncompressedSize) {
    length = LZ4_compress_default((char*)buffer.GetData(), (char*)data, (int)uncompressedSize, reservedLength);
}

void LZ4Compressor::Decompress(const uint8_t *dataToDecompress, const int compressedSize) {
    length = LZ4_decompress_safe((char*)dataToDecompress, (char*)data, (int)compressedSize, reservedLength);
}

void LZ4Compressor::Decompress(ByteBuffer &buffer, const int compressedSize) {
    length = LZ4_decompress_safe((char*)buffer.GetData(), (char*)data, (int)compressedSize, reservedLength);
}
    
}

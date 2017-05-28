#pragma once

#include <Monumentum/Engine/Event.hpp>
#include <Monumentum/Utility/ArrayItem.hpp>
#include <Monumentum/Utility/ByteBuffer.hpp>
#include <Monumentum/Utility/Compressor.hpp>
#include <enet/enet.h>
#include <cstdint>

namespace Monumentum {

enum PacketType { RELIABLE, UNRELIABLE };

class Peer : public ArrayItem {
public:
    Peer(ENetPeer *socket);
    virtual ~Peer();

    void Disconnect();
    void Send(const void *data, const size_t size, const uint8_t channel, const PacketType packetType);
    void Send(ByteBuffer &byteBuffer, const size_t size, const uint8_t channel, const PacketType packetType);
    void Send(ByteBuffer &byteBuffer, const uint8_t channel, const PacketType packetType);
    void SendCompressed(const void *data, const int size, Compressor &compressor, const uint8_t channel, const PacketType packetType);
    void SendCompressed(ByteBuffer &byteBuffer, int size, Compressor &compressor, const uint8_t channel, const PacketType packetType);
    void SendCompressed(ByteBuffer &byteBuffer, Compressor &compressor, const uint8_t channel, const PacketType packetType);

    friend bool operator==(const Peer &a, const Peer &b) {
        return a.socket == b.socket;
    }
protected:
    ENetPeer *socket;
};

}

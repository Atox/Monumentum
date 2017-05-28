#include <Monumentum/Engine/Peer.hpp>
#include <Monumentum/Utility/ByteBuffer.hpp>

namespace Monumentum {

Peer::Peer(ENetPeer *peerSocket) {
    socket = peerSocket;
}

Peer::~Peer() {

}

void Peer::Disconnect() {
    enet_peer_disconnect(socket, 0);
}

void Peer::Send(const void *data, const size_t size, const uint8_t channel, const PacketType packetType) {
    if (packetType == RELIABLE)
        enet_peer_send(socket, channel, enet_packet_create(data, size, ENET_PACKET_FLAG_RELIABLE));
    else
        enet_peer_send(socket, channel, enet_packet_create(data, size, ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT));
}

void Peer::Send(ByteBuffer &byteBuffer, const size_t size, const uint8_t channel, const PacketType packetType) {
    if (packetType == RELIABLE)
        enet_peer_send(socket, channel, enet_packet_create(byteBuffer.GetData(), size, ENET_PACKET_FLAG_RELIABLE));
    else
        enet_peer_send(socket, channel, enet_packet_create(byteBuffer.GetData(), size, ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT));
}

void Peer::Send(ByteBuffer &byteBuffer, const uint8_t channel, const PacketType packetType) {
    if (packetType == RELIABLE)
        enet_peer_send(socket, channel, enet_packet_create(byteBuffer.GetData(), byteBuffer.Index, ENET_PACKET_FLAG_RELIABLE));
    else
        enet_peer_send(socket, channel, enet_packet_create(byteBuffer.GetData(), byteBuffer.Index, ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT));
}

void Peer::SendCompressed(const void *data, const int size, Compressor &compressor, const uint8_t channel, const PacketType packetType) {
    compressor.Compress((uint8_t*)data, size);
    if (packetType == RELIABLE)
        enet_peer_send(socket, channel, enet_packet_create(compressor.GetData(), compressor.Length, ENET_PACKET_FLAG_RELIABLE));
    else
        enet_peer_send(socket, channel, enet_packet_create(compressor.GetData(), compressor.Length, ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT));

}

void Peer::SendCompressed(ByteBuffer &byteBuffer, const int size, Compressor &compressor, const uint8_t channel, const PacketType packetType) {
    compressor.Compress(byteBuffer, size);
    if (packetType == RELIABLE)
        enet_peer_send(socket, channel, enet_packet_create(compressor.GetData(), compressor.Length, ENET_PACKET_FLAG_RELIABLE));
    else
        enet_peer_send(socket, channel, enet_packet_create(compressor.GetData(), compressor.Length, ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT));
}

void Peer::SendCompressed(ByteBuffer &byteBuffer, Compressor &compressor, const uint8_t channel, const PacketType packetType) {
    compressor.Compress(byteBuffer, (int)byteBuffer.Index);
    if (packetType == RELIABLE)
        enet_peer_send(socket, channel, enet_packet_create(compressor.GetData(), compressor.Length, ENET_PACKET_FLAG_RELIABLE));
    else
        enet_peer_send(socket, channel, enet_packet_create(compressor.GetData(), compressor.Length, ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT));
}

}

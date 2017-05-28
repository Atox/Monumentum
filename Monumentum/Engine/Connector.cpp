#include <Monumentum/Engine/Connector.hpp>
#include <Monumentum/Engine/FixedTimestepService.hpp>

namespace Monumentum {

Connector::Connector() {
    FixedTimestepService::Initialize();
}

Connector::~Connector() {
    DeInit();
}

bool Connector::Init(const int bufferLength, const uint8_t channelCount) {
    host = enet_host_create(NULL, 1, channelCount, 0, 0);
    if (host) {
        reservedBufferSize = std::max(1, bufferLength);
        ReceivedData = new uint8_t[reservedBufferSize];
        initialized = true;
        return true;
    }
    else
        return false;
}

void Connector::DeInit() {
    if (initialized) {
        enet_host_destroy(host);
        delete ReceivedData;
        initialized = false;
    }
}

bool Connector::Connect(const char *serverAddress, const int port, const uint8_t channelCount) {
    if (initialized && host) {
        ENetAddress address;
        enet_address_set_host(&address, serverAddress);
        address.port = port;
        socket = enet_host_connect(host, &address, channelCount, 0);
        if (socket)
            return true;
        else
            return false;
    }
    else
        return false;
}

void Connector::Disconnect() {
    enet_peer_disconnect(socket, 0);
}

void Connector::ForceDisconnect() {
    enet_peer_disconnect_now(socket, 0);
}

void Connector::Send(const void *data, const size_t size, const uint8_t channel, const PacketType packetType) {
    if (packetType == RELIABLE)
        enet_peer_send(socket, channel, enet_packet_create(data, size, ENET_PACKET_FLAG_RELIABLE));
    else
        enet_peer_send(socket, channel, enet_packet_create(data, size, ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT));
}

void Connector::Send(ByteBuffer &byteBuffer, const size_t size, const uint8_t channel, const PacketType packetType) {
    if (packetType == RELIABLE)
        enet_peer_send(socket, channel, enet_packet_create(byteBuffer.GetData(), size, ENET_PACKET_FLAG_RELIABLE));
    else
        enet_peer_send(socket, channel, enet_packet_create(byteBuffer.GetData(), size, ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT));
}

void Connector::Send(ByteBuffer &byteBuffer, const uint8_t channel, const PacketType packetType) {
    if (packetType == RELIABLE)
        enet_peer_send(socket, channel, enet_packet_create(byteBuffer.GetData(), byteBuffer.Index, ENET_PACKET_FLAG_RELIABLE));
    else
        enet_peer_send(socket, channel, enet_packet_create(byteBuffer.GetData(), byteBuffer.Index, ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT));
}

void Connector::SendCompressed(const void *data, const int size, Compressor &compressor, const uint8_t channel, const PacketType packetType) {
    compressor.Compress((uint8_t*)data, size);
    if (packetType == RELIABLE)
        enet_peer_send(socket, channel, enet_packet_create(compressor.GetData(), compressor.Length, ENET_PACKET_FLAG_RELIABLE));
    else
        enet_peer_send(socket, channel, enet_packet_create(compressor.GetData(), compressor.Length, ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT));

}

void Connector::SendCompressed(ByteBuffer &byteBuffer, const int size, Compressor &compressor, const uint8_t channel, const PacketType packetType) {
    compressor.Compress(byteBuffer, size);
    if (packetType == RELIABLE)
        enet_peer_send(socket, channel, enet_packet_create(compressor.GetData(), compressor.Length, ENET_PACKET_FLAG_RELIABLE));
    else
        enet_peer_send(socket, channel, enet_packet_create(compressor.GetData(), compressor.Length, ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT));
}

void Connector::SendCompressed(ByteBuffer &byteBuffer, Compressor &compressor, const uint8_t channel, const PacketType packetType) {
    compressor.Compress(byteBuffer, (int)byteBuffer.Index);
    if (packetType == RELIABLE)
        enet_peer_send(socket, channel, enet_packet_create(compressor.GetData(), compressor.Length, ENET_PACKET_FLAG_RELIABLE));
    else
        enet_peer_send(socket, channel, enet_packet_create(compressor.GetData(), compressor.Length, ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT));
}

Connector::EventType Connector::Service() {
    if (enet_host_service(host, &event, 0) > 0) {
        switch (event.type) {
        case ENET_EVENT_TYPE_RECEIVE:
            ReceivedDataChannel = event.channelID;
            ReceivedDataSize = (int)event.packet->dataLength;
            memcpy(ReceivedData, event.packet->data, event.packet->dataLength);
            return EventType::RECEIVE;
        case ENET_EVENT_TYPE_DISCONNECT:
            return EventType::DISCONNECT;
        default:
            return EventType::DISCONNECT;
        }
    }
    else
        return EventType::NONE;
}

}

#include <Monumentum/Engine/Peer.hpp>
#include <cstdint>

namespace Monumentum {

class Connector {
public:
    enum EventType {
        NONE,
        RECEIVE,
        DISCONNECT
    };
    Connector();
    virtual ~Connector();
    bool Init(const int bufferLength, const uint8_t channelCount);
    void DeInit();
    bool Connect(const char *address, const int port, const uint8_t channelCount);
    void Disconnect();
    void ForceDisconnect();
    void Send(const void *data, const size_t size, const uint8_t channel, const PacketType packetType);
    void Send(ByteBuffer &byteBuffer, const size_t size, const uint8_t channel, const PacketType packetType);
    void Send(ByteBuffer &byteBuffer, const uint8_t channel, const PacketType packetType);
    void SendCompressed(const void *data, const int size, Compressor &compressor, const uint8_t channel, const PacketType packetType);
    void SendCompressed(ByteBuffer &byteBuffer, const int size, Compressor &compressor, const uint8_t channel, const PacketType packetType);
    void SendCompressed(ByteBuffer &byteBuffer, Compressor &compressor, const uint8_t channel, const PacketType packetType);
    EventType Service();
    friend bool operator==(const Connector &a, const Connector &b) {
        return (a.host == b.host && a.socket == b.socket);
    }
    uint8_t ReceivedDataChannel;
    int ReceivedDataSize;
    uint8_t *ReceivedData;
private:
    bool initialized;
    int reservedBufferSize;
    ENetHost *host;
    ENetPeer *socket;
    ENetEvent event;
};

}

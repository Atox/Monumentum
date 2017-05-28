#pragma once

#include <Monumentum/Engine/Peer.hpp>
#include <Monumentum/Utility/Array.hpp>
#include <enet/enet.h>
#include <chrono>

namespace Monumentum {

class FixedTimestepService {
    friend class Connector;
public:
    FixedTimestepService(const int maxConnections, const float tickTime);
    virtual ~FixedTimestepService();
    const int &Connections;
    const float &TickTime;
    bool SetTickTime(const float tickTime);
    void Run(const int port, const uint8_t channelCount = 2);
    void Stop();
    const bool &Running;
    const int &MaxConnections;
    Array<Peer*> Peers; // Read-only
protected:
    static bool Initialize();
    virtual void OnStart() = 0;
    virtual void OnConnect(Peer *peer) = 0;
    virtual void OnDisconnect(Peer *peer, const int id) = 0;
    virtual void OnReceive(Peer *peer, uint8_t *data, const int size, const uint8_t channel) = 0;
    virtual void OnTick(const float timeDelta) = 0;
    virtual void OnStop() = 0;
private:
    static bool enetInitialized;
    int maxConnections, connections;
    bool running;
    float tickTime;
};

}

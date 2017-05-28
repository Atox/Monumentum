#include <Monumentum/Engine/FixedTimestepService.hpp>
#include <Monumentum/Engine/Event.hpp>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>

namespace Monumentum {

bool FixedTimestepService::enetInitialized = false;

FixedTimestepService::FixedTimestepService(const int maximumConnections, const float tickTimeValue) :
Connections(connections),
MaxConnections(maxConnections),
Peers(maximumConnections),
Running(running),
TickTime(tickTime) {
    Initialize();
    connections = 0;
    maxConnections = std::max(1, maximumConnections);
    tickTime = (tickTimeValue > 0) ? tickTimeValue : 0.1f;
}

FixedTimestepService::~FixedTimestepService() {

}

bool FixedTimestepService::Initialize() {
    if (!enetInitialized && enet_initialize() == 0) {
        enetInitialized = true;
        atexit(enet_deinitialize);
        return true;
    }
    else
        return false;
}

bool FixedTimestepService::SetTickTime(const float tickTimeValue) {
    if (tickTimeValue > 0) {
        tickTime = tickTimeValue;
        return true;
    }
    else
        return false;
}

void FixedTimestepService::Run(const int port, const uint8_t channelCount) {
    running = true;
    OnStart();
    ENetAddress address;
    ENetHost *host;
    address.host = ENET_HOST_ANY;
    address.port = port;
    host = enet_host_create(&address, maxConnections, channelCount, 0, 0);
    if (host) {
        ENetEvent event;
        float timeDelta = 0;
        std::chrono::high_resolution_clock::time_point start, end;
        std::chrono::duration<float> duration;
        Peer *peer;
        int peerId;
        while (running) {
            start = std::chrono::high_resolution_clock::now();
            enet_host_service(host, &event, 0);
            switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                peer = new Peer(event.peer);
                event.peer->data = peer;
                Peers.Add(peer);
                OnConnect(peer);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                peer = (Peer*)event.peer->data;
                peerId = peer->Id;
                Peers.Remove(peer);
                OnDisconnect(peer, peerId);
                delete peer;
            case ENET_EVENT_TYPE_RECEIVE:
                if (event.packet) {
                    OnReceive((Peer*)event.peer->data, event.packet->data, (int)event.packet->dataLength, event.channelID);
                    enet_packet_destroy(event.packet);
                }
                break;
            }
            OnTick(timeDelta);
            enet_host_flush(host);
            end = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::duration<float>>(end - start);
            timeDelta = duration.count();
            if (timeDelta < tickTime) {
                duration = std::chrono::duration<float>(tickTime) - duration;
                std::this_thread::sleep_for(duration);
            }
            end = std::chrono::high_resolution_clock::now();
            timeDelta = std::chrono::duration_cast<std::chrono::duration<float>>(end - start).count();
        }
        OnStop();
        enet_host_destroy(host);
    }
    else
        std::cerr << "FixedTimeStepService::Run() -> An error occurred while trying to create a server host" << std::endl;
}

void FixedTimestepService::Stop() {
    running = false;
}

}

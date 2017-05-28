#pragma once

#include <cstdint>
#include <cstddef>

namespace Monumentum {

enum EVENT_TYPE {
    PeerConnect,
    PeerDisconnect,
    DataReceive
};

class Peer;

class Event {
public:
    Event();
    Event(EVENT_TYPE type, Peer *peer); // PeerConnect/PeerDisconnect
    Event(Peer *peer, uint8_t *data, size_t size); // DataReceive
    EVENT_TYPE EType;
    Peer *EPeer;
    uint8_t *EData;
    size_t ESize;
};

}
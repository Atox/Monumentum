#include <Monumentum/Engine/Event.hpp>

namespace Monumentum {

Event::Event() { }
Event::Event(EVENT_TYPE type, Peer *peer) : EType(type), EPeer(peer) { }
Event::Event(Peer *peer, uint8_t *data, size_t size) : EType(EVENT_TYPE::DataReceive), EPeer(peer),
    EData(data), ESize(size) { }

}
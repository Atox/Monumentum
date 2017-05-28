#include <Monumentum/Utility/Dispatcher.hpp>
#include <iostream>

namespace Monumentum {

Dispatcher::Dispatcher() {
    
}

Dispatcher::~Dispatcher() {

}

void Dispatcher::Remove(const uint16_t id) {
    if (commandRegistry.count(id) > 0)
        commandRegistry.erase(id);
    else
        std::cerr << "Dispatcher::Remove() -> Trying to remove unregistered command" << std::endl ;
}

void Dispatcher::Execute(ByteBuffer &commandBuffer, const int bufferLength) {
    if (bufferLength > 0) {
        commandBuffer.Reset();
        do {
            commandId = commandBuffer.ReadUInt16();
            if (commandRegistry.count(commandId) > 0)
                commandRegistry[commandId]->Execute(commandBuffer);
            else {
                std::cerr << "Dispatcher::Execute() -> Received command with ID = " << commandId << " is unregistered, aborting execution" << std::endl;
                break;
            }
        } while (commandBuffer.Index < bufferLength);
    }
}

}

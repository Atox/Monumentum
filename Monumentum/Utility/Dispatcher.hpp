#pragma once

#include <Monumentum/Utility/Command.hpp>
#include <cstdint>
#include <map>
#include <memory>

namespace Monumentum {

class Dispatcher {
public:
    Dispatcher();
    virtual ~Dispatcher();
    template<typename T, typename... Args>
    void Register(const uint16_t id, Args&&... args) {
        static_assert(std::is_base_of<Command, T>::value, "Dispatcher::RegisterCommand<T>() -> T must be derived from Command");
        if (commandRegistry.count(id) == 0)
            commandRegistry[id] = std::unique_ptr<T>(new T(std::forward<Args>(args)...));
        else
            std::cerr << "Dispatcher::Register() -> Overwriting already registered command with ID = " << commandId << std::endl;
    }
    void Remove(const uint16_t id);
    void Execute(ByteBuffer &commandBuffer, const int bufferLength);
private:
    uint16_t commandId;
    std::map<uint16_t, std::unique_ptr<Command>> commandRegistry;
};

}

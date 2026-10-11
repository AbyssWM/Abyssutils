#include <abyssutils/signal/Listener.hpp>
#include <tuple>

using namespace Abyssutils::Signal;

Abyssutils::Signal::CSignalListener::CSignalListener(std::function<void(void*)> handler) : m_fHandler(handler) {
    ;
}

void Abyssutils::Signal::CSignalListener::emitInternal(void* data) {
    if (!m_fHandler)
        return;

    m_fHandler(data);
}

void Abyssutils::Signal::CSignalListener::emit(std::any data) {
    auto dataTuple = std::tuple<std::any>(data);
    emitInternal(&dataTuple);
}

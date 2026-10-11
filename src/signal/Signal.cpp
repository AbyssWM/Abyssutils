#include "abyssutils/memory/SharedPtr.hpp"
#include <abyssutils/signal/Signal.hpp>
#include <abyssutils/memory/WeakPtr.hpp>
#include <algorithm>

using namespace Abyssutils::Signal;
using namespace Abyssutils::Memory;

#define SP CSharedPointer
#define WP CWeakPointer

void Abyssutils::Signal::CSignalBase::emitInternal(void* args) {

    // Save, an event can destroy thisptr
    const auto STATICS = m_vStaticListeners;

    if (!m_vListeners.empty()) {
        std::vector<SP<CSignalListener>> listeners;
        listeners.reserve(m_vListeners.size());

        for (const auto& l : m_vListeners) {
            if (l.expired())
                continue;

            listeners.emplace_back(l.lock());
        }

        for (const auto& l : listeners) {
            // if there is only one lock, it means the event is only held by the listeners
            // vector and was removed during our iteration
            if (l.strongRef() == 1)
                continue;

            l->emitInternal(args);
        }

        // release SPs
        listeners.clear();
    }

    if (!STATICS.empty()) {
        for (const auto& l : STATICS) {
            l->emitInternal(args);
        }
    }

    // we cannot release any expired refs here as one of the listeners could've removed this object and
    // as such we'd be doing a UAF
}

CAbyssSignalListener Abyssutils::Signal::CSignalBase::registerListenerInternal(std::function<void(void*)> handler) {
    CAbyssSignalListener listener = SP<CSignalListener>(new CSignalListener(handler));
    m_vListeners.emplace_back(listener);

    // housekeeping: remove any stale listeners
    std::erase_if(m_vListeners, [](const auto& other) { return other.expired(); });

    return listener;
}

void Abyssutils::Signal::CSignalBase::registerStaticListenerInternal(std::function<void(void*)> handler) {
    m_vStaticListeners.emplace_back(SP<CSignalListener>(new CSignalListener(handler)));
}

void Abyssutils::Signal::CSignal::emit(std::any data) {
    CSignalT::emit(data);
}

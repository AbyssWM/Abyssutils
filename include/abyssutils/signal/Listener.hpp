#pragma once

#include <any>
#include <functional>
#include <abyssutils/memory/SharedPtr.hpp>

namespace Abyssutils::Signal {
    class CSignalBase;

    class CSignalListener {
      public:
        CSignalListener(CSignalListener&&)       = delete;
        CSignalListener(CSignalListener&)        = delete;
        CSignalListener(const CSignalListener&)  = delete;
        CSignalListener(const CSignalListener&&) = delete;

        [[deprecated("Relic of the legacy untyped signal API. Using this with CSignalT is undefined behavior.")]] void emit(std::any data);

      private:
        CSignalListener(std::function<void(void*)> handler);

        void                       emitInternal(void* args);

        std::function<void(void*)> m_fHandler;

        friend class CSignalBase;
    };

    typedef Abyssutils::Memory::CSharedPointer<CSignalListener> CAbyssSignalListener;
}

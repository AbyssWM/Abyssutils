#pragma once

#include <abyssutils/eventLoop/PostDispatchHook.hpp>

#include <functional>

namespace Abyssutils::EventLoop {
    class CPostDispatchHook : public IPostDispatchHook {
      public:
        CPostDispatchHook(std::function<void()>&& callback) : m_callback(std::move(callback)) {}

        void call() {
            m_callback();
        }

      private:
        std::function<void()> m_callback;
    };
}

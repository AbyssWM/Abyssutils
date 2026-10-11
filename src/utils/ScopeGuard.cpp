#include <abyssutils/utils/ScopeGuard.hpp>

using namespace Abyssutils::Utils;

Abyssutils::Utils::CScopeGuard::CScopeGuard(const std::function<void()>& fn_) : fn(fn_) {
    ;
}

Abyssutils::Utils::CScopeGuard::~CScopeGuard() {
    if (fn)
        fn();
}

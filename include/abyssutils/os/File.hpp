#pragma once

#include <string>
#include <expected>
#include <string_view>

namespace Abyssutils::File {
    std::expected<std::string, std::string> readFileAsString(const std::string_view& path);
}
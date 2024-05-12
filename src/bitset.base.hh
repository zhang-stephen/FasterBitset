#pragma once

#include <cstddef>
#include <cstdint>

namespace vbitset
{
class BitsetBase
{
protected:
    constexpr BitsetBase()  = default;
    constexpr ~BitsetBase() = default;

    template <typename T>
    constexpr static auto divceil(const T& a, const T& b)
    {
        return (a + b - 1) / b;
    }

    using Unit = uint64_t;

    static constexpr auto numOfBitsPerUnit = sizeof(Unit);
};
} // namespace vbitset

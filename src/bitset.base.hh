#pragma once

#include <climits>
#include <cstddef>
#include <cstdint>
#include <span>
#include <type_traits>

namespace vbitset
{
template <size_t Nb, bool Neon = false, bool Sve = false>
class BitsetBase
{
private:
    static constexpr auto numOfNeonVectorBits     = 128; // fixed from ARM Neon manual
    static constexpr auto numOfScalableVectorBits = 128; // FIXME: not sure, it's defined by SoC vendor
    static constexpr auto numOfU64WordBits        = sizeof(uint64_t) * CHAR_BIT;

protected:
    constexpr BitsetBase()  = default;
    constexpr ~BitsetBase() = default;

    template <typename T>
    constexpr static auto divceil(const T& a, const T& b)
    {
        return (a + b - 1) / b;
    }

    using WordType = std::conditional_t<Sve, __uint128_t, std::conditional_t<Neon, __uint128_t, uint64_t>>;

    static constexpr auto numOfBitsPerWord
        = Sve ? numOfScalableVectorBits : (Neon ? numOfNeonVectorBits : numOfU64WordBits);
    static constexpr auto numOfWords = divceil(Nb, numOfBitsPerWord);

    WordType storage[numOfWords] { 0 };

public:
    auto getUnderlyingStorage() noexcept { return std::span<WordType, numOfWords>(storage); } // for unit-test

private:
    static_assert(!(Neon && Sve), "Neon & SVE cannot be enabled together!");
};
} // namespace vbitset

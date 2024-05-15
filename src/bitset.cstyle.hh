#pragma once

#include "bitset.base.hh"
#include "bitset.concept.hh"

#include <algorithm>
#include <bit>
#include <limits>
#include <numeric>

namespace vbitset
{
template <size_t N>
class BitsetCstyle : public BitsetBase
{
public:
    auto operator[](size_t pos) const
    {
        return static_cast<bool>(storage[pos / numOfBitsPerUnit] >> (pos % numOfBitsPerUnit) & 1);
    }

    auto test(size_t pos) const { return (*this)[pos]; }
    auto none() const noexcept { return !any(); }

    auto any() const noexcept
    {
        return std::count_if(cbegin(), cend(), [](auto&& v) { return v != 0; }) != 0;
    }

    auto all() const noexcept
    {
        return std::count_if(cbegin(), cend(), [](auto&& v) { return v == std::numeric_limits<Unit>::max(); })
            == numOfUnits;
    }

    auto count() const noexcept -> size_t
    {
        return std::accumulate(cbegin(), cend(), 0, [](auto&& init, auto&& v) { return init + std::popcount(v); });
    }

    constexpr auto size() const noexcept { return N; }

    auto& set() noexcept
    {
        std::fill(begin(), end(), std::numeric_limits<Unit>::max());
        return *this;
    }

    auto& set(size_t pos, bool value = true)
    {
        if (value)
        {
            storage[pos / numOfUnits] |= 1 << pos % numOfUnits;
        }
        else
        {
            storage[pos / numOfUnits] &= 1 << pos % numOfUnits;
        }

        return *this;
    }

    auto& reset() noexcept
    {
        std::fill(begin(), end(), 0);
        return *this;
    }

    auto& reset(size_t pos)
    {
        set(pos, false);
        return *this;
    }

    auto& flip() noexcept
    {
        std::for_each(begin(), end(), [](auto& v) { v = ~v; });
        return *this;
    }

    auto& flip(size_t pos) { return test(pos) ? reset(pos) : set(pos); }

    auto& operator&=(const BitsetCstyle<N>& other) noexcept
    {
        for (auto i = 0; i < numOfUnits; i++) storage[i] &= other.storage[i];
        return *this;
    }

    auto& operator|=(const BitsetCstyle<N>& other) noexcept
    {
        for (auto i = 0; i < numOfUnits; i++) storage[i] |= other.storage[i];
        return *this;
    }

    auto& operator^=(const BitsetCstyle<N>& other) noexcept
    {
        for (auto i = 0; i < numOfUnits; i++) storage[i] ^= other.storage[i];
        return *this;
    }

    auto operator~() noexcept
    {
        auto ret = BitsetCstyle<N>();
        for (auto i = 0; i < numOfUnits; i++) ret.storage[i] = ~storage[i];
        return ret;
    }

private:
    static_assert(N > 0, "number of bits should be greater than 0!");

    static constexpr auto numOfUnits = divceil(N, numOfBitsPerUnit);

    // HACK: they are not iterators for bitwise!
    constexpr auto* begin() noexcept { return storage; }
    constexpr auto* end() noexcept { return storage + numOfUnits; }
    constexpr auto* cbegin() const noexcept { return storage; }
    constexpr auto* cend() const noexcept { return storage + numOfUnits; }

    Unit storage[numOfUnits] { 0 };
};

static_assert(CompatibleBitset<BitsetCstyle<1>>);
} // namespace vbitset

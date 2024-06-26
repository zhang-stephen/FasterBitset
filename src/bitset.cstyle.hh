#pragma once

#include "bitset.base.hh"
#include "bitset.concept.hh"

#include <algorithm>
#include <bit>
#include <numeric>

namespace vbitset
{
template <size_t Nb>
class BitsetCstyle : public BitsetBase<Nb>
{
public:
    constexpr BitsetCstyle()  = default;
    constexpr ~BitsetCstyle() = default;

    auto operator<<(size_t pos) const noexcept
    {
        auto ret = BitsetCstyle<Nb>();

        for (auto i = 0; i < numOfWords; i++)
        {
            const auto cursor = numOfWords - i - 1;
            ret.storage[cursor]
                = storage[cursor] << pos | (i == numOfWords - 1 ? 0 : storage[cursor - 1] >> (numOfBitsPerWord - pos));
        }

        return ret;
    }

    auto operator>>(size_t pos) const noexcept
    {
        auto ret = BitsetCstyle<Nb>();

        for (auto i = 0; i < numOfWords; i++)
        {
            ret.storage[i] = storage[i] >> pos | (i == numOfWords - 1 ? 0 : storage[i + 1] << (numOfBitsPerWord - pos));
        }

        return ret;
    }

    auto operator[](size_t pos) const
    {
        // NOTE: do not support modify bit via operator[], the bitset reference is not implemented
        // to simplify the design
        return static_cast<bool>(storage[pos / numOfBitsPerWord] >> (pos % numOfBitsPerWord) & 1);
    }

    auto test(size_t pos) const { return (*this)[pos]; }
    auto none() const noexcept { return !any(); }

    auto any() const noexcept
    {
        return std::count_if(cbegin(), cend(), [](auto&& v) { return v != 0; }) != 0;
    }

    auto all() const noexcept
    {
        return std::count_if(cbegin(), cend(), [](auto&& v) { return v == std::numeric_limits<WordType>::max(); })
            == numOfWords;
    }

    auto count() const noexcept -> size_t
    {
        return std::accumulate(cbegin(), cend(), 0, [](auto&& init, auto&& v) { return init + std::popcount(v); });
    }

    constexpr auto size() const noexcept { return Nb; }

    auto& set() noexcept
    {
        std::fill(begin(), end(), std::numeric_limits<WordType>::max());
        return *this;
    }

    auto& set(size_t pos, bool value = true)
    {
        if (value)
        {
            storage[pos / numOfBitsPerWord] |= 1 << pos % numOfBitsPerWord;
        }
        else
        {
            storage[pos / numOfBitsPerWord] &= 1 << pos % numOfBitsPerWord;
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

    auto& operator&=(const BitsetCstyle<Nb>& other) noexcept
    {
        for (auto i = 0; i < numOfWords; i++) storage[i] &= other.storage[i];
        return *this;
    }

    auto& operator|=(const BitsetCstyle<Nb>& other) noexcept
    {
        for (auto i = 0; i < numOfWords; i++) storage[i] |= other.storage[i];
        return *this;
    }

    auto& operator^=(const BitsetCstyle<Nb>& other) noexcept
    {
        for (auto i = 0; i < numOfWords; i++) storage[i] ^= other.storage[i];
        return *this;
    }

    auto operator~() const noexcept
    {
        auto ret = BitsetCstyle<Nb>();
        for (auto i = 0; i < numOfWords; i++) ret.storage[i] = ~storage[i];
        return ret;
    }

private:
    static_assert(Nb > 0, "number of bits should be greater than 0!");

    // HACK: they are not iterators for bitwise!
    constexpr auto* begin() noexcept { return storage; }
    constexpr auto* end() noexcept { return storage + numOfWords; }
    constexpr auto* cbegin() const noexcept { return storage; }
    constexpr auto* cend() const noexcept { return storage + numOfWords; }

    using WordType = BitsetBase<Nb>::WordType;
    using BitsetBase<Nb>::storage;
    using BitsetBase<Nb>::numOfBitsPerWord;
    using BitsetBase<Nb>::numOfWords;
};

static_assert(CompatibleBitset<BitsetCstyle<1>>);
} // namespace vbitset

#pragma once

#include <bitset>
#include <concepts>

namespace vbitset
{
template <typename Bitset>
concept CompatibleBitset = requires(Bitset bs) {
    // methods - bit access, readonly
    { std::as_const(bs)[size_t()] } -> std::convertible_to<bool>;
    { std::as_const(bs).test(size_t()) } -> std::same_as<bool>;
    { std::as_const(bs).any() } noexcept -> std::same_as<bool>;
    { std::as_const(bs).all() } noexcept -> std::same_as<bool>;
    { std::as_const(bs).none() } noexcept -> std::same_as<bool>;
    { std::as_const(bs).count() } noexcept -> std::same_as<size_t>;
    { std::as_const(bs).size() } noexcept -> std::same_as<size_t>;
    // { std::as_const(bs) << size_t() } noexcept -> std::same_as<Bitset>;
    // { std::as_const(bs) >> size_t() } noexcept -> std::same_as<Bitset>;

    // methods - bit modifier
    { bs[size_t()] } -> std::convertible_to<bool>;
    { bs.set() } noexcept -> std::same_as<Bitset&>;
    { bs.set(size_t(), bool()) } -> std::same_as<Bitset&>;
    { bs.reset() } noexcept -> std::same_as<Bitset&>;
    { bs.reset(size_t()) } -> std::same_as<Bitset&>;
    { bs.flip() } noexcept -> std::same_as<Bitset&>;
    { bs.flip(size_t()) } -> std::same_as<Bitset&>;
    { bs &= Bitset() } noexcept -> std::same_as<Bitset&>;
    { bs |= Bitset() } noexcept -> std::same_as<Bitset&>;
    { bs ^= Bitset() } noexcept -> std::same_as<Bitset&>;
    { ~bs } noexcept -> std::same_as<Bitset>;
};

static_assert(CompatibleBitset<std::bitset<0>>,
              "CompatibleBitset<T> should be always evaluated as TRUE for STL bitset!");
}; // namespace vbitset

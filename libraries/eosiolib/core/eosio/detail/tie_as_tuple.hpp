#pragma once

namespace eosio { namespace detail {
template <size_t N>
struct tie_as_tuple_impl {
   template <typename T>
   static auto get(T& t) {
      return std::make_tuple(std::ref(t));
   }
};

template <typename T, size_t N>
struct tie_as_tuple;

#define EOSIO_CDT_CREATE_OVERLOAD(...) \
   template <typename T> \
   struct tie_as_tuple { \
      template <size_t N>
   constexpr auto tie_as_tuple(T& val, std::enable_if_t<

}} // ns eosio::detail

//
// Utility header for C++ Wrapper for STM32 HAL routines
//
// Created on 2018-09-15 by Tamas Karpati <tomikaa87@gmail.com>
//

#ifndef UTILS_HPP_
#define UTILS_HPP_

// Minimum necessary version is C++11
#if __cplusplus >= 201103l

#include <stdint.h>

namespace utils
{

    template <uint32_t... values>
    struct disjunction;

    template <uint32_t first, uint32_t... others>
    struct disjunction<first, others...>
    {
        static_assert(first < 32, "maximum 32 bits can be used");
        static const uint32_t value = first | disjunction<others...>::value;
    };

    template <>
    struct disjunction<>
    {
        static const uint32_t value = 0;
    };

    //////////////////////////////////////////////////////////////////////////

    template <uint8_t... values>
    struct flag_disjunction;

    template <uint8_t first, uint8_t... others>
    struct flag_disjunction<first, others...>
    {
        static_assert(first < 32, "maximum 32 bits can be used");
        static const uint32_t value = (1 << first) | flag_disjunction<others...>::value;
    };

    template <>
    struct flag_disjunction<>
    {
        static const uint32_t value = 0;
    };

    //////////////////////////////////////////////////////////////////////////

    namespace detail
    {
        template <uint32_t _value, uint8_t _bit_index>
        struct bit_weight_impl
        {
            static const uint8_t value =
                (_value & (1 << _bit_index) ? 1 : 0) + bit_weight_impl<_value, _bit_index + 1>::value;
        };

        template <uint32_t _value>
        struct bit_weight_impl<_value, 32>
        {
            static const uint8_t value = 0;
        };
    }

    template <uint32_t _value>
    struct bit_weight
    {
        static const uint8_t value = detail::bit_weight_impl<_value, 0>::value;
    };
}

#else
    #error utils library needs at least C++11 compiler support
#endif // __cplusplus

#endif /* UTILS_HPP_ */

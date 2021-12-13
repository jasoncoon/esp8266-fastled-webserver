/**

The MIT License (MIT)

Copyright (c) SimpleHacks, Henry Gabryjelski
https://github.com/SimpleHacks/UtilHeaders

All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#ifndef SIMPLEHACKS_INTEGER_SEQ_H
#define SIMPLEHACKS_INTEGER_SEQ_H

#include <cstddef>
#include <type_traits>
#include <array>

// A C++11 implementation of std::integer_sequence from C++14
//
// Two simpler examples:
//
//    // Create sequence of values from 0 ... 255, as type uint8_t:
//    auto sequence = SimpleHacks::CompileTime::make_integer_sequence<uint8_t, 255>
//
//    // Create size_t values from 0 ... 100:
//    auto sequence = SimpleHacks::CompileTime::make_index_sequenc( 100 );
//
namespace SimpleHacks {
namespace CompileTime {

    // Represents a compile-time sequence of integers.
    // When used as an argument to a function template,
    // the parameter pack Ints can be deduced and used in pack expansion.
    template<typename T, T... I>
    struct integer_sequence {
        static_assert( std::is_integral<T>::value, "Integral type" );
        using type = T;

        static constexpr T size = sizeof...(I);

        /// Generate an integer_sequence with an additional element.
        template<T N>
        using append = integer_sequence<T, I..., N>;

        using next = append<size>;
    };
    // C++11 rules require the static constexpr variable to be instantiated outside the template
    template<typename T, T... I>
    constexpr T integer_sequence<T, I...>::size;

    // ALIAS:  index_sequence<std::size_t... I> ==> integer_sequence<std::size_t, I...>
    template<std::size_t... I>
    using index_sequence = integer_sequence<std::size_t, I...>;

    // intended as private namespace to hide details from auto-completion and the like
    namespace _Detail
    {
        // Metafunction that generates an integer_sequence of T containing [0, N)
        template<typename T, T Nt, std::size_t N>
        struct iota
        {
            static_assert( Nt >= 0, "N cannot be negative" );
            using type = typename iota<T, Nt-1, N-1>::type::next;
        };

        // Terminal case of the recursive metafunction.
        template<typename T, T Nt>
        struct iota<T, Nt, 0ul>
        {
            using type = integer_sequence<T>;
        };
    }

    // Simplify creation of std::integer_sequence and std::index_sequence types with 0, 1, 2, ..., N-1 as Ints
    // ALIAS:  make_integer_sequence<T, N> ==> integer_sequence<T, 0,...N-1>
    template<typename T, T N>
    using make_integer_sequence = typename _Detail::iota<T, N, N>::type;

    // Simplify creation for the common case where T is std::size_t
    // ALIAS:  make_index_sequence<N>      ==> make_integer_sequence<std:size_t, N>
    //                                     ==> integer_sequence<std::size_t, 0, ...N-1>
    template<int N>
    using make_index_sequence = make_integer_sequence<std::size_t, N>;

    // Convert any type parameter pack into an index sequence of the same length 
    // ALIAS:  index_sequence_for<...>     ==> make_index_sequence<sizeof...(Args)>
    //                                     ==> make_integer_sequence<std:size_t, N>
    //                                     ==> integer_sequence<std::size_t, 0, ...N-1>
    template<typename... Args>
    using index_sequence_for = make_index_sequence<sizeof...(Args)>;

}  // namespace CompileTime
}  // namespace SimpleHacks

#endif  // SIMPLEHACKS_INTEGER_SEQ_H

/*
   The concepts are based on a file published by Jonathan Wakely.
   Without comment on its necessity, the following text is provided
   for your reference.
*/
/*
    Copyright Jonathan Wakely 2012-2013
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)

    Boost Software License - Version 1.0 - August 17th, 2003

    Permission is hereby granted, free of charge, to any person or organization
    obtaining a copy of the software and accompanying documentation covered by
    this license (the "Software") to use, reproduce, display, distribute,
    execute, and transmit the Software, and to prepare derivative works of the
    Software, and to permit third-parties to whom the Software is furnished to
    do so, all subject to the following:

    The copyright notices in the Software and this entire statement, including
    the above license grant, this restriction and the following disclaimer,
    must be included in all copies of the Software, in whole or in part, and
    all derivative works of the Software, unless such copies or derivative
    works are solely in the form of machine-executable object code generated by
    a source language processor.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
    SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
    FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
    ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

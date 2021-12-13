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

#ifndef CONSTEXPR_STRLEN_H
#define CONSTEXPR_STRLEN_H

// avr gcc  5.4  and higher
// clang    3.3  and higher, using -std=c++11
// gcc      5.4  and higher, using -std=c++11
// msvc    19.15 and higher
template< size_t N >
constexpr inline size_t constexpr_strlen( char const (&)[N] )
{
    return N-1;
}

#if __cpp_constexpr >= 201603
    // avr gcc -- not supported (as of v 5.4.0)
    // msvc    -- not supported (as of v 19.22)
    // gcc   7.2.1 and higher, using -std=c++17
    // clang 5.0.0 and higher, using -std=c++17
    constexpr inline size_t constexpr_strlen( const char* s )
    {
        return (
            (0 == s) ? 0 :
                (*s == '\0') ? 0 :
                    1 + constexpr_strlen((const char*)(s+1))
            );
    }
#endif




#endif // #ifndef CONSTEXPR_STRLEN_H



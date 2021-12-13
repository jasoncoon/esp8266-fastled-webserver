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

#ifndef STATIC_EVAL_H
#define STATIC_EVAL_H

// use to ensure static evaluation occcurs. e.g.,
// #include <constepr_strlen.h> // also in this repository
// if (static_eval<int, constexpr_strlen("hello, world2")>::value > 7) {
//     ....
// }

// avr gcc  5.4  and higher
// clang    3.3  and higher, using -std=c++11
// gcc      5.4  and higher, using -std=c++11
// msvc    19.15 and higher
template<typename T, T V>
struct static_eval
{
    static constexpr T value = V;
};

#endif // #ifndef STATIC_EVAL_H


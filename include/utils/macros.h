/* Created by Philip Smith on 10/17/2020.
MIT License

Copyright (c) 2021 Philip Arturo Smith

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

#ifndef UTILS_MACROS_H
#define UTILS_MACROS_H

#include <memory>


#define PTR(CLASS) typedef std::shared_ptr<CLASS> Ptr
#define USING_PTR(CLASS) using Ptr = std::shared_ptr<CLASS>
#define PLACEHOLDER
#define NOOP(...)
#define ID(X) X

// Private var base
#define MY_DECLARE(TYPE, NAME, ACCESS) ACCESS: TYPE m_##NAME
#define MY_VAR(TYPE, NAME, GETTER, GET_ACCESS, SETTER, SET_ACCESS) GETTER(NAME, TYPE, GET_ACCESS) SETTER(NAME, TYPE, SET_ACCESS) MY_DECLARE(TYPE, NAME, private)

// Base getter/setter
#define GETFUNC(NAME, TYPE, ACCESS, PRE, PREFIX, POST) ACCESS: PRE TYPE PREFIX##_##NAME() POST { return m_##NAME; }
#define SETFUNC(NAME, TYPE, ACCESS, PRE, PREFIX, VALUE_MOD, ARG_MOD) ACCESS: PRE void PREFIX##_##NAME(TYPE ARG_MOD(val)) { m_##NAME = VALUE_MOD(val); }

// Useful Getter/Setter permutations
#define GET(NAME, TYPE, ACCESS) GETFUNC(NAME, TYPE, ACCESS, inline, get, const)
#define GET_BOOL(NAME, _, ACCESS) GETFUNC(NAME, bool, ACCESS, inline, is,  const)
#define GET_CREF(NAME, TYPE, ACCESS) GETFUNC(NAME, const TYPE, ACCESS, inline, &get, const)
#define SET(NAME, TYPE, ACCESS) SETFUNC(NAME, TYPE, ACCESS, inline, set, ID, ID)

// Useful permutations of var base
#define VAR(TYPE, NAME, GET_ACCESS, SET_ACCESS) MY_VAR(TYPE, NAME, GET, GET_ACCESS, SET, SET_ACCESS)
#define VAR_GET(TYPE, NAME, ACCESS) MY_VAR(TYPE, NAME, GET, ACCESS, NOOP, PLACEHOLDER)
#define VAR_SET(TYPE, NAME, ACCESS) MY_VAR(TYPE, NAME, NOOP, PLACEHOLDER, SET, ACCESS)

// BVAR: boolean var gets use "is_X" instead of "get_X"
#define BVAR(NAME, GET_ACCESS, SET_ACCESS) MY_VAR(bool, NAME, GET_BOOL, GET_ACCESS, SET, SET_ACCESS)
#define BVAR_GET(NAME, ACCESS) MY_VAR(bool, NAME, GET_BOOL, ACCESS, NOOP, PLACEHOLDER)

// CREFVAR: var that is accessed with constant refs
#define CREFVAR(TYPE, NAME, GET_ACCESS, SET_ACCESS) MY_VAR(TYPE, NAME, GET_CREF, GET_ACCESS, SET_REF, SET_ACCESS)
#define CREFVAR_GET(TYPE, NAME, ACCESS) MY_VAR(TYPE, NAME, GET_CREF, ACCESS, NOOP, PLACEHOLDER)

#endif //UTILS_MACROS_H

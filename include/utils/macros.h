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

#ifndef PTR
#define PTR(CLASS) typedef std::shared_ptr<CLASS> Ptr
#endif

#ifndef USING_PTR
#define USING_PTR(CLASS) using Ptr = std::shared_ptr<CLASS>
#endif

#ifndef PLACEHOLDER
#define PLACEHOLDER
#endif

#ifndef NOOP
#define NOOP(...)
#endif

#ifndef ID
#define ID(X) X
#endif

// Private var base
#ifndef MY_DECLARE
#define MY_DECLARE(TYPE, NAME, ACCESS) ACCESS: TYPE m_##NAME
#endif

#ifndef MY_VAR
#define MY_VAR(TYPE, NAME, GETTER, GET_ACCESS, SETTER, SET_ACCESS) GETTER(NAME, TYPE, GET_ACCESS) SETTER(NAME, TYPE, SET_ACCESS) MY_DECLARE(TYPE, NAME, private)
#endif

// Base getter/setter
#ifndef GETFUNC
#define GETFUNC(NAME, TYPE, ACCESS, PRE, PREFIX, POST) ACCESS: PRE TYPE PREFIX##_##NAME() POST { return m_##NAME; }
#endif

#ifndef SETFUNC
#define SETFUNC(NAME, TYPE, ACCESS, PRE, PREFIX, VALUE_MOD, ARG_MOD) ACCESS: PRE void PREFIX##_##NAME(TYPE ARG_MOD(val)) { m_##NAME = VALUE_MOD(val); }
#endif

// Useful Getter/Setter permutations
#ifndef GET
#define GET(NAME, TYPE, ACCESS) GETFUNC(NAME, TYPE, ACCESS, inline, get, const)
#endif

#ifndef GET_BOOL
#define GET_BOOL(NAME, _, ACCESS) GETFUNC(NAME, bool, ACCESS, inline, is,  const)
#endif

#ifndef GET_CREF
#define GET_CREF(NAME, TYPE, ACCESS) GETFUNC(NAME, const TYPE, ACCESS, inline, &get, const)
#endif

#ifndef SET
#define SET(NAME, TYPE, ACCESS) SETFUNC(NAME, TYPE, ACCESS, inline, set, ID, ID)
#endif

// Useful permutations of var base
#ifndef VAR
#define VAR(TYPE, NAME, GET_ACCESS, SET_ACCESS) MY_VAR(TYPE, NAME, GET, GET_ACCESS, SET, SET_ACCESS)
#endif

#ifndef VAR_GET
#define VAR_GET(TYPE, NAME, ACCESS) MY_VAR(TYPE, NAME, GET, ACCESS, NOOP, PLACEHOLDER)
#endif

#ifndef VAR_SET
#define VAR_SET(TYPE, NAME, ACCESS) MY_VAR(TYPE, NAME, NOOP, PLACEHOLDER, SET, ACCESS)
#endif

// BVAR: boolean var gets use "is_X" instead of "get_X"
#ifndef BVAR
#define BVAR(NAME, GET_ACCESS, SET_ACCESS) MY_VAR(bool, NAME, GET_BOOL, GET_ACCESS, SET, SET_ACCESS)
#endif

#ifndef BVAR_GET
#define BVAR_GET(NAME, ACCESS) MY_VAR(bool, NAME, GET_BOOL, ACCESS, NOOP, PLACEHOLDER)
#endif

// CREFVAR: var that is accessed with constant refs
#ifndef CREFVAR
#define CREFVAR(TYPE, NAME, GET_ACCESS, SET_ACCESS) MY_VAR(TYPE, NAME, GET_CREF, GET_ACCESS, SET_REF, SET_ACCESS)
#endif

#ifndef CREFVAR_GET
#define CREFVAR_GET(TYPE, NAME, ACCESS) MY_VAR(TYPE, NAME, GET_CREF, ACCESS, NOOP, PLACEHOLDER)
#endif

#endif //UTILS_MACROS_H

//
// Created by Philip Smith on 10/17/2020.
//

#ifndef UTILS_MACROS_H
#define UTILS_MACROS_H

#include <memory>
#include <utility>


#define PTR(CLASS) typedef std::shared_ptr<CLASS> Ptr
#define USE_PTR(CLASS) using Ptr = std::shared_ptr<CLASS>
#define PLACEHOLDER
#define MOVE(X) std::move(X)
#define NOOP(...)
#define ID(X) X
#define REF(X) & X

// Private var base
#define _DECLARE(TYPE, NAME, ACCESS) ACCESS: TYPE m_##NAME
#define _VAR(TYPE, NAME, GETTER, GET_ACCESS, SETTER, SET_ACCESS) GETTER(NAME, TYPE, GET_ACCESS) SETTER(NAME, TYPE, SET_ACCESS) _DECLARE(TYPE, NAME, private)

// Base getter/setter
#define GETFUNC(NAME, TYPE, ACCESS, PRE, PREFIX, POST) ACCESS: PRE TYPE PREFIX##_##NAME() POST { return m_##NAME; }
#define SETFUNC(NAME, TYPE, ACCESS, PRE, PREFIX, VALUE_MOD, ARG_MOD) ACCESS: PRE void PREFIX##_##NAME(TYPE ARG_MOD(val)) { m_##NAME = VALUE_MOD(val); }

// Useful Getter/Setter permutations
#define GET(NAME, TYPE, ACCESS) GETFUNC(NAME, TYPE, ACCESS, inline, get, const)
#define GET_BOOL(NAME, _, ACCESS) GETFUNC(NAME, bool, ACCESS, inline, is,  const)
#define GET_REF(NAME, TYPE, ACCESS) GETFUNC(NAME, TYPE, ACCESS, inline, &get, PLACEHOLDER)
#define GET_CREF(NAME, TYPE, ACCESS) GETFUNC(NAME, const TYPE, ACCESS, inline, &get, const)
#define SET(NAME, TYPE, ACCESS) SETFUNC(NAME, TYPE, ACCESS, inline, set, ID, ID)
#define SET_REF(NAME, TYPE, ACCESS) SETFUNC(NAME, TYPE, ACCESS, inline, set, ID, REF)

// Useful permutations of var base
#define VAR(TYPE, NAME, GET_ACCESS, SET_ACCESS) _VAR(TYPE, NAME, GET, GET_ACCESS, SET, SET_ACCESS)
#define VAR_GET(TYPE, NAME, ACCESS) _VAR(TYPE, NAME, GET, ACCESS, NOOP, PLACEHOLDER)
#define VAR_SET(TYPE, NAME, ACCESS) _VAR(TYPE, NAME, NOOP, PLACEHOLDER, SET, ACCESS)

// BVAR: boolean var gets use "is_X" instead of "get_X"
#define BVAR(NAME, GET_ACCESS, SET_ACCESS) _VAR(bool, NAME, GET_BOOL, GET_ACCESS, SET, SET_ACCESS)
#define BVAR_GET(NAME, ACCESS) _VAR(bool, NAME, GET_BOOL, ACCESS, NOOP, PLACEHOLDER)

// CREFVAR: var that is accessed with constant refs
#define CREFVAR(TYPE, NAME, GET_ACCESS, SET_ACCESS) _VAR(TYPE, NAME, GET_CREF, GET_ACCESS, SET_REF, SET_ACCESS)
#define CREFVAR_GET(TYPE, NAME, ACCESS) _VAR(TYPE, NAME, GET_CREF, ACCESS, NOOP, PLACEHOLDER)

#endif //UTILS_MACROS_H

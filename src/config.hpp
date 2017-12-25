/*
** LHades
** Copyright (C) 2017 tacigar
*/

#ifndef LHADES_CONFIG_HPP
#define LHADES_CONFIG_HPP

#include <lua.hpp>
#include <luaconf.h>

#if LUAI_BITSINT >= 32
    typedef unsigned int Instruction;
#else
    typedef unsigned long Instruction;
#endif

#define LUA_TNUMFLT (LUA_TNUMBER | (0 << 4))
#define LUA_TNUMINT (LUA_TNUMBER | (1 << 4))

#define LUA_TSHRSTR (LUA_TSTRING | (0 << 4))
#define LUA_TLNGSTR (LUA_TSTRING | (1 << 4))

#endif // LHADES_CONFIG_HPP

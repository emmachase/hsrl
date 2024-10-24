#pragma once

#include "lstate.hpp"
#include "luaconf.hpp"
#include <offsets.hpp>

// #define LUA_REGISTRYINDEX	(-LUAI_MAXSTACK - 1000)
#define LUA_REGISTRYINDEX (-LUAI_MAXCSTACK - 2000)

#define LUA_OK 0

#define LUA_TSTRING 4

// typedef int (*lua_CFunction)(lua_State* L);
// typedef int (*lua_Continuation)(lua_State* L, int status);

DECLARE_METHOD(void, lua_settop, (lua_State* state, int index), (state, index));
DECLARE_METHOD(void, lua_pushvalue, (lua_State* state, int index), (state, index));

DECLARE_METHOD(int, lua_type, (lua_State* state, int index), (state, index));

DECLARE_METHOD(const char*, lua_tolstring, (lua_State* state, int index, size_t* len), (state, index, len));

DECLARE_METHOD(void, lua_pushinteger, (lua_State* state, long long val), (state, val));
// #define lua_pushfstring(state, fmt, ...) ((const char*(*)(lua_State* state, const char*, ...))(offsets::lua_pushfstring))(state, fmt, __VA_ARGS__)
// LUA_API LUA_PRINTF_ATTR(2, 3) const char* lua_pushfstringL(lua_State* L, const char* fmt, ...);
// #define lua_pushfstring(L, fmt, ...) lua_pushfstringL(L, fmt, ##__VA_ARGS__)
#define lua_pushfstring(L, fmt, ...) ((const char*(*)(lua_State*, const char*, ...))(offsets::lua_pushfstringL))(L, fmt, __VA_ARGS__)
// DECLARE_METHOD(void, lua_pushcclosure, (lua_State* state, void* func, int nup), (state, func, nup));
// (lua_State* L, lua_CFunction fn, const char* debugname, int nup, lua_Continuation cont);
DECLARE_METHOD(void, lua_pushcclosurek, (lua_State* state, void* func, const char* debugname, int nup, void* cont), (state, func, debugname, nup, cont));
#define lua_pushcclosure(L, fn, debugname, nup) lua_pushcclosurek(L, fn, debugname, nup, NULL)
DECLARE_METHOD(void, lua_pushboolean, (lua_State* state, int val), (state, val));

DECLARE_METHOD(int, lua_getfield, (lua_State* state, int index, const char* name), (state, index, name));
DECLARE_METHOD(void, lua_createtable, (lua_State* state, int narr, int nrec), (state, narr, nrec));
// DECLARE_METHOD(void*, lua_newuserdata, (lua_State* state, size_t size), (state, size));
// LUA_API void* lua_newuserdatatagged(lua_State* L, size_t sz, int tag);
DECLARE_METHOD(void*, lua_newuserdatatagged, (lua_State* state, size_t size, int tag), (state, size, tag));
#define lua_newuserdata(L, s) lua_newuserdatatagged(L, s, 0)

DECLARE_METHOD(void, lua_setfield, (lua_State* state, int index, const char* name), (state, index, name));

DECLARE_METHOD(int, lua_pcall, (lua_State* state, int nargs, int nresults, int errfunc), (state, nargs, nresults, errfunc));
// DECLARE_METHOD(int, lua_dump, (lua_State* state, int(*writer)(lua_State*, void*, size_t, void*), void* data, int strip), (state, writer, data, strip));

// int luau_load(lua_State* L, const char* chunkname, const char* data, size_t size, int env)
DECLARE_METHOD(int, luau_load, (lua_State* state, const char* chunkname, const char* data, size_t size, int env), (state, chunkname, data, size, env));

#define lua_pop(state, amount) lua_settop(state, -(amount) - 1)

#define lua_newtable(state) lua_createtable(state, 0, 0)

#define lua_pushcfunction(state, func) lua_pushcclosure(state, (func), 0)

#define lua_tostring(state, index) lua_tolstring(state, (index), nullptr)
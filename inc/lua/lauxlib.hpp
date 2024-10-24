#pragma once

#include "lua.hpp"

struct luaL_Reg {
  const char* name;
  void* func;
};

DECLARE_METHOD(const char*, luaL_checklstring, (lua_State* state, int index, size_t* len), (state, index, len));
DECLARE_METHOD(long long, luaL_checkinteger, (lua_State* state, int index), (state, index));
// void luaL_checkstack(lua_State* L, int space, const char* mes)
DECLARE_METHOD(void, luaL_checkstack, (lua_State* state, int space, const char* mes), (state, space, mes));

DECLARE_METHOD(void, luaL_newmetatable, (lua_State* state, const char* name), (state, name));
// DECLARE_METHOD(void, luaL_setmetatable, (lua_State* state, const char* name), (state, name));

// LUA_API int lua_setmetatable(lua_State* L, int objindex);
DECLARE_METHOD(int, lua_setmetatable, (lua_State* state, int objindex), (state, objindex));

#define luaL_getmetatable(L,n)	(lua_getfield(L, LUA_REGISTRYINDEX, (n)))

// defined in polyfills.cpp
void luaL_setmetatable (lua_State *L, const char *tname);
void luaL_setfuncs(lua_State *L, const luaL_Reg *l, int nup);
////////////////////////////////////////

DECLARE_METHOD(void*, luaL_checkudata, (lua_State* state, int index, const char* name), (state, index, name));

DECLARE_METHOD(int, luaL_loadstring, (lua_State* state, const char* str), (state, str));
DECLARE_METHOD(lua_State*, luaL_newstate, (), ());

// DECLARE_METHOD(void, luaL_setfuncs, (lua_State* state, const luaL_Reg* list, int nup), (state, list, nup));




#define luaL_newlibtable(state, list) lua_createtable(state, 0, sizeof(list) / sizeof((list)[0]) - 1)

#define luaL_newlib(state, list) (luaL_newlibtable(state, list), luaL_setfuncs(state, list, 0))

#define luaL_checkstring(state, index) (luaL_checklstring(state, (index), nullptr))
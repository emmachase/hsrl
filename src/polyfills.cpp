#include <lua/lua.hpp>
#include <lua/lauxlib.hpp>

void luaL_setmetatable (lua_State *L, const char *tname) {
  std::printf("Setting metatable\n");
  luaL_getmetatable(L, tname);
  std::printf("Got metatable\n");
  lua_setmetatable(L, -2);
  std::printf("Set metatable\n");
}

const char *NO_NAME = "no debug name";
void luaL_setfuncs(lua_State *L, const luaL_Reg *l, int nup) {
  luaL_checkstack(L, nup, "too many upvalues");
  for (; l->name != NULL; l++) {  /* fill the table with given functions */
    if (l->func == NULL)  /* placeholder? */
      lua_pushboolean(L, 0);
    else {
      int i;
      for (i = 0; i < nup; i++)  /* copy upvalues to the top */
        lua_pushvalue(L, -nup);
      lua_pushcclosure(L, l->func, NO_NAME, nup);  /* closure with those upvalues */
    }
    lua_setfield(L, -(nup + 2), l->name);
  }
  lua_pop(L, nup);  /* remove upvalues */
}

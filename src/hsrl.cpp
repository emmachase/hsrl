#include <hsrl.hpp>
#include <ui/console.hpp>
#include <ui/scripts.hpp>
#include <lua/lua.hpp>
#include <lua/lauxlib.hpp>
#include <lua/xlua.hpp>
#include <utils/fnv.hpp>
#include <fstream>

int hsrl::print(lua_State* state) {
  if (const auto str = luaL_checkstring(state, 1))
    ui::console::add(str);

  return 0;
}

int hsrl::clear([[maybe_unused]] lua_State* state) {
  ui::console::clear();
  return 0;
}

int hsrl::getreg(lua_State* state) {
  lua_pushvalue(state, LUA_REGISTRYINDEX);
  return 1;
}

int hsrl::writefile(lua_State* state) {
  const auto path = luaL_checkstring(state, 1);
  const auto data = luaL_checkstring(state, 2);

  if (!path || !data) {
    lua_pushboolean(state, false);
    return 1;
  }

  std::ofstream file { path };

  if (!file.is_open()) {
    lua_pushboolean(state, false);
    return 1;
  }

  file << data;
  file.close();

  lua_pushboolean(state, true);
  return 1;
}

int hsrl::window::_new(lua_State* state) {
  const auto title = luaL_checkstring(state, 1);

  std::printf("Creating a new window: %s\n", title);

  if (!title)
    return 0;

  const auto window = (ui::scripts::window_t*)(lua_newuserdata(state, sizeof(ui::scripts::window_t)));
  std::printf("Created userdata: %p\n", window);
  luaL_setmetatable(state, "HSRLWindow");
  std::printf("Set metatable\n");

  new (window) ui::scripts::window_t();
  window->title = title;
  std::printf("Initialized the window\n");

  std::unique_lock guard{ ui::scripts::windows_mutex };
  ui::scripts::windows.emplace_back(window);
  std::printf("Added the window to the list\n");

  return 1;
}

int hsrl::window::settitle(lua_State* state) {
  std::unique_lock guard{ ui::scripts::windows_mutex };

  if (const auto window = (ui::scripts::window_t*)(luaL_checkudata(state, 1 , "HSRLWindow")))
    if (const auto title = luaL_checkstring(state, 2))
      window->title = title;

  return 0;
}

int hsrl::window::button(lua_State* state) {
  const auto window = (ui::scripts::window_t*)(luaL_checkudata(state, 1 , "HSRLWindow"));
  const auto text = luaL_checkstring(state, 2);
  const auto flag_name = luaL_checkstring(state, 3);

  if (!window || !text || !flag_name)
    return 0;

  ui::scripts::clickable_t::create(window, text, flag_name, ui::scripts::window_object_type::button);

  return 0;
}

int hsrl::window::checkbox(lua_State* state) {
  const auto window = (ui::scripts::window_t*)(luaL_checkudata(state, 1 , "HSRLWindow"));
  const auto text = luaL_checkstring(state, 2);
  const auto flag_name = luaL_checkstring(state, 3);

  if (!window || !text || !flag_name)
    return 0;

  ui::scripts::clickable_t::create(window, text, flag_name, ui::scripts::window_object_type::checkbox);

  return 0;
}

int hsrl::window::sliderint(lua_State* state) {
  const auto window = (ui::scripts::window_t*)(luaL_checkudata(state, 1 , "HSRLWindow"));
  const auto text = luaL_checkstring(state, 2);
  const auto min = luaL_checkinteger(state, 3);
  const auto max = luaL_checkinteger(state, 4);
  const auto flag_name = luaL_checkstring(state, 5);

  if (!window || !text || !flag_name)
    return 0;

  const auto flag = new ui::scripts::flag_t();
  flag->hash = utils::fnv::hash(flag_name);
  flag->type = ui::scripts::flag_type::integer;

  const auto slider = new ui::scripts::slider_t();
  slider->text = text;
  slider->min = (int)(min);
  slider->max = (int)(max);
  slider->flag = flag;
  slider->type = ui::scripts::window_object_type::slider_int;

  flag->object = slider;
  flag->i = (int)(min);

  std::unique_lock guard{ ui::scripts::windows_mutex };

  window->objects.emplace_back(slider);
  ui::scripts::flags.emplace_back(flag);

  return 0;
}

int hsrl::getflag(lua_State* state) {
  const auto flag_name = luaL_checkstring(state, 1);

  if (!flag_name)
    return 0;

  const auto flag_hash = utils::fnv::hash(flag_name);

  for (const auto flag : ui::scripts::flags) {
    if (flag->hash != flag_hash)
      continue;

    switch (flag->type) {
      case ui::scripts::flag_type::boolean: {
        lua_pushboolean(state, flag->b);

        if (flag->object->type == ui::scripts::window_object_type::button)
          flag->b = false;

        return 1;
      }

      case ui::scripts::flag_type::integer: {
        lua_pushinteger(state, flag->i);
        return 1;
      }
    }
  }

  return 0;
}

static constexpr luaL_Reg hsrllib[] = {
  { "print", hsrl::print },
  { "clear", hsrl::clear },
  { "getreg", hsrl::getreg },
  { "getflag", hsrl::getflag },
  { "writefile", hsrl::writefile },
  { nullptr, nullptr }
};

static constexpr luaL_Reg windowlib[] = {
  { "new", hsrl::window::_new },
  { "settitle", hsrl::window::settitle },
  { "button", hsrl::window::button },
  { "checkbox", hsrl::window::checkbox },
  { "sliderint", hsrl::window::sliderint },
  { nullptr, nullptr }
};

void hsrl::open(lua_State* state) {
  luaL_newlib(state, hsrllib);

  luaL_newmetatable(state, "HSRLWindow");
  luaL_setfuncs(state, windowlib, 0);

  lua_pushvalue(state, -1);
  lua_setfield(state, -2, "__index");

  lua_setfield(state, -2, "window");

  xlua_setglobal(state, "hsrl");
}

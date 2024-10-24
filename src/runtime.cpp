#include <runtime.hpp>
#include <hsrl.hpp>
#include <hooks/hooks.hpp>
#include <ui/console.hpp>
#include <lua/lua.hpp>
#include <lua/lauxlib.hpp>
#include <lua/xlua.hpp>
#include <luau/luacode.h>

#include <cstdio>
#include <thread>
#include <sstream>

using namespace std::chrono_literals;

static std::string init_script = R"(
local updateCallbacks = { };

hsrl.on_update = function(func)
  table.insert(updateCallbacks, func);
end

hsrl.invoke_update = function()
  for _, func in next, updateCallbacks do
    pcall(func);
  end
end
)";

void runtime::initialize(lua_State* state) {
  std::printf("Initializing the lua runtime\n");
  std::printf("HSR state: %p\n", state);

  runtime::hsr_state = state;

  std::printf("Creating a new lua state\n");

  runtime::lua_state = luaL_newstate();

  std::printf("Opening the lua hsrl ibrary\n");

  hsrl::open(state);

  std::printf("Compiling the init script\n");

  const auto compiled = runtime::compile(init_script);

  if (compiled.has_value()) {
    std::printf("Executing the init script\n");
    runtime::do_buffer(compiled.value());
  }
}

void runtime::do_buffer(const std::string& compiled) {
  // hooks::loadbuffer.get_trampoline<decltype(&xluaL_loadbuffer)>()(runtime::hsr_state, compiled.c_str(), compiled.size(), "hsrl");
  if (luaL_loadstring(runtime::hsr_state, compiled.c_str()) != LUA_OK) {
    // ui::console::add(std::string("Failed to compile a script: ") + lua_tostring(runtime::hsr_state, 1));
    std::printf("Failed to execute a script: %s\n", lua_tostring(runtime::hsr_state, -1));
    lua_pop(runtime::hsr_state, 1);
    return;
  }
  // int result = luau_load(runtime::hsr_state, "hsrl", compiled.c_str(), compiled.size(), 0);
  // if (result != LUA_OK) {
  //   std::printf("Failed to load a script: %s\n", lua_tostring(runtime::hsr_state, -1));
  //   lua_pop(runtime::hsr_state, 1);
  //   return;
  // }

  if (lua_pcall(runtime::hsr_state, 0, 0, 0) != LUA_OK) {
    // ui::console::add(std::string("Failed to execute a script: ") + lua_tostring(runtime::hsr_state, 1));
    std::printf("Failed to execute a script: %s\n", lua_tostring(runtime::hsr_state, -1));
    lua_pop(runtime::hsr_state, 1);
  }
  std::printf("Executed the init script\n");
}

std::optional<std::string> runtime::compile(const std::string& script) {
  return std::make_optional(script);
//   if (luaL_loadstring(runtime::lua_state, script.c_str()) != LUA_OK) {
//     ui::console::add(std::string("Failed to compile a script: ") + lua_tostring(runtime::lua_state, 1));
//     lua_pop(runtime::lua_state, 1);
//     return std::nullopt;
//   }

// // LUACODE_API char* luau_compile(const char* source, size_t size, lua_CompileOptions* options, size_t* outsize);

//   size_t bytecodeSize = 0;
//   char* bytecode = luau_compile(script.c_str(), script.length(), NULL, &bytecodeSize);

//   return std::make_optional(std::string(bytecode, bytecodeSize));

  // std::ostringstream compiled;

  // const auto writer = [](lua_State* state, void* p, size_t s, void* ud) {
  //   ((std::ostringstream*)(ud))->write((const char*)(p), (std::streamsize)(s));
  //   return 0;
  // };

  // lua_dump(runtime::lua_state, writer, &compiled, 0);
  // lua_pop(runtime::lua_state, 1);

  // return std::make_optional(compiled.str());
}
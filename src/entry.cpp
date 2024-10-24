#include <utils/console.hpp>
#include <hooks/hooks.hpp>
#include <offsets.hpp>

#include <thread>
#include <windows.h>

using namespace std::chrono_literals;

void initialize() {
  utils::console::attach("HSRL - Honkai: Star Rail Lua");
  std::printf("Waiting for GameAssembly.dll and xluau.dll\n");

  while (!GetModuleHandleA("GameAssembly.dll")) {
    std::this_thread::sleep_for(10ms);
  }

  std::printf("GameAssembly.dll found\n");

  while (!GetModuleHandleA("xluau.dll")) {
    std::this_thread::sleep_for(10ms);
  }

  std::printf("GameAssembly.dll and xluau.dll found\n");

  // std::this_thread::sleep_for(15000ms);

  std::printf("running\n");

  // while (!GetModuleHandleA("xlua.dll")) {
  //   std::this_thread::sleep_for(10ms);
  // }

  // std::printf("GameAssembly.dll and xlua.dll found\n");

  offsets::initialize();
  hooks::initialize();
}

[[maybe_unused]] bool DllMain(const HMODULE module, const unsigned int reason, [[maybe_unused]] void* reserved) {
  DisableThreadLibraryCalls(module);

  if (reason == DLL_PROCESS_ATTACH)
    if (const auto handle = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)(initialize), nullptr, 0, nullptr))
      CloseHandle(handle);

  return true;
}
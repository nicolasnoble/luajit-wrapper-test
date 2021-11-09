#include <iostream>
#include <stdexcept>
#include "lua.hpp"

// Catch C++ exceptions and convert them to Lua error messages.
// Customize as needed for your own exception classes.
static int wrap_exceptions(lua_State *L, lua_CFunction f)
{
  try {
    return f(L);  // Call wrapped function and return result.
  } catch (const char *s) {  // Catch and convert exceptions.
    lua_pushstring(L, s);
  } catch (std::exception& e) {
    lua_pushstring(L, e.what());
  } catch (...) {
    lua_pushliteral(L, "caught (...)");
  }
  return lua_error(L);  // Rethrow as a Lua error.
}

int main(void)
{
  lua_State *L = lua_open();
  lua_atpanic(L, [](lua_State* L) -> int { throw std::runtime_error(lua_tostring(L, 1)); });
  try {
    // Define wrapper function and enable it.
    lua_pushlightuserdata(L, (void *)wrap_exceptions);
    luaJIT_setmode(L, -1, LUAJIT_MODE_WRAPCFUNC | LUAJIT_MODE_ON);
    lua_pop(L, 1);
  } catch (std::exception &e) {
    std::cout << e.what();
  }

  lua_close(L);

  return 0;
}

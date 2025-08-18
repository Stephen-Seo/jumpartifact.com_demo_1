// ISC License
//
// Copyright (c) 2025 Stephen Seo
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
// REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
// INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
// LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
// OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

#ifndef SEODISPARATE_COM_JUMPARTIFACT_DEMO_1_TEST_LUA_SCENE_H_
#define SEODISPARATE_COM_JUMPARTIFACT_DEMO_1_TEST_LUA_SCENE_H_

#include "scene_system.h"

// standard library includes
#include <string>

// third party includes
extern "C" {
#include "lua.h"
}

constexpr int TEXT_BUF_SIZE = 65536;
constexpr int FILENAME_BUF_SIZE = 1024;

const static char *LUA_DEFAULT_TEXT =
    "-- Input Lua code here.\n"
    "-- basic, coroutine, package, string manip., utf-8, table manip., and "
    "math modules are available\n";

const static char *LUA_LPEG_LOAD_SCRIPT = "return luaopen_lpeg_global()";

class TestLuaScene : public Scene {
 public:
  TestLuaScene(SceneSystem *ctx);
  virtual ~TestLuaScene() override;

  virtual void update(SceneSystem *ctx, float dt) override;
  virtual void draw(SceneSystem *ctx) override;
  virtual void draw_rlimgui(SceneSystem *ctx) override;
  virtual bool allow_draw_below(SceneSystem *ctx) override;

 private:
  enum class ExecState { PENDING, SUCCESS, FAILURE };

  std::array<char, TEXT_BUF_SIZE> buf;
  std::array<char, FILENAME_BUF_SIZE> filename;
  std::string error_text;
  std::string save_error_text;
  lua_State *lua_ctx;
  ExecState exec_state;
};

#endif

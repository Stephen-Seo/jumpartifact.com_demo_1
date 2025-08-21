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
#include <bitset>
#include <optional>
#include <string>

// third party includes
extern "C" {
#include "lua.h"
}

constexpr int TEXT_BUF_SIZE = 65536;
constexpr int FILENAME_BUF_SIZE = 1024;

const static char *LUA_DEFAULT_TEXT =
    "-- Input Lua code here.\n"
    "-- basic, coroutine, package, string manip., utf-8, table manip.,\n"
    "-- math, io, os, and debug modules are available\n";

const static char *MOONSCRIPT_HELP_TEXT =
    "-- moonscript is loaded.\n"
    "-- This means you can save a \"test.moon\",\n"
    "-- and executing it will run as moonscript.\n"
    "-- Also try require('test') for \"test.moon\".\n";

const static char *LUA_LPEG_LOAD_SCRIPT = "return luaopen_lpeg_global()";

class TestLuaScene : public Scene {
 public:
  TestLuaScene(SceneSystem *ctx);
  virtual ~TestLuaScene() override;

  virtual void update(SceneSystem *ctx, float dt) override;
  virtual void draw(SceneSystem *ctx) override;
  virtual void draw_rlimgui(SceneSystem *ctx) override;
  virtual bool allow_draw_below(SceneSystem *ctx) override;

  void reset();

  std::optional<const char *> get_buffer_once();
  void upload_text(const char *);

 private:
  enum class ExecState {
    PENDING,
    GENERIC_SUCCESS,
    GENERIC_FAILURE,
    SAVE_SUCCESS,
    SAVE_FAILURE,
    LOAD_SUCCESS,
    LOAD_FAILURE,
    DL_SUCCESS,
    DL_FAILURE,
    UP_SUCCESS,
    UP_FAILURE,
    UPDATED
  };

  std::array<char, TEXT_BUF_SIZE> buf;
  std::array<char, FILENAME_BUF_SIZE> filename;
  std::string error_text;
  std::string save_error_text;
  std::string save_error_text_err;
  // 0 - moonscript is loaded
  // 1 - buffer fetched once this frame
  std::bitset<32> flags;
  ExecState exec_state;
  ExecState saveload_state;

  static std::optional<std::string> load_from_file(const char *filename);

  std::optional<lua_State *> get_lctx(SceneSystem *ctx) const;

  void reset_error_texts();
};

#endif

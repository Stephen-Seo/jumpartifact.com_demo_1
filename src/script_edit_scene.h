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

#ifndef SEODISPARATE_COM_JUMPARTIFACT_DEMO_1_SCRIPT_EDIT_SCENE_H_
#define SEODISPARATE_COM_JUMPARTIFACT_DEMO_1_SCRIPT_EDIT_SCENE_H_

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
    "-- All Lua modules are available\n";

const static char *MOONSCRIPT_HELP_TEXT = "-- Moonscript is loaded.\n";

const static char *DEFAULT_BALL_SCENE_SCRIPT =
    "scene_ball.elapsed = 0.0\n"
    "scene_ball.update = (dt) ->\n"
    "  scene_ball.elapsed += dt\n"
    "  bx, by = scene_ball.getballpos!\n"
    "  if by > 10.0\n"
    "    scene_ball.setballpos 2, 0\n"
    "  if scene_ball.elapsed > 1.8\n"
    "    scene_ball.elapsed = 0.0\n"
    "    if math.random! > 0.33\n"
    "      random_x = math.random!\n"
    "      if bx > 2.0\n"
    "        random_x = -random_x\n"
    "      random_y = -math.random! * 0.3\n"
    "      ret = scene_ball.applyballimpulse random_x, random_y\n"
    "      if ret\n"
    "        print \"Applied impulse of \" .. random_x .. \", \" .. random_y\n"
    "      else\n"
    "        print \"Failed to apply impulse\"\n";

class ScriptEditScene : public Scene {
 public:
  ScriptEditScene(SceneSystem *ctx);
  virtual ~ScriptEditScene() override;

  virtual void update(SceneSystem *ctx, float dt) override;
  virtual void draw(SceneSystem *ctx) override;
  virtual void draw_rlimgui(SceneSystem *ctx) override;
  virtual bool allow_draw_below(SceneSystem *ctx) override;

  void reset(SceneSystem *ctx);

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
  // 0 - UNUSED
  // 1 - buffer fetched once this frame
  std::bitset<32> flags;
  ExecState exec_state;
  ExecState saveload_state;

  static std::optional<std::string> load_from_file(const char *filename);

  std::optional<lua_State *> get_lctx(SceneSystem *ctx) const;

  void reset_error_texts();
};

#endif

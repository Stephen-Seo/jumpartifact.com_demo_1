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
    "scene_2d.ball_elapsed = 0.0\n"
    "scene_2d.trapezoid_elapsed = -0.8\n"
    "scene_2d.key_pressed_callback = (key) ->\n"
    "  print \"Key \" .. key .. \" pressed\"\n"
    "  if key == 262\n"
    "    print \"trapezoid to right\"\n"
    "    scene_2d.applytrapezoidimpulse 0.24, 0.0\n"
    "  elseif key == 263\n"
    "    print \"trapezoid to left\"\n"
    "    scene_2d.applytrapezoidimpulse -0.24, 0.0\n"
    "  elseif key == 264\n"
    "    print \"trapezoid to down\"\n"
    "    scene_2d.applytrapezoidimpulse 0.0, 0.24\n"
    "  elseif key == 265\n"
    "    print \"trapezoid to up\"\n"
    "    scene_2d.applytrapezoidimpulse 0.0, -0.24\n"
    "  elseif key == 68\n"
    "    print \"ball to right\"\n"
    "    scene_2d.applyballimpulse 0.24, 0.0\n"
    "  elseif key == 65\n"
    "    print \"ball to left\"\n"
    "    scene_2d.applyballimpulse -0.24, 0.0\n"
    "  elseif key == 83\n"
    "    print \"ball to down\"\n"
    "    scene_2d.applyballimpulse 0.0, 0.24\n"
    "  elseif key == 87\n"
    "    print \"ball to up\"\n"
    "    scene_2d.applyballimpulse 0.0, -0.24\n"
    "scene_2d.update = (dt) ->\n"
    "  scene_2d.ball_elapsed += dt\n"
    "  scene_2d.trapezoid_elapsed += dt\n"
    "  bx, by = scene_2d.getballpos!\n"
    "  if by > 10.0\n"
    "    scene_2d.setballpos 2, 0\n"
    "  tx, ty = scene_2d.gettrapezoidpos!\n"
    "  if ty > 10.0\n"
    "    scene_2d.settrapezoidpos 2.4, 0\n"
    "  if scene_2d.ball_elapsed > 1.8\n"
    "    scene_2d.ball_elapsed = 0.0\n"
    "    if math.random! > 0.33\n"
    "      random_x = math.random!\n"
    "      if bx > 2.0\n"
    "        random_x = -random_x\n"
    "      random_y = -math.random! * 0.3\n"
    "      ret = scene_2d.applyballimpulse random_x, random_y\n"
    "      rx = math.floor(random_x * 100.0 + 0.5) / 100.0\n"
    "      ry = math.floor(random_y * 100.0 + 0.5) / 100.0\n"
    "      if ret\n"
    "        print \"Ball Impulse of \" .. rx .. \", \" .. ry\n"
    "      else\n"
    "        print \"Failed to apply impulse to Ball\"\n"
    "  if scene_2d.trapezoid_elapsed > 1.8\n"
    "    scene_2d.trapezoid_elapsed = 0.0\n"
    "    if math.random! > 0.33\n"
    "      random_x = math.random!\n"
    "      if tx > 2.0\n"
    "        random_x = -random_x\n"
    "      random_y = -math.random! * 0.2\n"
    "      ret = scene_2d.applytrapezoidimpulse random_x, random_y\n"
    "      tx = math.floor(random_x * 100.0 + 0.5) / 100.0\n"
    "      ty = math.floor(random_y * 100.0 + 0.5) / 100.0\n"
    "      if ret\n"
    "        print \"Trapezoid Impulse of \" .. tx .. \", \" .. ty\n"
    "      else\n"
    "        print \"Failed to apply impulse to Trapezoid!\"\n";

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

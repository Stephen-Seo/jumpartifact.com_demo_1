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
    "scene_2d.gamepad_pressed_callback = (btn) ->\n"
    "  print \"Button \" .. btn .. \" pressed\"\n"
    "scene_2d.gamepad_axis_callback = (axis, v) ->\n"
    "  if v > 0.15 or v < -0.15\n"
    "    print \"Axis \" .. axis .. \" value \" .. v\n"
    "scene_2d.get_ball = ->\n"
    "  for k, v in pairs scene_2d.balls\n"
    "    return k\n"
    "  return 0\n"
    "scene_2d.get_trapezoid = ->\n"
    "  for k, v in pairs scene_2d.trapezoids\n"
    "    return k\n"
    "  return 0\n"
    "scene_2d.get_octagon = ->\n"
    "  for k, v in pairs scene_2d.octagons\n"
    "    return k\n"
    "  return 0\n"
    "scene_2d.key_pressed_callback = (key) ->\n"
    "  print \"Key \" .. key .. \" pressed\"\n"
    "  if scene_2d.scene_init\n"
    "    if key == 262\n"
    "      print \"trapezoid to right\"\n"
    "      scene_2d.applytrapezoidimpulse scene_2d.get_trapezoid!, 0.24, 0.0\n"
    "    elseif key == 263\n"
    "      print \"trapezoid to left\"\n"
    "      scene_2d.applytrapezoidimpulse scene_2d.get_trapezoid!, -0.24, 0.0\n"
    "    elseif key == 264\n"
    "      print \"trapezoid to down\"\n"
    "      scene_2d.applytrapezoidimpulse scene_2d.get_trapezoid!, 0.0, 0.24\n"
    "    elseif key == 265\n"
    "      print \"trapezoid to up\"\n"
    "      scene_2d.applytrapezoidimpulse scene_2d.get_trapezoid!, 0.0, -0.24\n"
    "    elseif key == 68\n"
    "      print \"ball to right\"\n"
    "      scene_2d.applyballimpulse scene_2d.get_ball!, 0.24, 0.0\n"
    "    elseif key == 65\n"
    "      print \"ball to left\"\n"
    "      scene_2d.applyballimpulse scene_2d.get_ball!, -0.24, 0.0\n"
    "    elseif key == 83\n"
    "      print \"ball to down\"\n"
    "      scene_2d.applyballimpulse scene_2d.get_ball!, 0.0, 0.24\n"
    "    elseif key == 87\n"
    "      print \"ball to up\"\n"
    "      scene_2d.applyballimpulse scene_2d.get_ball!, 0.0, -0.24\n"
    "    elseif key == 73\n"
    "      print \"octagon to up\"\n"
    "      scene_2d.applyoctagonimpulse scene_2d.get_octagon!, 0.0, -0.24\n"
    "    elseif key == 74\n"
    "      print \"octagon to left\"\n"
    "      scene_2d.applyoctagonimpulse scene_2d.get_octagon!, -0.24, 0.0\n"
    "    elseif key == 75\n"
    "      print \"octagon to down\"\n"
    "      scene_2d.applyoctagonimpulse scene_2d.get_octagon!, 0.0, 0.24\n"
    "    elseif key == 76\n"
    "      print \"octagon to right\"\n"
    "      scene_2d.applyoctagonimpulse scene_2d.get_octagon!, 0.24, 0.0\n"
    "    elseif key == 91\n"
    "      for k, v in pairs scene_2d.octagons\n"
    "        scene_2d.destroyoctagon k\n"
    "        scene_2d.octagons[k] = nil\n"
    "        print \"Removed octagon \" .. k\n"
    "        break\n"
    "    elseif key == 93\n"
    "      new_o_id = scene_2d.createoctagon!\n"
    "      scene_2d.octagons[new_o_id] = {\n"
    "        elapsed: -math.random! * 2.8\n"
    "      }\n"
    "    elseif key == 57\n"
    "      for k, v in pairs scene_2d.trapezoids\n"
    "        scene_2d.destroytrapezoid k\n"
    "        scene_2d.trapezoids[k] = nil\n"
    "        print \"Removed trapezoid \" .. k\n"
    "        break\n"
    "    elseif key == 48\n"
    "      new_t_id = scene_2d.createtrapezoid!\n"
    "      scene_2d.trapezoids[new_t_id] = {\n"
    "        elapsed: -math.random! * 2.8\n"
    "      }\n"
    "      print \"Created trapezoid \" .. new_t_id\n"
    "    elseif key == 44\n"
    "      for k, v in pairs scene_2d.balls\n"
    "        scene_2d.destroyball k\n"
    "        scene_2d.balls[k] = nil\n"
    "        print \"Removed ball \" .. k\n"
    "        break\n"
    "    elseif key == 46\n"
    "      new_b_id = scene_2d.createball!\n"
    "      scene_2d.balls[new_b_id] = {\n"
    "        elapsed: -math.random! * 2.8\n"
    "      }\n"
    "      print \"Created ball \" .. new_b_id\n"
    "scene_2d.init = ->\n"
    "  scene_2d.scene_init = true\n"
    "  scene_2d.balls = {}\n"
    "  scene_2d.trapezoids = {}\n"
    "  scene_2d.octagons = {}\n"
    "  elapsed_temp = 0.0\n"
    "  for i = 1, 2\n"
    "    scene_2d.balls[scene_2d.createball!] = {\n"
    "      elapsed: elapsed_temp\n"
    "    }\n"
    "    elapsed_temp -= 0.7\n"
    "  for k, v in pairs scene_2d.balls\n"
    "    scene_2d.setballcolor k, 255, 90, 90\n"
    "    break\n"
    "  for i = 1, 2\n"
    "    scene_2d.trapezoids[scene_2d.createtrapezoid!] = {\n"
    "      elapsed: elapsed_temp\n"
    "    }\n"
    "    elapsed_temp -= 0.7\n"
    "  for k, v in pairs scene_2d.trapezoids\n"
    "    scene_2d.settrapezoidcolor k, 90, 255, 90\n"
    "    break\n"
    "  for i = 1, 2\n"
    "    scene_2d.octagons[scene_2d.createoctagon!] = {\n"
    "      elapsed: elapsed_temp\n"
    "    }\n"
    "    elapsed_temp -= 0.7\n"
    "  for k, v in pairs scene_2d.octagons\n"
    "    scene_2d.setoctagoncolor k, 90, 90, 255\n"
    "    break\n"
    "scene_2d.update = (dt) ->\n"
    "  for k, v in pairs scene_2d.balls\n"
    "    v.elapsed += dt\n"
    "    bx, by = scene_2d.getballpos k\n"
    "    if by > 10.0\n"
    "      scene_2d.setballpos k, 1.7, 0\n"
    "    if v.elapsed > 2.8\n"
    "      v.elapsed -= 2.0 + math.random!\n"
    "      if math.random! > 0.33\n"
    "        random_x = math.random!\n"
    "        if bx > 2.0\n"
    "          random_x = -random_x\n"
    "        random_y = -math.random! * 0.3\n"
    "        scene_2d.applyballimpulse k, random_x, random_y\n"
    "        rx = math.floor(random_x * 100.0 + 0.5) / 100.0\n"
    "        ry = math.floor(random_y * 100.0 + 0.5) / 100.0\n"
    "        print \"Ball \" .. k .. \" impulse of \" .. rx .. "
    "\", \" .. ry\n"
    "  for k, v in pairs scene_2d.trapezoids\n"
    "    v.elapsed += dt\n"
    "    tx, ty = scene_2d.gettrapezoidpos k\n"
    "    if ty > 10.0\n"
    "      scene_2d.settrapezoidpos k, 1.7, 0\n"
    "    if v.elapsed > 2.8\n"
    "      v.elapsed -= 2.0 + math.random!\n"
    "      if math.random! > 0.33\n"
    "        random_x = math.random!\n"
    "        if tx > 2.0\n"
    "          random_x = -random_x\n"
    "        random_y = -math.random! * 0.3\n"
    "        scene_2d.applytrapezoidimpulse k, random_x, random_y\n"
    "        rx = math.floor(random_x * 100.0 + 0.5) / 100.0\n"
    "        ry = math.floor(random_y * 100.0 + 0.5) / 100.0\n"
    "        print \"Trapezoid \" .. k .. \" impulse of \" .. rx .. "
    "\", \" .. ry\n"
    "  for k, v in pairs scene_2d.octagons\n"
    "    v.elapsed += dt\n"
    "    bx, by = scene_2d.getoctagonpos k\n"
    "    if by > 10.0\n"
    "      scene_2d.setoctagonpos k, 1.7, 0\n"
    "    if v.elapsed > 2.8\n"
    "      v.elapsed -= 2.0 + math.random!\n"
    "      if math.random! > 0.33\n"
    "        random_x = math.random!\n"
    "        if bx > 2.0\n"
    "          random_x = -random_x\n"
    "        random_y = -math.random! * 0.3\n"
    "        scene_2d.applyoctagonimpulse k, random_x, random_y\n"
    "        rx = math.floor(random_x * 100.0 + 0.5) / 100.0\n"
    "        ry = math.floor(random_y * 100.0 + 0.5) / 100.0\n"
    "        print \"Octagon \" .. k .. \" impulse of \" .. rx .. "
    "\", \" .. ry\n";

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

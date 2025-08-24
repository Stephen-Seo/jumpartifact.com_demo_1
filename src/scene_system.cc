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

#include "scene_system.h"

// third party includes
extern "C" {
#include <lauxlib.h>
#include <lualib.h>
}
#include <lpeg_exported.h>
#ifndef NDEBUG
#include <emscripten/console.h>
#endif
#include <emscripten/html5.h>
#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>

// standard library includes
#include <fstream>
#include <print>

// local includes
#include "2d_world_scene.h"
#include "script_edit_scene.h"

Scene::Scene(SceneSystem *) {}
Scene::~Scene() {}

std::optional<uint32_t> Scene::get_scene_id(SceneSystem *ctx) {
  return ctx->get_scene_id(this);
}

SceneSystem::SceneSystem()
    : scene_stack(),
      dt{1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.0F},
      dt_idx(0),
      flags(),
      private_flags(),
      scene_type_counter(0) {
  init_lua();
}

SceneSystem::~SceneSystem() {}

void SceneSystem::update() {
  auto next_time_point = std::chrono::steady_clock::now();
  float delta_time =
      (float)std::chrono::duration_cast<std::chrono::microseconds>(
          next_time_point - this->time_point)
          .count() /
      1000000.0F;
  this->time_point = std::move(next_time_point);

  dt[dt_idx++] = delta_time;
  if (dt_idx >= dt.size()) {
    dt_idx = 0;
  }

  handle_actions();

  for (auto iter = scene_stack.rbegin(); iter != scene_stack.rend(); ++iter) {
    (*iter)->update(this, delta_time);
  }
}

void SceneSystem::draw() {
  size_t disallow_below_idx = 0;
  for (size_t idx = 0; idx < scene_stack.size(); ++idx) {
    if (!scene_stack[idx]->allow_draw_below(this)) {
      disallow_below_idx = idx;
    }
  }

  for (size_t idx = scene_stack.size(); idx-- > disallow_below_idx;) {
    scene_stack[idx]->draw(this);
  }

  rlImGuiBegin();

  // Font size doubling.
  if (!private_flags.test(1)) {
    ImGui::PushFont(NULL, ImGui::GetFontSize() * 2.0F);
  }

  for (size_t idx = scene_stack.size(); idx-- > disallow_below_idx;) {
    scene_stack[idx]->draw_rlimgui(this);
  }

  if (private_flags.test(3)) {
    ImGui::ShowDemoWindow();
  }

  if (!private_flags.test(4)) {
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImVec2 new_size{viewport->Size.x / 2.0F, viewport->Size.y / 2.0F};
    ImGui::SetNextWindowSize(new_size);
    private_flags.set(4);
  }

  ImGui::Begin("Config Window");
  ImGui::BeginTabBar("TabBar", ImGuiTabBarFlags_None);
  if (ImGui::BeginTabItem("2DSimulation")) {
    std::optional<uint32_t> top_id = get_top_scene_id();
    if (!top_id.has_value() ||
        top_id != get_scene_id_by_template<TwoDimWorldScene>()) {
      clear_scenes();
      push_scene([](SceneSystem *ctx) {
        return std::make_unique<TwoDimWorldScene>(ctx);
      });
      std::println(stdout, "Pushed 2DWorldScene.");
    }

    ImGui::TextWrapped("scene_2d is a global table in 2DSimulation.");
    ImGui::TextWrapped(
        "Create an update function \"scene_2d.update\" that accepts "
        "one number parameter delta-time.");
    ImGui::TextWrapped(
        "Create a callback function \"scene_2d.key_pressed_callback\" that "
        "accepts one integer parameter (the key that was pressed).");
    ImGui::TextWrapped("\nAvailable functions:");
    ImGui::TextWrapped("  scene_2d.getballpos() -> number, number");
    ImGui::TextWrapped("  scene_2d.setballpos(number, number) -> boolean");
    ImGui::TextWrapped("  scene_2d.getballvel() -> number, number");
    ImGui::TextWrapped(
        "  scene_2d.applyballimpulse(number, number) -> boolean");
    ImGui::TextWrapped("  scene_2d.gettrapezoidpos() -> number, number");
    ImGui::TextWrapped("  scene_2d.settrapezoidpos(number, number) -> boolean");
    ImGui::TextWrapped("  scene_2d.gettrapezoidvel() -> number, number");
    ImGui::TextWrapped(
        "  scene_2d.applytrapezoidimpulse(number, number) -> boolean");

    ImGui::EndTabItem();
  }
  if (ImGui::BeginTabItem("Settings")) {
    std::optional<uint32_t> top_id = get_top_scene_id();
    if (top_id.has_value()) {
      clear_scenes();
      std::println(stdout, "Cleared Scenes.");
    }

    bool is_fullscreen = flags.test(0);
    ImGui::Checkbox("Fullscreen Enabled", &is_fullscreen);
    if (is_fullscreen != flags.test(0)) {
      if (is_fullscreen) {
        if (emscripten_request_fullscreen("canvas", true) !=
            EMSCRIPTEN_RESULT_SUCCESS) {
          is_fullscreen = false;
        }
      } else {
        if (emscripten_exit_fullscreen() != EMSCRIPTEN_RESULT_SUCCESS) {
          is_fullscreen = true;
        }
      }
    }
    flags.set(0, is_fullscreen);

    bool is_font_big = !private_flags.test(1);
    ImGui::Checkbox("Font Size Big", &is_font_big);
    if (is_font_big == private_flags.test(1)) {
      private_flags.set(2);
    }

    bool is_demo_window_open = private_flags.test(3);
    ImGui::Checkbox("Demo Window Active", &is_demo_window_open);
    if (is_demo_window_open != private_flags.test(3)) {
      private_flags.flip(3);
    }

    float avg = 0.0F;
    for (int idx = 0; idx < dt.size(); ++idx) {
      avg += dt[idx];
    }
    avg /= static_cast<float>(dt.size());
    ImGui::Text("Current FPS is: %0.1f", 1.0F / avg);

    ImGui::EndTabItem();
  }
  if (ImGui::BeginTabItem("ScriptEditor")) {
    std::optional<uint32_t> top_id = get_top_scene_id();
    if (!top_id.has_value() ||
        top_id != get_scene_id_by_template<ScriptEditScene>()) {
      clear_scenes();
      push_scene([](SceneSystem *ctx) {
        return std::make_unique<ScriptEditScene>(ctx);
      });
      std::println(stdout, "Pushed ScriptEditScene.");
    }
    ImGui::EndTabItem();
  }
  ImGui::EndTabBar();
  ImGui::End();

  // Font size doubling cleanup.
  if (!private_flags.test(1)) {
    ImGui::PopFont();
  }

  rlImGuiEnd();

  // Toggle doubling of font size.
  if (private_flags.test(2)) {
    private_flags.reset(2);
    private_flags.flip(1);
  }
}

void SceneSystem::clear_scenes() {
  queued_actions.emplace_back(ActionType::CLEAR, std::nullopt);
}

void SceneSystem::push_scene(SceneFnType scene_builder) {
  queued_actions.emplace_back(ActionType::PUSH, scene_builder);
}

void SceneSystem::pop_scene() {
  queued_actions.emplace_back(ActionType::POP, std::nullopt);
  private_flags.set(0);
}

bool SceneSystem::pop_was_queued() const { return private_flags.test(0); }

SceneSystem::FlagsType &SceneSystem::get_flags() { return flags; }

const SceneSystem::FlagsType &SceneSystem::get_flags() const { return flags; }

const std::deque<SceneSystem::SceneType> *SceneSystem::get_scene_stack() const {
  return &scene_stack;
}

std::optional<SceneSystem::SceneType *> SceneSystem::get_top() {
  if (scene_stack.empty()) {
    return std::nullopt;
  }
  return &scene_stack.back();
}

bool SceneSystem::set_map_value(std::string name, void *value,
                                std::function<void(void *)> cleanup_fn) {
  if (auto iter = generic_map.find(name); iter != generic_map.end()) {
    return false;
  }
  generic_map.insert(std::make_pair(name, std::make_pair(value, cleanup_fn)));
  return true;
}

std::optional<void *> SceneSystem::get_map_value(std::string name) {
  if (auto iter = generic_map.find(name); iter != generic_map.end()) {
    return iter->second.first;
  }
  return std::nullopt;
}

bool SceneSystem::clear_map_value(
    std::string name,
    std::optional<std::function<void(void *)> > override_cleanup_fn) {
  if (auto iter = generic_map.find(name); iter != generic_map.end()) {
    if (override_cleanup_fn.has_value()) {
      override_cleanup_fn.value()(iter->second.first);
    } else {
      iter->second.second(iter->second.first);
    }
    generic_map.erase(iter);
    return true;
  }
  return false;
}

std::optional<uint32_t> SceneSystem::get_scene_id(Scene *scene) {
  if (scene != nullptr) {
    if (auto iter = scene_type_map.find(typeid(*scene).name());
        iter != scene_type_map.end()) {
      return iter->second;
    }
  } else {
    return std::nullopt;
  }

  scene_type_map.insert(
      std::make_pair(typeid(scene).name(), scene_type_counter++));
  return scene_type_counter - 1;
}

std::optional<uint32_t> SceneSystem::get_top_scene_id() {
  if (scene_stack.empty()) {
    return std::nullopt;
  }

  return get_scene_id(scene_stack.back().get());
}

void SceneSystem::init_lua() {
  if (get_map_value("lua_state").has_value()) {
    return;
  }

  lua_State *lua_ctx = luaL_newstate();
  set_map_value("lua_state", lua_ctx, [](void *ud) {
    lua_State *lctx = reinterpret_cast<lua_State *>(ud);
    lua_close(lctx);
  });

  EM_ASM(FS.mkdir('/preloaded'););

  luaL_requiref(lua_ctx, LUA_GNAME, luaopen_base, 1);           // +1
  lua_pop(lua_ctx, 1);                                          // -1
  luaL_requiref(lua_ctx, LUA_LOADLIBNAME, luaopen_package, 1);  // +1
  lua_pop(lua_ctx, 1);                                          // -1
  luaL_requiref(lua_ctx, LUA_COLIBNAME, luaopen_coroutine, 1);  // +1
  lua_pop(lua_ctx, 1);                                          // -1
  luaL_requiref(lua_ctx, LUA_STRLIBNAME, luaopen_string, 1);    // +1
  lua_pop(lua_ctx, 1);                                          // -1
  luaL_requiref(lua_ctx, LUA_UTF8LIBNAME, luaopen_utf8, 1);     // +1
  lua_pop(lua_ctx, 1);                                          // -1
  luaL_requiref(lua_ctx, LUA_TABLIBNAME, luaopen_table, 1);     // +1
  lua_pop(lua_ctx, 1);                                          // -1
  luaL_requiref(lua_ctx, LUA_MATHLIBNAME, luaopen_math, 1);     // +1
  lua_pop(lua_ctx, 1);                                          // -1
  luaL_requiref(lua_ctx, LUA_IOLIBNAME, luaopen_io, 1);         // +1
  lua_pop(lua_ctx, 1);                                          // -1
  luaL_requiref(lua_ctx, LUA_OSLIBNAME, luaopen_os, 1);         // +1
  lua_pop(lua_ctx, 1);                                          // -1
  luaL_requiref(lua_ctx, LUA_DBLIBNAME, luaopen_debug, 1);      // +1
  lua_pop(lua_ctx, 1);                                          // -1

  // Set "package.path"
  lua_getglobal(lua_ctx, "package");  // +1
  lua_pushstring(lua_ctx, "path");    // +1
  lua_pushstring(lua_ctx,
                 "/preloaded/?/init.lua;/preloaded/?.lua;"
                 "/assets_embed/?/init.lua;/assets_embed/?.lua;"
                 "/?/init.lua;/?.lua");  // +1
  lua_settable(lua_ctx, -3);             // -2
  lua_pop(lua_ctx, 1);                   // -1

  lua_pushcfunction(lua_ctx, luaopen_lpeg);       // +1
  lua_setglobal(lua_ctx, "luaopen_lpeg_global");  // -1

  std::ofstream lua_lpeg_of("/preloaded/lpeg.lua",
                            std::ios_base::out | std::ios_base::trunc);
  lua_lpeg_of << "return luaopen_lpeg_global()";
  lua_lpeg_of.close();

  lua_newtable(lua_ctx);               // +1
  lua_setglobal(lua_ctx, "scene_2d");  // -1

  int ret = luaL_dostring(lua_ctx, "require('moonscript')");
  if (ret == 1) {
    lua_pop(lua_ctx, 1);
    flags.reset(1);
  } else {
    flags.set(1);

    // Load Default script.
    const auto temp_fn_load_default_script = [](lua_State *lctx) -> int {
      lua_pushstring(lctx, DEFAULT_BALL_SCENE_SCRIPT);  // +1
      return 1;
    };
    lua_pushcfunction(lua_ctx, temp_fn_load_default_script);  // +1
    lua_setglobal(lua_ctx, "temp_fn_load_default_global");    // -1
    int ret = luaL_dostring(
        lua_ctx,
        "local moonscript = require('moonscript.base')\n"
        "local moon_string = temp_fn_load_default_global()\n"
        "local to_call, error_obj = moonscript.loadstring(moon_string)\n"
        "if error_obj ~= nil then\n"
        "print(error_obj)\n"
        "error(error_obj)\n"
        "else\n"
        "to_call()\n"
        "end\n");
    if (ret == 1) {
      lua_pop(lua_ctx, 1);
    } else {
      push_scene([](SceneSystem *ctx) {
        return std::make_unique<TwoDimWorldScene>(ctx);
      });
    }
    lua_pushnil(lua_ctx);                                   // +1
    lua_setglobal(lua_ctx, "temp_fn_load_default_global");  // -1
  }
}

void SceneSystem::handle_actions() {
  while (!queued_actions.empty()) {
    switch (queued_actions.front().type) {
      case ActionType::CLEAR:
        scene_stack.clear();
        break;
      case ActionType::PUSH:
        if (queued_actions.front().scene_builder) {
          scene_stack.push_back(
              queued_actions.front().scene_builder.value()(this));
        }
        break;
      case ActionType::POP:
        if (!scene_stack.empty()) {
          scene_stack.pop_back();
        }
        break;
    }
    queued_actions.pop_front();
  }
  private_flags.reset(0);
}

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

#include "test_lua_scene.h"

// third party includes
#include <imgui.h>
extern "C" {
#include <lauxlib.h>
#include <lualib.h>
}
#include <emscripten.h>
#include <lpeg_exported.h>
#include <rlImGui.h>

// standard library includes
#include <cstdio>
#include <cstring>
#include <format>
#include <fstream>

extern "C" {
int internal_lua_load_buffer(lua_State *ctx) {
  TestLuaScene *scene = reinterpret_cast<TestLuaScene *>(
      lua_touserdata(ctx, lua_upvalueindex(1)));

  std::optional<const char *> buf_ptr = scene->get_buffer_once();

  if (buf_ptr.has_value()) {
    lua_pushstring(ctx, buf_ptr.value());  // +1
  } else {
    lua_pushnil(ctx);  // +1
  }

  return 1;
}
}  // extern "C"

TestLuaScene::TestLuaScene(SceneSystem *ctx)
    : Scene(ctx),
      buf{},
      error_text(),
      lua_ctx(luaL_newstate()),
      exec_state(ExecState::PENDING),
      saveload_state(ExecState::PENDING) {
  std::strcpy(buf.data(), LUA_DEFAULT_TEXT);
  std::strcpy(filename.data(), "/test.lua");

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

  // Put "assets_embed/?/init.lua" to "package.path"
  lua_getglobal(lua_ctx, "package");  // +1
  lua_pushstring(lua_ctx, "path");    // +1
  lua_pushstring(
      lua_ctx,
      "/?/init.lua;/?.lua;assets_embed/?/init.lua;assets_embed/?.lua");  // +1
  lua_settable(lua_ctx, -3);                                             // -2
  lua_pop(lua_ctx, 1);                                                   // -1

  //// Put "assets_embed/?.so" to "package.cpath"
  // lua_getglobal(lua_ctx, "package");             // +1
  // lua_pushstring(lua_ctx, "cpath");              // +1
  // lua_pushstring(lua_ctx, "assets_embed/?.so");  // +1
  // lua_settable(lua_ctx, -3);                     // -2
  // lua_pop(lua_ctx, 1);                           // -1

  lua_pushcfunction(lua_ctx, luaopen_lpeg);       // +1
  lua_setglobal(lua_ctx, "luaopen_lpeg_global");  // -1

  std::ofstream lua_lpeg_of("/lpeg.lua",
                            std::ios_base::out | std::ios_base::trunc);
  lua_lpeg_of << LUA_LPEG_LOAD_SCRIPT;
  lua_lpeg_of.close();

  int ret = luaL_dostring(lua_ctx, "require('moonscript')");
  if (ret == 1) {
    lua_pop(lua_ctx, 1);
    flags.reset(0);
  } else {
    flags.set(0);
    size_t idx = std::strlen(buf.data());
    sprintf(buf.data() + idx, "%s\n", MOONSCRIPT_HELP_TEXT);
  }
}

TestLuaScene::~TestLuaScene() { lua_close(lua_ctx); }

void TestLuaScene::update(SceneSystem *ctx, float dt) { flags.reset(1); }

void TestLuaScene::draw(SceneSystem *ctx) {}

void TestLuaScene::draw_rlimgui(SceneSystem *ctx) {
  const ImGuiViewport *viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->Pos);
  ImGui::SetNextWindowSize(viewport->Size);

  ImGui::Begin("Test Lua");

  ImGui::InputTextMultiline("Lua Code", buf.data(), TEXT_BUF_SIZE);
  if (ImGui::Button("ExecuteAsLua")) {
    int ret = luaL_dostring(lua_ctx, buf.data());
    if (ret == 1) {
      exec_state = ExecState::FAILURE;
      if (lua_isstring(lua_ctx, -1) == 1) {
        error_text = lua_tostring(lua_ctx, -1);
      } else {
        error_text = "Error object not a string!";
      }
      lua_pop(lua_ctx, 1);
    } else {
      exec_state = ExecState::SUCCESS;
    }
    saveload_state = ExecState::PENDING;
    save_error_text_err.clear();
  }
  ImGui::SameLine();
  if (ImGui::Button("ExecuteAsMoonscript")) {
    lua_pushlightuserdata(lua_ctx, this);                          // +1
    lua_pushcclosure(lua_ctx, internal_lua_load_buffer, 1);        // -1, +1
    lua_setglobal(lua_ctx, "EXECUTE_AS_MOONSCRIPT_FETCH_BUF_FN");  // -1

    std::string exec_str = std::format(
        "moonscript = require('moonscript.base')\n"
        "moon_string = EXECUTE_AS_MOONSCRIPT_FETCH_BUF_FN()\n"
        "to_call = moonscript.loadstring(moon_string)\n"
        "to_call()");

    int ret = luaL_dostring(lua_ctx, exec_str.c_str());
    if (ret == 1) {
      exec_state = ExecState::FAILURE;
      if (lua_isstring(lua_ctx, -1) == 1) {
        error_text = lua_tostring(lua_ctx, -1);
      } else {
        error_text = "Error object not a string!";
      }
      lua_pop(lua_ctx, 1);
    } else {
      exec_state = ExecState::SUCCESS;
    }
    saveload_state = ExecState::PENDING;
    save_error_text_err.clear();
  }
  ImGui::SameLine();
  if (ImGui::Button("Reset")) {
    reset();
  }
  switch (exec_state) {
    case ExecState::PENDING:
      break;
    case ExecState::SUCCESS:
      ImGui::Text("Script run Success!");
      break;
    case ExecState::FAILURE:
      ImGui::Text("Script run failure! %s", error_text.c_str());
      break;
    case ExecState::UPDATED:
      break;
  }
  ImGui::InputText("Filename (tmp storage)", filename.data(), filename.size());
  if (ImGui::Button("Save")) {
    std::ofstream ofs = std::ofstream(
        filename.data(), std::ios_base::out | std::ios_base::trunc);
    ofs.write(buf.data(), std::strlen(buf.data()));
    if (ofs.good()) {
      saveload_state = ExecState::SUCCESS;
    } else {
      saveload_state = ExecState::FAILURE;
    }
    exec_state = ExecState::PENDING;
    save_error_text_err.clear();
    flags.set(2);
  }
  ImGui::SameLine();
  if (ImGui::Button("Load")) {
    exec_state = ExecState::PENDING;
    saveload_state = ExecState::SUCCESS;
    std::ifstream ifs = std::ifstream(filename.data());
    std::string file_content{};
    size_t idx = 0;
    if (!ifs.good()) {
      saveload_state = ExecState::FAILURE;
      std::strcpy(buf.data(), "Failed to load.");
    }
    while (ifs.good()) {
      ifs.getline(buf.data() + idx, buf.size() - idx);
      idx += ifs.gcount();
      if (idx + 2 >= buf.size()) {
        saveload_state = ExecState::FAILURE;
        std::strcpy(buf.data(), "Failed to load.");
        break;
      }
      buf.data()[idx - 1] = '\n';
      if (ifs.eof()) {
        buf.data()[idx] = 0;
      }
    }
    ifs.close();
    save_error_text_err.clear();
    flags.reset(2);
  }
  ImGui::SameLine();
  if (ImGui::Button("ExecLuaFile")) {
    exec_state = ExecState::PENDING;
    int ret = luaL_dofile(lua_ctx, filename.data());
    if (ret == 1) {
      saveload_state = ExecState::FAILURE;
      if (lua_isstring(lua_ctx, -1) == 1) {
        save_error_text_err = lua_tostring(lua_ctx, -1);
      } else {
        save_error_text_err = "Error object not a string!";
      }
      lua_pop(lua_ctx, 1);
    } else {
      saveload_state = ExecState::SUCCESS;
      save_error_text_err.clear();
    }
  }
  ImGui::SameLine();
  if (ImGui::Button("ExecMoonscriptFile")) {
    exec_state = ExecState::PENDING;
    std::string exec_str = std::format(
        "moonscript = require('moonscript.base')\n"
        "to_call = moonscript.loadfile('{}')\n"
        "to_call()",
        filename.data());
    int ret = luaL_dostring(lua_ctx, exec_str.c_str());
    if (ret == 1) {
      saveload_state = ExecState::FAILURE;
      if (lua_isstring(lua_ctx, -1) == 1) {
        save_error_text_err = lua_tostring(lua_ctx, -1);
      } else {
        save_error_text_err = "Error object not a string!";
      }
      lua_pop(lua_ctx, 1);
    } else {
      saveload_state = ExecState::SUCCESS;
      save_error_text_err.clear();
    }
  }
  switch (saveload_state) {
    case ExecState::PENDING:
      save_error_text.clear();
      saveload_state = ExecState::UPDATED;
      break;
    case ExecState::SUCCESS:
      save_error_text = std::format(
          "{} '{}' Success!", flags.test(2) ? "Saving to" : "Loading from",
          filename.data());
      saveload_state = ExecState::UPDATED;
      break;
    case ExecState::FAILURE:
      save_error_text = std::format(
          "{} '{}' Failure!", flags.test(2) ? "Saving to" : "Loading from",
          filename.data());
      saveload_state = ExecState::UPDATED;
      break;
    case ExecState::UPDATED:
      // intentionally left blank
      break;
  }
  ImGui::Text("%s\n%s", save_error_text.c_str(), save_error_text_err.c_str());
  ImGui::End();  // Test Lua
}

bool TestLuaScene::allow_draw_below(SceneSystem *ctx) { return true; }

void TestLuaScene::reset() {
  exec_state = ExecState::PENDING;
  saveload_state = ExecState::PENDING;
  error_text.clear();
  save_error_text.clear();
  save_error_text_err.clear();
  std::strcpy(buf.data(), LUA_DEFAULT_TEXT);
  if (flags.test(0)) {
    size_t idx = std::strlen(buf.data());
    sprintf(buf.data() + idx, "%s\n", MOONSCRIPT_HELP_TEXT);
  }
}

std::optional<const char *> TestLuaScene::get_buffer_once() {
  if (flags.test(1)) {
    return std::nullopt;
  }

  flags.set(1);
  return buf.data();
}

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
#include <rlImGui.h>

// standard library includes
#include <cstdio>
#include <cstring>

TestLuaScene::TestLuaScene(SceneSystem *ctx)
    : Scene(ctx),
      buf{},
      error_text(),
      lua_ctx(luaL_newstate()),
      exec_state(ExecState::PENDING) {
  std::strcpy(buf.data(), LUA_DEFAULT_TEXT);

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

  // Put "assets_embed/?/init.lua" to "package.path"
  lua_getglobal(lua_ctx, "package");                                      // +1
  lua_pushstring(lua_ctx, "path");                                        // +1
  lua_pushstring(lua_ctx, "/?/init.lua;/?.lua;assets_embed/?/init.lua;assets_embed/?.lua");  // +1
  lua_settable(lua_ctx, -3);                                              // -2
  lua_pop(lua_ctx, 1);                                                    // -1

  //// Put "assets_embed/?.so" to "package.cpath"
  //lua_getglobal(lua_ctx, "package");             // +1
  //lua_pushstring(lua_ctx, "cpath");              // +1
  //lua_pushstring(lua_ctx, "assets_embed/?.so");  // +1
  //lua_settable(lua_ctx, -3);                     // -2
  //lua_pop(lua_ctx, 1);                           // -1
}

TestLuaScene::~TestLuaScene() { lua_close(lua_ctx); }

void TestLuaScene::update(SceneSystem *ctx, float dt) {}

void TestLuaScene::draw(SceneSystem *ctx) {}

void TestLuaScene::draw_rlimgui(SceneSystem *ctx) {
  const ImGuiViewport *viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->Pos);
  ImGui::SetNextWindowSize(viewport->Size);

  ImGui::Begin("Test Lua");
  ImGui::InputTextMultiline("Lua Code", buf.data(), TEXT_BUF_SIZE);
  if (ImGui::Button("Execute")) {
    int ret = luaL_dostring(lua_ctx, buf.data());
    if (ret == 1) {
      exec_state = ExecState::FAILURE;
      if (lua_isstring(lua_ctx, -1) == 1) {
        error_text = std::string(lua_tostring(lua_ctx, -1));
      } else {
        error_text = std::string("Error object not a string!");
      }
      lua_pop(lua_ctx, 1);
    } else {
      exec_state = ExecState::SUCCESS;
    }
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
  }
  ImGui::End();  // Test Lua
}

bool TestLuaScene::allow_draw_below(SceneSystem *ctx) { return true; }

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
      exec_state = ExecState::GENERIC_FAILURE;
      if (lua_isstring(lua_ctx, -1) == 1) {
        error_text = lua_tostring(lua_ctx, -1);
      } else {
        error_text = "Error object not a string!";
      }
      lua_pop(lua_ctx, 1);
    } else {
      exec_state = ExecState::GENERIC_SUCCESS;
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
      exec_state = ExecState::GENERIC_FAILURE;
      if (lua_isstring(lua_ctx, -1) == 1) {
        error_text = lua_tostring(lua_ctx, -1);
      } else {
        error_text = "Error object not a string!";
      }
      lua_pop(lua_ctx, 1);
    } else {
      exec_state = ExecState::GENERIC_SUCCESS;
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
    case ExecState::GENERIC_SUCCESS:
      ImGui::Text("Script run Success!");
      break;
    case ExecState::GENERIC_FAILURE:
      ImGui::Text("Script run failure! %s", error_text.c_str());
      break;
    default:
      // Intentionally left blank
      break;
  }
  ImGui::InputText("Filename (tmp storage)", filename.data(), filename.size());
  if (ImGui::Button("Save")) {
    std::ofstream ofs = std::ofstream(
        filename.data(), std::ios_base::out | std::ios_base::trunc);
    if (ofs.good()) {
      ofs.write(buf.data(), std::strlen(buf.data()));
      if (ofs.good()) {
        saveload_state = ExecState::SAVE_SUCCESS;
      } else {
        saveload_state = ExecState::SAVE_FAILURE;
      }
    } else {
      saveload_state = ExecState::SAVE_FAILURE;
    }
    exec_state = ExecState::PENDING;
    save_error_text_err.clear();
  }
  ImGui::SameLine();
  if (ImGui::Button("Load")) {
    exec_state = ExecState::PENDING;
    saveload_state = ExecState::LOAD_SUCCESS;

    std::optional<std::string> res = load_from_file(filename.data());
    if (res.has_value()) {
      std::strcpy(buf.data(), res.value().c_str());
    } else {
      saveload_state = ExecState::LOAD_FAILURE;
      std::strcpy(buf.data(), "Failed to load.");
    }

    save_error_text_err.clear();
  }
  ImGui::SameLine();
  if (ImGui::Button("ExecLuaFile")) {
    exec_state = ExecState::PENDING;
    int ret = luaL_dofile(lua_ctx, filename.data());
    if (ret == 1) {
      saveload_state = ExecState::GENERIC_FAILURE;
      if (lua_isstring(lua_ctx, -1) == 1) {
        save_error_text_err = lua_tostring(lua_ctx, -1);
      } else {
        save_error_text_err = "Error object not a string!";
      }
      lua_pop(lua_ctx, 1);
    } else {
      saveload_state = ExecState::GENERIC_SUCCESS;
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
      saveload_state = ExecState::GENERIC_FAILURE;
      if (lua_isstring(lua_ctx, -1) == 1) {
        save_error_text_err = lua_tostring(lua_ctx, -1);
      } else {
        save_error_text_err = "Error object not a string!";
      }
      lua_pop(lua_ctx, 1);
    } else {
      saveload_state = ExecState::GENERIC_SUCCESS;
      save_error_text_err.clear();
    }
  }
  if (ImGui::Button("Download File")) {
    std::optional<std::string> loaded = load_from_file(filename.data());
    if (loaded.has_value()) {
      EM_ASM(const string_content = UTF8ToString($0);
             const string_filename = UTF8ToString($1);
             const blob = new Blob([string_content],
                                   {
                                     type:
                                       'text/plain'
                                   });
             const url = URL.createObjectURL(blob);
             const link = document.createElement('a'); link.href = url;
             link.download = string_filename; document.body.appendChild(link);
             link.click(); document.body.removeChild(link);
             URL.revokeObjectURL(url);
             , loaded.value().c_str(), filename.data());
      saveload_state = ExecState::DL_SUCCESS;
    } else {
      saveload_state = ExecState::DL_FAILURE;
    }
  }
  ImGui::SameLine();
  if (ImGui::Button("Upload File")) {
    EM_ASM(const file_input = document.createElement('input');
           file_input.type = 'file'; file_input.accept = '.txt,.lua,.moon';

           file_input.addEventListener(
               'change',
               function(event) {
                 const file = event.target.files[0];
                 if (file) {
                   const reader = new FileReader();
                   reader.onload = (e) => {
                     const file_content = e.target.result;
                     Module.ccall('upload_script_to_test_lua', 'number',
                                  [ 'number', 'number' ],
                                  [ stringToNewUTF8(file_content), $0 ]);
                   };
                   reader.readAsText(file);
                 }
               });

           file_input.click();, this);
  }
  switch (saveload_state) {
    case ExecState::PENDING:
      save_error_text.clear();
      saveload_state = ExecState::UPDATED;
      break;
    case ExecState::GENERIC_SUCCESS:
      // Intentionally left blank
      break;
    case ExecState::GENERIC_FAILURE:
      // Intentionally left blank
      break;
    case ExecState::SAVE_SUCCESS:
      save_error_text = std::format("Saving to '{}' Success!", filename.data());
      saveload_state = ExecState::UPDATED;
      break;
    case ExecState::SAVE_FAILURE:
      save_error_text = std::format("Saving to '{}' Failure!", filename.data());
      saveload_state = ExecState::UPDATED;
      break;
    case ExecState::LOAD_SUCCESS:
      save_error_text =
          std::format("Loading from '{}' Success!", filename.data());
      saveload_state = ExecState::UPDATED;
      break;
    case ExecState::LOAD_FAILURE:
      save_error_text =
          std::format("Loading from '{}' Failure!", filename.data());
      saveload_state = ExecState::UPDATED;
      break;
    case ExecState::DL_SUCCESS:
      save_error_text =
          std::format("Downloading '{}' Success!", filename.data());
      saveload_state = ExecState::UPDATED;
      break;
    case ExecState::DL_FAILURE:
      save_error_text =
          std::format("Downloading '{}' Failure!", filename.data());
      saveload_state = ExecState::UPDATED;
      break;
    case ExecState::UP_SUCCESS:
      save_error_text = std::format("Uploading '{}' Success!", filename.data());
      saveload_state = ExecState::UPDATED;
      break;
    case ExecState::UP_FAILURE:
      save_error_text = std::format("Uploading '{}' Failure!", filename.data());
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

void TestLuaScene::upload_text(const char *text) {
  std::ofstream ofs =
      std::ofstream(filename.data(), std::ios_base::out | std::ios_base::trunc);
  if (ofs.good()) {
    ofs.write(text, std::strlen(text));
    if (ofs.good()) {
      saveload_state = ExecState::UP_SUCCESS;
    } else {
      saveload_state = ExecState::UP_FAILURE;
    }
  } else {
    saveload_state = ExecState::UP_FAILURE;
  }
}

std::optional<std::string> TestLuaScene::load_from_file(const char *filename) {
  std::ifstream ifs(filename);
  std::string content{};
  std::array<char, 2048> buf;

  if (!ifs.good()) {
    return std::nullopt;
  }
  while (ifs.good()) {
    ifs.getline(buf.data(), buf.size());
    if (ifs.fail()) {
      if (ifs.eof() && !content.empty()) {
        return content;
      }
      return std::nullopt;
    }
    const auto read_size = ifs.gcount();
    if (read_size > 0) {
      if (ifs.eof()) {
        buf[read_size] = 0;
      } else {
        buf[read_size - 1] = '\n';
        buf[read_size] = 0;
      }
      content += buf.data();
    }
  }

  if (ifs.fail() && content.empty()) {
    return std::nullopt;
  }

  return content;
}

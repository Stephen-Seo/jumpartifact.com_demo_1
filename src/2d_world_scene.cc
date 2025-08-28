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

#include "2d_world_scene.h"

// third party includes
extern "C" {
#include <lua.h>
}
#include <raylib.h>

// standard library includes
#include <cstdlib>
#include <format>
#include <print>
#include <string>

// Lua functions
int lua_interface_create_ball(lua_State *lctx) {
  std::weak_ptr<TDWSPtrHolder> *wptr =
      reinterpret_cast<std::weak_ptr<TDWSPtrHolder> *>(
          lua_touserdata(lctx, lua_upvalueindex(1)));

  void *sptr_data = std::malloc(sizeof(std::shared_ptr<TDWSPtrHolder>));
  std::shared_ptr<TDWSPtrHolder> *sptr =
      new (sptr_data) std::shared_ptr<TDWSPtrHolder>(std::move(wptr->lock()));

  if (!(*sptr)) {
    sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
    std::free(sptr_data);

    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out =
          std::format("\"{}\" is only available in 2DSimulation Scene.", name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }
  TwoDimWorldScene *scene = (*sptr)->scene_ptr;
  sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
  std::free(sptr_data);

  uint32_t id = scene->create_ball();

  lua_pushinteger(lctx, id);
  return 1;
}

int lua_interface_destroy_ball(lua_State *lctx) {
  std::weak_ptr<TDWSPtrHolder> *wptr =
      reinterpret_cast<std::weak_ptr<TDWSPtrHolder> *>(
          lua_touserdata(lctx, lua_upvalueindex(1)));

  void *sptr_data = std::malloc(sizeof(std::shared_ptr<TDWSPtrHolder>));
  std::shared_ptr<TDWSPtrHolder> *sptr =
      new (sptr_data) std::shared_ptr<TDWSPtrHolder>(std::move(wptr->lock()));

  if (!(*sptr)) {
    sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
    std::free(sptr_data);

    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out =
          std::format("\"{}\" is only available in 2DSimulation Scene.", name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }
  TwoDimWorldScene *scene = (*sptr)->scene_ptr;
  sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
  std::free(sptr_data);

  if (lua_gettop(lctx) != 1 || lua_isinteger(lctx, -1) != 1) {
    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out =
          std::format("\"{}\" expects 1 argument: integer (ball id).", name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }

  bool ret = scene->destroy_ball(lua_tointeger(lctx, -1));

  lua_pushboolean(lctx, ret ? 1 : 0);
  return 1;
}

int lua_interface_get_ball_pos(lua_State *lctx) {
  std::weak_ptr<TDWSPtrHolder> *wptr =
      reinterpret_cast<std::weak_ptr<TDWSPtrHolder> *>(
          lua_touserdata(lctx, lua_upvalueindex(1)));

  void *sptr_data = std::malloc(sizeof(std::shared_ptr<TDWSPtrHolder>));
  std::shared_ptr<TDWSPtrHolder> *sptr =
      new (sptr_data) std::shared_ptr<TDWSPtrHolder>(std::move(wptr->lock()));

  if (!(*sptr)) {
    sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
    std::free(sptr_data);

    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out =
          std::format("\"{}\" is only available in 2DSimulation Scene.", name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }
  TwoDimWorldScene *scene = (*sptr)->scene_ptr;
  sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
  std::free(sptr_data);

  if (lua_gettop(lctx) != 1 || lua_isinteger(lctx, -1) != 1) {
    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out =
          std::format("\"{}\" expects 1 integer argument ball id!", name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }

  uint32_t idx = lua_tointeger(lctx, -1);

  b2Vec2 pos = scene->get_ball_pos(idx);

  lua_pushnumber(lctx, pos.x);
  lua_pushnumber(lctx, pos.y);

  return 2;
}

int lua_interface_set_ball_pos(lua_State *lctx) {
  std::weak_ptr<TDWSPtrHolder> *wptr =
      reinterpret_cast<std::weak_ptr<TDWSPtrHolder> *>(
          lua_touserdata(lctx, lua_upvalueindex(1)));

  void *sptr_data = std::malloc(sizeof(std::shared_ptr<TDWSPtrHolder>));
  std::shared_ptr<TDWSPtrHolder> *sptr =
      new (sptr_data) std::shared_ptr<TDWSPtrHolder>(std::move(wptr->lock()));

  if (!(*sptr)) {
    sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
    std::free(sptr_data);

    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out =
          std::format("\"{}\" is only available in 2DSimulation Scene.", name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }
  TwoDimWorldScene *scene = (*sptr)->scene_ptr;
  sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
  std::free(sptr_data);

  if (lua_gettop(lctx) != 3 || lua_isinteger(lctx, -3) != 1 ||
      lua_isnumber(lctx, -2) != 1 || lua_isnumber(lctx, -1) != 1) {
    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out = std::format(
          "\"{}\" expects 3 args: integer (ball id), number (x pos), number (y "
          "pos)!",
          name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }

  scene->set_ball_pos(lua_tointeger(lctx, -3), lua_tonumber(lctx, -2),
                      lua_tonumber(lctx, -1));

  return 0;
}

int lua_interface_get_ball_vel(lua_State *lctx) {
  std::weak_ptr<TDWSPtrHolder> *wptr =
      reinterpret_cast<std::weak_ptr<TDWSPtrHolder> *>(
          lua_touserdata(lctx, lua_upvalueindex(1)));

  void *sptr_data = std::malloc(sizeof(std::shared_ptr<TDWSPtrHolder>));
  std::shared_ptr<TDWSPtrHolder> *sptr =
      new (sptr_data) std::shared_ptr<TDWSPtrHolder>(std::move(wptr->lock()));

  if (!(*sptr)) {
    sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
    std::free(sptr_data);

    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out =
          std::format("\"{}\" is only available in 2DSimulation Scene.", name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }
  TwoDimWorldScene *scene = (*sptr)->scene_ptr;
  sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
  std::free(sptr_data);

  if (lua_gettop(lctx) != 1 || lua_isinteger(lctx, -1) != 1) {
    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out =
          std::format("\"{}\" expects 1 integer argument ball id!", name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }

  uint32_t idx = lua_tointeger(lctx, -1);

  b2Vec2 vel = scene->get_ball_vel(idx);

  lua_pushnumber(lctx, vel.x);
  lua_pushnumber(lctx, vel.y);

  return 2;
}

int lua_interface_apply_ball_impulse(lua_State *lctx) {
  std::weak_ptr<TDWSPtrHolder> *wptr =
      reinterpret_cast<std::weak_ptr<TDWSPtrHolder> *>(
          lua_touserdata(lctx, lua_upvalueindex(1)));

  void *sptr_data = std::malloc(sizeof(std::shared_ptr<TDWSPtrHolder>));
  std::shared_ptr<TDWSPtrHolder> *sptr =
      new (sptr_data) std::shared_ptr<TDWSPtrHolder>(std::move(wptr->lock()));

  if (!(*sptr)) {
    sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
    std::free(sptr_data);

    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out =
          std::format("\"{}\" is only available in 2DSimulation Scene.", name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }
  TwoDimWorldScene *scene = (*sptr)->scene_ptr;
  sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
  std::free(sptr_data);

  if (lua_gettop(lctx) != 3 || lua_isinteger(lctx, -3) != 1 ||
      lua_isnumber(lctx, -2) != 1 || lua_isnumber(lctx, -1) != 1) {
    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out = std::format(
          "\"{}\" expects 3 args: integer (ball id), number (x), number (y)!",
          name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }

  scene->apply_ball_impulse(lua_tointeger(lctx, -3), lua_tonumber(lctx, -2),
                            lua_tonumber(lctx, -1));

  return 0;
}

int lua_interface_set_ball_color(lua_State *lctx) {
  std::weak_ptr<TDWSPtrHolder> *wptr =
      reinterpret_cast<std::weak_ptr<TDWSPtrHolder> *>(
          lua_touserdata(lctx, lua_upvalueindex(1)));

  void *sptr_data = std::malloc(sizeof(std::shared_ptr<TDWSPtrHolder>));
  std::shared_ptr<TDWSPtrHolder> *sptr =
      new (sptr_data) std::shared_ptr<TDWSPtrHolder>(std::move(wptr->lock()));

  if (!(*sptr)) {
    sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
    std::free(sptr_data);

    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out =
          std::format("\"{}\" is only available in 2DSimulation Scene.", name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }
  TwoDimWorldScene *scene = (*sptr)->scene_ptr;
  sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
  std::free(sptr_data);

  if (lua_gettop(lctx) < 4 || lua_gettop(lctx) > 5 ||
      (lua_gettop(lctx) == 4 &&
       (lua_isinteger(lctx, -4) != 1 || lua_isinteger(lctx, -3) != 1 ||
        lua_isinteger(lctx, -2) != 1 || lua_isinteger(lctx, -1) != 1 ||
        lua_tointeger(lctx, -3) < 0 || lua_tointeger(lctx, -3) > 255 ||
        lua_tointeger(lctx, -2) < 0 || lua_tointeger(lctx, -2) > 255 ||
        lua_tointeger(lctx, -1) < 0 || lua_tointeger(lctx, -1) > 255)) ||
      (lua_gettop(lctx) == 5 &&
       (lua_isinteger(lctx, -5) != 1 || lua_isinteger(lctx, -4) != 1 ||
        lua_isinteger(lctx, -3) != 1 || lua_isinteger(lctx, -2) != 1 ||
        lua_isinteger(lctx, -1) != 1 || lua_tointeger(lctx, -4) < 0 ||
        lua_tointeger(lctx, -4) > 255 || lua_tointeger(lctx, -3) < 0 ||
        lua_tointeger(lctx, -3) > 255 || lua_tointeger(lctx, -2) < 0 ||
        lua_tointeger(lctx, -2) > 255 || lua_tointeger(lctx, -1) < 0 ||
        lua_tointeger(lctx, -1) > 255))) {
    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out = std::format(
          "\"{}\" expects 4-5 args: integer (ball id), integer (red 0-255), "
          "integer (green 0-255), integer (blue 0-255), integer (optional; "
          "alpha 0-255)!",
          name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }

  if (lua_gettop(lctx) == 4) {
    scene->set_ball_color(
        lua_tointeger(lctx, -4),
        Color{static_cast<uint8_t>(lua_tointeger(lctx, -3)),
              static_cast<uint8_t>(lua_tointeger(lctx, -2)),
              static_cast<uint8_t>(lua_tointeger(lctx, -1)), 255});
  } else if (lua_gettop(lctx) == 5) {
    scene->set_ball_color(lua_tointeger(lctx, -5),
                          Color{static_cast<uint8_t>(lua_tointeger(lctx, -4)),
                                static_cast<uint8_t>(lua_tointeger(lctx, -3)),
                                static_cast<uint8_t>(lua_tointeger(lctx, -2)),
                                static_cast<uint8_t>(lua_tointeger(lctx, -1))});
  }

  return 0;
}

int lua_interface_create_octagon(lua_State *lctx) {
  std::weak_ptr<TDWSPtrHolder> *wptr =
      reinterpret_cast<std::weak_ptr<TDWSPtrHolder> *>(
          lua_touserdata(lctx, lua_upvalueindex(1)));

  void *sptr_data = std::malloc(sizeof(std::shared_ptr<TDWSPtrHolder>));
  std::shared_ptr<TDWSPtrHolder> *sptr =
      new (sptr_data) std::shared_ptr<TDWSPtrHolder>(std::move(wptr->lock()));

  if (!(*sptr)) {
    sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
    std::free(sptr_data);

    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out =
          std::format("\"{}\" is only available in 2DSimulation Scene.", name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }
  TwoDimWorldScene *scene = (*sptr)->scene_ptr;
  sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
  std::free(sptr_data);

  uint32_t id = scene->create_octagon();

  lua_pushinteger(lctx, id);
  return 1;
}

int lua_interface_destroy_octagon(lua_State *lctx) {
  std::weak_ptr<TDWSPtrHolder> *wptr =
      reinterpret_cast<std::weak_ptr<TDWSPtrHolder> *>(
          lua_touserdata(lctx, lua_upvalueindex(1)));

  void *sptr_data = std::malloc(sizeof(std::shared_ptr<TDWSPtrHolder>));
  std::shared_ptr<TDWSPtrHolder> *sptr =
      new (sptr_data) std::shared_ptr<TDWSPtrHolder>(std::move(wptr->lock()));

  if (!(*sptr)) {
    sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
    std::free(sptr_data);

    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out =
          std::format("\"{}\" is only available in 2DSimulation Scene.", name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }
  TwoDimWorldScene *scene = (*sptr)->scene_ptr;
  sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
  std::free(sptr_data);

  if (lua_gettop(lctx) != 1 || lua_isinteger(lctx, -1) != 1) {
    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out =
          std::format("\"{}\" expects 1 argument: integer (octagon id).", name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }

  bool ret = scene->destroy_octagon(lua_tointeger(lctx, -1));

  lua_pushboolean(lctx, ret ? 1 : 0);
  return 1;
}

int lua_interface_get_octagon_pos(lua_State *lctx) {
  std::weak_ptr<TDWSPtrHolder> *wptr =
      reinterpret_cast<std::weak_ptr<TDWSPtrHolder> *>(
          lua_touserdata(lctx, lua_upvalueindex(1)));

  void *sptr_data = std::malloc(sizeof(std::shared_ptr<TDWSPtrHolder>));
  std::shared_ptr<TDWSPtrHolder> *sptr =
      new (sptr_data) std::shared_ptr<TDWSPtrHolder>(std::move(wptr->lock()));

  if (!(*sptr)) {
    sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
    std::free(sptr_data);

    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out =
          std::format("\"{}\" is only available in 2DSimulation Scene.", name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }
  TwoDimWorldScene *scene = (*sptr)->scene_ptr;
  sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
  std::free(sptr_data);

  if (lua_gettop(lctx) != 1 || lua_isinteger(lctx, -1) != 1) {
    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out =
          std::format("\"{}\" expects 1 integer argument octagon id!", name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }

  uint32_t idx = lua_tointeger(lctx, -1);

  b2Vec2 pos = scene->get_octagon_pos(idx);

  lua_pushnumber(lctx, pos.x);
  lua_pushnumber(lctx, pos.y);

  return 2;
}

int lua_interface_set_octagon_pos(lua_State *lctx) {
  std::weak_ptr<TDWSPtrHolder> *wptr =
      reinterpret_cast<std::weak_ptr<TDWSPtrHolder> *>(
          lua_touserdata(lctx, lua_upvalueindex(1)));

  void *sptr_data = std::malloc(sizeof(std::shared_ptr<TDWSPtrHolder>));
  std::shared_ptr<TDWSPtrHolder> *sptr =
      new (sptr_data) std::shared_ptr<TDWSPtrHolder>(std::move(wptr->lock()));

  if (!(*sptr)) {
    sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
    std::free(sptr_data);

    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out =
          std::format("\"{}\" is only available in 2DSimulation Scene.", name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }
  TwoDimWorldScene *scene = (*sptr)->scene_ptr;
  sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
  std::free(sptr_data);

  if (lua_gettop(lctx) != 3 || lua_isinteger(lctx, -3) != 1 ||
      lua_isnumber(lctx, -2) != 1 || lua_isnumber(lctx, -1) != 1) {
    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out = std::format(
          "\"{}\" expects 3 args: integer (octagon id), number (x pos), number "
          "(y "
          "pos)!",
          name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }

  scene->set_octagon_pos(lua_tointeger(lctx, -3), lua_tonumber(lctx, -2),
                         lua_tonumber(lctx, -1));

  return 0;
}

int lua_interface_get_octagon_vel(lua_State *lctx) {
  std::weak_ptr<TDWSPtrHolder> *wptr =
      reinterpret_cast<std::weak_ptr<TDWSPtrHolder> *>(
          lua_touserdata(lctx, lua_upvalueindex(1)));

  void *sptr_data = std::malloc(sizeof(std::shared_ptr<TDWSPtrHolder>));
  std::shared_ptr<TDWSPtrHolder> *sptr =
      new (sptr_data) std::shared_ptr<TDWSPtrHolder>(std::move(wptr->lock()));

  if (!(*sptr)) {
    sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
    std::free(sptr_data);

    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out =
          std::format("\"{}\" is only available in 2DSimulation Scene.", name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }
  TwoDimWorldScene *scene = (*sptr)->scene_ptr;
  sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
  std::free(sptr_data);

  if (lua_gettop(lctx) != 1 || lua_isinteger(lctx, -1) != 1) {
    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out =
          std::format("\"{}\" expects 1 integer argument octagon id!", name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }

  uint32_t idx = lua_tointeger(lctx, -1);

  b2Vec2 vel = scene->get_octagon_vel(idx);

  lua_pushnumber(lctx, vel.x);
  lua_pushnumber(lctx, vel.y);

  return 2;
}

int lua_interface_apply_octagon_impulse(lua_State *lctx) {
  std::weak_ptr<TDWSPtrHolder> *wptr =
      reinterpret_cast<std::weak_ptr<TDWSPtrHolder> *>(
          lua_touserdata(lctx, lua_upvalueindex(1)));

  void *sptr_data = std::malloc(sizeof(std::shared_ptr<TDWSPtrHolder>));
  std::shared_ptr<TDWSPtrHolder> *sptr =
      new (sptr_data) std::shared_ptr<TDWSPtrHolder>(std::move(wptr->lock()));

  if (!(*sptr)) {
    sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
    std::free(sptr_data);

    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out =
          std::format("\"{}\" is only available in 2DSimulation Scene.", name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }
  TwoDimWorldScene *scene = (*sptr)->scene_ptr;
  sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
  std::free(sptr_data);

  if (lua_gettop(lctx) != 3 || lua_isinteger(lctx, -3) != 1 ||
      lua_isnumber(lctx, -2) != 1 || lua_isnumber(lctx, -1) != 1) {
    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out = std::format(
          "\"{}\" expects 3 args: integer (octagon id), number (x), number "
          "(y)!",
          name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }

  scene->apply_octagon_impulse(lua_tointeger(lctx, -3), lua_tonumber(lctx, -2),
                               lua_tonumber(lctx, -1));

  return 0;
}

int lua_interface_set_octagon_color(lua_State *lctx) {
  std::weak_ptr<TDWSPtrHolder> *wptr =
      reinterpret_cast<std::weak_ptr<TDWSPtrHolder> *>(
          lua_touserdata(lctx, lua_upvalueindex(1)));

  void *sptr_data = std::malloc(sizeof(std::shared_ptr<TDWSPtrHolder>));
  std::shared_ptr<TDWSPtrHolder> *sptr =
      new (sptr_data) std::shared_ptr<TDWSPtrHolder>(std::move(wptr->lock()));

  if (!(*sptr)) {
    sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
    std::free(sptr_data);

    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out =
          std::format("\"{}\" is only available in 2DSimulation Scene.", name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }
  TwoDimWorldScene *scene = (*sptr)->scene_ptr;
  sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
  std::free(sptr_data);

  if (lua_gettop(lctx) < 4 || lua_gettop(lctx) > 5 ||
      (lua_gettop(lctx) == 4 &&
       (lua_isinteger(lctx, -4) != 1 || lua_isinteger(lctx, -3) != 1 ||
        lua_isinteger(lctx, -2) != 1 || lua_isinteger(lctx, -1) != 1 ||
        lua_tointeger(lctx, -3) < 0 || lua_tointeger(lctx, -3) > 255 ||
        lua_tointeger(lctx, -2) < 0 || lua_tointeger(lctx, -2) > 255 ||
        lua_tointeger(lctx, -1) < 0 || lua_tointeger(lctx, -1) > 255)) ||
      (lua_gettop(lctx) == 5 &&
       (lua_isinteger(lctx, -5) != 1 || lua_isinteger(lctx, -4) != 1 ||
        lua_isinteger(lctx, -3) != 1 || lua_isinteger(lctx, -2) != 1 ||
        lua_isinteger(lctx, -1) != 1 || lua_tointeger(lctx, -4) < 0 ||
        lua_tointeger(lctx, -4) > 255 || lua_tointeger(lctx, -3) < 0 ||
        lua_tointeger(lctx, -3) > 255 || lua_tointeger(lctx, -2) < 0 ||
        lua_tointeger(lctx, -2) > 255 || lua_tointeger(lctx, -1) < 0 ||
        lua_tointeger(lctx, -1) > 255))) {
    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out = std::format(
          "\"{}\" expects 4-5 args: integer (octagon id), integer (red 0-255), "
          "integer (green 0-255), integer (blue 0-255), integer (optional; "
          "alpha 0-255)!",
          name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }

  if (lua_gettop(lctx) == 4) {
    scene->set_octagon_color(
        lua_tointeger(lctx, -4),
        Color{static_cast<uint8_t>(lua_tointeger(lctx, -3)),
              static_cast<uint8_t>(lua_tointeger(lctx, -2)),
              static_cast<uint8_t>(lua_tointeger(lctx, -1)), 255});
  } else if (lua_gettop(lctx) == 5) {
    scene->set_octagon_color(
        lua_tointeger(lctx, -5),
        Color{static_cast<uint8_t>(lua_tointeger(lctx, -4)),
              static_cast<uint8_t>(lua_tointeger(lctx, -3)),
              static_cast<uint8_t>(lua_tointeger(lctx, -2)),
              static_cast<uint8_t>(lua_tointeger(lctx, -1))});
  }

  return 0;
}

int lua_interface_create_trapezoid(lua_State *lctx) {
  std::weak_ptr<TDWSPtrHolder> *wptr =
      reinterpret_cast<std::weak_ptr<TDWSPtrHolder> *>(
          lua_touserdata(lctx, lua_upvalueindex(1)));

  void *sptr_data = std::malloc(sizeof(std::shared_ptr<TDWSPtrHolder>));
  std::shared_ptr<TDWSPtrHolder> *sptr =
      new (sptr_data) std::shared_ptr<TDWSPtrHolder>(std::move(wptr->lock()));

  if (!(*sptr)) {
    sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
    std::free(sptr_data);

    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out =
          std::format("\"{}\" is only available in 2DSimulation Scene.", name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }
  TwoDimWorldScene *scene = (*sptr)->scene_ptr;
  sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
  std::free(sptr_data);

  uint32_t id = scene->create_trapezoid();

  lua_pushinteger(lctx, id);
  return 1;
}

int lua_interface_destroy_trapezoid(lua_State *lctx) {
  std::weak_ptr<TDWSPtrHolder> *wptr =
      reinterpret_cast<std::weak_ptr<TDWSPtrHolder> *>(
          lua_touserdata(lctx, lua_upvalueindex(1)));

  void *sptr_data = std::malloc(sizeof(std::shared_ptr<TDWSPtrHolder>));
  std::shared_ptr<TDWSPtrHolder> *sptr =
      new (sptr_data) std::shared_ptr<TDWSPtrHolder>(std::move(wptr->lock()));

  if (!(*sptr)) {
    sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
    std::free(sptr_data);

    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out =
          std::format("\"{}\" is only available in 2DSimulation Scene.", name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }
  TwoDimWorldScene *scene = (*sptr)->scene_ptr;
  sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
  std::free(sptr_data);

  if (lua_gettop(lctx) != 1 || lua_isinteger(lctx, -1) != 1) {
    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out = std::format(
          "\"{}\" expects 1 argument: integer (trapezoid id).", name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }

  bool ret = scene->destroy_trapezoid(lua_tointeger(lctx, -1));

  lua_pushboolean(lctx, ret ? 1 : 0);
  return 1;
}

int lua_interface_get_trapezoid_pos(lua_State *lctx) {
  std::weak_ptr<TDWSPtrHolder> *wptr =
      reinterpret_cast<std::weak_ptr<TDWSPtrHolder> *>(
          lua_touserdata(lctx, lua_upvalueindex(1)));

  void *sptr_data = std::malloc(sizeof(std::shared_ptr<TDWSPtrHolder>));
  std::shared_ptr<TDWSPtrHolder> *sptr =
      new (sptr_data) std::shared_ptr<TDWSPtrHolder>(std::move(wptr->lock()));

  if (!(*sptr)) {
    sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
    std::free(sptr_data);

    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out =
          std::format("\"{}\" is only available in 2DSimulation Scene.", name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }
  TwoDimWorldScene *scene = (*sptr)->scene_ptr;
  sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
  std::free(sptr_data);

  if (lua_gettop(lctx) != 1 || lua_isinteger(lctx, -1) != 1) {
    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out =
          std::format("\"{}\" expects 1 integer argument trapezoid id!", name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }

  uint32_t idx = lua_tointeger(lctx, -1);

  b2Vec2 pos = scene->get_trapezoid_pos(idx);

  lua_pushnumber(lctx, pos.x);
  lua_pushnumber(lctx, pos.y);

  return 2;
}

int lua_interface_set_trapezoid_pos(lua_State *lctx) {
  std::weak_ptr<TDWSPtrHolder> *wptr =
      reinterpret_cast<std::weak_ptr<TDWSPtrHolder> *>(
          lua_touserdata(lctx, lua_upvalueindex(1)));

  void *sptr_data = std::malloc(sizeof(std::shared_ptr<TDWSPtrHolder>));
  std::shared_ptr<TDWSPtrHolder> *sptr =
      new (sptr_data) std::shared_ptr<TDWSPtrHolder>(std::move(wptr->lock()));

  if (!(*sptr)) {
    sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
    std::free(sptr_data);

    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out =
          std::format("\"{}\" is only available in 2DSimulation Scene.", name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }
  TwoDimWorldScene *scene = (*sptr)->scene_ptr;
  sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
  std::free(sptr_data);

  if (lua_gettop(lctx) != 3 || lua_isinteger(lctx, -3) != 1 ||
      lua_isnumber(lctx, -2) != 1 || lua_isnumber(lctx, -1) != 1) {
    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out = std::format(
          "\"{}\" expects 3 args: integer (trapezoid id), number (x pos), "
          "number (y pos)!",
          name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }

  scene->set_trapezoid_pos(lua_tointeger(lctx, -3), lua_tonumber(lctx, -2),
                           lua_tonumber(lctx, -1));

  return 0;
}

int lua_interface_get_trapezoid_vel(lua_State *lctx) {
  std::weak_ptr<TDWSPtrHolder> *wptr =
      reinterpret_cast<std::weak_ptr<TDWSPtrHolder> *>(
          lua_touserdata(lctx, lua_upvalueindex(1)));

  void *sptr_data = std::malloc(sizeof(std::shared_ptr<TDWSPtrHolder>));
  std::shared_ptr<TDWSPtrHolder> *sptr =
      new (sptr_data) std::shared_ptr<TDWSPtrHolder>(std::move(wptr->lock()));

  if (!(*sptr)) {
    sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
    std::free(sptr_data);

    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out =
          std::format("\"{}\" is only available in 2DSimulation Scene.", name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }
  TwoDimWorldScene *scene = (*sptr)->scene_ptr;
  sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
  std::free(sptr_data);

  if (lua_gettop(lctx) != 1 || lua_isinteger(lctx, -1) != 1) {
    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out =
          std::format("\"{}\" expects 1 integer argument trapezoid id!", name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }

  uint32_t idx = lua_tointeger(lctx, -1);

  b2Vec2 vel = scene->get_trapezoid_vel(idx);

  lua_pushnumber(lctx, vel.x);
  lua_pushnumber(lctx, vel.y);

  return 2;
}

int lua_interface_apply_trapezoid_impulse(lua_State *lctx) {
  std::weak_ptr<TDWSPtrHolder> *wptr =
      reinterpret_cast<std::weak_ptr<TDWSPtrHolder> *>(
          lua_touserdata(lctx, lua_upvalueindex(1)));

  void *sptr_data = std::malloc(sizeof(std::shared_ptr<TDWSPtrHolder>));
  std::shared_ptr<TDWSPtrHolder> *sptr =
      new (sptr_data) std::shared_ptr<TDWSPtrHolder>(std::move(wptr->lock()));

  if (!(*sptr)) {
    sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
    std::free(sptr_data);

    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out =
          std::format("\"{}\" is only available in 2DSimulation Scene.", name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }
  TwoDimWorldScene *scene = (*sptr)->scene_ptr;
  sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
  std::free(sptr_data);

  if (lua_gettop(lctx) != 3 || lua_isinteger(lctx, -3) != 1 ||
      lua_isnumber(lctx, -2) != 1 || lua_isnumber(lctx, -1) != 1) {
    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out = std::format(
          "\"{}\" 3 args: integer (trapezoid id), number (x), number (y)!",
          name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }

  scene->apply_trapezoid_impulse(
      lua_tointeger(lctx, -3), lua_tonumber(lctx, -2), lua_tonumber(lctx, -1));

  return 0;
}

int lua_interface_set_trapezoid_color(lua_State *lctx) {
  std::weak_ptr<TDWSPtrHolder> *wptr =
      reinterpret_cast<std::weak_ptr<TDWSPtrHolder> *>(
          lua_touserdata(lctx, lua_upvalueindex(1)));

  void *sptr_data = std::malloc(sizeof(std::shared_ptr<TDWSPtrHolder>));
  std::shared_ptr<TDWSPtrHolder> *sptr =
      new (sptr_data) std::shared_ptr<TDWSPtrHolder>(std::move(wptr->lock()));

  if (!(*sptr)) {
    sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
    std::free(sptr_data);

    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out =
          std::format("\"{}\" is only available in 2DSimulation Scene.", name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }
  TwoDimWorldScene *scene = (*sptr)->scene_ptr;
  sptr->std::shared_ptr<TDWSPtrHolder>::~shared_ptr();
  std::free(sptr_data);

  if (lua_gettop(lctx) < 4 || lua_gettop(lctx) > 5 ||
      (lua_gettop(lctx) == 4 &&
       (lua_isinteger(lctx, -4) != 1 || lua_isinteger(lctx, -3) != 1 ||
        lua_isinteger(lctx, -2) != 1 || lua_isinteger(lctx, -1) != 1 ||
        lua_tointeger(lctx, -3) < 0 || lua_tointeger(lctx, -3) > 255 ||
        lua_tointeger(lctx, -2) < 0 || lua_tointeger(lctx, -2) > 255 ||
        lua_tointeger(lctx, -1) < 0 || lua_tointeger(lctx, -1) > 255)) ||
      (lua_gettop(lctx) == 5 &&
       (lua_isinteger(lctx, -5) != 1 || lua_isinteger(lctx, -4) != 1 ||
        lua_isinteger(lctx, -3) != 1 || lua_isinteger(lctx, -2) != 1 ||
        lua_isinteger(lctx, -1) != 1 || lua_tointeger(lctx, -4) < 0 ||
        lua_tointeger(lctx, -4) > 255 || lua_tointeger(lctx, -3) < 0 ||
        lua_tointeger(lctx, -3) > 255 || lua_tointeger(lctx, -2) < 0 ||
        lua_tointeger(lctx, -2) > 255 || lua_tointeger(lctx, -1) < 0 ||
        lua_tointeger(lctx, -1) > 255))) {
    {
      const char *name = lua_tostring(lctx, lua_upvalueindex(2));
      std::string out = std::format(
          "\"{}\" expects 4-5 args: integer (trapezoid id), integer (red "
          "0-255), "
          "integer (green 0-255), integer (blue 0-255), integer (optional; "
          "alpha 0-255)!",
          name);
      std::println(stdout, "{}", out);
      lua_pushstring(lctx, out.c_str());
    }

    return lua_error(lctx);
  }

  if (lua_gettop(lctx) == 4) {
    scene->set_trapezoid_color(
        lua_tointeger(lctx, -4),
        Color{static_cast<uint8_t>(lua_tointeger(lctx, -3)),
              static_cast<uint8_t>(lua_tointeger(lctx, -2)),
              static_cast<uint8_t>(lua_tointeger(lctx, -1)), 255});
  } else if (lua_gettop(lctx) == 5) {
    scene->set_trapezoid_color(
        lua_tointeger(lctx, -5),
        Color{static_cast<uint8_t>(lua_tointeger(lctx, -4)),
              static_cast<uint8_t>(lua_tointeger(lctx, -3)),
              static_cast<uint8_t>(lua_tointeger(lctx, -2)),
              static_cast<uint8_t>(lua_tointeger(lctx, -1))});
  }

  return 0;
}

int lua_interface_helper_cleanup_ptr_holder(lua_State *lctx) {
  std::weak_ptr<TDWSPtrHolder> *wptr_ptr =
      reinterpret_cast<std::weak_ptr<TDWSPtrHolder> *>(
          lua_touserdata(lctx, -1));

  wptr_ptr->std::weak_ptr<TDWSPtrHolder>::~weak_ptr();

  return 0;
}

// Lua: -0, +1
void lua_interface_helper_push_ptr_holder(lua_State *lctx,
                                          std::weak_ptr<TDWSPtrHolder> wptr) {
  void *alloc_buf =
      lua_newuserdatauv(lctx, sizeof(std::weak_ptr<TDWSPtrHolder>), 0);  // +1
  std::weak_ptr<TDWSPtrHolder> *wptr_ptr =
      new (alloc_buf) std::weak_ptr<TDWSPtrHolder>(wptr);

  lua_newtable(lctx);                                                // +1
  lua_pushcfunction(lctx, lua_interface_helper_cleanup_ptr_holder);  // +1
  lua_setfield(lctx, -2, "__gc");                                    // -1
  lua_setmetatable(lctx, -2);                                        // -1
}

TwoDimWorldScene::TwoDimWorldScene(SceneSystem *ctx)
    : Scene(ctx),
      lua_error_text{},
      ptr_ctx(std::make_shared<TDWSPtrHolder>(this)),
      rand_e(std::random_device()()),
      real_dist(),
      ball_idx_counter(0),
      octagon_idx_counter(0),
      trapezoid_idx_counter(0) {
  if (!ctx->get_map_value("lua_state").has_value()) {
    ctx->init_lua();
  }

  // Create Box2D World
  b2WorldDef world_def = b2DefaultWorldDef();

  world_def.userData = ctx;
  world_def.gravity = b2Vec2{0.0F, 10.0F};

  this->world_id = b2CreateWorld(&world_def);

  // Create Box2D Ground
  b2BodyDef ground_body_def = b2DefaultBodyDef();
  ground_body_def.position = b2Vec2{GROUND_X, GROUND_Y};
  this->ground_id = b2CreateBody(this->world_id, &ground_body_def);

  b2Polygon ground_box = b2MakeBox(GROUND_HW, GROUND_HH);
  b2ShapeDef ground_shape_def = b2DefaultShapeDef();
  ground_shape_def.material.restitution = 0.3F;
  ground_shape_def.material.friction = 0.15F;
  ground_shape_def.material.rollingResistance = 0.15F;
  b2CreatePolygonShape(this->ground_id, &ground_shape_def, &ground_box);

  // Create Box2D Walls
  b2BodyDef wall_body_def = b2DefaultBodyDef();
  wall_body_def.position = b2Vec2{LWALL_X, LWALL_Y};
  this->left_wall_id = b2CreateBody(this->world_id, &wall_body_def);

  b2Polygon wall_box = b2MakeBox(WALL_HW, WALL_HH);
  b2ShapeDef wall_shape_def = b2DefaultShapeDef();
  wall_shape_def.material.restitution = 0.8F;
  b2CreatePolygonShape(this->left_wall_id, &wall_shape_def, &wall_box);

  wall_body_def = b2DefaultBodyDef();
  wall_body_def.position = b2Vec2{RWALL_X, RWALL_Y};
  this->right_wall_id = b2CreateBody(this->world_id, &wall_body_def);

  wall_box = b2MakeBox(WALL_HW, WALL_HH);
  wall_shape_def = b2DefaultShapeDef();
  wall_shape_def.material.restitution = 0.8F;
  b2CreatePolygonShape(this->right_wall_id, &wall_shape_def, &wall_box);

  // Set up Lua stuff
  lua_State *lua_ctx =
      reinterpret_cast<lua_State *>(ctx->get_map_value("lua_state").value());

  lua_getglobal(lua_ctx, "scene_2d");  // +1
  if (lua_istable(lua_ctx, -1) != 1) {
    lua_pop(lua_ctx, 1);                 // -1
    lua_newtable(lua_ctx);               // +1
    lua_setglobal(lua_ctx, "scene_2d");  // -1
  } else {
    lua_pop(lua_ctx, 1);  // -1
  }

  lua_getglobal(lua_ctx, "scene_2d");  // +1

  lua_interface_helper_push_ptr_holder(lua_ctx, ptr_ctx);   // +1
  lua_pushstring(lua_ctx, "createball");                    // +1
  lua_pushcclosure(lua_ctx, lua_interface_create_ball, 2);  // -2, +1
  lua_setfield(lua_ctx, -2, "createball");                  // -1

  lua_interface_helper_push_ptr_holder(lua_ctx, ptr_ctx);    // +1
  lua_pushstring(lua_ctx, "destroyball");                    // +1
  lua_pushcclosure(lua_ctx, lua_interface_destroy_ball, 2);  // -2, +1
  lua_setfield(lua_ctx, -2, "destroyball");                  // -1

  lua_interface_helper_push_ptr_holder(lua_ctx, ptr_ctx);    // +1
  lua_pushstring(lua_ctx, "getballpos");                     // +1
  lua_pushcclosure(lua_ctx, lua_interface_get_ball_pos, 2);  // -2, +1
  lua_setfield(lua_ctx, -2, "getballpos");                   // -1

  lua_interface_helper_push_ptr_holder(lua_ctx, ptr_ctx);    // +1
  lua_pushstring(lua_ctx, "setballpos");                     // +1
  lua_pushcclosure(lua_ctx, lua_interface_set_ball_pos, 2);  // -2, +1
  lua_setfield(lua_ctx, -2, "setballpos");                   // -1

  lua_interface_helper_push_ptr_holder(lua_ctx, ptr_ctx);    // +1
  lua_pushstring(lua_ctx, "getballvel");                     // +1
  lua_pushcclosure(lua_ctx, lua_interface_get_ball_vel, 2);  // -2, +1
  lua_setfield(lua_ctx, -2, "getballvel");                   // -1

  lua_interface_helper_push_ptr_holder(lua_ctx, ptr_ctx);          // +1
  lua_pushstring(lua_ctx, "applyballimpulse");                     // +1
  lua_pushcclosure(lua_ctx, lua_interface_apply_ball_impulse, 2);  // -2, +1
  lua_setfield(lua_ctx, -2, "applyballimpulse");                   // -1

  lua_interface_helper_push_ptr_holder(lua_ctx, ptr_ctx);      // +1
  lua_pushstring(lua_ctx, "setballcolor");                     // +1
  lua_pushcclosure(lua_ctx, lua_interface_set_ball_color, 2);  // -2, +1
  lua_setfield(lua_ctx, -2, "setballcolor");                   // -1

  lua_interface_helper_push_ptr_holder(lua_ctx, ptr_ctx);      // +1
  lua_pushstring(lua_ctx, "createoctagon");                    // +1
  lua_pushcclosure(lua_ctx, lua_interface_create_octagon, 2);  // -2, +1
  lua_setfield(lua_ctx, -2, "createoctagon");                  // -1

  lua_interface_helper_push_ptr_holder(lua_ctx, ptr_ctx);       // +1
  lua_pushstring(lua_ctx, "destroyoctagon");                    // +1
  lua_pushcclosure(lua_ctx, lua_interface_destroy_octagon, 2);  // -2, +1
  lua_setfield(lua_ctx, -2, "destroyoctagon");                  // -1

  lua_interface_helper_push_ptr_holder(lua_ctx, ptr_ctx);       // +1
  lua_pushstring(lua_ctx, "getoctagonpos");                     // +1
  lua_pushcclosure(lua_ctx, lua_interface_get_octagon_pos, 2);  // -2, +1
  lua_setfield(lua_ctx, -2, "getoctagonpos");                   // -1

  lua_interface_helper_push_ptr_holder(lua_ctx, ptr_ctx);       // +1
  lua_pushstring(lua_ctx, "setoctagonpos");                     // +1
  lua_pushcclosure(lua_ctx, lua_interface_set_octagon_pos, 2);  // -2, +1
  lua_setfield(lua_ctx, -2, "setoctagonpos");                   // -1

  lua_interface_helper_push_ptr_holder(lua_ctx, ptr_ctx);       // +1
  lua_pushstring(lua_ctx, "getoctagonvel");                     // +1
  lua_pushcclosure(lua_ctx, lua_interface_get_octagon_vel, 2);  // -2, +1
  lua_setfield(lua_ctx, -2, "getoctagonvel");                   // -1

  lua_interface_helper_push_ptr_holder(lua_ctx, ptr_ctx);             // +1
  lua_pushstring(lua_ctx, "applyoctagonimpulse");                     // +1
  lua_pushcclosure(lua_ctx, lua_interface_apply_octagon_impulse, 2);  // -2, +1
  lua_setfield(lua_ctx, -2, "applyoctagonimpulse");                   // -1

  lua_interface_helper_push_ptr_holder(lua_ctx, ptr_ctx);         // +1
  lua_pushstring(lua_ctx, "setoctagoncolor");                     // +1
  lua_pushcclosure(lua_ctx, lua_interface_set_octagon_color, 2);  // -2, +1
  lua_setfield(lua_ctx, -2, "setoctagoncolor");                   // -1

  lua_interface_helper_push_ptr_holder(lua_ctx, ptr_ctx);        // +1
  lua_pushstring(lua_ctx, "createtrapezoid");                    // +1
  lua_pushcclosure(lua_ctx, lua_interface_create_trapezoid, 2);  // -2, +1
  lua_setfield(lua_ctx, -2, "createtrapezoid");                  // -1

  lua_interface_helper_push_ptr_holder(lua_ctx, ptr_ctx);         // +1
  lua_pushstring(lua_ctx, "destroytrapezoid");                    // +1
  lua_pushcclosure(lua_ctx, lua_interface_destroy_trapezoid, 2);  // -2, +1
  lua_setfield(lua_ctx, -2, "destroytrapezoid");                  // -1

  lua_interface_helper_push_ptr_holder(lua_ctx, ptr_ctx);         // +1
  lua_pushstring(lua_ctx, "gettrapezoidpos");                     // +1
  lua_pushcclosure(lua_ctx, lua_interface_get_trapezoid_pos, 2);  // -2, +1
  lua_setfield(lua_ctx, -2, "gettrapezoidpos");                   // -1

  lua_interface_helper_push_ptr_holder(lua_ctx, ptr_ctx);         // +1
  lua_pushstring(lua_ctx, "settrapezoidpos");                     // +1
  lua_pushcclosure(lua_ctx, lua_interface_set_trapezoid_pos, 2);  // -2, +1
  lua_setfield(lua_ctx, -2, "settrapezoidpos");                   // -1

  lua_interface_helper_push_ptr_holder(lua_ctx, ptr_ctx);         // +1
  lua_pushstring(lua_ctx, "gettrapezoidvel");                     // +1
  lua_pushcclosure(lua_ctx, lua_interface_get_trapezoid_vel, 2);  // -2, +1
  lua_setfield(lua_ctx, -2, "gettrapezoidvel");                   // -1

  lua_interface_helper_push_ptr_holder(lua_ctx, ptr_ctx);  // +1
  lua_pushstring(lua_ctx, "applytrapezoidimpulse");        // +1
  lua_pushcclosure(lua_ctx, lua_interface_apply_trapezoid_impulse,
                   2);                                 // -2, +1
  lua_setfield(lua_ctx, -2, "applytrapezoidimpulse");  // -1

  lua_interface_helper_push_ptr_holder(lua_ctx, ptr_ctx);           // +1
  lua_pushstring(lua_ctx, "settrapezoidcolor");                     // +1
  lua_pushcclosure(lua_ctx, lua_interface_set_trapezoid_color, 2);  // -2, +1
  lua_setfield(lua_ctx, -2, "settrapezoidcolor");                   // -1

  int ret = lua_getfield(lua_ctx, -1, "init");  // +1
  if (ret == LUA_TFUNCTION) {
    ret = lua_pcall(lua_ctx, 0, 0, 0);  // -1
    if (ret != LUA_OK) {                // +1 error
      const char *error_str = lua_tostring(lua_ctx, -1);
      if (error_str) {
        lua_error_text = error_str;
      } else {
        lua_error_text = "WARNING: Unknown Lue error!";
      }
      lua_pop(lua_ctx, 1);  // -1
      flags.set(0);
    }
  } else {
    lua_pop(lua_ctx, 1);  // -1
  }

  lua_pop(lua_ctx, 1);  // -1

  if (IsGamepadAvailable(0)) {
    flags.set(1);
  }
}

TwoDimWorldScene::~TwoDimWorldScene() { b2DestroyWorld(this->world_id); }

void TwoDimWorldScene::update(SceneSystem *ctx, float dt) {
  if (flags.test(0)) {
    return;
  }

  lua_State *lua_ctx =
      reinterpret_cast<lua_State *>(ctx->get_map_value("lua_state").value());

  int lua_ret_type = lua_getglobal(lua_ctx, "scene_2d");  // +1

  if (lua_ret_type == LUA_TTABLE) {
    // Set "scene_2d.dt"
    lua_pushnumber(lua_ctx, dt);      // +1
    lua_setfield(lua_ctx, -2, "dt");  // -1

    // Keyboard events
    lua_ret_type = lua_getfield(lua_ctx, -1, "key_pressed_callback");  // +1
    if (lua_ret_type != LUA_TFUNCTION) {
      lua_pop(lua_ctx, 1);  // -1
    } else {
      int ret = GetKeyPressed();
      while (ret != 0) {
        lua_pushinteger(lua_ctx, ret);              // +1
        int lua_ret = lua_pcall(lua_ctx, 1, 0, 0);  // -2
        if (lua_ret != LUA_OK) {                    // error +1
          lua_error_text = std::format("{}", lua_tostring(lua_ctx, -1));
          flags.set(0);
          lua_pop(lua_ctx, 2);  // -2
          return;
        }
        lua_getfield(lua_ctx, -1, "key_pressed_callback");  // +1
        ret = GetKeyPressed();
      }
      lua_pop(lua_ctx, 1);  // -1
    }

    // Gamepad events
    if (flags.test(1)) {
      // Gamepad buttons
      lua_ret_type =
          lua_getfield(lua_ctx, -1, "gamepad_pressed_callback");  // +1
      if (lua_ret_type != LUA_TFUNCTION) {
        lua_pop(lua_ctx, 1);  // -1
      } else {
        for (int idx = 0; idx < 32; ++idx) {
          if (IsGamepadButtonPressed(0, idx)) {
            lua_pushinteger(lua_ctx, idx);              // +1
            int lua_ret = lua_pcall(lua_ctx, 1, 0, 0);  // -2
            if (lua_ret != LUA_OK) {                    // error +1
              lua_error_text = std::format("{}", lua_tostring(lua_ctx, -1));
              flags.set(0);
              lua_pop(lua_ctx, 2);  // -2
              return;
            }
            lua_getfield(lua_ctx, -1, "gamepad_pressed_callback");  // +1
          }
        }  // for idx 0..32
        lua_pop(lua_ctx, 1);  // -1
      }
      // Gamepad axis
      const int axis_count = GetGamepadAxisCount(0);
      lua_ret_type = lua_getfield(lua_ctx, -1, "gamepad_axis_callback");  // +1
      if (lua_ret_type != LUA_TFUNCTION) {
        lua_pop(lua_ctx, 1);  // -1
      } else {
        for (int idx = 0; idx < axis_count; ++idx) {
          lua_pushinteger(lua_ctx, idx);                            // +1
          lua_pushnumber(lua_ctx, GetGamepadAxisMovement(0, idx));  // +1
          int lua_ret = lua_pcall(lua_ctx, 2, 0, 0);                // -3
          if (lua_ret != LUA_OK) {                                  // error +1
            lua_error_text = std::format("{}", lua_tostring(lua_ctx, -1));
            flags.set(0);
            lua_pop(lua_ctx, 2);  // -2
            return;
          }
          lua_getfield(lua_ctx, -1, "gamepad_axis_callback");  // +1
        }
        lua_pop(lua_ctx, 1);  // -1
      }
    }

    // scene_2d.update
    int ret = lua_getfield(lua_ctx, -1, "update");  // +1
    if (ret == LUA_TFUNCTION) {
      lua_pushnumber(lua_ctx, dt);                          // +1
      ret = lua_pcall(lua_ctx, 1, 0, 0);                    // -2
      if (ret != LUA_OK) {                                  // +1
        const char *error_str = lua_tostring(lua_ctx, -1);  // +0
        if (error_str) {
          lua_error_text = error_str;
        } else {
          lua_error_text = "WARNING: Unknown Lua error!";
        }
        lua_pop(lua_ctx, 1);  // -1
        flags.set(0);
      }
    } else {
      lua_pop(lua_ctx, 1);  // -1
    }
    lua_pop(lua_ctx, 1);  // -1
  } else {
    lua_pop(lua_ctx, 1);  // -1
  }

  b2World_Step(world_id, dt, 4);
}

void TwoDimWorldScene::draw(SceneSystem *ctx) {
  // Draw ground
  DrawRectangle(PIXEL_B2UNIT_RATIO * (GROUND_X - GROUND_HW),
                PIXEL_B2UNIT_RATIO * (GROUND_Y - GROUND_HH),
                PIXEL_B2UNIT_RATIO * GROUND_HW * 2.0F,
                PIXEL_B2UNIT_RATIO * GROUND_HH * 2.0F, DARKGREEN);

  // Draw walls
  DrawRectangle(PIXEL_B2UNIT_RATIO * (LWALL_X - WALL_HW),
                PIXEL_B2UNIT_RATIO * (LWALL_Y - WALL_HH),
                PIXEL_B2UNIT_RATIO * WALL_HW * 2.0F,
                PIXEL_B2UNIT_RATIO * WALL_HH * 2.0F, BROWN);
  DrawRectangle(PIXEL_B2UNIT_RATIO * (RWALL_X - WALL_HW),
                PIXEL_B2UNIT_RATIO * (RWALL_Y - WALL_HH),
                PIXEL_B2UNIT_RATIO * WALL_HW * 2.0F,
                PIXEL_B2UNIT_RATIO * WALL_HH * 2.0F, BROWN);

  // Draw ball
  for (auto iter = ball_ids.begin(); iter != ball_ids.end(); ++iter) {
    b2Vec2 pos = b2Body_GetPosition(iter->second.id);
    DrawCircle(pos.x * PIXEL_B2UNIT_RATIO, pos.y * PIXEL_B2UNIT_RATIO,
               BALL_R * PIXEL_B2UNIT_RATIO, iter->second.color);
  }

  // Draw octagon
  Vector2 b_vertices[8];
  for (auto iter = octagon_ids.begin(); iter != octagon_ids.end(); ++iter) {
    // b2Vec2 octagon_pos = b2Body_GetPosition(iter->second.id);
    // DrawCircle(octagon_pos.x * PIXEL_B2UNIT_RATIO, octagon_pos.y *
    // PIXEL_B2UNIT_RATIO,
    //            BALL_R * PIXEL_B2UNIT_RATIO, iter->second.color);
    b2Transform b_tr = b2Body_GetTransform(iter->second.id);
    for (int idx = 0; idx < 8; ++idx) {
      b_vertices[7 - idx].x =
          (b_tr.p.x + b_tr.q.c * cached_octagon_polygon->vertices[idx].x -
           b_tr.q.s * cached_octagon_polygon->vertices[idx].y) *
          PIXEL_B2UNIT_RATIO;
      b_vertices[7 - idx].y =
          (b_tr.p.y + b_tr.q.s * cached_octagon_polygon->vertices[idx].x +
           b_tr.q.c * cached_octagon_polygon->vertices[idx].y) *
          PIXEL_B2UNIT_RATIO;
    }

    DrawTriangleFan(b_vertices, 8, iter->second.color);
  }

  // Draw trapezoid
  // b2AABB t_aabb = b2Body_ComputeAABB(trapezoid_id);
  // DrawRectangleLines(
  //    PIXEL_B2UNIT_RATIO * t_aabb.lowerBound.x,
  //    PIXEL_B2UNIT_RATIO * t_aabb.lowerBound.y,
  //    PIXEL_B2UNIT_RATIO * (t_aabb.upperBound.x - t_aabb.lowerBound.x),
  //    PIXEL_B2UNIT_RATIO * (t_aabb.upperBound.y - t_aabb.lowerBound.y), BLUE);
  Vector2 t_vertices[4];
  for (auto iter = trapezoid_ids.begin(); iter != trapezoid_ids.end(); ++iter) {
    b2Transform t_tr = b2Body_GetTransform(iter->second.id);
    for (int idx = 0; idx < 4; ++idx) {
      t_vertices[idx].x =
          (t_tr.p.x + t_tr.q.c * cached_trapezoid_polygon->vertices[idx].x -
           t_tr.q.s * cached_trapezoid_polygon->vertices[idx].y) *
          PIXEL_B2UNIT_RATIO;
      t_vertices[idx].y =
          (t_tr.p.y + t_tr.q.s * cached_trapezoid_polygon->vertices[idx].x +
           t_tr.q.c * cached_trapezoid_polygon->vertices[idx].y) *
          PIXEL_B2UNIT_RATIO;
    }

    DrawTriangle(t_vertices[2], t_vertices[1], t_vertices[0],
                 iter->second.color);
    DrawTriangle(t_vertices[2], t_vertices[0], t_vertices[3],
                 iter->second.color);
  }

  if (!lua_error_text.empty()) {
    DrawText(lua_error_text.c_str(), 0, 0, 10, WHITE);
  }
}

void TwoDimWorldScene::draw_rlimgui(SceneSystem *ctx) {}

bool TwoDimWorldScene::allow_draw_below(SceneSystem *ctx) { return true; }

uint32_t TwoDimWorldScene::create_ball() {
  // Create dynamic body ball
  b2BodyDef ball_body = b2DefaultBodyDef();
  ball_body.type = b2_dynamicBody;
  ball_body.position = b2Vec2{1.5F, -get_rand() * 5.0F};
  std::println("Created ball at {}, {}", ball_body.position.x,
               ball_body.position.y);
  b2BodyId ball_id = b2CreateBody(this->world_id, &ball_body);
  b2Body_SetLinearVelocity(ball_id, {-1.0F, 0.0F});

  b2Circle circle{{0.0F, 0.0F}, BALL_R};
  b2ShapeDef b_shape_def = b2DefaultShapeDef();
  b_shape_def.density = 1.0F;
  b_shape_def.material.friction = 0.3F;
  b_shape_def.material.restitution = 0.4F;
  b_shape_def.material.rollingResistance = 0.15F;
  b2CreateCircleShape(ball_id, &b_shape_def, &circle);

  while (ball_ids.contains(ball_idx_counter)) {
    ++ball_idx_counter;
  }
  ball_ids.insert({ball_idx_counter++, {ball_id, get_random_color()}});
  return ball_idx_counter - 1;
}

bool TwoDimWorldScene::destroy_ball(uint32_t idx) {
  if (auto iter = ball_ids.find(idx); iter != ball_ids.end()) {
    b2DestroyBody(iter->second.id);
    ball_ids.erase(iter);
    return true;
  }

  return false;
}

b2Vec2 TwoDimWorldScene::get_ball_pos(uint32_t idx) const {
  if (auto iter = ball_ids.find(idx); iter != ball_ids.end()) {
    return b2Body_GetPosition(iter->second.id);
  }

  return {0, 0};
}

void TwoDimWorldScene::set_ball_pos(uint32_t idx, float x, float y) {
  if (auto iter = ball_ids.find(idx); iter != ball_ids.end()) {
    b2Rot rot = b2Body_GetRotation(iter->second.id);
    b2Body_SetTransform(iter->second.id, b2Vec2{x, y}, rot);
  }
}

b2Vec2 TwoDimWorldScene::get_ball_vel(uint32_t idx) const {
  if (auto iter = ball_ids.find(idx); iter != ball_ids.end()) {
    return b2Body_GetLinearVelocity(iter->second.id);
  }

  return {0, 0};
}

void TwoDimWorldScene::apply_ball_impulse(uint32_t idx, float x, float y) {
  if (auto iter = ball_ids.find(idx); iter != ball_ids.end()) {
    b2Body_ApplyLinearImpulseToCenter(iter->second.id, b2Vec2{x, y}, true);
  }
}

void TwoDimWorldScene::set_ball_color(uint32_t idx, Color color) {
  if (auto iter = ball_ids.find(idx); iter != ball_ids.end()) {
    iter->second.color = color;
  }
}

uint32_t TwoDimWorldScene::create_octagon() {
  // Create dynamic body octagon
  b2BodyDef octagon_body = b2DefaultBodyDef();
  octagon_body.type = b2_dynamicBody;
  octagon_body.position = b2Vec2{1.7F, -get_rand() * 5.0F};
  std::println("Created octagon at {}, {}", octagon_body.position.x,
               octagon_body.position.y);
  b2BodyId octagon_id = b2CreateBody(this->world_id, &octagon_body);
  b2Body_SetLinearVelocity(octagon_id, {-1.0F, 0.0F});

  b2Hull b_hull = b2ComputeHull(B_POINTS, 8);
  b2Polygon b_polygon = b2MakePolygon(&b_hull, 0.0F);
  b2ShapeDef b_shape_def = b2DefaultShapeDef();
  b_shape_def.density = 1.0F;
  b_shape_def.material.friction = 0.3F;
  b_shape_def.material.restitution = 0.4F;
  b_shape_def.material.rollingResistance = 0.15F;
  b2CreatePolygonShape(octagon_id, &b_shape_def, &b_polygon);

  if (!cached_octagon_polygon.has_value()) {
    b2ShapeId b_shape;
    b2Body_GetShapes(octagon_id, &b_shape, 1);
    cached_octagon_polygon = b2Shape_GetPolygon(b_shape);
  }

  while (octagon_ids.contains(octagon_idx_counter)) {
    ++octagon_idx_counter;
  }
  octagon_ids.insert({octagon_idx_counter++, {octagon_id, get_random_color()}});
  return octagon_idx_counter - 1;
}

bool TwoDimWorldScene::destroy_octagon(uint32_t idx) {
  if (auto iter = octagon_ids.find(idx); iter != octagon_ids.end()) {
    b2DestroyBody(iter->second.id);
    octagon_ids.erase(iter);
    return true;
  }

  return false;
}

b2Vec2 TwoDimWorldScene::get_octagon_pos(uint32_t idx) const {
  if (auto iter = octagon_ids.find(idx); iter != octagon_ids.end()) {
    return b2Body_GetPosition(iter->second.id);
  }

  return {0, 0};
}

void TwoDimWorldScene::set_octagon_pos(uint32_t idx, float x, float y) {
  if (auto iter = octagon_ids.find(idx); iter != octagon_ids.end()) {
    b2Rot rot = b2Body_GetRotation(iter->second.id);
    b2Body_SetTransform(iter->second.id, b2Vec2{x, y}, rot);
  }
}

b2Vec2 TwoDimWorldScene::get_octagon_vel(uint32_t idx) const {
  if (auto iter = octagon_ids.find(idx); iter != octagon_ids.end()) {
    return b2Body_GetLinearVelocity(iter->second.id);
  }

  return {0, 0};
}

void TwoDimWorldScene::apply_octagon_impulse(uint32_t idx, float x, float y) {
  if (auto iter = octagon_ids.find(idx); iter != octagon_ids.end()) {
    b2Body_ApplyLinearImpulseToCenter(iter->second.id, b2Vec2{x, y}, true);
  }
}

void TwoDimWorldScene::set_octagon_color(uint32_t idx, Color color) {
  if (auto iter = octagon_ids.find(idx); iter != octagon_ids.end()) {
    iter->second.color = color;
  }
}

uint32_t TwoDimWorldScene::create_trapezoid() {
  // Create dynamic body trapezoid
  b2BodyDef t_body = b2DefaultBodyDef();
  t_body.type = b2_dynamicBody;
  t_body.position = b2Vec2{3.0F, -get_rand() * 5.0F};
  std::println("Created trapezoid at {}, {}", t_body.position.x,
               t_body.position.y);
  b2BodyId trapezoid_id = b2CreateBody(this->world_id, &t_body);

  b2Hull t_hull = b2ComputeHull(T_POINTS, 4);
  b2Polygon t_polygon = b2MakePolygon(&t_hull, T_RADIUS);
  b2ShapeDef t_shape_def = b2DefaultShapeDef();
  t_shape_def.density = 1.0F;
  t_shape_def.material.friction = 0.6F;
  b2CreatePolygonShape(trapezoid_id, &t_shape_def, &t_polygon);

  if (!cached_trapezoid_polygon.has_value()) {
    b2ShapeId t_shape;
    b2Body_GetShapes(trapezoid_id, &t_shape, 1);
    cached_trapezoid_polygon = b2Shape_GetPolygon(t_shape);
  }

  while (trapezoid_ids.contains(trapezoid_idx_counter)) {
    ++trapezoid_idx_counter;
  }
  trapezoid_ids.insert(
      {trapezoid_idx_counter++, {trapezoid_id, get_random_color()}});

  return trapezoid_idx_counter - 1;
}

bool TwoDimWorldScene::destroy_trapezoid(uint32_t idx) {
  if (auto iter = trapezoid_ids.find(idx); iter != trapezoid_ids.end()) {
    b2DestroyBody(iter->second.id);
    trapezoid_ids.erase(iter);
    return true;
  }

  return false;
}

b2Vec2 TwoDimWorldScene::get_trapezoid_pos(uint32_t idx) const {
  if (auto iter = trapezoid_ids.find(idx); iter != trapezoid_ids.end()) {
    return b2Body_GetPosition(iter->second.id);
  }

  return {0, 0};
}

void TwoDimWorldScene::set_trapezoid_pos(uint32_t idx, float x, float y) {
  if (auto iter = trapezoid_ids.find(idx); iter != trapezoid_ids.end()) {
    b2Rot rot = b2Body_GetRotation(iter->second.id);
    b2Body_SetTransform(iter->second.id, b2Vec2{x, y}, rot);
  }
}

b2Vec2 TwoDimWorldScene::get_trapezoid_vel(uint32_t idx) const {
  if (auto iter = trapezoid_ids.find(idx); iter != trapezoid_ids.end()) {
    return b2Body_GetLinearVelocity(iter->second.id);
  }

  return {0, 0};
}

void TwoDimWorldScene::apply_trapezoid_impulse(uint32_t idx, float x, float y) {
  if (auto iter = trapezoid_ids.find(idx); iter != trapezoid_ids.end()) {
    b2Body_ApplyLinearImpulseToCenter(iter->second.id, b2Vec2{x, y}, true);
  }
}

void TwoDimWorldScene::set_trapezoid_color(uint32_t idx, Color color) {
  if (auto iter = trapezoid_ids.find(idx); iter != trapezoid_ids.end()) {
    iter->second.color = color;
  }
}

float TwoDimWorldScene::get_rand() { return real_dist(rand_e); }

Color TwoDimWorldScene::get_random_color() {
  return Color{static_cast<uint8_t>(GetRandomValue(127, 255)),
               static_cast<uint8_t>(GetRandomValue(127, 255)),
               static_cast<uint8_t>(GetRandomValue(127, 255)), 255};
}

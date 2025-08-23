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

  b2Vec2 pos = scene->get_ball_pos();

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

  int n = lua_gettop(lctx);
  if (n != 2 || lua_isnumber(lctx, -2) != 1 || lua_isnumber(lctx, -1) != 1) {
    lua_pushboolean(lctx, 0);
    return 1;
  }

  scene->set_ball_pos(lua_tonumber(lctx, -2), lua_tonumber(lctx, -1));

  lua_pushboolean(lctx, 1);
  return 1;
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

  b2Vec2 vel = scene->get_ball_vel();

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

  int n = lua_gettop(lctx);
  if (n != 2 || lua_isnumber(lctx, -2) != 1 || lua_isnumber(lctx, -1) != 1) {
    lua_pushboolean(lctx, 0);
    return 1;
  }

  scene->apply_ball_impulse(lua_tonumber(lctx, -2), lua_tonumber(lctx, -1));

  lua_pushboolean(lctx, 1);
  return 1;
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
      ptr_ctx(std::make_shared<TDWSPtrHolder>(this)) {
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

  // Create dynamic body
  b2BodyDef ball_body = b2DefaultBodyDef();
  ball_body.type = b2_dynamicBody;
  ball_body.position = b2Vec2{2.0F, 0.0F};
  ball_body.linearVelocity.x = -3.0F;
  this->ball_id = b2CreateBody(this->world_id, &ball_body);

  b2Circle circle{b2Vec2{0.0F, 0.0F}, BALL_R};
  b2ShapeDef ball_shape_def = b2DefaultShapeDef();
  ball_shape_def.density = 1.0F;
  ball_shape_def.material.friction = 0.3F;
  b2CreateCircleShape(this->ball_id, &ball_shape_def, &circle);

  // Set up Lua stuff
  lua_State *lua_ctx =
      reinterpret_cast<lua_State *>(ctx->get_map_value("lua_state").value());

  lua_getglobal(lua_ctx, "scene_ball");  // +1
  if (lua_istable(lua_ctx, -1) != 1) {
    lua_pop(lua_ctx, 1);                   // -1
    lua_newtable(lua_ctx);                 // +1
    lua_setglobal(lua_ctx, "scene_ball");  // -1
  } else {
    lua_pop(lua_ctx, 1);  // -1
  }

  lua_getglobal(lua_ctx, "scene_ball");  // +1

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

  lua_pop(lua_ctx, 1);  // -1
}

TwoDimWorldScene::~TwoDimWorldScene() { b2DestroyWorld(this->world_id); }

void TwoDimWorldScene::update(SceneSystem *ctx, float dt) {
  lua_State *lua_ctx =
      reinterpret_cast<lua_State *>(ctx->get_map_value("lua_state").value());

  // Lua update
  lua_getglobal(lua_ctx, "scene_ball");  // +1
  if (lua_istable(lua_ctx, -1) != 1) {
    lua_pop(lua_ctx, 1);  // -1
  } else {
    lua_pushnumber(lua_ctx, dt);                    // +1
    lua_setfield(lua_ctx, -2, "dt");                // -1
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
      }
    } else {
      lua_pop(lua_ctx, 1);  // -1
    }
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
  b2Vec2 ball_pos = b2Body_GetPosition(ball_id);
  DrawCircle(ball_pos.x * PIXEL_B2UNIT_RATIO, ball_pos.y * PIXEL_B2UNIT_RATIO,
             BALL_R * PIXEL_B2UNIT_RATIO, RED);

  if (!lua_error_text.empty()) {
    DrawText(lua_error_text.c_str(), 0, 0, 10, WHITE);
  }
}

void TwoDimWorldScene::draw_rlimgui(SceneSystem *ctx) {}

bool TwoDimWorldScene::allow_draw_below(SceneSystem *ctx) { return true; }

b2Vec2 TwoDimWorldScene::get_ball_pos() const {
  return b2Body_GetPosition(ball_id);
}

void TwoDimWorldScene::set_ball_pos(float x, float y) {
  b2Body_SetTransform(ball_id, b2Vec2{x, y}, b2Rot{});
}

b2Vec2 TwoDimWorldScene::get_ball_vel() const {
  return b2Body_GetLinearVelocity(ball_id);
}

void TwoDimWorldScene::apply_ball_impulse(float x, float y) {
  b2Body_ApplyLinearImpulseToCenter(ball_id, b2Vec2{x, y}, true);
}

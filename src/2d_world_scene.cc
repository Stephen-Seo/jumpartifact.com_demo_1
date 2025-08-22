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
#include <raylib.h>

TwoDimWorldScene::TwoDimWorldScene(SceneSystem *ctx) : Scene(ctx) {
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
  b2CreatePolygonShape(this->ground_id, &ground_shape_def, &ground_box);

  // Create Box2D Walls
  b2BodyDef wall_body_def = b2DefaultBodyDef();
  wall_body_def.position = b2Vec2{LWALL_X, LWALL_Y};
  this->left_wall_id = b2CreateBody(this->world_id, &wall_body_def);

  b2Polygon wall_box = b2MakeBox(WALL_HW, WALL_HH);
  b2ShapeDef wall_shape_def = b2DefaultShapeDef();
  b2CreatePolygonShape(this->left_wall_id, &wall_shape_def, &wall_box);

  wall_body_def = b2DefaultBodyDef();
  wall_body_def.position = b2Vec2{RWALL_X, RWALL_Y};
  this->right_wall_id = b2CreateBody(this->world_id, &wall_body_def);

  wall_box = b2MakeBox(WALL_HW, WALL_HH);
  wall_shape_def = b2DefaultShapeDef();
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
}

TwoDimWorldScene::~TwoDimWorldScene() { b2DestroyWorld(this->world_id); }

void TwoDimWorldScene::update(SceneSystem *ctx, float dt) {
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
}

void TwoDimWorldScene::draw_rlimgui(SceneSystem *ctx) {}

bool TwoDimWorldScene::allow_draw_below(SceneSystem *ctx) { return true; }

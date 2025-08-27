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

#ifndef SEODISPARATE_COM_JUMPARTIFACT_DEMO_1_2D_WORLD_SCENE_H_
#define SEODISPARATE_COM_JUMPARTIFACT_DEMO_1_2D_WORLD_SCENE_H_

#include "scene_system.h"

// third party includes
#include <box2d/box2d.h>
#include <raylib.h>

// standard library includes
#include <bitset>
#include <cstdint>
#include <memory>
#include <optional>
#include <random>
#include <string>
#include <unordered_map>

constexpr float PIXEL_B2UNIT_RATIO = 200.0F;

constexpr float GROUND_X = 2.0F;
constexpr float GROUND_Y = 4.0F;
constexpr float GROUND_HW = 2.0F;
constexpr float GROUND_HH = 1.0F;

constexpr float LWALL_X = 0.0F;
constexpr float LWALL_Y = 1.5F;
constexpr float RWALL_X = 4.0F;
constexpr float RWALL_Y = 1.5F;

constexpr float WALL_HW = 0.1F;
constexpr float WALL_HH = 1.5F;

constexpr float BALL_R = 0.1F;

constexpr b2Vec2 T_POINTS[4] = {
    {0.1F, -0.1F}, {-0.1F, -0.1F}, {-0.15F, 0.1F}, {0.15F, 0.1F}};
constexpr float T_RADIUS = 0.0F;

// Forward declaration
class TwoDimWorldScene;

struct TDWSPtrHolder {
  TwoDimWorldScene *scene_ptr;
};

struct BodyInfo {
  b2BodyId id;
  Color color;
};

class TwoDimWorldScene : public Scene {
 public:
  TwoDimWorldScene(SceneSystem *ctx);
  virtual ~TwoDimWorldScene() override;

  virtual void update(SceneSystem *ctx, float dt) override;
  virtual void draw(SceneSystem *ctx) override;
  virtual void draw_rlimgui(SceneSystem *ctx) override;
  virtual bool allow_draw_below(SceneSystem *ctx) override;

  uint32_t create_ball();
  bool destroy_ball(uint32_t idx);
  b2Vec2 get_ball_pos(uint32_t idx) const;
  void set_ball_pos(uint32_t idx, float x, float y);
  b2Vec2 get_ball_vel(uint32_t indx) const;
  void apply_ball_impulse(uint32_t idx, float x, float y);

  uint32_t create_trapezoid();
  bool destroy_trapezoid(uint32_t idx);
  b2Vec2 get_trapezoid_pos(uint32_t idx) const;
  void set_trapezoid_pos(uint32_t idx, float x, float y);
  b2Vec2 get_trapezoid_vel(uint32_t idx) const;
  void apply_trapezoid_impulse(uint32_t idx, float x, float y);

  float get_rand();

 private:
  std::string lua_error_text;
  std::shared_ptr<TDWSPtrHolder> ptr_ctx;
  std::unordered_map<uint32_t, BodyInfo> ball_ids;
  std::unordered_map<uint32_t, BodyInfo> trapezoid_ids;
  std::default_random_engine rand_e;
  std::uniform_real_distribution<float> real_dist;
  // 0 - error occurred
  // 1 - gamepad 0 is available
  std::bitset<32> flags;
  std::optional<b2Polygon> cached_trapezoid_polygon;
  b2WorldId world_id;
  b2BodyId ground_id;
  b2BodyId left_wall_id;
  b2BodyId right_wall_id;
  uint32_t ball_idx_counter;
  uint32_t trapezoid_idx_counter;

  static Color get_random_color();
};

#endif

#ifndef SEODISPARATE_COM_JUMPARTIFACT_DEMO_1_DEMO_SCENE_H_
#define SEODISPARATE_COM_JUMPARTIFACT_DEMO_1_DEMO_SCENE_H_

#include "scene_system.h"

// standard library includes
#include <bitset>

constexpr int DT_ARR_SIZE = 10;

class DemoScene : public Scene {
 public:
  DemoScene();
  virtual ~DemoScene() override;

  virtual void update(SceneSystem *ctx, float dt) override;
  virtual bool draw(SceneSystem *ctx) override;

 private:
  // 0 - double font size disabled
  // 1 - queued toggle flag 0
  std::bitset<32> flags;
  float dt[DT_ARR_SIZE];
  int dt_idx;
};

#endif

#ifndef SEODISPARATE_COM_JUMPARTIFACT_DEMO_1_SCENE_SYSTEM_H_
#define SEODISPARATE_COM_JUMPARTIFACT_DEMO_1_SCENE_SYSTEM_H_

#include <chrono>
#include <deque>
#include <functional>
#include <memory>
#include <optional>

// Forward declarations.
class SceneSystem;

class Scene {
 public:
  Scene();
  virtual ~Scene();

  virtual void update(SceneSystem* ctx, float dt) = 0;
  // Return true to DISALLOW drawing next scene.
  virtual bool draw(SceneSystem* ctx) = 0;

 private:
};

class SceneSystem {
 public:
  using SceneType = std::unique_ptr<Scene>;
  using OptBuilderType = std::optional<std::function<SceneType()>>;

  SceneSystem();
  ~SceneSystem();

  // Disable copy.
  SceneSystem(const SceneSystem&) = delete;
  SceneSystem& operator=(const SceneSystem&) = delete;

  // Enable move.
  SceneSystem(SceneSystem&&) = default;
  SceneSystem& operator=(SceneSystem&&) = default;

  void update();
  void draw();

  void clear_scenes();
  void push_scene(std::function<SceneType()> scene_builder);
  void pop_scene();

 private:
  enum class ActionType { CLEAR, PUSH, POP };
  struct Action {
    ActionType type;
    OptBuilderType scene_builder;
  };

  std::chrono::time_point<std::chrono::steady_clock> time_point;
  std::deque<SceneType> scene_stack;
  std::deque<Action> queued_actions;

  void handle_actions();
};

#endif

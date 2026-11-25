# Enemy, Bullet, and Boss Behavior Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking. Note: the implementer here is the project owner, who is writing the code directly.

**Goal:** Build the C++ building blocks, rule and script runners, shapes, bullet pool, and boss state machines so that enemies, bullets, and bosses are authored in JSON and new mechanics are added in C++.

**Architecture:** Game logic only, with no raylib calls. Shared conditions, actions, shapes, and the loader live in `behavior`. Bullets are stored in a capped vector with generation handles. Basic enemies run ordered scripts, bullets run rule lists, and bosses run state machines. Shapes are `Pattern` objects in a C++ vector, and JSON overrides their numeric parameters.

**Tech Stack:** C++17, CMake, doctest (existing test harness), cJSON through the existing JSON loader, raylib only in the debug overlay.

**Spec:** `docs/superpowers/specs/2026-11-25-enemy-bullet-behavior-design.md`. Where this plan and the spec disagree, the spec wins. Also relevant: `bullet_hell_roguelike_design.md`, which covers the overall rewrite.

## Global Constraints

- Language: C++17.
- Timing: 60 frames per second. All durations in JSON and code are in frames unless a field name says `seconds`.
- Content is JSON, loaded through the existing `load_json` with `DataError` naming the file and key.
- Balance and pool values live in `Data/tuning.json`.
- No gameplay code calls raylib.
- Shapes keep their structure in C++. JSON may override numeric parameters only.
- Bullets are referenced by handle, never by raw pointer across frames.
- Failed spawns are counted, never silent.

## Review Focus

1. A JSON field with an unknown name is rejected at load with a `DataError` naming the file, the object, and the field. It is not silently ignored. (Task 7)
2. A handle to a bullet that was removed and whose slot was reused must not refer to the new bullet. (Task 3)
3. A spawn at the pool cap fails, increments the failed-spawn counter, and does not crash or overwrite a live bullet. (Task 3)
4. A `once` rule fires exactly once even when its condition stays true. A `continuous` rule applies its actions every frame while its condition is true, and stops when it becomes false. (Task 5)
5. A boss whose last state is not a spell state, a transition to a missing state, or a state unreachable from the start is rejected at load. (Task 8)
6. A script or rule list with an action that has neither a duration nor an until-condition, or a `restart` script with zero total duration, is rejected at load because it would loop forever. (Tasks 6, 7)

## File Structure

- `src/gameplay/behavior.h/.cpp`: the shared condition and action registries, the shape registry and parameter resolver, the JSON loaders for scripts, rules, and boss state machines, and the per-frame update order.
- `src/gameplay/bullet_pool.h/.cpp`: the bullet vector, handles, swap-remove, cap, and failed-spawn counter.
- `src/gameplay/bullets.h/.cpp`: the bullet struct and its rule evaluation (`once` and `continuous`).
- `src/gameplay/enemies.h/.cpp`: the sequential enemy script runner and the boss state machine.
- `src/render/debug_overlay.h/.cpp`: the failed-spawn overlay. This is the only render file in this plan.
- `Data/tuning.json`: adds `max_bullets` (placeholder value 4096, to be tuned).
- `Data/behaviors/*.json`: example enemy, bullet, and boss files.
- `tests/test_behavior.cpp`, `tests/test_bullets.cpp`, `tests/test_enemies.cpp`, `tests/test_debug_overlay.cpp`. Fixtures for invalid JSON go in `tests/fixtures/`.

---

## Task 1: Shared Condition Registry

**Files:**
- Create: `src/gameplay/behavior.h`, `src/gameplay/behavior.cpp`
- Test: `tests/test_behavior.cpp`

**Interfaces:**
- Consumes: none
- Produces: `struct ConditionContext { int frame_in_action; int lifetime_frames; float y; float x; bool wall_hit; bool screen_exit; bool player_hit; bool bullet_collided; float health_fraction; }`. `class Condition { virtual bool holds(const ConditionContext&) const = 0; }`. `std::unique_ptr<Condition> make_condition(const std::string& name, const JsonDoc& params)`. An unknown name throws `DataError` naming it.

- [ ] **Step 1: Write failing tests**

```cpp
TEST_CASE("timer holds at the given frame") {
    auto c = make_condition("timer", params_frames(30));
    ConditionContext ctx{}; ctx.frame_in_action = 29;
    CHECK_FALSE(c->holds(ctx));
    ctx.frame_in_action = 30;
    CHECK(c->holds(ctx));
}
TEST_CASE("y_position above holds when y is above the value") { /* y_position, direction "above", value 20 */ }
TEST_CASE("unknown condition name throws DataError") { CHECK_THROWS_AS(make_condition("nope", {}), DataError); }
```

- [ ] **Step 2: Run tests to verify they fail**

Run: `./build/tests -tc="behavior*"`
Expected: FAIL, functions not defined.

- [ ] **Step 3: Implement the context struct, base class, and the eight condition types**

Each condition type reads only the context fields it needs. `health` reads `health_fraction` and is valid only in boss states (validated in Task 8).

- [ ] **Step 4: Run tests to verify they pass**

Run: `./build/tests -tc="behavior*"`
Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git commit -am "feat: shared condition registry"
```

## Task 2: Shared Action Registry

**Files:**
- Modify: `src/gameplay/behavior.h`, `src/gameplay/behavior.cpp`
- Test: `tests/test_behavior.cpp`

**Interfaces:**
- Consumes: `ConditionContext` (Task 1)
- Produces: `struct ActionContext` holding a mutable reference to the owning entity's state (position, velocity, type name, sprite, pattern request queue, child-spawn request queue, despawn flag, and transition request). `class Action { virtual void apply(ActionContext&) = 0; virtual bool finished(const ActionContext&) const = 0; }`. `std::unique_ptr<Action> make_action(const std::string& name, const JsonDoc& params)`. Unknown names throw `DataError`.

Each action records what it asked for in the context (for example, `fire_pattern` pushes a request); it does not spawn bullets directly. Spawning happens in the frame update (Task 9).

- [ ] **Step 1: Write failing tests**

```cpp
TEST_CASE("set_speed updates the bullet velocity magnitude") {
    auto a = make_action("set_speed", params_speed(3.0f));
    ActionContext ctx = bullet_ctx_with_velocity(1.0f, 0.0f);
    a->apply(ctx);
    CHECK(ctx.speed() == doctest::Approx(3.0f));
}
TEST_CASE("wait finishes after its duration in frames") { /* wait 10 frames */ }
TEST_CASE("unknown action name throws DataError") { CHECK_THROWS_AS(make_action("nope", {}), DataError); }
```

- [ ] **Step 2: Run tests to verify they fail**

Run: `./build/tests -tc="behavior*"`
Expected: FAIL.

- [ ] **Step 3: Implement the nine action types**

`move` and `wait` finish by duration or until-condition. `fire_pattern`, `spawn_child`, `despawn`, and `transition` set a request on the context. `set_type` and `set_sprite` change the bullet's type name and glyph and color.

- [ ] **Step 4: Run tests to verify they pass**

Run: `./build/tests -tc="behavior*"`
Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git commit -am "feat: shared action registry"
```

## Task 3: Bullet Pool with Handles and Failed-Spawn Counter

**Files:**
- Modify: `Data/tuning.json` (add `"max_bullets": 4096`)
- Create: `src/gameplay/bullet_pool.h`, `src/gameplay/bullet_pool.cpp`
- Create: `src/gameplay/bullets.h`, `src/gameplay/bullets.cpp`
- Test: `tests/test_bullets.cpp`

**Interfaces:**
- Consumes: `Tuning` (existing loader)
- Produces: `struct BulletHandle { uint32_t index; uint32_t generation; }`. `struct Bullet { float x, y, vx, vy; std::string type; std::string glyph; uint32_t color; int lifetime_frames; std::vector<Rule> rules; }` (`Rule` is defined in Task 5 in `bullets.h`). `class BulletPool { explicit BulletPool(int cap); std::optional<BulletHandle> spawn(Bullet); bool alive(BulletHandle) const; Bullet* get(BulletHandle); void remove(BulletHandle); int size() const; int failed_spawns() const; }`.

Rules:
- `get` returns null for a stale handle.
- `remove` swaps with the last element, updates the moved element's position mapping, and increments the generation of the freed slot.
- `spawn` returns `std::nullopt` at the cap and increments `failed_spawns`.

- [ ] **Step 1: Write failing tests**

```cpp
TEST_CASE("stale handle does not reach a reused slot") {
    BulletPool pool(4);
    auto h1 = *pool.spawn(bullet_at(0, 0));
    pool.remove(h1);
    auto h2 = *pool.spawn(bullet_at(5, 5));
    CHECK(pool.get(h1) == nullptr);
    CHECK(pool.get(h2)->x == doctest::Approx(5.0f));
}
TEST_CASE("spawn at cap fails and counts") {
    BulletPool pool(2);
    pool.spawn(bullet_at(0,0)); pool.spawn(bullet_at(0,0));
    CHECK_FALSE(pool.spawn(bullet_at(0,0)).has_value());
    CHECK(pool.failed_spawns() == 1);
    CHECK(pool.size() == 2);
}
TEST_CASE("remove keeps the other handles valid") { /* remove the first of three, the last two still resolve */ }
```

- [ ] **Step 2: Run tests to verify they fail**

Run: `./build/tests -tc="bullets*"`
Expected: FAIL.

- [ ] **Step 3: Implement the pool with a vector of slots, a generation per slot, and a map from slot to vector position for swap-remove**

Keep the slot-to-position map and generations in parallel vectors. Do not store pointers across calls.

- [ ] **Step 4: Run tests to verify they pass**

Run: `./build/tests -tc="bullets*"`
Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git commit -am "feat: capped bullet pool with generation handles"
```

## Task 4: Shapes (Patterns) with Numeric Overrides

**Files:**
- Modify: `src/gameplay/behavior.h`, `src/gameplay/behavior.cpp`
- Test: `tests/test_behavior.cpp`

**Interfaces:**
- Consumes: `load_json`, `JsonDoc`
- Produces: `struct ParamSpec { std::string name; float default_value; }`. `class Pattern { virtual std::string name() const = 0; virtual std::vector<ParamSpec> params() const = 0; virtual std::vector<SpawnRequest> emit(const std::map<std::string, float>& values, float origin_x, float origin_y) const = 0; }`. `struct SpawnRequest { float x, y, vx, vy; std::string bullet_type; }`. `const std::vector<std::unique_ptr<Pattern>>& shape_registry()`. `const Pattern* find_shape(const std::string& name)`. `std::map<std::string, float> resolve_params(const Pattern&, const JsonDoc& overrides)` throws `DataError` on an unknown parameter name.

Initial shapes: `ring` (params: `count`, `speed`), `arc` (params: `count`, `speed`, `from_deg`, `to_deg`), `line` (params: `count`, `speed`, `angle_deg`).

- [ ] **Step 1: Write failing tests**

```cpp
TEST_CASE("ring with default count emits that many bullets evenly spaced") {
    auto v = resolve_params(*find_shape("ring"), empty_json());
    auto reqs = find_shape("ring")->emit(v, 0, 0);
    CHECK(reqs.size() == 12);   // ring default count
}
TEST_CASE("override changes count only") { /* ring with count 8 emits 8 */ }
TEST_CASE("unknown parameter in override throws DataError") { CHECK_THROWS_AS(resolve_params(*find_shape("ring"), json_with("bogus", 1)), DataError); }
TEST_CASE("unknown shape name returns nullptr") { CHECK(find_shape("nope") == nullptr); }
```

Use the default count of 12 for `ring`, which is set in the C++ shape definition.

- [ ] **Step 2: Run tests to verify they fail**

Run: `./build/tests -tc="behavior*"`
Expected: FAIL.

- [ ] **Step 3: Implement the base class, the registry, the three shapes, and the parameter resolver**

Each shape's structure is fixed in its `emit` body. Parameters are only the numbers in `params()`.

- [ ] **Step 4: Run tests to verify they pass**

Run: `./build/tests -tc="behavior*"`
Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git commit -am "feat: shape registry with numeric parameter overrides"
```

## Task 5: Bullet Rules (once and continuous)

**Files:**
- Modify: `src/gameplay/bullets.h`, `src/gameplay/bullets.cpp`
- Test: `tests/test_bullets.cpp`

**Interfaces:**
- Consumes: `Condition` and `ConditionContext` (Task 1), `Action` and `ActionContext` (Task 2), `Bullet` (Task 3)
- Produces: `struct Rule { std::unique_ptr<Condition> when; std::vector<std::unique_ptr<Action>> actions; bool continuous; bool fired; }`. `void run_rules(Bullet&, ConditionContext&, ActionContext&)` evaluates each rule of the bullet.

Rules:
- A `once` rule (the default) fires its actions on the first frame its condition holds and sets `fired`. It never fires again.
- A `continuous` rule applies its actions on every frame its condition holds.

- [ ] **Step 1: Write failing tests**

```cpp
TEST_CASE("once rule fires exactly once while its condition stays true") {
    // y_position above 20, action set_speed 3, condition true for 5 frames
    // expect set_speed applied once
}
TEST_CASE("continuous rule applies every frame it holds and stops when false") {
    // y_position above 20 continuous, y 25 for 3 frames then 10 for 2 frames
    // expect applied on 3 frames, not on the last 2
}
TEST_CASE("lifetime rule at 60 frames sets speed 3") { /* lifetime 60, set_speed 3 */ }
```

- [ ] **Step 2: Run tests to verify they fail**

Run: `./build/tests -tc="bullets*"`
Expected: FAIL.

- [ ] **Step 3: Implement rule evaluation with `fired` tracking for `once` and no tracking for `continuous`**

- [ ] **Step 4: Run tests to verify they pass**

Run: `./build/tests -tc="bullets*"`
Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git commit -am "feat: bullet rules with once and continuous modes"
```

## Task 6: Enemy Script Runner

**Files:**
- Create: `src/gameplay/enemies.h`, `src/gameplay/enemies.cpp`
- Test: `tests/test_enemies.cpp`

**Interfaces:**
- Consumes: `Action`, `ActionContext` (Task 2), `ConditionContext` (Task 1)
- Produces: `enum class EndBehavior { Stop, Restart }`. `class EnemyScript { EnemyScript(std::vector<std::unique_ptr<Action>> steps, EndBehavior end); void tick(ActionContext&, ConditionContext&); bool finished() const; }`. `tick` advances the current action and moves to the next when it finishes. `finished` is true only when `end == Stop` and the last action has finished.

- [ ] **Step 1: Write failing tests**

```cpp
TEST_CASE("script moves through actions in order") {
    // move right 300 frames, wait 30 frames, move left 300 frames
    // after 300 ticks the second action is current
}
TEST_CASE("restart loops back to the first action") { /* after total duration, first action runs again */ }
TEST_CASE("stop script reports finished after its last action") { /* finished() false until last done */ }
```

- [ ] **Step 2: Run tests to verify they fail**

Run: `./build/tests -tc="enemies*"`
Expected: FAIL.

- [ ] **Step 3: Implement the runner with a step index and a per-action frame counter**

- [ ] **Step 4: Run tests to verify they pass**

Run: `./build/tests -tc="enemies*"`
Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git commit -am "feat: sequential enemy script runner"
```

## Task 7: Behavior JSON Loader with Strict Field Checking

**Files:**
- Modify: `src/gameplay/behavior.h`, `src/gameplay/behavior.cpp`
- Create: `Data/behaviors/example_enemy.json`, `Data/behaviors/example_bullet.json`
- Test: `tests/test_behavior.cpp`

**Interfaces:**
- Consumes: `load_json`, `JsonDoc`, `make_condition`, `make_action`, `find_shape`, `resolve_params`, `EnemyScript`
- Produces: `EnemyScript load_enemy_script(const std::string& path)`, `std::vector<Rule> load_bullet_rules(const std::string& path)`. `JsonDoc` gains `void require_only(const std::set<std::string>& allowed)` which throws `DataError` naming the file and the first unknown key.

Rules:
- Every object in the file is checked with `require_only`.
- A script `Restart` with a total duration of 0 is rejected.
- An action with neither a duration nor an until-condition (for `move` and `wait`) is rejected.

- [ ] **Step 1: Write failing tests**

```cpp
TEST_CASE("unknown field is rejected with the file and key named") {
    // inline JSON with "speeed" typo
    CHECK_THROWS_WITH_AS(load_bullet_rules(path), doctest::Contains("speeed"), DataError);
}
TEST_CASE("restart script with zero total duration is rejected") { /* ... */ }
TEST_CASE("example enemy script loads") { CHECK_NOTHROW(load_enemy_script("Data/behaviors/example_enemy.json")); }
```

- [ ] **Step 2: Run tests to verify they fail**

Run: `./build/tests -tc="behavior*"`
Expected: FAIL.

- [ ] **Step 3: Implement `require_only` and the two loaders, and author the example files**

- [ ] **Step 4: Run tests to verify they pass**

Run: `./build/tests -tc="behavior*"`
Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git commit -am "feat: behavior loader with strict field checking"
```

## Task 8: Boss State Machine and Phase Rules

**Files:**
- Modify: `src/gameplay/enemies.h`, `src/gameplay/enemies.cpp`
- Modify: `src/gameplay/behavior.h`, `src/gameplay/behavior.cpp` (add `load_boss_fsm`)
- Create: `Data/behaviors/example_boss.json`
- Create: `tests/fixtures/bad_last_plain.json`, `tests/fixtures/bad_target.json`, `tests/fixtures/bad_unreachable.json`
- Test: `tests/test_enemies.cpp`

**Interfaces:**
- Consumes: `Condition`, `Action`, `ConditionContext`, `ActionContext`, `EnemyScript` (Tasks 1, 2, 6)
- Produces: `struct State { std::string id; bool is_spell; int capture_bonus; std::vector<std::unique_ptr<Action>> actions; std::vector<Transition> transitions; }`. `struct Transition { std::unique_ptr<Condition> when; std::string target; }`. `class BossFsm { void tick(ActionContext&, ConditionContext&); const std::string& current() const; bool finished() const; bool captured() const; void note_bomb_used(); void note_life_lost(); }`. `BossFsm load_boss_fsm(const std::string& path)` lives in `behavior.cpp`.

Rules:
- `captured()` is true only when a spell state ended by its `health` transition, with no `note_bomb_used` and no `note_life_lost` during that state.
- A state that ends by a timer transition never captures.
- `finished()` is true after the last spell state ends.
- Load rejects: a transition to a missing state id, a state unreachable from the start state, a last state that is not a spell state, and a `health` condition in a non-boss context.

- [ ] **Step 1: Write failing tests**

```cpp
TEST_CASE("spell ended by health with no bomb or life lost is captured") { /* ... */ }
TEST_CASE("spell ended by timer is not captured") { /* ... */ }
TEST_CASE("bomb during spell prevents capture") { /* note_bomb_used then health end */ }
TEST_CASE("boss whose last state is plain is rejected at load") { CHECK_THROWS_AS(load_boss_fsm("tests/fixtures/bad_last_plain.json"), DataError); }
TEST_CASE("transition to missing state is rejected at load") { CHECK_THROWS_AS(load_boss_fsm("tests/fixtures/bad_target.json"), DataError); }
TEST_CASE("unreachable state is rejected at load") { CHECK_THROWS_AS(load_boss_fsm("tests/fixtures/bad_unreachable.json"), DataError); }
```

The `bad_*.json` fixtures live in `tests/fixtures/`, not `Data/`, so they are not loaded as content.

- [ ] **Step 2: Run tests to verify they fail**

Run: `./build/tests -tc="enemies*"`
Expected: FAIL.

- [ ] **Step 3: Implement the state machine, the capture flags reset on each state entry, and the load validation**

- [ ] **Step 4: Run tests to verify they pass**

Run: `./build/tests -tc="enemies*"`
Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git commit -am "feat: boss state machine with spell capture and load validation"
```

## Task 9: Frame Update Order and Integration

**Files:**
- Modify: `src/gameplay/behavior.h`, `src/gameplay/behavior.cpp`
- Test: `tests/test_behavior.cpp`

**Interfaces:**
- Consumes: `BulletPool` (Task 3), `EnemyScript` (Task 6), `BossFsm` (Task 8), `run_rules` (Task 5), `find_shape` and `resolve_params` (Task 4)
- Produces: `void run_frame(FrameState&)`, where `FrameState` holds the pool, the active enemies and their scripts, the boss FSM if any, and the frame counter. Fixed update order each frame: (1) enemy scripts tick and their `fire_pattern` requests are turned into `SpawnRequest`s, (2) boss FSM ticks, (3) each bullet's rules run and its requests are processed, (4) bullets move by velocity, (5) despawn requests and off-screen bullets are removed, (6) collision flags are set for the next frame's conditions.

- [ ] **Step 1: Write failing tests**

```cpp
TEST_CASE("fire_pattern from a script spawns the shape's bullets on the same frame") { /* ring of 12 */ }
TEST_CASE("bullets with the same frame order produce the same positions across two runs") { /* determinism */ }
TEST_CASE("a bullet despawned by a rule is removed the same frame") { /* ... */ }
```

- [ ] **Step 2: Run tests to verify they fail**

Run: `./build/tests -tc="behavior*"`
Expected: FAIL.

- [ ] **Step 3: Implement `run_frame` in the order above**

- [ ] **Step 4: Run tests to verify they pass**

Run: `./build/tests -tc="behavior*"`
Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git commit -am "feat: frame update order for scripts, bosses, rules, and bullets"
```

## Task 10: Debug Overlay for Failed Spawns

**Files:**
- Create: `src/render/debug_overlay.h`, `src/render/debug_overlay.cpp`
- Modify: `src/main.cpp` (toggle and draw call)
- Test: `tests/test_debug_overlay.cpp` (pure text formatting only)

**Interfaces:**
- Consumes: `BulletPool::failed_spawns()`, `BulletPool::size()`
- Produces: `std::string overlay_text(int bullets, int cap, int failed_spawns)`, which returns a line such as `bullets 1234/4096  failed spawns 0`. `void draw_overlay(const std::string&)` is the only raylib call, and it is drawn only when the debug build flag is set.

- [ ] **Step 1: Write failing test**

```cpp
TEST_CASE("overlay text shows bullets, cap, and failed spawns") {
    CHECK(overlay_text(1234, 4096, 0) == "bullets 1234/4096  failed spawns 0");
}
```

- [ ] **Step 2: Run test to verify it fails**

Run: `./build/tests -tc="debug_overlay*"`
Expected: FAIL.

- [ ] **Step 3: Implement `overlay_text`, `draw_overlay`, and the debug toggle key (F3, a value set here)**

- [ ] **Step 4: Run test to verify it passes**

Run: `./build/tests -tc="debug_overlay*"`
Expected: PASS.

- [ ] **Step 5: Manual check and commit**

Run: `./build/game` in a debug build. Press F3 and confirm the line appears. Spawn past the cap with a test script and confirm the failed-spawn count rises.

```bash
git commit -am "feat: debug overlay for bullet counts and failed spawns"
```

## Self-Review

1. **Spec coverage:** Shared building blocks and conditions (Tasks 1, 2). Enemy scripts (Task 6). Bullet rules, once and continuous (Task 5). Vertical position and lifetime conditions (Task 1, used in Task 5). Boss state machines and capture (Task 8). Shapes and numeric overrides (Task 4). Bullet pool with handles, cap, and failed-spawn counter (Tasks 3, 10). Tuning value (Task 3). Strict loading (Task 7). Update order (Task 9). Deferred items (ramps, health reactions for basic enemies) are not in any task, as specified.
2. **Step scan:** Each step is one action. Test bodies are shown as names and assertions. Bodies are left to the implementer where the signature and test determine them.
3. **Type consistency:** `ConditionContext`, `ActionContext`, `Action`, `Condition`, `Bullet`, `BulletHandle`, `Rule`, `EnemyScript`, `BossFsm`, and `SpawnRequest` keep their names across tasks.
4. **Review Focus:** Each of the six items has a test in its owning task.
5. **Proportion:** The plan gives signatures, files, and test assertions, and leaves bodies to the implementer.

## Execution Handoff

The implementer is the project owner, who is writing the code directly. Each task ends with a passing test and a commit, so progress can be checked task by task.

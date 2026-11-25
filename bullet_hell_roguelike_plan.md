# Bullet-Hell Roguelike Clean C++ Rewrite Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Build a playable C++ roguelike engine where all stats, waves, bosses, stages, routes, dialogue, cutscenes, and upgrades load from JSON without recompiling.

**Architecture:** Four layers: data (JSON loading), game logic (run, map, stage, wave, boss, currency, cutscenes, save; no raylib calls), rendering (ASCII grid drawn with raylib), and platform (window, borderless mode, scaling, input, audio). Game logic depends only on data and its own classes, so pixel art can replace the rendering layer later.

**Tech Stack:** C++17, CMake, raylib (FetchContent, pinned to the version in the existing CMakeLists.txt), cJSON (already in repo), miniaudio (already in repo), doctest (single-header test framework, vendored into `third_party/`).

**Spec:** `bullet_hell_roguelike_design.md` (the approved design doc). Where this plan and the spec disagree, the spec wins.

## Assumptions

These were open in the design doc. This plan assumes the defaults below. Change them before execution if needed.

- C++17 standard.
- No skip option in the upgrade shop for v1.
- A locked harder exit is shown on the map as locked, with a label, and its reason is explained through dialogue.

## Global Constraints

- Language: C++17.
- Test framework: doctest, single header, vendored.
- Playfield: a fixed character grid of 50 columns by 30 rows (to be tuned later).
- Continues per run: 5.
- Bombs at start: 3. Bomb cap: 10.
- Speed factor: 1.0 up to 30 seconds, falling linearly to 0.6 at 90 seconds.
- Lives penalty: 0.1 per life lost (placeholder, tuned later).
- Multiplier clamp: minimum 0.6, maximum 1.0, applied after the speed factor and lives penalty are combined.
- Shop offer size: 8 upgrades. Each upgrade can be bought once per run.
- Ships are chosen before a run and fixed for it.
- Two ship types for now: a cannon ship (high damage, narrow forward cone, normal speed) and a missile ship (low-damage homing missiles, slower speed). Each stage must be balanced for both.
- Score and currency are separate. Every currency gain adds the same amount to score. Spending reduces currency only.
- All content lives in JSON. Balance values live in `Data/tuning.json`.
- No gameplay code calls raylib. Rendering reads game state and never writes it.
- Visual clarity is vital. Color coding distinguishes players, enemies, bosses, and backgrounds.
- Bullets never blend into the background. Every bullet color must meet a contrast ratio of at least 4.5:1 against every background color it can appear over (WCAG relative luminance).
- Player bullets and enemy bullets use colors that are distinct from each other and from the player, enemy, and boss colors.
- The palette lives in `Data/palette.json`, so colors can be changed without recompiling.
- The game has a sci-fi theme. Gameplay visuals, HUD, and screens use sci-fi styling where possible.
- Each boss is based on a sci-fi concept, and each boss's concept is recorded in its data.
- Bosses and player ships each have a hand-drawn portrait, loaded from a file path in their data.

## Review Focus

These are inputs the spec implies but no task's tests would exercise by default. Each has a test in the task that owns it.

1. A malformed or missing JSON file must produce a clear error naming the file, not a crash. (Task 2)
2. A stage whose harder exit is locked must refuse the transition, and the input must not change the run's difficulty. (Task 10)
3. A continue after the last continue must end the run and not restore lives. (Task 11)
4. A suspended run must not be loadable twice. Resuming must delete the suspend file. (Task 13)
5. A clear time of 0 seconds, or lives lost greater than the cap, must produce a multiplier inside the clamp range. (Task 4)

## File Structure

- `src/data/json_loader.h/.cpp`: wraps cJSON. Reads a file, reads typed fields, reports errors with file names.
- `src/data/tuning.h/.cpp`: loads and holds `tuning.json` values.
- `src/economy/currency.h/.cpp`: payout multiplier, wave and boss payouts, currency and score ledger.
- `src/content/bullets.h/.cpp`: bullet type and weapon definitions.
- `src/content/patterns.h/.cpp`: reusable bullet patterns.
- `src/content/waves.h/.cpp`: wave definitions and wave pools.
- `src/content/bosses.h/.cpp`: boss definitions and phases.
- `src/content/stages.h/.cpp`: stage definitions.
- `src/content/map.h/.cpp`: route graph, exits, gates, difficulty, endings.
- `src/content/upgrades.h/.cpp`: upgrade pool and shop offers.
- `src/content/cutscenes.h/.cpp`: cutscene scripts and triggers.
- `src/gameplay/enemy.h/.cpp`: enemy with multiple weapons.
- `src/gameplay/wave_runner.h/.cpp`: runs a wave on a frame timeline.
- `src/gameplay/boss_fight.h/.cpp`: runs boss phases, spell card rules, capture bonus.
- `src/gameplay/player.h/.cpp`: lives, bombs, specials and cooldowns.
- `src/run/run.h/.cpp`: run state (lives, continues, bombs, currency, score, upgrades, route flags).
- `src/run/cutscene_queue.h/.cpp`: queues cutscenes, pauses gameplay, fires triggers.
- `src/run/save.h/.cpp`: suspend and resume, high scores.
- `src/render/grid.h/.cpp`: ASCII character grid and cell drawing with raylib.
- `src/render/screens.h/.cpp`: title, results, map, shop, cutscene screens.
- `src/platform/window.h/.cpp`: window, borderless toggle, scaling.
- `src/platform/input.h/.cpp`: key mapping (arrows and WASD move, Space fires, X bomb, C special, D swap, P pause).
- `src/main.cpp`: state machine that wires screens to run logic.
- `tests/`: one test file per task area.

## Task 1: Project Skeleton and Test Harness

**Files:**
- Modify: `CMakeLists.txt` (replace the raylib template version)
- Create: `third_party/doctest/doctest.h`
- Create: `tests/test_smoke.cpp`
- Create: `src/main.cpp` (empty window that opens and closes)

**Interfaces:**
- Consumes: none
- Produces: CMake targets `game` (executable) and `tests` (doctest executable). Both link against raylib and cJSON.

- [ ] **Step 1: Write the failing smoke test**

```cpp
TEST_CASE("test harness runs") { CHECK(1 + 1 == 2); }
```

- [ ] **Step 2: Run test to verify it fails**

Run: `cmake --build build --target tests`
Expected: FAIL, because the target does not exist yet.

- [ ] **Step 3: Write CMake with `game` and `tests` targets, fetching raylib at the pinned version and compiling cJSON and doctest's main**

- [ ] **Step 4: Run test to verify it passes**

Run: `./build/tests`
Expected: all test cases pass.

- [ ] **Step 5: Build the game target**

Run: `cmake --build build --target game`
Expected: builds without warnings. Running `game` opens a window and closes on Escape.

- [ ] **Step 6: Commit**

```bash
git commit -am "build: CMake skeleton with raylib, cJSON, doctest"
```

## Task 2: JSON Loader

**Files:**
- Create: `src/data/json_loader.h`, `src/data/json_loader.cpp`
- Test: `tests/test_json_loader.cpp`

**Interfaces:**
- Consumes: none
- Produces: `JsonDoc load_json(const std::string& path)` throws `DataError` with the file name and reason on failure. `JsonDoc` exposes `int get_int(key)`, `float get_float(key)`, `std::string get_string(key)`, and `bool has(key)`. A missing key or wrong type throws `DataError` naming the key and file.

- [ ] **Step 1: Write failing tests**

Test cases: a valid file loads and returns an int; a missing file throws `DataError` whose message contains the file path; a wrong-type field throws `DataError` whose message contains the key name.

- [ ] **Step 2: Run tests to verify they fail**

Run: `./build/tests -tc="json*"`
Expected: FAIL, functions not defined.

- [ ] **Step 3: Implement `load_json` and the typed getters**

- [ ] **Step 4: Run tests to verify they pass**

Run: `./build/tests -tc="json*"`
Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git commit -am "feat: JSON loader with named errors"
```

## Task 3: Tuning Config

**Files:**
- Create: `src/data/tuning.h`, `src/data/tuning.cpp`
- Create: `Data/tuning.json`
- Test: `tests/test_tuning.cpp`

**Interfaces:**
- Consumes: `load_json`, `JsonDoc` (Task 2)
- Produces: `struct Tuning { int continues; int bombs_start; int bombs_max; float speed_full_seconds; float speed_floor_seconds; float speed_floor; float lives_penalty; float multiplier_min; float multiplier_max; int shop_size; }` and `Tuning load_tuning(const std::string& path)`.

- [ ] **Step 1: Write failing test**

Test: `load_tuning("Data/tuning.json")` returns `continues == 5`, `bombs_start == 3`, `bombs_max == 10`, `speed_full_seconds == 30.0f`, `speed_floor_seconds == 90.0f`, `speed_floor == 0.6f`, `lives_penalty == 0.1f`, `multiplier_min == 0.6f`, `multiplier_max == 1.0f`, `shop_size == 8`.

- [ ] **Step 2: Run test to verify it fails**

Run: `./build/tests -tc="tuning*"`
Expected: FAIL.

- [ ] **Step 3: Create `Data/tuning.json` with the values above, then implement `load_tuning`**

- [ ] **Step 4: Run test to verify it passes**

Run: `./build/tests -tc="tuning*"`
Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git commit -am "feat: tuning config loaded from JSON"
```

## Task 4: Payout Multiplier and Wave Payout

**Files:**
- Create: `src/economy/currency.h`, `src/economy/currency.cpp`
- Test: `tests/test_currency.cpp`

**Interfaces:**
- Consumes: `Tuning` (Task 3)
- Produces: `float speed_factor(float clear_seconds, const Tuning& t)`, `float payout_multiplier(float clear_seconds, int lives_lost, const Tuning& t)`, `int wave_payout(int base_payout, float clear_seconds, int lives_lost, const Tuning& t)`. The multiplier is `speed_factor - lives_lost * t.lives_penalty`, clamped to `[multiplier_min, multiplier_max]`.

- [ ] **Step 1: Write failing tests**

Using the default tuning:
- `speed_factor(0, t) == 1.0f`
- `speed_factor(30, t) == 1.0f`
- `speed_factor(60, t) == 0.8f` (linear from 1.0 at 30s to 0.6 at 90s)
- `speed_factor(90, t) == 0.6f`
- `speed_factor(500, t) == 0.6f`
- `payout_multiplier(30, 0, t) == 1.0f`
- `payout_multiplier(30, 3, t) == 0.7f`
- `payout_multiplier(30, 10, t) == 0.6f` (clamped to the minimum)
- `payout_multiplier(0, 0, t) == 1.0f` (zero clear time is valid)
- `wave_payout(100, 30, 0, t) == 100`

- [ ] **Step 2: Run tests to verify they fail**

Run: `./build/tests -tc="currency*"`
Expected: FAIL.

- [ ] **Step 3: Implement the three functions**

- [ ] **Step 4: Run tests to verify they pass**

Run: `./build/tests -tc="currency*"`
Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git commit -am "feat: wave payout multiplier from clear time and lives lost"
```

## Task 5: Currency and Score Ledger

**Files:**
- Modify: `src/economy/currency.h`, `src/economy/currency.cpp`
- Test: `tests/test_ledger.cpp`

**Interfaces:**
- Consumes: none beyond Task 4
- Produces: `class Ledger { void earn(int amount); bool spend(int amount); int currency() const; int score() const; }`. `earn` adds the amount to both currency and score. `spend` subtracts from currency only, and returns false without changing anything if currency is less than the amount.

- [ ] **Step 1: Write failing tests**

- `earn(100)` gives currency 100 and score 100.
- After `earn(100)`, `spend(40)` gives currency 60 and score 100.
- `spend(500)` with currency 60 returns false and leaves currency at 60.

- [ ] **Step 2: Run tests to verify they fail**

Run: `./build/tests -tc="ledger*"`
Expected: FAIL.

- [ ] **Step 3: Implement `Ledger`**

- [ ] **Step 4: Run tests to verify they pass**

Run: `./build/tests -tc="ledger*"`
Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git commit -am "feat: currency and score ledger"
```

## Task 6: Bullet Types and Weapons Data

**Files:**
- Create: `src/content/bullets.h`, `src/content/bullets.cpp`
- Create: `Data/bullet_types.json`, `Data/weapons.json` (port from existing files, keeping their field names)
- Test: `tests/test_bullets.cpp`

**Interfaces:**
- Consumes: `load_json`, `JsonDoc` (Task 2)
- Produces: `BulletRegistry load_bullets(path)` and `WeaponRegistry load_weapons(path, const BulletRegistry&)`. Each bullet type has a `color` field and an `owner` field (`Player` or `Enemy`), so the renderer can color bullets by owner. A weapon referencing an unknown bullet name fails with `DataError` naming both the weapon and the bullet.

- [ ] **Step 1: Write failing tests**

- Loading `Data/bullet_types.json` returns a registry where `find("BULLET")` succeeds.
- A weapon referencing a bullet name that does not exist throws `DataError` whose message contains that name. Use a small inline test file.

- [ ] **Step 2: Run tests to verify they fail**

Run: `./build/tests -tc="bullets*"`
Expected: FAIL.

- [ ] **Step 3: Implement the loaders and port the JSON files**

- [ ] **Step 4: Run tests to verify they pass**

Run: `./build/tests -tc="bullets*"`
Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git commit -am "feat: bullet and weapon data loaded from JSON"
```

## Task 7: Patterns, Waves, and Wave Pools

**Files:**
- Create: `src/content/patterns.h`, `src/content/patterns.cpp`
- Create: `src/content/waves.h`, `src/content/waves.cpp`
- Create: `Data/patterns.json` (port), `Data/waves/example_wave.json`, `Data/wave_pools/example_pool.json`
- Test: `tests/test_waves.cpp`

**Interfaces:**
- Consumes: `load_json`, `BulletRegistry` (Task 6)
- Produces: `struct Wave { std::string id; int base_payout; int duration_frames; std::vector<WaveSpawn> spawns; }` where `WaveSpawn` holds `start_frame`, `pattern`, `x`, `y`. `struct WavePool` maps wave ids to a weight for each difficulty. `Wave load_wave(path)`, `WavePool load_pool(path)`, and `std::string pick_wave(const WavePool&, Difficulty, Rng&)`.

- [ ] **Step 1: Write failing tests**

- `load_wave("Data/waves/example_wave.json")` returns spawns sorted by `start_frame`.
- A pool entry with weight 0 is never picked over 1000 draws, using a seeded `Rng`.
- A spawn referencing an undefined pattern throws `DataError` naming the pattern.

- [ ] **Step 2: Run tests to verify they fail**

Run: `./build/tests -tc="waves*"`
Expected: FAIL.

- [ ] **Step 3: Implement the loaders and picker**

- [ ] **Step 4: Run tests to verify they pass**

Run: `./build/tests -tc="waves*"`
Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git commit -am "feat: wave, pattern, and weighted wave pool loading"
```

## Task 8: Enemy and Wave Runner

**Files:**
- Create: `src/gameplay/enemy.h`, `src/gameplay/enemy.cpp`
- Create: `src/gameplay/wave_runner.h`, `src/gameplay/wave_runner.cpp`
- Create: `Data/enemies.json` (port)
- Test: `tests/test_wave_runner.cpp`

**Interfaces:**
- Consumes: `Wave` (Task 7), `WeaponRegistry` (Task 6)
- Produces: `class Enemy` holding `std::vector<Weapon>`, so it can carry several weapons at once. `class WaveRunner { explicit WaveRunner(const Wave&); void tick(); bool finished() const; int frame() const; float elapsed_seconds() const; }`. `tick` advances one frame and spawns enemies whose `start_frame` has been reached. `elapsed_seconds` is `frame / 60.0f`.

- [ ] **Step 1: Write failing tests**

- A wave with one spawn at frame 10: after 9 ticks no enemy exists; after 10 ticks one exists.
- `finished()` is false at `duration_frames - 1` and true at `duration_frames`.
- An enemy with two weapons holds both.

- [ ] **Step 2: Run tests to verify they fail**

Run: `./build/tests -tc="wave_runner*"`
Expected: FAIL.

- [ ] **Step 3: Implement `Enemy` and `WaveRunner`**

- [ ] **Step 4: Run tests to verify they pass**

Run: `./build/tests -tc="wave_runner*"`
Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git commit -am "feat: enemies with multiple weapons and frame-based wave runner"
```

## Task 9: Boss Phases and Capture Bonus

**Files:**
- Create: `src/content/bosses.h`, `src/content/bosses.cpp`
- Create: `src/gameplay/boss_fight.h`, `src/gameplay/boss_fight.cpp`
- Create: `Data/bosses/example_boss.json`
- Test: `tests/test_boss_fight.cpp`

**Interfaces:**
- Consumes: `load_json` (Task 2)
- Produces: `struct Boss` also has `std::string theme` (the sci-fi concept, for example "swarm intelligence" or "gravity collapse") and `std::string portrait` (file path, see Task 17). `enum class PhaseKind { Plain, Spell }`, `struct Phase { PhaseKind kind; int timer_frames; float health_threshold; std::vector<std::string> patterns; int capture_bonus; }`, `struct Boss { std::string id; int clear_bonus; std::vector<Phase> phases; }`, and `Boss load_boss(path)`. `class BossFight` tracks the current phase, whether a bomb or life was used during it, and whether the fight has finished.

Rules:
- The loader rejects a boss whose last phase is Plain, and any Plain phase that comes after the last Spell phase.
- A phase ends when its health falls to or below `health_threshold`, or when its timer reaches `timer_frames`. Either ends the phase, and the boss moves to the next one.
- `captured()` is true only when a Spell phase ended by health threshold, with no bomb and no lost life during it. A timer ending never captures.
- `finished()` is true after the last Spell phase ends.

- [ ] **Step 1: Write failing tests**

- A boss whose last phase is Plain fails to load with `DataError`.
- A Spell phase ended by timer: `captured()` is false, and the fight moves to the next phase.
- A Spell phase ended by health with no bomb and no life lost: `captured()` is true.
- A Spell phase ended by health with one life lost: `captured()` is false.
- `finished()` is false after a Plain phase ends and true after the last Spell phase ends.

- [ ] **Step 2: Run tests to verify they fail**

Run: `./build/tests -tc="boss*"`
Expected: FAIL.

- [ ] **Step 3: Implement boss loading and `BossFight`**

- [ ] **Step 4: Run tests to verify they pass**

Run: `./build/tests -tc="boss*"`
Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git commit -am "feat: boss phases, spell cards, and capture bonus"
```

## Task 10: Stages, Map Graph, Gates, and Endings

**Files:**
- Create: `src/content/stages.h`, `src/content/stages.cpp`
- Create: `src/content/map.h`, `src/content/map.cpp`
- Create: `Data/stages/*.json`, `Data/map.json`, `Data/endings.json`
- Test: `tests/test_map.cpp`

**Interfaces:**
- Consumes: `load_json` (Task 2), `load_pool` (Task 7), `load_boss` (Task 9)
- Produces: `enum class Difficulty { Easy, Medium, Hard }`, `struct Exit { std::string target_stage; std::string gate; }`, `struct StageDef { std::string id; Difficulty difficulty; std::string background; std::string wave_pool; std::string boss; Exit harder_exit; Exit easier_exit; }`, `class MapGraph` with `const StageDef& stage(id)` and `std::vector<Exit> available_exits(id, const RunStats&)`. `Difficulty step_difficulty(Difficulty, bool harder)` clamps at the ends. `std::string select_ending(Difficulty source)` returns `"secret"`, `"neutral"`, or `"bad"`.

Rules:
- `harder_exit` is unavailable when its gate fails. `RunStats` holds the run's lives lost, continues used, and score for the stage.
- Stepping past an end keeps the same difficulty.
- The loader rejects a map where some path cannot reach the final stage.

- [ ] **Step 1: Write failing tests**

- `step_difficulty(Easy, false) == Easy` (capped).
- `step_difficulty(Hard, true) == Hard` (capped).
- `step_difficulty(Medium, true) == Hard`.
- With the gate failed, `available_exits` omits the harder exit.
- `select_ending(Hard) == "secret"`, `select_ending(Medium) == "neutral"`, `select_ending(Easy) == "bad"`.
- A map with an unreachable final stage fails to load with `DataError`.

- [ ] **Step 2: Run tests to verify they fail**

Run: `./build/tests -tc="map*"`
Expected: FAIL.

- [ ] **Step 3: Implement stage loading, `MapGraph`, and the endings table**

- [ ] **Step 4: Run tests to verify they pass**

Run: `./build/tests -tc="map*"`
Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git commit -am "feat: branching map with difficulty gates and endings"
```

## Task 11: Player and Run State

**Files:**
- Create: `src/gameplay/player.h`, `src/gameplay/player.cpp`
- Create: `src/run/run.h`, `src/run/run.cpp`
- Create: `src/content/upgrades.h`, `src/content/upgrades.cpp`
- Create: `Data/upgrades.json`, `Data/players.json` (port)
- Test: `tests/test_run.cpp`

**Interfaces:**
- Consumes: `Tuning` (Task 3), `Ledger` (Task 5)
- Produces:
  - `class Player` with `lives`, `max_lives`, `bombs`, per-special cooldown timers, `equip_special(index)`, `use_special(index)` (fails during cooldown), and `use_bomb()` (fails at zero bombs).
  - `struct UpgradeDef { std::string id; UpgradeKind kind; }` where `UpgradeKind` is `Special`, `Weapon`, `Stat`, or `Life`. `std::vector<UpgradeDef> roll_shop(const UpgradePool&, const std::set<std::string>& owned, int count, Rng&)`.
  - `class Run` with `Ledger ledger`, `Player player`, `int continues_left`, `std::set<std::string> bought`, `bool hard_locked`, `bool continue_run()`, `bool buy(const UpgradeDef&)`, and `void lose_life()`.

Rules:
- `continue_run()` restores max lives, sets `hard_locked = true`, decrements `continues_left`, and returns true. When `continues_left` is 0 it returns false and does not restore lives, and the caller ends the run. Bullets are cleared by the caller, once, on a successful continue.
- `buy` fails for an upgrade already in `bought`.
- Each special's cooldown is independent. Swapping an equipped special does not reset its timer.
- `roll_shop` never returns an owned upgrade and returns at most `count` distinct items.
- `Data/players.json` defines two ships: a cannon ship and a missile ship. Each has base speed, a starting weapon (a forward cone weapon for the cannon ship, homing missiles for the missile ship), and a `portrait` path. The loader rejects a ship whose starting weapon does not exist in `weapons.json`.

- [ ] **Step 1: Write failing tests**

- Continue with `continues_left == 5` restores lives, sets `hard_locked`, and decrements to 4.
- Continue with `continues_left == 0` returns false and leaves lives at 0.
- Buying the same upgrade twice: the second returns false.
- Using a special puts it on cooldown. Swapping to another special and back keeps the first special's remaining cooldown.
- `use_bomb()` with zero bombs returns false.
- `roll_shop` with a pool of 20 and `count == 8` returns 8 distinct items, none already owned.

- [ ] **Step 2: Run tests to verify they fail**

Run: `./build/tests -tc="run*"`
Expected: FAIL.

- [ ] **Step 3: Implement `Player`, `Run`, upgrade loading, and `roll_shop`**

- [ ] **Step 4: Run tests to verify they pass**

Run: `./build/tests -tc="run*"`
Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git commit -am "feat: run state with continues, bombs, specials, and shop"
```

## Task 12: Cutscene Queue and Triggers

**Files:**
- Create: `src/content/cutscenes.h`, `src/content/cutscenes.cpp`
- Create: `src/run/cutscene_queue.h`, `src/run/cutscene_queue.cpp`
- Create: `Data/cutscenes/example.json`
- Test: `tests/test_cutscenes.cpp`

**Interfaces:**
- Consumes: `load_json` (Task 2)
- Produces: `struct Trigger { enum class Kind { Frame, Event } kind; int frame; std::string event; }`, `struct CutsceneDef { std::string id; Trigger trigger; std::vector<std::string> lines; }`, and `class CutsceneQueue { void on_frame(int frame); void on_event(const std::string& name); bool paused() const; const CutsceneDef* current() const; void advance(); }`.

Rules:
- Triggers that fire while another cutscene is playing are queued, and play in the order they fired.
- `paused()` is true while any cutscene is current or queued. The game loop does not tick waves, bosses, bullets, or timers while paused.
- A frame trigger fires once, on the first call with `frame >= trigger.frame`.

- [ ] **Step 1: Write failing tests**

- A frame trigger at frame 5 does not fire on frame 4 and fires on frame 5.
- Two events fired at once: `current()` is the first, `advance()` moves to the second, and `paused()` stays true until both are done.
- A frame trigger does not fire twice.

- [ ] **Step 2: Run tests to verify they fail**

Run: `./build/tests -tc="cutscene*"`
Expected: FAIL.

- [ ] **Step 3: Implement the cutscene loader and `CutsceneQueue`**

- [ ] **Step 4: Run tests to verify they pass**

Run: `./build/tests -tc="cutscene*"`
Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git commit -am "feat: cutscene queue with frame and event triggers"
```

## Task 13: Suspend, Resume, and High Scores

**Files:**
- Create: `src/run/save.h`, `src/run/save.cpp`
- Test: `tests/test_save.cpp`

**Interfaces:**
- Consumes: `Run` (Task 11)
- Produces: `void suspend_run(const Run&, const std::string& path)`, `std::optional<Run> resume_run(const std::string& path)`, `void record_high_score(int score, const std::string& path)`, and `std::vector<int> load_high_scores(const std::string& path)`.

Rules:
- `resume_run` deletes the suspend file after reading it. A second call on the same path returns `std::nullopt`.
- Suspend files are written atomically: write to a temp file, then rename.
- High scores persist and stay sorted, highest first.

- [ ] **Step 1: Write failing tests**

- Suspend then resume restores currency, score, lives, and continues.
- After `resume_run`, the suspend file is gone, and a second `resume_run` returns `std::nullopt`.
- `record_high_score` keeps the list sorted descending across two calls.

- [ ] **Step 2: Run tests to verify they fail**

Run: `./build/tests -tc="save*"`
Expected: FAIL.

- [ ] **Step 3: Implement suspend, resume, and high score functions**

- [ ] **Step 4: Run tests to verify they pass**

Run: `./build/tests -tc="save*"`
Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git commit -am "feat: suspend and resume with single-use suspend file"
```

## Task 14: ASCII Grid Rendering and Window Scaling

**Files:**
- Create: `src/render/grid.h`, `src/render/grid.cpp`
- Create: `src/platform/window.h`, `src/platform/window.cpp`
- Test: `tests/test_grid.cpp` (pure math only; drawing is checked by running the game)

**Interfaces:**
- Consumes: none from game logic
- Produces: `struct Palette` loaded from `Data/palette.json`, with named roles for player, enemy, boss, player bullet, enemy bullet, HUD, and each background. `float contrast_ratio(Color a, Color b)` implements the WCAG formula. `struct GridSize { int cols; int rows; }` with `GridSize playfield()` returning 100 by 26. `int cell_size_for(int window_w, int window_h, GridSize)` returns the largest whole-pixel cell size that fits, never less than 1. `struct CharGrid` holds a character and a color per cell. `void draw_grid(const CharGrid&, int cell)` is the only function that draws gameplay with raylib. `void set_borderless(bool)` lives in `window.cpp`.

- [ ] **Step 1: Write failing tests**

- `cell_size_for` returns the cell size that fits 100 by 26 cells in a given window, computed from the font's character aspect.
- `cell_size_for(1, 1, playfield())` returns 1, not 0.
- For every bullet color in `Data/palette.json` and every background color, `contrast_ratio` is at least 4.5. A bullet color that fails this check fails the test and names the pair.
- The player, enemy, and boss colors are pairwise distinct, and each differs from every background color.

- [ ] **Step 2: Run tests to verify they fail**

Run: `./build/tests -tc="grid*"`
Expected: FAIL.

- [ ] **Step 3: Implement `cell_size_for`, `CharGrid`, `draw_grid`, and the window helper**

- [ ] **Step 4: Run tests to verify they pass**

Run: `./build/tests -tc="grid*"`
Expected: PASS.

- [ ] **Step 5: Manual check and commit**

Run: `./build/game` and toggle borderless with F11. The grid fills the screen with no border, and resizing rescales it.

```bash
git commit -am "feat: ASCII grid rendering with scaling and borderless mode"
```

## Task 15: Screens and Main Loop

**Files:**
- Create: `src/render/screens.h`, `src/render/screens.cpp`
- Create: `src/platform/input.h`, `src/platform/input.cpp`
- Modify: `src/main.cpp`
- Test: `tests/test_state.cpp`

**Interfaces:**
- Consumes: the game-logic classes from Tasks 4 to 13
- Produces: a screen state machine with states `Title`, `Playing`, `Cutscene`, `Results`, `Map`, `Shop`, `Ending`, and `Quit`. Each state has `update(input)` and `draw()`.

Rules:
- Gameplay ticks only in `Playing` and when no cutscene is pausing it.
- HUD and screens use the palette from Task 14 and a sci-fi look: panel borders, monospace readouts, and status labels. No screen uses colors outside the palette.
- After a stage, `Results` comes first, then `Map`. After a boss, `Shop` comes before `Map`.

- [ ] **Step 1: Write failing tests for transitions**

- From `Playing`, a boss defeat moves to `Shop`.
- From `Shop`, leaving moves to `Map`.
- From `Map`, choosing an exit moves to `Playing` with the new stage.
- From `Playing`, a stage end moves to `Results`.

- [ ] **Step 2: Run tests to verify they fail**

Run: `./build/tests -tc="state*"`
Expected: FAIL.

- [ ] **Step 3: Implement the state machine, screens, and input mapping, and wire them in `main.cpp`**

- [ ] **Step 4: Run tests to verify they pass**

Run: `./build/tests -tc="state*"`
Expected: PASS.

- [ ] **Step 5: Manual playtest and commit**

Run: `./build/game`, play stage 1 to its boss, then the shop, then the map. Confirm each screen appears in order.

```bash
git commit -am "feat: screen state machine and main loop"
```

## Task 16: Content Port and Playtest

**Files:**
- Create: `Data/stages/*.json`, `Data/waves/*.json`, `Data/wave_pools/*.json`, `Data/bosses/*.json`, `Data/map.json`, `Data/endings.json`
- Test: `tests/test_content.cpp`

**Interfaces:**
- Consumes: all loaders from Tasks 2 to 12
- Produces: a checked content set. `test_content` loads every file under `Data/` and fails on any undefined reference or any route that cannot reach the final stage.

- [ ] **Step 1: Write failing test**

Load the full `Data/` directory through the loaders and expect no `DataError`. This fails until every file exists.

- [ ] **Step 2: Run test to verify it fails**

Run: `./build/tests -tc="content*"`
Expected: FAIL, naming the first missing reference.

- [ ] **Step 3: Author content files until the test passes**

- [ ] **Step 4: Run test to verify it passes**

Run: `./build/tests -tc="content*"`
Expected: PASS.

- [ ] **Step 5: Playtest and commit**

Play a full run from title to ending on each of the three routes, once with the cannon ship and once with the missile ship. Each stage should be clearable by both ships, and each should favor one ship's strengths. Tune balance numbers in `Data/tuning.json` and the wave data, without recompiling.

```bash
git commit -am "content: first playable route and checked data set"
```

## Task 17: Portraits, Theme Assets, and Visual Check

**Files:**
- Create: `src/render/portrait.h`, `src/render/portrait.cpp`
- Create: `Data/palette.json`
- Create: `assets/portraits/` (hand-drawn portraits, one per ship and per boss)
- Create: `assets/backgrounds/` (one background per stage theme)
- Modify: `Data/players.json`, `Data/bosses/*.json` (add `portrait` fields), `Data/stages/*.json` (add `background` fields)
- Test: `tests/test_portrait.cpp`

**Interfaces:**
- Consumes: `load_json` (Task 2), `Boss` (Task 9), `StageDef` (Task 10), the screens from Task 15
- Produces: `struct Portrait { std::string path; int width; int height; }`, `Portrait load_portrait(path)`, and `void draw_portrait(const Portrait&, Rect)`. `Data/palette.json` provides the palette used in Task 14.

Rules:
- A portrait is shown on the boss intro, the shop screen for the ship, and cutscenes that name a speaker. Every named character who speaks in a cutscene has a `portrait` field in their cutscene data, the same as ships and bosses.
- The title screen draws its art from a `title_art` path in `Data/tuning.json` or a dedicated title data file. A missing title image shows a plain title, with no crash.
- Placeholder frames keep the same size as the final art, so swapping in real images does not change the layout.
- A missing portrait file shows a neutral placeholder frame with the name, and logs a `DataError` as a warning. It does not crash.
- Portrait images are not scaled beyond their native size; they are scaled down to fit their frame.
- Backgrounds use the stage's `background` field and must pass the Task 14 contrast check against all bullet colors.

- [ ] **Step 1: Write failing tests**

- `load_portrait` on a valid file returns non-zero width and height.
- `load_portrait` on a missing file returns the placeholder and records a warning, without throwing.
- Every ship in `Data/players.json` and every boss in `Data/bosses/` has a `portrait` field that points to an existing file.
- Every stage's background passes the bullet contrast check from Task 14.

- [ ] **Step 2: Run tests to verify they fail**

Run: `./build/tests -tc="portrait*"`
Expected: FAIL.

- [ ] **Step 3: Implement portrait loading, the placeholder, and `draw_portrait`, and add the `portrait` and `background` fields to the data**

- [ ] **Step 4: Run tests to verify they pass**

Run: `./build/tests -tc="portrait*"`
Expected: PASS.

- [ ] **Step 5: Visual check and commit**

Run: `./build/game` and check each boss intro, the ship shop card, and one stage of each sci-fi theme. Confirm that player bullets, enemy bullets, and the player ship are readable against each background, and that each portrait displays without distortion.

```bash
git commit -am "feat: portraits, sci-fi palette, and stage backgrounds"
```

## Self-Review

1. **Spec coverage:** Map and endings (Task 10), stages and wave pools (Tasks 7, 10), waves and currency (Task 4), score and currency (Task 5), bosses and spell cards (Task 9), bombs and specials (Task 11), upgrades (Task 11), cutscenes (Task 12), continues and death (Task 11), suspend and high scores (Task 13), data layout (Tasks 2, 3, 6, 7, 9, 10, 11, 12, 16), rendering and scaling (Task 14), screens (Task 15), enemies with multiple weapons (Task 8), visual clarity and palette (Tasks 6, 14, 15), sci-fi theme and boss concepts (Tasks 9, 10, 17), portraits (Task 17). All covered.
2. **Step scan:** Each step is one action. Code blocks show only test names and assertions. Signatures are in Interfaces. No function bodies.
3. **Type consistency:** `Tuning`, `Ledger`, `Wave`, `Boss`, `Phase`, `MapGraph`, `Run`, `Player`, and `CutsceneQueue` keep one name and signature across the tasks that use them.
4. **Review Focus:** Each of the five inputs has a test in its owning task.
5. **Proportion:** The plan describes decisions, tests, and signatures rather than implementation.

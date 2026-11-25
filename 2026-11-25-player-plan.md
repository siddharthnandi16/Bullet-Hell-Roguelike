# Player, Ships, Upgrades, and Weapon Power-Up Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking. This plan replaces Task 11 of `bullet_hell_roguelike_plan.md`.

**Goal:** Build the `Player` class, JSON-defined ships and upgrades, a shared rule registry, and the main weapon power-up, so that new ships and upgrades need no C++ when they reuse existing rules and effects.

**Architecture:** `Player` holds run state and exposes actions. It has no input code. The platform layer maps keys to those actions. Ships and upgrades are JSON entries that reference effects and named rules. One rule registry in C++ defines each rule name once and marks it general or special-only. `Run` owns the bought set, the shop, and the stage-clear power-up.

**Tech Stack:** C++17, CMake, doctest, cJSON through the existing `load_json`.

**Spec:** `2026-11-25-player-design.md`. Where this plan and the spec disagree, the spec wins.

## Global Constraints

- Language: C++17.
- Ships and upgrades are JSON. Bombs are the same for every ship.
- Bombs: start 3, cap 10 (`Data/tuning.json`).
- Continues per run: 5.
- Score and currency are separate. Every currency gain adds the same amount to score. Spending reduces currency only.
- Each upgrade can be bought once per run.
- Each special keeps its own cooldown timer. Swapping does not reset it.
- The suspend file stores upgrade ids, never copies of upgrade data.
- No gameplay code calls raylib or reads key codes.
- Unknown field names and unknown rule or effect names fail at load with `DataError` naming the file, entry, and key.

## Review Focus

1. An unknown rule name on a ship or upgrade fails at load, naming the file, the entry, and the rule. (Tasks 1, 2, 5)
2. A special-only rule on a non-special upgrade fails at load, naming the upgrade and the rule. (Task 5)
3. Using a bomb with zero bombs does nothing. A bomb grant never raises bombs above 10. (Tasks 3, 6)
4. A power-up at the highest weapon level does nothing and does not crash. (Task 7)
5. A suspended run restores its ship, owned upgrades, and weapon level exactly, and the suspend file cannot be loaded twice. Buying the same upgrade twice fails. (Task 8)

## File Structure

- `src/gameplay/rules.h/.cpp`: rule registry. Maps names to behavior hooks and their general or special-only flag.
- `src/gameplay/ship.h/.cpp`: ship definition and `players.json` loader.
- `src/gameplay/player.h/.cpp`: `Player` state and actions.
- `src/content/upgrades.h/.cpp`: upgrade definition, effect list, and `upgrades.json` loader.
- `src/content/weapons.h/.cpp`: adds `levels` to the main weapon definition.
- `src/run/run.h/.cpp`: bought set, shop, stage-clear power-up, continues.
- `src/run/save.h/.cpp`: writes and reads ship id, owned upgrades, and weapon level.
- `src/platform/input.h/.cpp`: maps keys to Player and Run actions.
- `Data/players.json`, `Data/upgrades.json`, `Data/weapons.json`: content.
- `tests/test_rules.cpp`, `tests/test_ship.cpp`, `tests/test_player.cpp`, `tests/test_upgrades.cpp`, `tests/test_run_player.cpp`, `tests/test_input.cpp`.

---

## Task 1: Rule Registry

**Files:**
- Create: `src/gameplay/rules.h`, `src/gameplay/rules.cpp`
- Test: `tests/test_rules.cpp`

**Interfaces:**
- Consumes: `DataError` (existing)
- Produces: `enum class RuleScope { General, SpecialOnly }`. `struct RuleInfo { std::string name; RuleScope scope; }`. `const RuleInfo* find_rule(const std::string& name)` returns null for an unknown name. `void check_rule(const std::string& name, RuleScope used_on, const std::string& file, const std::string& entry)` throws `DataError` if the name is unknown, or if `used_on` is `General` and the rule is `SpecialOnly`.

- [ ] **Step 1: Write the failing tests**

```cpp
TEST_CASE("unknown rule throws naming file, entry, and rule") {
    CHECK_THROWS_WITH_AS(check_rule("no_such_rule", RuleScope::General, "players.json", "vanguard"),
                         doctest::Contains("no_such_rule"), DataError);
}
TEST_CASE("special-only rule on a non-special is rejected") {
    // use a registry entry marked SpecialOnly, used_on General
    CHECK_THROWS_AS(check_rule(special_only_name(), RuleScope::General, "upgrades.json", "x"), DataError);
}
TEST_CASE("special-only rule on a special is accepted") {
    CHECK_NOTHROW(check_rule(special_only_name(), RuleScope::SpecialOnly, "upgrades.json", "x"));
}
```

- [ ] **Step 2: Run the tests to verify they fail**

Run: `./build/tests -tc="rules*"`
Expected: FAIL, functions not defined.

- [ ] **Step 3: Implement the registry**

Use a static table. Start it with one general and one special-only placeholder entry, so the loader can be tested. Real rules are added in the tasks that need them.

- [ ] **Step 4: Run the tests to verify they pass**

Run: `./build/tests -tc="rules*"`
Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git commit -am "feat: rule registry with general and special-only scope"
```

## Task 2: Ship Definitions

**Files:**
- Create: `src/gameplay/ship.h`, `src/gameplay/ship.cpp`
- Create: `Data/players.json`
- Test: `tests/test_ship.cpp`

**Interfaces:**
- Consumes: `load_json`, `JsonDoc::require_only` (existing), `check_rule` (Task 1)
- Produces: `struct ShipStats { float speed; int max_lives; }`. `struct ShipDef { std::string id; std::string name; std::string portrait; ShipStats stats; std::string starting_weapon; std::vector<std::string> rules; }`. `std::vector<ShipDef> load_ships(const std::string& path)`. `const ShipDef* find_ship(const std::vector<ShipDef>&, const std::string& id)`.

- [ ] **Step 1: Write the failing tests**

```cpp
TEST_CASE("players.json loads vanguard with speed 4.0 and 3 lives") {
    auto ships = load_ships("Data/players.json");
    auto* v = find_ship(ships, "vanguard");
    REQUIRE(v != nullptr);
    CHECK(v->stats.speed == doctest::Approx(4.0f));
    CHECK(v->stats.max_lives == 3);
    CHECK(v->rules.empty());
}
TEST_CASE("unknown field in a ship entry is rejected") { /* inline JSON with "spead" */ }
TEST_CASE("unknown rule in a ship entry is rejected") { /* inline JSON with rules ["bogus"] */ }
```

- [ ] **Step 2: Run the tests to verify they fail**

Run: `./build/tests -tc="ship*"`
Expected: FAIL.

- [ ] **Step 3: Implement the loader and create `Data/players.json` with the Vanguard entry from the design doc**

Each object is checked with `require_only`. Each rule is checked with `check_rule(name, RuleScope::General, file, id)`.

- [ ] **Step 4: Run the tests to verify they pass**

Run: `./build/tests -tc="ship*"`
Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git commit -am "feat: ship definitions loaded from players.json"
```

## Task 3: Player State

**Files:**
- Create: `src/gameplay/player.h`, `src/gameplay/player.cpp`
- Test: `tests/test_player.cpp`

**Interfaces:**
- Consumes: `ShipDef` (Task 2), `Tuning` (existing), `Ledger` (existing)
- Produces: `class Player` with a constructor `Player(const ShipDef&, const Tuning&)`. Accessors: `int lives() const`, `int max_lives() const`, `int bombs() const`, `int score() const`, `int currency() const`, `const std::string& ship_id() const`, `const std::vector<std::string>& owned_upgrades() const`, `int weapon_level() const`. Mutators: `void lose_life()` (clamps at 0), `bool use_bomb()` (false at 0), `void add_bombs(int)` (clamps at `bombs_max`), `void add_owned(const std::string& upgrade_id)`, `void set_weapon_level(int)`. Currency and score go through an internal `Ledger`.

- [ ] **Step 1: Write the failing tests**

```cpp
TEST_CASE("new player starts with 3 bombs and no owned upgrades") { /* tuning bombs_start 3 */ }
TEST_CASE("use_bomb at zero returns false and keeps bombs at 0") { /* ... */ }
TEST_CASE("add_bombs clamps at 10") {
    Player p = test_player(); p.add_bombs(50);
    CHECK(p.bombs() == 10);
}
TEST_CASE("lose_life never drops below 0") { /* ... */ }
TEST_CASE("add_owned records the id once in order") { /* ... */ }
```

- [ ] **Step 2: Run the tests to verify they fail**

Run: `./build/tests -tc="player*"`
Expected: FAIL.

- [ ] **Step 3: Implement the state and accessors**

- [ ] **Step 4: Run the tests to verify they pass**

Run: `./build/tests -tc="player*"`
Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git commit -am "feat: player state with bombs, lives, and owned upgrades"
```

## Task 4: Player Actions and Special Cooldowns

**Files:**
- Modify: `src/gameplay/player.h`, `src/gameplay/player.cpp`
- Test: `tests/test_player.cpp`

**Interfaces:**
- Consumes: `Player` (Task 3)
- Produces: `struct Special { std::string id; float cooldown_seconds; float remaining_seconds; }`. `class Player` gains: `void equip_special(const std::string& id)`, `const Special* equipped_special() const`, `bool fire_special()` (false if none equipped or on cooldown; starts the cooldown on success), `void tick(float dt_seconds)` (reduces every special's remaining time, not only the equipped one), `bool swap_special()` (cycles to the next owned special, leaves cooldowns unchanged), `void add_special(const std::string& id, float cooldown_seconds)`.

- [ ] **Step 1: Write the failing tests**

```cpp
TEST_CASE("firing a special starts its cooldown") { /* cooldown 5s; fire then fire again returns false */ }
TEST_CASE("swapping away and back keeps the remaining cooldown") {
    // special A cooldown 5s, fire, tick 2s, swap to B, swap back to A
    // remaining on A is 3s, not 5s
}
TEST_CASE("cooldowns tick even when not equipped") { /* A fired, swap to B, tick 5s, swap back, fire A works */ }
TEST_CASE("fire_special with none equipped returns false") { /* ... */ }
```

- [ ] **Step 2: Run the tests to verify they fail**

Run: `./build/tests -tc="player*"`
Expected: FAIL.

- [ ] **Step 3: Implement the special list, cooldown ticking, and swap**

Bomb use does not touch special cooldowns. Bombs have no cooldown.

- [ ] **Step 4: Run the tests to verify they pass**

Run: `./build/tests -tc="player*"`
Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git commit -am "feat: player specials with independent cooldowns"
```

## Task 5: Upgrade Definitions

**Files:**
- Create: `src/content/upgrades.h`, `src/content/upgrades.cpp`
- Create: `Data/upgrades.json`
- Test: `tests/test_upgrades.cpp`

**Interfaces:**
- Consumes: `load_json`, `require_only`, `check_rule` (Tasks 1, 2)
- Produces: `enum class UpgradeKind { Special, Weapon, Stat, Life }`. `struct Effect { std::string kind; std::string target; float value; }` where `kind` is one of `stat`, `grant_special`, `grant_weapon`, `raise_weapon_level`, `grant_life`. `struct UpgradeDef { std::string id; UpgradeKind kind; int price; std::vector<Effect> effects; std::vector<std::string> rules; float cooldown_seconds; }`. `std::vector<UpgradeDef> load_upgrades(const std::string& path)`.

Rules:
- A `special` upgrade must set `cooldown_seconds` and may use special-only rules. Its effect list must include `grant_special`.
- A non-special upgrade may not set `cooldown_seconds`, and may not list a special-only rule.
- An unknown effect `kind` fails with `DataError` naming the file, the upgrade, and the effect.

- [ ] **Step 1: Write the failing tests**

```cpp
TEST_CASE("upgrades.json loads and every upgrade has a price") { /* ... */ }
TEST_CASE("special-only rule on a stat upgrade fails at load") { /* inline JSON, kind stat, rule special-only */ }
TEST_CASE("unknown effect kind fails naming the effect") { /* inline JSON, effect "teleport" */ }
TEST_CASE("cooldown on a non-special upgrade is rejected") { /* inline JSON */ }
```

- [ ] **Step 2: Run the tests to verify they fail**

Run: `./build/tests -tc="upgrades*"`
Expected: FAIL.

- [ ] **Step 3: Implement the loader and create `Data/upgrades.json` with at least one upgrade of each kind**

- [ ] **Step 4: Run the tests to verify they pass**

Run: `./build/tests -tc="upgrades*"`
Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git commit -am "feat: upgrade definitions with effects, rules, and special cooldowns"
```

## Task 6: Applying Upgrades to the Player

**Files:**
- Modify: `src/gameplay/player.h`, `src/gameplay/player.cpp`
- Create: `src/content/apply_upgrade.h`, `src/content/apply_upgrade.cpp`
- Test: `tests/test_player.cpp`

**Interfaces:**
- Consumes: `Player` (Tasks 3, 4), `UpgradeDef` and `Effect` (Task 5)
- Produces: `void apply_upgrade(Player&, const UpgradeDef&)`. It records the id with `add_owned`, then applies each effect: `stat` adds `value` to a named stat, `grant_special` calls `add_special`, `grant_life` calls `add_lives` (clamped to max), `raise_weapon_level` calls `set_weapon_level(weapon_level() + 1)` (the cap is enforced in Task 7). Any `grant_bomb` effect goes through `add_bombs`.

- [ ] **Step 1: Write the failing tests**

```cpp
TEST_CASE("applying a special upgrade adds the special and records the id") { /* ... */ }
TEST_CASE("applying a life grant does not exceed max lives") { /* ... */ }
TEST_CASE("applying a bomb grant never exceeds 10 bombs") { /* ... */ }
```

- [ ] **Step 2: Run the tests to verify they fail**

Run: `./build/tests -tc="apply*"`
Expected: FAIL.

- [ ] **Step 3: Implement `apply_upgrade` with a switch on the effect kind, and add `add_lives` to `Player`**

- [ ] **Step 4: Run the tests to verify they pass**

Run: `./build/tests -tc="apply*"`
Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git commit -am "feat: apply upgrade effects to the player"
```

## Task 7: Main Weapon Levels and Power-Up

**Files:**
- Modify: `src/content/weapons.h`, `src/content/weapons.cpp`
- Modify: `Data/weapons.json` (add `levels` to the main weapon)
- Modify: `src/run/run.h`, `src/run/run.cpp` (Task 8 creates `Run`; create the function here if it is not yet present)
- Test: `tests/test_weapon_levels.cpp`

**Interfaces:**
- Consumes: `Player` (Tasks 3, 6), `WeaponRegistry` (existing)
- Produces: `struct WeaponLevel { int bullet_count; float spread_degrees; }`. `std::vector<WeaponLevel> load_weapon_levels(const JsonDoc& weapon)`. `int max_weapon_level(const WeaponDef&)` returns `levels.size()`. `void power_up_main_weapon(Player&, const WeaponDef&)` raises the level by one, stopping at `max_weapon_level`. It does nothing when already at the max.

- [ ] **Step 1: Write the failing tests**

```cpp
TEST_CASE("starting weapon is level 1") { /* player.weapon_level() == 1 */ }
TEST_CASE("power-up raises level and bullet count from the data") { /* level 2 has bullet_count 3 */ }
TEST_CASE("power-up at the highest level does nothing") {
    // call power_up_main_weapon max+3 times; level equals max, no throw
}
TEST_CASE("weapon without levels fails at load") { /* inline JSON, levels empty */ }
```

- [ ] **Step 2: Run the tests to verify they fail**

Run: `./build/tests -tc="weapon_levels*"`
Expected: FAIL.

- [ ] **Step 3: Implement the level loader and the power-up function, and add a `levels` array to the main weapon in `Data/weapons.json`**

- [ ] **Step 4: Run the tests to verify they pass**

Run: `./build/tests -tc="weapon_levels*"`
Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git commit -am "feat: main weapon levels with capped power-up"
```

## Task 8: Run Integration and Save

**Files:**
- Create: `src/run/run.h`, `src/run/run.cpp` (if not created in Task 7)
- Modify: `src/run/save.h`, `src/run/save.cpp`
- Test: `tests/test_run_player.cpp`

**Interfaces:**
- Consumes: `Player` (Tasks 3, 4, 6), `UpgradeDef` (Task 5), `power_up_main_weapon` (Task 7), `suspend_run` and `resume_run` (existing save code)
- Produces: `class Run` with `Player player`, `std::set<std::string> bought`, `int continues_left` (starts at 5), `bool buy(const UpgradeDef&)` (false if already bought, or if currency is too low; spends the price and calls `apply_upgrade`), `void on_stage_clear(const WeaponDef&)` (calls `power_up_main_weapon`), `bool continue_run()` (restores lives, decrements continues, returns false and does not restore lives at 0). Save writes and reads the ship id, owned upgrade ids, weapon level, and `bought`.

- [ ] **Step 1: Write the failing tests**

```cpp
TEST_CASE("buying the same upgrade twice fails") { /* second buy returns false, currency unchanged */ }
TEST_CASE("continue with 0 left returns false and does not restore lives") { /* ... */ }
TEST_CASE("stage clear raises the weapon level") { /* ... */ }
TEST_CASE("suspend and resume restores ship, owned upgrades, and weapon level") { /* ... */ }
TEST_CASE("resumed suspend file cannot be loaded twice") { /* second resume returns nullopt */ }
```

- [ ] **Step 2: Run the tests to verify they fail**

Run: `./build/tests -tc="run_player*"`
Expected: FAIL.

- [ ] **Step 3: Implement `Run` and extend the save format**

Bullets are cleared by the caller on a successful continue, as in the main plan.

- [ ] **Step 4: Run the tests to verify they pass**

Run: `./build/tests -tc="run_player*"`
Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git commit -am "feat: run integration for shop, continues, and weapon power-up"
```

## Task 9: Input Mapping

**Files:**
- Create: `src/platform/input.h`, `src/platform/input.cpp`
- Test: `tests/test_input.cpp`

**Interfaces:**
- Consumes: none from gameplay
- Produces: `enum class Action { Fire, Bomb, Special, SwapSpecial, Pause, MoveLeft, MoveRight, MoveUp, MoveDown }`. `std::optional<Action> map_key(int key_code)` is a pure lookup. Arrow keys and WASD move. Space fires, X bombs, C fires the special, D swaps, P pauses. The game loop calls `Player::fire_special`, `use_bomb`, and `swap_special` for these actions. The input layer does not change player state itself.

- [ ] **Step 1: Write the failing tests**

```cpp
TEST_CASE("C maps to Special, D to SwapSpecial, X to Bomb, Space to Fire") { /* ... */ }
TEST_CASE("an unbound key maps to nullopt") { CHECK_FALSE(map_key(KEY_F12).has_value()); }
```

- [ ] **Step 2: Run the tests to verify they fail**

Run: `./build/tests -tc="input*"`
Expected: FAIL.

- [ ] **Step 3: Implement the lookup table**

Use raylib key codes in the platform file only. The tests may include raylib, since they check the table, not gameplay.

- [ ] **Step 4: Run the tests to verify they pass**

Run: `./build/tests -tc="input*"`
Expected: PASS.

- [ ] **Step 5: Commit**

```bash
git commit -am "feat: input mapping from keys to player actions"
```

## Self-Review

1. **Spec coverage:** Player state (Task 3), specials and cooldowns (Task 4), ships as JSON (Task 2), rule registry with general and special-only scope (Tasks 1, 5), upgrades with effects, rules, and cooldowns (Task 5), applying upgrades and owned ids (Task 6), main weapon levels and capped power-up (Task 7), run integration, bought set, continues, and save (Task 8), input kept out of `Player` (Task 9). Bombs are shared by all ships (Tasks 3, 6).
2. **Step scan:** Each step is one action. Test bodies are shown as names and assertions, with the spec's values pinned.
3. **Type consistency:** `ShipDef`, `UpgradeDef`, `Effect`, `RuleScope`, `Player`, `WeaponLevel`, and `Run` keep one name and signature across the tasks that use them. `add_lives` and `add_bombs` are defined in Tasks 3 and 6.
4. **Review Focus:** Each of the five items has a test in its owning task.
5. **Proportion:** The plan gives signatures, test names, and assertions, and leaves bodies to the implementer.

## Execution Handoff

The plan is saved to `2026-11-25-player-plan.md`. Please review it and confirm it captures what you want. Then choose an execution method:

- **Native:** I implement every task in this session, then one review checks the whole branch. This is the cheapest and fastest option.
- **Subagent-driven:** a fresh subagent implements each task, and a reviewer checks it before the next task starts.

I recommend **Native**. The tasks depend on each other's interfaces, but there are only nine, and you're writing the code yourself, so a per-task review adds cost without much gain.

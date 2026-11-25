# Player, Ships, Upgrades, and Weapon Power-Up: Design

Status: Approved in conversation (2026-11-25)
Owner: Siddharth Nandi
Related: `bullet_hell_roguelike_design.md`, `bullet_hell_roguelike_plan.md` (Task 11)

## 1. Intent and Why

The player needs one class that holds the state every run uses, and content must be able to change without recompiling. Ships and upgrades differ in numbers and special behaviors, so they are data. Only the behaviors need C++, and they are shared through one rule registry.

## 2. Goals

- `Player` holds the common run state: lives, max lives, score, currency, bombs, special cooldowns, the main weapon level, and the owned upgrade ids.
- Each ship is a JSON entry. Adding a ship with existing rules needs no C++.
- Upgrades are JSON entries built from a fixed set of effects and named rules.
- Stage clears power up the main weapon by one level, up to a cap set in data.

## 3. Non-Goals

- C++ subclasses per ship.
- Ships that cannot bomb. Bombs are a core mechanic for every ship.
- Balance values. Numbers are placeholders until playtesting.

## 4. Anti-Goals

- Ship or upgrade behavior hardcoded by name in C++ (for example, `if (ship == "vanguard")`).
- A rule that fails silently. Unknown or misplaced rule names must fail at load.

## 5. Constraints

- C++17, no raylib calls in gameplay code.
- Content is JSON loaded through the existing `load_json`, with `DataError` naming the file and key.
- The suspend file stores upgrade ids, not copies of upgrade data.

## 6. Approaches Considered

**A. C++ subclass per ship.** Each ship is a class overriding behavior. Clear for a learning project, but every new ship needs a rebuild and the no-recompile goal fails.

**B. Ships and upgrades as JSON data with a shared rule registry.** Ships and upgrades reference behaviors by name. C++ defines each behavior once. New content reuses existing rules without code.

**C. Behavior flags only.** Ships set fixed switches such as `can_bomb`. Simple, but adding a new kind of behavior means adding a field and changing the loader.

**Chosen: B.** It keeps content in data and lets future ships with special behaviors reuse a rule name. Option C was rejected because special behaviors will grow beyond a fixed set of flags.

## 7. Design

### 7.1 Player

`Player` holds:

- Lives, max lives, score, currency (through `Ledger`), and bombs. Bombs are the same for every ship.
- Special cooldown timers, one per special, so swapping does not reset them.
- The main weapon name and its current level.
- The owned upgrade ids, as an array.
- The ship's stats and rules, loaded from its ship entry.

### 7.2 Ships

Each ship is a JSON entry in `players.json`:

- Id, display name, portrait path.
- Stats: speed, max lives, and other numbers.
- Starting weapon name, which refers to `weapons.json`.
- `rules`: a list of rule names. Empty for ships with no special behavior.

### 7.3 Rule Registry

- C++ keeps one table mapping each rule name to a behavior hook in `Player` or in the upgrade logic.
- Each entry is marked **general** (usable on ships and upgrades) or **special-only** (usable only on specials).
- At load, an unknown rule name fails with a `DataError` naming the file, the entry, and the rule.
- At load, a special-only rule on a non-special upgrade fails with a `DataError` naming the upgrade and the rule.

### 7.4 Upgrades

Each upgrade is a JSON entry in `upgrades.json`:

- Id, price, kind (special, weapon, stat, or life).
- Effects from a fixed set (stat change, grant special, grant weapon, raise main weapon level, grant life).
- Rules from the registry, under the rules in 7.3.
- Cooldown, for specials only.

Each special keeps its own cooldown timer, as in the main design.

### 7.5 Owned Upgrades

- `Player` stores owned upgrades as an array of ids. Ids are written to the suspend file.
- `Run` stores the set of bought ids, so each upgrade can be bought once per run. This survives continues.

### 7.6 Main Weapon Power-Up

- Each main weapon has a `levels` array in `weapons.json`. Each level sets a bullet count and a spread angle. Level 1 is the starting weapon.
- After each stage clear, `Run` calls the power-up function, which raises the main weapon level by one.
- The level stops at the highest level in the data. It does not give anything else after that.
- The level is run state and survives continues and suspends.

## 8. Calls Made in This Doc

These were not explicitly decided and can be changed:

- Ship and upgrade ids are strings.
- A power-up after the highest level does nothing.
- A ship's starting weapon is at level 1.
- Each registry rule is marked general or special-only in C++, not in JSON.

## 9. Left to the Builder

- Exact field names and types in `players.json` and `upgrades.json`.
- The first set of rule names and their hooks.
- The effect set for upgrades, beyond the five listed in 7.4.
- Where the power-up function sits inside `Run`.

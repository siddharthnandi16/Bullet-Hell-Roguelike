# Enemy, Bullet, and Boss Behavior: Design

Status: Draft for approval
Date: 2026-11-25
Relates to: `bullet_hell_roguelike_design.md` (the clean C++ rewrite). This doc fills the gap that the rewrite left for enemy movement, in-flight bullet behavior, and shape firing.

## 1. Intent and Why

The existing bullet and enemy system is a fixed timeline of launches. Bullets get a fixed velocity at spawn and cannot change. Enemies cannot move through a sequence of actions. Special behaviors (LASER, CHAINLIGHTNING, EMP) are hardcoded name checks in C.

The rewrite needs enemies and bosses with unique movement and attacks, and bullets that change over their lifetime, without a C++ change for every new enemy. Content should be authored in JSON, and new mechanics should be added as C++ building blocks that JSON can then use.

## 2. Goals

- A new enemy that reuses existing behaviors needs only JSON.
- Enemies run scripted sequences: for example, move right for 5 seconds, stop and fire, then reverse.
- Bullets can change speed, type, and sprite during flight, and can spawn child bullets when a condition is met.
- Bullet speed can change based on vertical position or lifetime.
- Enemies fire geometric shapes (rings, arcs, lines, and similar) defined once and reused.
- Bosses and minibosses run as state machines. Each boss phase from the rewrite design is a state.
- New mechanics can be added in C++ as building blocks, usable from JSON afterward.

## 3. Non-Goals

- Scripting in a general-purpose language. JSON composes building blocks. It does not define new logic.
- Gradual speed changes (ramps) in v1. The mechanism is designed so they can be added later.
- Health-based or player-distance reactions for basic enemies in v1. Bosses can react through their state machines.
- Structural changes to shapes from JSON (for example, arm count in a star).
- Pixel art or other rendering changes.

## 4. Anti-Goals

- A new basic enemy or bullet type that needs C++ when it reuses existing blocks.
- Behavior hidden in name checks or switch statements on bullet or enemy names.
- Bullets held by raw pointer across frames (see section 9).
- Silent loss of bullets. A bullet that cannot spawn must be counted.

## 5. Constraints

- C++17, following the rewrite design.
- The game logic layer has no raylib calls. Behavior code is part of game logic.
- Content is JSON, loaded through the existing JSON loader with named errors.
- Balance values live in `tuning.json`.
- Frame-based timing (60 frames per second), so timing is deterministic and testable.

## 6. Approaches Considered

**A. Everything as a state machine.** One model for enemies, bullets, and bosses. Rejected. Simple enemies and bullets would need states and transitions for what is a plain sequence, which makes authoring slower and validation harder.

**B. Everything as a script.** Enemies, bullets, and bosses all run ordered action lists. Rejected. A boss's phase changes depend on health and on which phase it is in. A flat list would need a tangle of jump actions to express that.

**C. Hybrid: scripts for basic enemies and bullets, state machines for bosses and minibosses, both built from one shared set of building blocks and conditions.** Chosen. Each model fits its use. The shared blocks mean a boss state and a bullet trigger use the same parts.

## 7. Design

### 7.1 Building Blocks and Conditions

The engine provides a library of named C++ building blocks. JSON composes them with parameters.

Actions (initial set):
- `move` (velocity or target, duration or until-condition)
- `fire_pattern` (references a shape from section 7.5, with optional numeric overrides)
- `wait` (duration or until-condition)
- `set_speed`
- `set_type` (changes the bullet's type and its collision and behavior)
- `set_sprite` (changes the glyph and color)
- `spawn_child` (spawns bullets with given parameters)
- `despawn`
- `transition` (boss states only)

Conditions (shared by enemies, bullets, and bosses):
- `timer` (frames elapsed in the current action or state)
- `player_hit`
- `wall_hit`
- `screen_exit`
- `bullet_collision`
- `y_position` (above or below a value)
- `lifetime` (frames since spawn)
- `health` (boss states only)

New actions and conditions are added in C++. Once added, JSON can use them without further code changes.

### 7.2 Basic Enemy Scripts

Each basic enemy has an ordered list of actions. Each action ends when its duration or until-condition is met. The script then moves to the next action. A script that reaches its end restarts or stops, as set in its data.

Example, in outline: `move` right with a 5-second duration, `wait` until the timer reaches 0.5 seconds, `fire_pattern` with a named shape, `move` left with a 5-second duration. The exact JSON format is part of the implementation plan.

### 7.3 Bullet Rules

Each bullet has a list of rules. Each rule has a condition and one or more actions. Rules are evaluated every frame, and by default a rule fires once.

Examples:
- Lifetime 60 frames: `set_speed` to 3.
- `y_position` passes 20 (moving down): `set_sprite` to a different glyph.
- `wall_hit`: `spawn_child` three times with a spread.

Position and lifetime conditions fire once by default. A rule can set `continuous: true`, so that its actions apply every frame while the condition holds. Example: while `y_position` is above 20, speed is 3.

Gradual changes (ramps) are not in v1, but a rule's action can later become a ramp over a duration without changing the rule structure.

### 7.4 Boss and Miniboss State Machines

Each boss is a state machine. Each state has actions that run while the state is active, and transitions to other states, each with a condition.

- Each phase in the rewrite design is one state. Phases alternate plain and spell, and the last state is a spell.
- The phase-end rule (health threshold or timer) becomes a transition from the state.
- Spell-card states carry the capture-bonus rule. A capture happens only when the state ends by health threshold, with no bomb and no lost life during it.
- The boss runs the same building blocks as enemies. Bosses may use `transition` and `health`, which basic enemies cannot.

Load-time validation: every transition target must exist, every state must be reachable from the start, and a boss's last state must be a spell state.

### 7.5 Shapes (Patterns)

Shapes are `Pattern` objects stored in a C++ vector. Each shape defines its structure in code (for example, a ring is always a closed circle of bullets) and declares its numeric parameters with default values (for example, bullet count, speed, radius).

JSON references a shape by name and may override any of its numeric parameters. Structural changes are not allowed from JSON.

Adding a new shape is a C++ change. Using an existing shape with new numbers is a JSON change.

### 7.6 Bullet Pool

Bullets are stored in a vector. Other code refers to a bullet by index or by a generation handle, never by pointer, because the vector can reallocate. Dead bullets are removed by swapping them with the last element, so removal is constant time.

The vector has a hard cap, set in `tuning.json`. When a spawn would exceed the cap, the spawn fails and the game increments a failed-spawn counter. The counter is visible in a debug overlay so the cap can be tuned. Failed spawns are not silent.

### 7.7 Tuning Values

Added to `tuning.json`:
- `max_bullets` (pool cap)
- Any default numbers for shapes that the design sets globally (for example, the default fire rate for `fire_pattern`), if they are shared across shapes

## 8. Calls Made in This Doc

These were not all explicitly decided. They can be changed.

- Shapes are C++ `Pattern` objects, with JSON overriding numeric parameters only, as agreed.
- The initial action and condition sets are the ones listed in section 7.1. Additions will come from content needs.
- Position and lifetime conditions fire once by default. Continuous rules use `continuous: true`.
- Basic enemy scripts stop at the end of their list unless the data sets them to restart.
- The bullet pool cap is a hard cap. Spawns above it fail and are counted.
- The debug overlay for the failed-spawn counter is shown only in debug builds.

## 9. Known Risks

- Child bullets spawned in large numbers can hit the pool cap. Failed-spawn counting exists to detect this.
- Rules that fire every frame (continuous rules) on many bullets cost CPU. Keep continuous rules few, and measure on a full screen.
- Bullet references held across frames must use indices or handles. Any code that stores a pointer to a bullet is a bug.
- JSON that is valid but has the wrong parameter names must be rejected at load time with a named error, not ignored.

## 10. What's Left to the Builder

- The exact JSON schema for scripts, rules, and state machines, and the names of each field.
- The C++ class layout for building blocks, scripts, rules, state machines, shapes, and the bullet pool.
- How a script or state machine is stepped each frame, and in what order relative to bullet updates and collisions.
- The handle format for bullet references (index plus generation, or another scheme).
- The debug overlay's layout and its toggle key.
- Load-time validation rules beyond those listed in 7.4.

## 11. Approval

Please review and approve this design. Once approved, the next step is the implementation plan, using the writing-plans skill.

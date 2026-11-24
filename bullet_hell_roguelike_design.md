# Bullet-Hell Roguelike: Clean C++ Rewrite Design

Status: Draft for approval
Owner: Siddharth Nandi (solo learning project)

## 1. Intent and Why

The current game is a text-mode C shoot-'em-up built for a fixed arcade loop. The new game is a roguelike: each run is a branching map of stages, with upgrades, currency, bosses with spell cards, and multiple endings. The arcade structure does not support this, so the engine will be rewritten in C++ around the run systems instead of patched.

The rewrite also serves a learning goal: practicing object-oriented C++ on a project large enough to need it.

Success means: a complete run can be played from title screen to ending, and any stat, wave, boss, stage, route, or upgrade can be changed by editing JSON without recompiling.

## 2. Goals

- Stats, waves, bosses, stages, routes, dialogue, cutscenes, and upgrades are defined in JSON and loaded at runtime.
- Adding a new wave, boss, stage, or upgrade requires no C++ changes when it uses existing behaviors.
- The branching map and difficulty routes work as described in Section 5.
- Gameplay is displayed as an ASCII grid rendered with raylib.
- The window can run borderless and scale the grid to fit the screen.
- Runs can be suspended and resumed. Suspends cannot be reloaded to retry a moment.
- Score and currency are tracked separately.

## 3. Non-Goals (for now)

- Pixel art. ASCII stays. Pixel art may come later, so rendering must be separate from game logic.
- Enemy drops or currency from cutscenes. Only waves and bosses pay out.
- Multiple save slots or cloud saves.
- Multiplayer.
- Balance tuning. Values are placeholders until playtesting.

## 4. Anti-Goals

- Hardcoding content in C++ that belongs in JSON. If a new wave needs a code change, the design has failed.
- A rendering layer that leaks into gameplay code.
- A save system that allows reloading a suspend file to retry a failed fight.

## 5. Constraints

- Language: C++ (standard to be decided at build setup; C++17 or later assumed).
- Rendering and window: raylib. Text is drawn in a monospace font on a character grid.
- JSON parsing: cJSON, already in the repo.
- Audio: miniaudio, already in the repo.
- Primary platform: Windows. Other platforms are not required for v1.
- The playfield size is a constant set in code, but the cell size and window size are display settings.

## 6. Approaches Considered

**A. Incremental refactor of the existing C code.** Keep the codebase, move rendering to raylib, and add the run systems alongside the old ones. Less work at the start. Downside: arcade assumptions (fixed playfield, single-weapon flow, hardcoded special-behavior switch, name checks for LASER, CHAINLIGHTNING, EMP) stay in the code and fight the new systems.

**B. Clean rewrite in C++ around the new systems.** Build the run, map, stage, wave, boss, and upgrade systems first, then port the old bullet and enemy logic into them. More work up front. Fits the roguelike structure and the OOP learning goal.

**Chosen: B.** The roguelike systems change how the game runs, and patching the arcade loop would fight them.

## 7. Run Structure

A run starts from the title screen with a ship choice. The ship is fixed for the whole run and defines base stats and the starting weapon.

There are two ship types for now:

- **Cannon ship:** high damage in a narrow cone in front. Moves at normal speed. Its weakness is targets off to the side or behind, and it has to be lined up to hit well.
- **Missile ship:** low-damage homing missiles. Moves slower. It covers the whole screen, but each hit does little, and it struggles against fast or heavily armored targets.

Every stage must be balanced for both ships. Each stage should reward the strengths of one ship and punish the weaknesses of the other, so neither ship is the best choice for every route.

Each run has:

- A route map of stages (Section 8).
- A currency balance and a score, tracked separately (Section 13).
- Lives, which reset to max on a continue.
- 5 continues. Running out of continues returns the player to the title screen.
- Bombs, starting at 3, capped at 10.
- Equipped and owned specials (Section 11).
- Owned upgrades, each buyable only once per run.

The run ends in one of three endings (Section 9) or when the continues are exhausted.

## 8. Map

The map is a graph. Each node is a stage, drawn as a labelled celestial body in ASCII art of a star system. Edges are color-coded lines between stages, showing which route the edge belongs to.

- Start: stage 1, on the easy route. Stage 1 and the final stage are fixed.
- Each stage has two exits: a **harder exit** and an **easier exit**.
- Taking the harder exit moves the difficulty up one level. Taking the easier exit moves it down one level.
- Difficulty has three levels: easy, medium, hard.
- Ends are capped. Easy's easier exit leads to another easy stage. Hard's harder exit leads to another hard stage.
- Every path converges on the same final stage.
- The map is data. Nodes store stage references, their two exits, and the rule that gates the harder exit.

## 9. Endings

The final stage is always the same stage, always at the same difficulty. Its boss and waves are normal. The ending plays after that boss is defeated, and its dialogue depends on the difficulty of the stage the player came from:

- From hard: secret ending.
- From medium: neutral ending.
- From easy: bad ending.

## 10. Stages, Waves, and Routing Gates

A stage is a list of 10 to 20 waves drawn from a fixed pool, weighted by difficulty, followed by a boss. The stage file points to separate files for its wave pool, boss, and other assets, so these can be reused across stages.

**Ship balance:** every stage and boss must be playable and fair for both ship types. A stage that one ship can clear easily and the other cannot is a balance fault, but neither ship should be strictly better across all stages.

**Performance gate:** each stage defines what a poor result means for the harder exit. A poor result blocks the harder exit and the game explains why through story dialogue. A good result offers both exits.

Poor-result criteria are data. Examples from the design: too many lives lost, continue used, low score.

## 11. Specials, Bombs, and Upgrades

Upgrades come in four types:

1. **Special attacks.** Activated with C. Each has a cooldown. Each special keeps its own cooldown timer, so swapping away does not reset it. Examples: screen clear, high-damage attack, shield. D swaps the equipped special. There is no limit on how many specials a player can hold.
2. **New weapons.** Fire independently of and simultaneously with the current weapon.
3. **Stat buffs and side-grades.**
4. **Extra lives.**

Bombs are a separate resource, not specials:

- Fired with X. No cooldown.
- A bomb clears the screen, deals high damage, and uses bomb ammo.
- Bomb ammo comes from pickups or stage rewards.
- Starts at 3, caps at 10.

Each upgrade offer shows 8 upgrades drawn from the pool. The player buys one. Bought upgrades do not reappear in the run.

## 12. Waves and Currency

Each wave has a base payout set in its data.

When a wave is cleared, the payout is multiplied by a multiplier:

- **Speed factor:** based on clear time in seconds. Full value (1.0) up to a breakpoint, then linear down to 0.6 at a longer time. Placeholder: 1.0 up to 30 seconds, 0.6 at 90 seconds. To be tuned in playtesting.
- **Lives penalty:** a flat amount subtracted for each life lost. Placeholder: 0.1 per life. To be tuned.
- The multiplier is speed factor minus lives penalty, clamped to a minimum of 0.6 and a maximum of 1.0.

Bosses pay the same wave-style payout plus a fixed clear bonus set in data. The clear bonus is not multiplied by the speed or lives factor.

All currency comes from waves and bosses. Enemies do not drop currency during cutscenes.

## 13. Score and Currency

Score and currency are separate values.

- Every currency source also adds the same amount to score.
- Spending reduces currency. Spending never reduces score.

## 14. Spell Cards and Capture Bonus

Bosses fight in phases. Every boss follows this pattern:

**Plain attack phase → spell card → plain attack phase → spell card → … (until no spell cards remain)**

- The boss has no plain phase after its last spell card. The fight ends when the last spell card is cleared.
- Each phase sets its own timer and health threshold in the boss data.
- Both phase types end when their health threshold is reached or their timer runs out. When a phase ends, the boss moves to its next phase.
- Plain phases give no capture bonus.
- Spell cards give a **capture bonus** if the player beats the card without using a bomb and without losing a life. The bonus is a fixed amount in the spell card's data.
- If a spell card's timer runs out first, there is no capture bonus and the attack ends.

## 15. Cutscenes

Cutscenes are text-only. They pause all gameplay, including boss fights, so bullets, timers, and patterns all stop until the cutscene ends.

Cutscene triggers come in two types, and both are needed:

- **Timeline triggers:** fire at a wave or frame number on the stage timeline.
- **Event triggers:** fire on a named game event, such as losing a life or a boss dropping below a threshold.

When multiple cutscenes trigger at the same time, they are queued and play in order. Gameplay stays paused until the queue is empty.

Cutscene scripts and dialogue live in JSON.

## 16. Stage Flow

When a stage ends, the results screen appears first. It shows score, lives lost, continues used, and currency earned. It reads the run's stats, so the same screen is reused after every stage.

After results, the map screen appears. The player picks the next stage from the two exits of the current stage, subject to the performance gate.

After a boss is defeated, the upgrade shop appears before the map.

## 17. Death and Continues

When lives reach zero:

- If continues remain, the player continues. This restores max lives, keeps inventory, and resumes the same stage at the moment of death. Bullets on screen are cleared once. Currency is kept.
- Using a continue locks the hard route for the rest of the run.
- If no continues remain, the run ends and the player returns to the title screen.

## 18. Saving and Suspend

- Quitting during a run suspends it. The player resumes from the same point.
- A suspended run stays on disk until it is resumed or deleted.
- Resuming deletes the suspend file, so the same state cannot be loaded twice. This prevents save-scumming.
- High scores persist after the game closes.

## 19. Data Layout

All content is JSON. Files reference each other by name rather than embedding content.

- `players.json`: ship definitions, base stats, starting weapon.
- `weapons.json`: weapon definitions. Existing format is kept where possible.
- `bullet_types.json`: bullet definitions. Existing format is kept where possible.
- `patterns.json`: reusable bullet patterns. Existing format is kept where possible.
- `enemies.json`: enemy definitions, including zero or more weapons.
- `waves/*.json`: wave definitions with spawn timelines, base payout, and references to patterns and enemies.
- `wave_pools/*.json`: wave pools with weights by difficulty.
- `bosses/*.json`: bosses with phases. Each phase has a type (plain or spell), a timer, a health threshold, patterns, and a capture bonus for spell cards.
- `stages/*.json`: stage definitions. References a wave pool, a boss, and cutscene triggers. Includes the stage's difficulty.
- `map.json`: the route graph. Nodes are stages, edges are exits with their gate rules.
- `upgrades.json`: upgrade pool with type and effects.
- `cutscenes/*.json`: text scripts and their triggers.
- `endings.json`: ending dialogue, keyed by source difficulty.
- `tuning.json`: speed breakpoints, lives penalty, clamp range, continue count, bomb start and cap, shop size. Keeps balance values out of code.

A Python script that builds JSON from a spreadsheet is planned in the project roadmap. It is out of scope for v1 and may be added later.

## 20. Engine Architecture (Outline)

The rewrite separates four layers:

1. **Data:** JSON loading and validation. Produces plain data objects.
2. **Game logic:** run, map, stage, wave, boss, phase, spell card, upgrade, currency, score, cutscene queue, and save. No raylib calls.
3. **Rendering:** draws the ASCII grid and screens with raylib. Reads game state, never writes it.
4. **Platform:** window setup, borderless mode, scaling, input mapping, audio.

Game logic depends only on data and its own classes. Rendering and platform depend on game logic. This keeps pixel art possible later.

Likely core classes: `Run`, `MapGraph`, `Stage`, `Wave`, `Boss`, `Phase`, `SpellCard`, `Weapon`, `Bullet`, `Enemy`, `Upgrade`, `CutsceneQueue`, `SaveManager`, `Renderer`. Final class design is part of the implementation plan, not this doc.

## 21. Display

- The playfield is a fixed character grid (currently 100×26).
- Cell size is a display setting. Scaling chooses the cell size to fit the window.
- Borderless windowed mode is a display setting that does not change gameplay.
- Stage results, map, shop, title, and cutscene screens are also ASCII.

## Art Assets and Placeholders

Hand-drawn art is planned for these, and each one is referenced by file path in its data:

- **Portraits** for every character that appears: each playable ship pilot, each boss, and any named character who speaks in a cutscene. A cutscene line names its speaker, and that speaker's portrait is shown.
- **Title screen art**, referenced from the title screen's data.
- **Stage backgrounds**, one per stage theme (Section 10 and the data layout).

Until the real art exists, the game uses placeholders:

- A missing portrait shows a neutral framed box with the character's name.
- Missing title art shows a plain title with no image.
- A missing image logs a warning and never crashes the game.
- Placeholders keep the same frame size as the real art, so swapping in the final image does not change the layout.

## 22. Deferred Values (Tuned in Playtesting)

- Speed breakpoints (placeholder: 30s and 90s).
- Lives penalty per life (placeholder: 0.1).
- Spell card capture bonus amount.
- Boss clear bonus amount.
- Per-wave base payouts.
- Upgrade pool contents and their numbers.
- Special attack cooldowns and effects.
- Bomb damage and screen-clear behavior.
- Poor-result thresholds for each stage's harder-exit gate.

## 23. Calls Made in This Doc

These were not explicitly decided by the owner and can be changed:

- Each special keeps its own cooldown timer when swapped out.
- Resuming a suspended run deletes the suspend file.
- The boss clear bonus is not multiplied by the speed or lives factor.
- The shop appears after the boss and before the map.
- Results appear before the map after every stage.
- Balance values live in `tuning.json`.

## 24. Open Questions

- Which C++ standard to target.
- Whether the upgrade shop offers a skip option.
- How the map screen handles a locked harder exit: shown as locked, hidden, or both.

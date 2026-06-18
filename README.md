# VOID RUNNER
#### Video Demo: (https://youtu.be/0rcE3-FuzAA)
#### Description:

Void Runner is a vertical-scrolling arcade survival game built in C++ using the [Raylib](https://www.raylib.com/) framework. The player pilots a spaceship through an endless field of falling asteroids and power-up items, trying to survive for 60 seconds without losing all their health. The game features multiple playable ship skins, a health and shield system, dynamic difficulty scaling, animated sprites, a full HUD, background music, and sound effects — all packaged into a clean scene-management architecture.

The core gameplay loop is simple but challenging: asteroids fall from the top of the screen at increasing speed and frequency as time goes on. The player moves left and right to dodge or tank hits, collecting health potions to recover and shield potions to become temporarily invincible. Surviving the full 60 seconds wins the game; losing all HP ends it early.

---

## File Structure and Design

### `main.cpp`

The entry point of the program. It initializes a 540×960 window (a portrait aspect ratio chosen to evoke the feel of classic mobile arcade games), sets up the audio device, and runs the main game loop. The loop delegates all logic and rendering to the `Scene_selector` object. Keeping `main.cpp` thin was a deliberate choice — it does nothing more than bootstrap the window and hand off control, keeping concerns cleanly separated.

### `entity.hpp` / `entity.cpp`

The `Entity` base class is the foundation of the entire game's object hierarchy. It holds the shared state common to every drawable, movable object: `position`, `speed`, `scale`, `rotation`, `origin`, `dest_rec` (destination rectangle for rendering), and `hitbox_shrink`. It also provides `initAnimation()` for loading textures and computing source rectangles, and a virtual `get_collision_rec()` method for hitbox calculation. Using inheritance here avoided duplicating boilerplate across the player, enemies, background, and buttons — each of those classes extends `Entity` and inherits the draw infrastructure for free.

### `player.hpp` / `player.cpp`

The `Player` class manages all player state: skin selection (Red, White, Green, Purple), movement state (Center, Left, Right), health points, and the shield system. The player animates differently depending on direction — each skin has three sprite variants. The shield is implemented as a timed buff with a smooth fade-in/fade-out animation using a sprite sheet. A rocket flame animation plays continuously beneath the ship. The `update()` method returns a boolean (`dead`) rather than void, so the game scene can detect death without polling a separate getter — a small but clean design decision. The `reset()` method restores all fields to their initial values to support restarting without reconstructing the object.

### `enemies.hpp` / `enemies.cpp`

The `Enemy` class handles both obstacles (small and big asteroids) and power-ups (health and shield potions), unified under a single class with an `EnemyType` enum to differentiate behavior. This was a deliberate design choice: rather than creating separate subclasses for each item type, a single Enemy with a type field and a switch-case in `attack()` keeps the pool-based spawning logic simple. Enemies are pooled in a fixed-size array of 7 in `GameScene` — they are never allocated or freed at runtime, just reset and reactivated. Each enemy tracks its own `active` state and deactivates itself when it scrolls off screen.

### `background.hpp` / `background.cpp`

The `Background` class implements an infinite vertical scrolling effect using two texture layers. Each layer moves downward at a fixed speed, and when one scrolls fully off screen, it is repositioned just above the other — creating a seamless loop. A small `layer_gap` offset prevents visible seams between tiles. The scaling factor (4.2×) stretches the pixel-art texture to fill the portrait screen.

### `game_scene.hpp` / `game_scene.cpp`

`GameScene` is the core gameplay manager. It owns the `Player`, the array of `Enemy` objects, the HUD icons, and all gameplay logic: collision detection, spawning, difficulty progression, and win/loss conditions. The spawn interval starts at 0.8 seconds and shrinks over time (floored at 0.2 seconds), making the game progressively harder. An `aid_delay_counter` ensures power-ups appear only every 7th spawn, so the player isn't overwhelmed with helpful items. The HUD draws HP and shield bars with dynamic color — the health bar shifts from orange-red at full health toward darker red as HP drops.

### `scene_selector.hpp` / `scene_selector.cpp`

`Scene_selector` acts as the top-level state machine managing five scenes: `MAIN_MENU`, `GAME`, `PAUSE`, `OVER`, and `WIN`. Each scene has its own update and draw logic inside a switch statement, making transitions explicit and easy to follow. The `Button` class (also defined here) extends `Entity` and handles three visual states (normal, hover, clicked) with mouse collision detection. Music volume and pitch are adjusted when transitioning between gameplay and menus to create a muffled, lower-energy feel on non-game screens. All audio assets — BGM, hit, aid, lose, win, and button sounds — are owned by `Scene_selector` and passed by pointer to `GameScene` to avoid duplicate loading.

### `aid.hpp` / `aid.cpp`

Currently a stub that includes `entity.hpp`. This file was scaffolded in anticipation of a dedicated `Aid` class (separate from `Enemy`) for collectible items, but the design was ultimately consolidated into the unified `Enemy` class with an `EnemyType` to keep the object pool simpler. The file remains as a placeholder for potential future expansion.

---

## Design Decisions

One key decision was whether to separate aid items (health, shield potions) from obstacle enemies into distinct class hierarchies. Ultimately, keeping them as a single `Enemy` class with a type field made pool management far simpler — the fixed 7-slot array works uniformly for all item types without needing to track separate pools. The trade-off is a slightly less clean semantic model, but the practical benefits in spawn logic outweighed that concern.

Another decision was using a fixed-size enemy array rather than dynamic allocation. Dynamic allocation would allow arbitrary numbers of enemies, but for a fixed 60-second game with tuned spawn rates, a pool of 7 is sufficient and avoids any heap allocation overhead during gameplay — keeping frame timing consistent.

The portrait 540×960 resolution was chosen deliberately to give the game a vertical-scroller feel reminiscent of classic mobile shoot-em-ups, while still being comfortable in a desktop window.

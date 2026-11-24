# Velocity Rush 3D
CSE 402 Computer Graphics & Image Processing Lab - endless procedural scenic driving simulator.
C++ / OpenGL / FreeGLUT. Source is split by feature (see "Source layout" below); gameplay is unchanged.

## Build
**Windows - Code::Blocks (MinGW)**
1. Install FreeGLUT for MinGW (put `include/GL`, `lib`, and `bin/freeglut.dll` where your compiler finds them).
2. New Console Project -> add **all** `.cpp` files (and the `.h` files) from this folder.
3. Project > Build options > Linker settings, add in this order: `freeglut`, `glu32`, `opengl32`, `winmm`, `gdi32`.
4. Build and run. Keep `freeglut.dll` next to the .exe.

**Windows - Visual Studio**
1. Install FreeGLUT via vcpkg (`vcpkg install freeglut:x64-windows`) and integrate, or add include/lib paths by hand.
2. Add all `.cpp` files to an empty C++ project. Link `freeglut.lib`, `glu32.lib`, `opengl32.lib`.

**Linux / macOS:** `make && ./velocity_rush`  (Ubuntu: `sudo apt install freeglut3-dev libglu1-mesa-dev`)

## Controls
| Key | Action |
|---|---|
| ENTER | start from menu |
| W / S (or Up / Down) | throttle / brake |
| A / D (or Left / Right) | steer |
| SPACE | boost (charged by collecting coins) |
| T | autodrive on/off |
| C | camera: 3rd person / 1st person / free (mouse drag + wheel also work) |
| F | weather: sunny / foggy / rainy |
| N, `,` `.` | freeze day-night cycle, scrub time of day |
| H | force headlights (automatic at night, fog, rain) |
| 1 / 2 / 3, Left/Right in menu | pick car |
| P / R / M / ESC | pause / restart run / menu / pause then quit |

## Implemented (from the proposal)
- Endless procedural road, hills, mountains, trees, reflector posts (generated from the distance travelled, nothing stored)
- Manual drive + autodrive; third-person, first-person and free cameras
- Sunny / foggy / rainy (exp2 fog, rain particles, wet-road specular, grey sky)
- Full day-night cycle: sun, moon, stars, sunset glow, clouds, headlights with light beams
- Three selectable cars (sedan, sports, SUV) with rotating wheels, steering and body roll
- Speedometer, distance counter, menu, pause, off-road reset
- Optional extras: rain, headlights, free camera, coins, boost, best-distance record (`velocity_rush_best.txt`)

## Source layout (one file pair per feature)
| Feature | Files |
|---|---|
| Entry point, GLUT setup | `main.cpp` |
| Shared data / globals | `game_state.h/.cpp`, `common.h`, `mathutil.h` |
| Procedural noise | `noise.h/.cpp` |
| Endless road curve + terrain height | `road.h/.cpp` |
| Driving physics, autodrive, coins pickup, frame update | `simulation.h/.cpp` |
| Cameras (3rd / 1st / free / menu) | `camera.h/.cpp` |
| Frame render order + GLUT display/timer/reshape | `renderer.h/.cpp` |
| Keyboard / mouse input | `input.h/.cpp` |
| Sun, moon, sky colours, weather blending | `environment.h/.cpp` |
| Sky dome, stars, clouds | `sky.h/.cpp` |
| Scene lighting + fog | `lighting.h/.cpp` |
| Cars (3 models, wheels, shadow) | `car.h/.cpp` |
| Headlights + beams | `headlights.h/.cpp` |
| Terrain | `terrain.h/.cpp` |
| Road surface, lines, posts | `road_render.h/.cpp` |
| Trees | `trees.h/.cpp` |
| Coins (drawing) | `coins.h/.cpp` |
| Rain | `rain.h/.cpp` |
| HUD overlay | `hud.h/.cpp`, `hud_util.h/.cpp` |
| Speedometer | `speedometer.h/.cpp` |
| Menu + pause screen | `menu.h/.cpp` |
| Textures + tree display lists | `resources.h/.cpp` |
| Best-distance save file | `persistence.h/.cpp` |
| Small draw helpers | `gfx_util.h/.cpp` |

## Notes
- Uses only the OpenGL fixed pipeline + FreeGLUT, so GLEW/GLAD, GLM and stb_image are not required. The road/terrain detail texture is generated procedurally at startup.
- Dynamic shadows, reflections, volumetric clouds, traffic and multiplayer are out of scope, as in the proposal (the car has a simple blob shadow).
- 4x MSAA is requested; if your driver refuses it, delete `GLUT_MULTISAMPLE` from `glutInitDisplayMode` and the `glutSetOption` line.

## v2 changes
- Sharper, more varied road bends (`road.cpp`)
- Automatic 5-speed gearbox with engine torque curve, engine braking, shift dips and rev limiter (`simulation.cpp`)
- Slope effect on speed; speed-sensitive steering (less lock at high speed)
- Body pitch: nose dips under braking, lifts under acceleration (`car.cpp`)
- More car detail: mirrors, door seams, grille, chrome exhaust, rear wing (sports), running boards (SUV)
- Tachometer with gear display (`speedometer.cpp`)
- Mini-map of the road ahead with turn warnings (`minimap.h/.cpp`, **add `minimap.cpp` to your project**)
- Autodrive slows down for bends

# Vehicle Physics and Handling Update

This update is on the `task-1-vehicle-physics` branch. It changes the game source; the existing `velocityRush3D.exe` is a prebuilt artifact and is not rebuilt by a source commit. To see the new handling, build this branch using the instructions below.

## Windows (VS Code / Visual Studio / Code::Blocks)
Install a C++11 compiler, FreeGLUT, and CMake. In VS Code, install the C/C++ and CMake Tools extensions and configure the project folder. In Visual Studio, open the folder as a CMake project. For Code::Blocks, add every `.cpp` in `src/`, add `include/` and FreeGLUT include directories, then link `freeglut`, `glu32`, `opengl32`, `winmm`, and `gdi32`.

Copy `velocity_rush_physics.cfg` beside the executable (and `freeglut.dll` on Windows). Run the generated executable from that directory so it can find the tuning file and save file.

## Physics adjustments
- Brake deceleration, rolling resistance and aerodynamic drag are configurable.
- Dry/wet grip, lateral acceleration, steering/lateral response, drift grip and low-speed steering stability are configurable.
- Suspension response now smooths longitudinal weight transfer; off-road deceleration and edge response are tuned rather than instantly resetting the car.
- Existing keyboard and arrow controls are unchanged. FreeGLUT joystick steering is enabled where a supported gamepad is detected.
- Tuning values are loaded from `velocity_rush_physics.cfg`, validated and clamped; safe compiled-in defaults apply if the file is absent.

The project currently has no mobile/touch input target. The pre-existing exe will remain unchanged until rebuilt with a Windows toolchain.

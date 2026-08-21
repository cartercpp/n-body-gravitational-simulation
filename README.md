# N-Body Gravitational Simulation

A real-time 2D N-body gravitational simulation rendered directly in the terminal.

Bodies attract each other under Newtonian gravity (with softening) and are visualized using ANSI escape codes. The simulation runs continuously until a key is pressed.

## Features

- Custom lightweight `vector<T, Dim>` template (supports structured bindings)
- Softened gravity to avoid singularities
- 15 randomly initialized bodies
- Terminal rendering with ANSI cursor control
- C++23 + CMake

## Physics

The acceleration on body \(i\) is computed as:

\[
\mathbf{a}_i = G \sum_{j \neq i} \frac{m_j \, (\mathbf{r}_j - \mathbf{r}_i)}{\bigl(|\mathbf{r}_j - \mathbf{r}_i|^2 + \varepsilon^2\bigr)^{3/2}}
\]

where \(\varepsilon\) is the softening parameter.

Integration is a simple Euler step:

```cpp
velocity += acceleration * dt;
position += velocity * dt;
```

## Parameters

```cpp
constexpr int rows = 88;
constexpr int columns = 150;

constexpr double softening = 0.01;
constexpr double gravity   = 100;
constexpr double dt        = 0.02;
```

Bodies are initialized with random positions inside the grid and masses in \([1, 10]\). Initial velocities are zero.

## Building

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

Requires a C++23-capable compiler.

## Running

```bash
./NBodyGravitationalSimulation
```

The simulation starts immediately. Press any key to exit.

## Notes

- Rendering uses ANSI escape sequences (`\033[...`) and works in most modern terminals.
- The grid is cleared and redrawn every frame (~30 ms).
- Bodies that leave the visible area are simply not drawn until they re-enter.
```

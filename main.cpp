#include <iostream>
#include <iomanip>
#include <format>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <stop_token>
#include <cmath>
#include <cstddef>
#include "vector.h"

struct Body
{
    double m_mass;
    vector<double, 2> m_position,
                      m_velocity;
};

int main()
{
    constexpr int rows = 88,
                  columns = 150;
    constexpr double softening = 0.01,
                     gravity = 100,
                     dt = 0.02;

    std::random_device rd;
    std::uniform_real_distribution<double> xDist(0, columns - 1),
                                           yDist(0, rows - 1),
                                           massDist(1, 10);

    std::vector<Body> bodies(15);
    for (Body& bodyRef : bodies)
    {
        bodyRef.m_mass = massDist(rd);
        bodyRef.m_position = vector<double, 2>{xDist(rd), yDist(rd)};
        bodyRef.m_velocity = vector<double, 2>{0.0, 0.0};
    }

    auto moveCursor = [](int row, int column) {
        std::cout << std::format("\033[{};{}H", row + 1, column + 1) << std::flush;
    };

    std::cout << "\033[2J" << std::flush;

    std::jthread render{[&](std::stop_token st) {
        while (!st.stop_requested())
        {
            std::cout << "\033[2J\033[H" << std::flush;
            std::cout << "\033[?25l" << std::flush;

            std::vector<vector<double, 2>> accelerations(bodies.size(), vector<double, 2>{0.0, 0.0});

            for (std::size_t i = 0; i < bodies.size(); ++i)
            {
                for (std::size_t i2 = 0; i2 < bodies.size(); ++i2)
                {
                    if (i == i2)
                        continue;

                    const auto delta = bodies[i2].m_position - bodies[i].m_position;
                    const double rSquared = delta.squared_length() + softening * softening;
                    const double rCubed = rSquared * std::sqrt(rSquared);
                    const double factor = gravity * bodies[i2].m_mass / rCubed;
                    accelerations[i] += delta * factor;
                }
            }

            for (std::size_t i = 0; i < bodies.size(); ++i)
            {
                bodies[i].m_velocity += accelerations[i] * dt;
                bodies[i].m_position += bodies[i].m_velocity * dt;

                const auto [x, y] = bodies[i].m_position;
                if ((x >= 0) && (x < columns) && (y >= 0) && (y < rows))
                {
                    moveCursor(static_cast<int>(rows - 1 - y), static_cast<int>(x));
                    std::cout << 'o' << std::flush;
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(30));
        }
    }};
    std::cin.get();
}
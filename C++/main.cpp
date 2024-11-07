#include <iostream>
#include <vector>
#include <tuple>
#include "Env.h"
#include "Agent.h"

int main() {
    srand(time(nullptr)); // Seed for randomness

    // Configuration for three environment types
    std::vector<std::tuple<std::string, double, double, bool>> environments = {
            {"full", 0.0, 0.0, false},
            {"full", 0.2, 0.2, false},
            {"full", 0.0, 0.0, true}
    };

    for (size_t i = 0; i < environments.size(); ++i) {
        std::cout << "\nSimulation for environment " << i + 1 << std::endl;
        auto [visibility, movement_uncertainty, suck_uncertainty, dynamic_dirt] = environments[i];

        Env env(visibility, movement_uncertainty, suck_uncertainty, dynamic_dirt);
        Agent agent;
        agent.set_env(&env);
        env.add_agent(&agent);

        // Run the simulation for 100 steps
        int num_steps = 100;
        for (int step = 0; step < num_steps; ++step) {
            env.step();
        }

        // Display evaluation results
        std::cout << "Evaluation Results:" << std::endl;
        std::cout << "1. Successful moves: " << env.f1 << std::endl;
        std::cout << "2. Failed moves: " << env.f2 << std::endl;
        std::cout << "3. Total sucks (successful and unsuccessful): " << env.f3 << std::endl;
        std::cout << "4. Successful sucks: " << env.f4 << std::endl;
        std::cout << "5. Average number of dirty rooms at the end of each step: " << static_cast<double>(env.f5) / num_steps << std::endl;
    }

    return 0;
}

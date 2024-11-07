#include "Agent.h"
#include "Env.h"
#include <deque>
#include <set>
#include <iostream>

Agent::Agent() : state_memory({false, false}), env(nullptr) {}

VacuumAction Agent::step(Env::Perception perception) {
    // Decision-making based on perception and memory
    if (perception.has_state && perception.state == DIRTY) {
        return SUCK;
    } else if (perception.has_position && bfs_find_dirty(*env)) {
        return MOVE;
    }
    return SLEEP;
}

bool Agent::bfs_find_dirty(const Env& env) {
    std::deque<std::pair<int, int>> queue = { {env.get_position(), 0} }; // Position and depth
    std::set<int> visited;

    while (!queue.empty()) {
        auto [position, depth] = queue.front();
        queue.pop_front();

        if (visited.find(position) == visited.end()) {
            visited.insert(position);
            if (env.get_rooms()[position] == DIRTY) {
                return true; // Found a nearby dirty room
            }

            // Add neighbors (assuming three connected rooms)
            queue.push_back({(position + 1) % 3, depth + 1});
            queue.push_back({(position - 1 + 3) % 3, depth + 1});
        }
    }
    return false; // No dirty room found
}

void Agent::set_env(Env* env) {
    this->env = env;
}

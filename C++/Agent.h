#ifndef AGENT_H
#define AGENT_H

#include "Env.h"

class Agent {
public:
    Agent();

    VacuumAction step(Env::Perception perception);
    bool bfs_find_dirty(const Env& env);  // Pass Env by reference

    void set_env(Env* env);  // Set the environment for the agent

private:
    std::vector<bool> state_memory;
    Env* env;
};

#endif // AGENT_H

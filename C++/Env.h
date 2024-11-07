#ifndef ENV_H
#define ENV_H

#include <vector>
#include <string>
#include <random>

enum RoomState {
    CLEAN = 0,
    DIRTY = 1
};

enum VacuumAction {
    MOVE,
    SUCK,
    SLEEP
};

class Agent;  // Forward declaration of Agent class

class Env {
public:
    Env(std::string visibility, double movement_uncertainty, double suck_uncertainty, bool dynamic_dirt);

    void add_agent(Agent* agent);
    void step();

    struct Perception {
        int position;
        RoomState state;
        bool has_position;
        bool has_state;
    };

    Perception get_perception() const;
    int get_position() const;
    const std::vector<RoomState>& get_rooms() const;

    // Evaluation metrics
    int f1, f2, f3, f4, f5;

private:
    std::vector<RoomState> rooms;
    int position;
    std::string visibility;
    double movement_uncertainty, suck_uncertainty;
    bool dynamic_dirt;
    Agent* agent;

    double random_probability() const;
    int count_dirty_rooms() const;
    void execute_action(VacuumAction action);
};

#endif // ENV_H

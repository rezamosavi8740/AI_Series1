#include "Env.h"
#include "Agent.h"
#include <iostream>
#include <random>

Env::Env(std::string visibility, double movement_uncertainty, double suck_uncertainty, bool dynamic_dirt)
        : position(0), visibility(visibility), movement_uncertainty(movement_uncertainty),
          suck_uncertainty(suck_uncertainty), dynamic_dirt(dynamic_dirt), f1(0), f2(0), f3(0), f4(0), f5(0) {
    rooms = {DIRTY, DIRTY, DIRTY};
    agent = nullptr;
}

void Env::add_agent(Agent* agent) {
    this->agent = agent;
}

Env::Perception Env::get_perception() const {
    Perception perception = { position, rooms[position], false, false };

    if (visibility == "full") {
        perception.has_position = true;
        perception.has_state = true;
    } else if (visibility == "no_position_sensor") {
        perception.has_state = true;
    } else if (visibility == "no_clean_sensor") {
        perception.has_position = true;
    }

    return perception;
}

int Env::get_position() const {
    return position;
}

const std::vector<RoomState>& Env::get_rooms() const {
    return rooms;
}

double Env::random_probability() const {
    return rand() / static_cast<double>(RAND_MAX);
}

int Env::count_dirty_rooms() const {
    int count = 0;
    for (RoomState room : rooms) {
        if (room == DIRTY) count++;
    }
    return count;
}

void Env::execute_action(VacuumAction action) {
    if (action == MOVE) {
        if (random_probability() > movement_uncertainty) {
            // Successful move
            position = (position + 1) % 3;
            f1++;
        } else {
            // Failed move
            f2++;
        }
    } else if (action == SUCK) {
        f3++;
        if (random_probability() > suck_uncertainty && rooms[position] == DIRTY) {
            // Successful suck
            rooms[position] = CLEAN;
            f4++;
        }
    }
}

void Env::step() {
    // Generate random dirt in a dynamic environment
    if (dynamic_dirt && random_probability() < 0.2) {
        int random_room = rand() % 3;
        rooms[random_room] = DIRTY;
    }

    // Execute agent step
    if (agent) {
        VacuumAction action = agent->step(get_perception());
        execute_action(action);
    }

    // Record the number of dirty rooms at this step
    f5 += count_dirty_rooms();
}

import random
from collections import deque


# Room states
class RoomState:
    CLEAN = 0
    DIRTY = 1


# Vacuum actions
class VacuumAction:
    MOVE = 'move'
    SUCK = 'suck'
    SLEEP = 'sleep'


# Simulation environment
class Env:
    def __init__(self, visibility, movement_uncertainty, suck_uncertainty, dynamic_dirt):
        self.rooms = [RoomState.DIRTY, RoomState.DIRTY, RoomState.DIRTY]
        self.position = 0  # Vacuum position: 0, 1, or 2
        self.visibility = visibility
        self.movement_uncertainty = movement_uncertainty
        self.suck_uncertainty = suck_uncertainty
        self.dynamic_dirt = dynamic_dirt
        self.agent = None

        # Evaluation metrics
        self.f1 = 0  # Number of successful moves
        self.f2 = 0  # Number of failed moves
        self.f3 = 0  # Total number of suction actions
        self.f4 = 0  # Number of successful suction actions
        self.f5 = 0  # Number of dirty rooms at each step

    def add_agent(self, agent):
        self.agent = agent

    def step(self):
        # Generate random dirt in dynamic environment
        if self.dynamic_dirt and random.random() < 0.2:
            random_room = random.choice([0, 1, 2])
            self.rooms[random_room] = RoomState.DIRTY

        # Agent performs its action for this step
        if self.agent:
            action = self.agent.step(self.get_perception())
            self.execute_action(action)

        # Record the number of dirty rooms at this step
        self.f5 += self.rooms.count(RoomState.DIRTY)

    def get_perception(self):
        # Provides cleanliness/dirtiness information based on vacuum visibility
        if self.visibility == 'full':
            return {'position': self.position, 'state': self.rooms[self.position]}
        elif self.visibility == 'no_position_sensor':
            return {'state': self.rooms[self.position]}
        elif self.visibility == 'no_clean_sensor':
            return {'position': self.position}

    def execute_action(self, action):
        if action == VacuumAction.MOVE:
            if random.random() > self.movement_uncertainty:
                # Successful move
                self.position = (self.position + 1) % 3
                self.f1 += 1
            else:
                # Failed move
                self.f2 += 1
        elif action == VacuumAction.SUCK:
            self.f3 += 1
            if random.random() > self.suck_uncertainty:
                # Successful suction
                if self.rooms[self.position] == RoomState.DIRTY:
                    self.rooms[self.position] = RoomState.CLEAN
                    self.f4 += 1
        elif action == VacuumAction.SLEEP:
            pass  # Vacuum goes to sleep


# Vacuum agent with BFS for finding dirty rooms
class Agent:
    def __init__(self):
        self.state_memory = [False, False]  # Two memory variables

    def bfs_find_dirty(self, env):
        # BFS search to find the nearest dirty room
        queue = deque([(env.position, 0)])  # Position and depth
        visited = set()
        while queue:
            position, depth = queue.popleft()
            if position not in visited:
                visited.add(position)
                if env.rooms[position] == RoomState.DIRTY:
                    return depth  # Nearest dirty room found
                # Add neighbors (assuming three connected rooms)
                queue.append(((position + 1) % 3, depth + 1))
                queue.append(((position - 1) % 3, depth + 1))
        return None  # No dirty room found

    def step(self, perception):
        # Decision-making based on perception and memory
        if 'state' in perception and perception['state'] == RoomState.DIRTY:
            return VacuumAction.SUCK
        elif 'position' in perception:
            if self.bfs_find_dirty(env) is not None:
                return VacuumAction.MOVE
        return VacuumAction.SLEEP


# Main program
if __name__ == "__main__":
    # Configuration for three types of environments
    environments = [
        {'visibility': 'full', 'movement_uncertainty': 0.0, 'suck_uncertainty': 0.0, 'dynamic_dirt': False},
        {'visibility': 'full', 'movement_uncertainty': 0.2, 'suck_uncertainty': 0.2, 'dynamic_dirt': False},
        {'visibility': 'full', 'movement_uncertainty': 0.0, 'suck_uncertainty': 0.0, 'dynamic_dirt': True}
    ]

    for i, config in enumerate(environments):
        print(f"\nSimulation Environment {i + 1}")
        env = Env(**config)
        agent = Agent()
        env.add_agent(agent)

        # Run simulation for 100 steps
        num_steps = 100
        for _ in range(num_steps):
            env.step()

        # Display evaluation results
        print("Evaluation Results:")
        print(f"1. Number of successful moves: {env.f1}")
        print(f"2. Number of failed moves (hit a wall): {env.f2}")
        print(f"3. Total number of suction actions (successful and unsuccessful): {env.f3}")
        print(f"4. Number of successful suctions: {env.f4}")
        print(f"5. Average number of dirty rooms at the end of each step: {env.f5 / num_steps}")

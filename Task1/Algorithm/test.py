from main import Main
from main import initialize
from path_finding.hamiltonian import Hamiltonian
from robot.robot import Robot

from grid.obstacle import Obstacle
from misc.positioning import Position
from misc.direction import Direction
from TaskTwoSimulation import Simulation
import constants

# tmp = [[50, 50, Direction.TOP], [90, 90, Direction.BOTTOM],
#        [40, 180, Direction.LEFT], [120, 150, Direction.RIGHT]]
# obstacles = []
# i = 0
# for x, y, direction in tmp:
#     position: Position = Position(x, y, direction)
#     obstacle: Obstacle = Obstacle(position, i)
#     i += 1
#     obstacles.append(obstacle)
#
# print("list of obstacles: ")
# print(obstacles)
# grid = Grid(obstacles)
# print("list of target positions")
# for x in grid.obstacles:
#     print(x.target_position)
# robot = Robot(grid)
# test = Hamiltonian(grid=grid, robot=robot)
# test.plan_path()

# x = 'ALG:10,17,S,0;17,17,W,1;2,16,S,2;16,4,S,3;13,1,W,4;6,6,N,5;9,11,W,6;3,3,E,7;'.encode(
#     'utf-8')
# initialize()

# tmp = [[50, 50, Direction.TOP]]
# obstacles = []
# i = 0
# for x, y, direction in tmp:
#     position: Position = Position(x, y, direction)
#     obstacle: Obstacle = Obstacle(position, i)
#     i += 1
#     obstacles.append(obstacle)

# grid = Grid(obstacles)
# print(grid.gridcells2)

obstacleX = 70
obstacleY = constants.TASK2_LENGTH - constants.GRID_CELL_LENGTH - 20
distance1 = 0
distance2 = 0
while distance1 < 60 or distance1 > 150:
    distance1 = int(input("First obstacle distance from robot (60-150): "))
while distance2 < 60 or distance2 > 150:
    distance2 = int(
        input("Second obstacle distance from first obstacle (60-150): "))
world1 = [
    [70, distance1, Direction.BOTTOM],
    [70, distance1 + distance2 + constants.GRID_CELL_LENGTH, Direction.BOTTOM]
]
obstacles = []
i = 0
for x, y, direction in world1:
    position: Position = Position(x, y, direction)
    obstacle: Obstacle = Obstacle(position, i)
    i += 1
    obstacles.append(obstacle)
grid = Grid(obstacles)
bot = Robot(grid)
direction = bot.get_current_pos().direction
currentPos = (obstacleY // 10, obstacleX // 10, direction)
print(f"CURRENT POS: {currentPos}")
bot.setCurrentPosTask2(
    currentPos[0], currentPos[1], bot.get_current_pos().direction)
direction = []
obstacle1 = input("Arrow Direction: ")
if obstacle1 == "":
    while obstacle1 == "":
        obstacle1 = input("Arrow Direction: ")
direction.append(obstacle1)

obstacle2 = input("Second Arrow Direction: ")
if obstacle2 == "":
    while obstacle2 == "":
        obstacle2 = input("Second Arrow Direction: ")
direction.append(obstacle2)
sim = Simulation(direction)
sim.runTask2Simulation(bot)

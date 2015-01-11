package falstad;

public class BasicRobot implements Robot {

	public float energy = 2500;
	public int startx;
	public int starty;
	int[] currPos = new int[2];
	Cells mazeCells;
	Maze mazeMaze;
	
	public BasicRobot() {}
		
	public void setMaze(Maze maze) {
		mazeMaze = maze;
		mazeCells = maze.mazecells;
		startx = mazeMaze.px;
		starty = mazeMaze.py;
		currPos[0] = startx;
		currPos[1] = starty;
	}
	
	public Maze getMaze() {
		return mazeMaze;
	}
	
	/**
	 * Turn robot on the spot. If given degree is not supported by existing robot, method throws a corresponding exception. 
	 * For example, a robot may only be able to turn left (90) degrees or right (-90) degrees. The angle is measured in a counterclockwise manner
	 * as it is common for polar coordinates.
	 * @param degree specifies in which direction to turn (negative values turn right, positive values turn left), actual implementation may be limited to a few discrete value settings. 
	 * @throws UnsupportedArgumentException if the robot does not support a given degree value. 
	 */
	public void rotate(int degree) throws UnsupportedArgumentException {
		if (degree != 90 && degree != -90) {
			throw new UnsupportedArgumentException();
		}
		else {
			if (degree == -90) {
				mazeMaze.rotate(-1);
				energy -= 3;
			}
			else {
				mazeMaze.rotate(1);
				energy -= 3;
			}
		}
	}
	
	/**
	 * Moves robot forward or backward a given number of steps. A step matches a single cell.
	 * Since a robot may only have a distance sensor in its front, driving backwards may happen blindly as distance2Obstacle may not provide values for that direction.
	 * If the robot runs out of energy somewhere on its way, it stops, which can be checked by hasStopped() and by checking the battery level. 
	 * @param distance is the number of cells to move according to the robots current direction if forward = true, opposite direction if forward = false
	 * @param forward specifies if the robot should move forward (true) or backward (false)
	 * @throws HitObstacleException if robot hits an obstacle like a wall or border, which also make the robot stop, i.e. hasStopped() = true 
	 */
	public void move(int distance, boolean forward) throws HitObstacleException {
		for (int x = 0; x < distance; x ++) {
			System.out.println("X: "+ currPos[0] +" Y: " + currPos[1]);
			energy -= 5;
			if (!forward && !hasStopped()) {
				mazeMaze.walk(-1);
				currPos[0] = mazeMaze.px;
				currPos[1] = mazeMaze.py;
			}
			else if (forward && !hasStopped()) {
				mazeMaze.walk(1);
				currPos[0] = mazeMaze.px;
				currPos[1] = mazeMaze.py;
			}
		}
	}
	
	/**
	 * Provides the current position as (x,y) coordinates for the maze cell as an array of length 2 with [x,y].
	 * Note that 0 <= x < width, 0 <= y < height of the maze. 
	 * @return array of length 2, x = array[0], y=array[1]
	 */
	public int[] getCurrentPosition() {
		return currPos;
	}
	
	/**
	 * Tells if current position is at the goal. Used to recognize termination of a search.
	 * Note that goal recognition is limited by the sensing functionality of robot such that isAtGoal returns false
	 * even if it is positioned directly at the exit but has no distance sensor towards the exit direction. 
	 * @return true if robot is at the goal and has a distance sensor in the direction of the goal, false otherwise
	 */
	public boolean isAtGoal() {
		return mazeMaze.isEndPosition(mazeMaze.px, mazeMaze.py);
	}
	
	/**
	 * Provides the current direction as (dx,dy) values for the robot as an array of length 2 with [dx,dy].
	 * Note that dx,dy are elements of {-1,0,1} and, as in bitmasks, masks in Cells.java and dirsx,dirsy in MazeBuilder.java.
	 * 
	 * @return array of length 2, dx = array[0], dy=array[1]
	 */	
	public int[] getCurrentDirection() {
		int[] currDir = new int[2];
		currDir[0] = mazeMaze.dx;
		currDir[1] = mazeMaze.dy;
		return currDir;
	}
	
	/**
	 * The robot has a given battery level (energy level) that it draws energy from during operations. 
	 * The particular energy consumption is device dependent such that a call for distance2Obstacle may use less energy than a move forward operation.
	 * If battery level <= 0 then robot stops to function and hasStopped() is true.
	 * @return current battery level, level is > 0 if operational. 
	 */
	public float getCurrentBatteryLevel() {
		return energy;
	}
	
	/**
	 * Gives the energy consumption for a full 360 degree rotation.
	 * Scaling by other degrees approximates the corresponding consumption. 
	 * @return energy for a full rotation
	 */
	public float getEnergyForFullRotation() {
		return 12;
	}
	
	/**
	 * Gives the energy consumption for moving 1 step forward.
	 * For simplicity, we assume that this equals the energy necessary to move 1 step backwards and that scaling by a larger number of moves is 
	 * approximately the corresponding multiple.
	 * @return energy for a single step forward
	 */
	public float getEnergyForStepForward() {
		return 5;
	}
	
	/**
	 * Tells if the robot has stopped for reasons like lack of energy, hitting an obstacle, etc.
	 * @return true if the robot has stopped, false otherwise
	 */
	public boolean hasStopped() {
		if (energy <= 4) {
			return true;
		}
		try {
			if (distanceToObstacleAhead() == 0) {
				return true;
			}
		} catch (UnsupportedMethodException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return false;
	}
	
	/**
	 * Tells if a sensor can identify the goal in the robot's current forward direction from the current position.
	 * @return true if the goal (here: exit of the maze) is visible in a straight line of sight
	 * @throws UnsupportedMethodException if robot has no sensor in this direction
	 */
	public boolean canSeeGoalAhead() throws UnsupportedMethodException {
		int seeX = getCurrentPosition()[0];
		int seeY = getCurrentPosition()[1];
		boolean obstacle = false;
		while (energy > 0 && !obstacle && seeX > 0 && seeY > 0 && seeX < mazeCells.width && seeY < mazeCells.height) {
			if (mazeMaze.dx == 1 && mazeCells.hasWallOnRight(seeX, seeY)) {
				obstacle = true;
			}
			else if (mazeMaze.dx == 1 && !mazeCells.hasWallOnRight(seeX, seeY)) {
				seeX += 1;
				if (mazeMaze.isEndPosition(seeX, seeY) == true) {
					return true;
				}
			}
			else if (mazeMaze.dx == -1 && mazeCells.hasWallOnLeft(seeX, seeY)) {
				obstacle = true;
			}
			else if (mazeMaze.dx == -1 && !mazeCells.hasWallOnLeft(seeX, seeY)) {
				seeX -= 1;
				if (mazeMaze.isEndPosition(seeX, seeY) == true) {
					return true;
				}
			}
			else if (mazeMaze.dy == 1 && mazeCells.hasWallOnTop(seeX, seeY)) {
				obstacle = true;
			}
			else if (mazeMaze.dy == 1 && !mazeCells.hasWallOnTop(seeX, seeY)) {
				seeY += 1;
				if (mazeMaze.isEndPosition(seeX, seeY) == true) {
					return true;
				}
			}
			else if (mazeMaze.dy == -1 && mazeCells.hasWallOnBottom(seeX, seeY)) {
				obstacle = true;
			}
			else if (mazeMaze.dy == -1 && !mazeCells.hasWallOnBottom(seeX, seeY)) {
				seeY -= 1;
				if (mazeMaze.isEndPosition(seeX, seeY) == true) {
					return true;
				}
			}
		}
		energy =- 1;
		return false;
	}

	/**
	 * Methods analogous to canSeeGoalAhead but for the robot's current left direction (left relative to forward)
	 * @return true if the goal (here: exit of the maze) is visible in a straight line of sight
	 * @throws UnsupportedMethodException if robot has no sensor in this direction
	 */
	public boolean canSeeGoalOnLeft() throws UnsupportedMethodException {
		int seeX = getCurrentPosition()[0];
		int seeY = getCurrentPosition()[1];
		boolean obstacle = false;
		while (!obstacle && energy > 0 && seeX > 0 && seeY > 0 && seeX < mazeCells.width && seeY < mazeCells.height) {
			if (mazeMaze.dx == 1 && mazeCells.hasWallOnTop(seeX, seeY)) {
				obstacle = true;
			}
			else if (mazeMaze.dx == 1 && !mazeCells.hasWallOnTop(seeX, seeY)) {
				seeY += 1;
				if (mazeMaze.isEndPosition(seeX, seeY) == true) {
					return true;
				}
			}
			else if (mazeMaze.dx == -1 && mazeCells.hasWallOnBottom(seeX, seeY)) {
				obstacle = true;
			}
			else if (mazeMaze.dx == -1 && !mazeCells.hasWallOnBottom(seeX, seeY)) {
				seeY -= 1;
				if (mazeMaze.isEndPosition(seeX, seeY) == true) {
					return true;
				}
			}
			else if (mazeMaze.dy == 1 && mazeCells.hasWallOnLeft(seeX, seeY)) {
				obstacle = true;
			}
			else if (mazeMaze.dy == 1 && !mazeCells.hasWallOnLeft(seeX, seeY)) {
				seeX += 1;
				if (mazeMaze.isEndPosition(seeX, seeY) == true) {
					return true;
				}
			}
			else if (mazeMaze.dy == -1 && mazeCells.hasWallOnRight(seeX, seeY)) {
				obstacle = true;
			}
			else if (mazeMaze.dy == -1 && !mazeCells.hasWallOnRight(seeX, seeY)) {
				seeX -= 1;
				if (mazeMaze.isEndPosition(seeX, seeY) == true) {
					return true;
				}
			}
		}
		energy -= 1;
		return false;
	}

	/**
	 * Tells the distance to an obstacle (a wall or border) for a the robot's current forward direction.
	 * Distance is measured in the number of cells towards that obstacle, e.g. 0 if current cell has a wall in this direction
	 * @return number of steps towards obstacle if obstacle is visible in a straight line of sight, Integer.MAX_VALUE otherwise
	 * @throws UnsupportedArgumentException if not supported by robot
	 */
	public int distanceToObstacleAhead() throws UnsupportedMethodException {
		int seeX = getCurrentPosition()[0];
		int seeY = getCurrentPosition()[1];
		int distance = 0;
		boolean obstacle = false;
		while (!obstacle && energy > 0 && seeX >= 0 && seeY >= 0 && seeX < mazeCells.width && seeY < mazeCells.height) {
			if (mazeMaze.dy == -1 && mazeCells.hasWallOnTop(seeX, seeY)) {
				obstacle = true;
			}
			else if (mazeMaze.dy == -1)  {
				seeX += 1;
				distance += 1;
			}
			else if (mazeMaze.dy == 1 && mazeCells.hasWallOnBottom(seeX, seeY)) {
				obstacle = true;
			}
			else if (mazeMaze.dy == 1) {
				seeX -= 1;
				distance += 1;
			}
			else if (mazeMaze.dx == 1 && mazeCells.hasWallOnRight(seeX, seeY)) {
				obstacle = true;
			}
			else if (mazeMaze.dx == 1) {
				seeX += 1;
				distance += 1;
			}
			else if (mazeMaze.dx == -1 && mazeCells.hasWallOnLeft(seeX, seeY)) {
				obstacle = true;
			}
			else if (mazeMaze.dx == -1) {
				seeX -= 1;
				distance += 1;
			}
		}
		energy -= 1;
		return distance;
	}
	
	/**
	 * Methods analogous to distanceToObstacleAhead but for the robot's current left direction (left relative to forward)
	 * @return number of steps towards obstacle if obstacle is visible in a straight line of sight, Integer.MAX_VALUE otherwise
	 * @throws UnsupportedArgumentException if not supported by robot
	 */
	public int distanceToObstacleOnLeft() throws UnsupportedMethodException {
		int seeX = getCurrentPosition()[0];
		int seeY = getCurrentPosition()[1];
		int distance = 0;
		boolean obstacle = false;
		while (!obstacle && energy > 0 && seeX >= 0 && seeY >= 0 && seeX < mazeCells.width && seeY < mazeCells.height) {
			if (mazeMaze.dy == -1 && mazeCells.hasWallOnRight(seeX, seeY)) {
				obstacle = true;
			}
			else if (mazeMaze.dy == -1 && !mazeCells.hasWallOnRight(seeX, seeY))  {
				seeX += 1;
				distance += 1;
			}
			else if (mazeMaze.dy == 1 && mazeCells.hasWallOnLeft(seeX, seeY)) {
				obstacle = true;
			}
			else if (mazeMaze.dy == 1 && !mazeCells.hasWallOnLeft(seeX, seeY)) {
				seeX -= 1;
				distance += 1;
			}
			else if (mazeMaze.dx == 1 && mazeCells.hasWallOnBottom(seeX, seeY)) {
				obstacle = true;
			}
			else if (mazeMaze.dx == 1 && !mazeCells.hasWallOnBottom(seeX, seeY)) {
				seeY += 1;
				distance += 1;
			}
			else if (mazeMaze.dx == -1 && mazeCells.hasWallOnTop(seeX, seeY)) {
				obstacle = true;
			}
			else if (mazeMaze.dx == -1 && !mazeCells.hasWallOnTop(seeX, seeY)) {
				seeY -= 1;
				distance += 1;
			}
		}
		energy -= 1;
		return distance;
	}
	
	@Override
	public boolean canSeeGoalBehind() throws UnsupportedMethodException {
		throw new UnsupportedMethodException();
	}

	@Override
	public boolean canSeeGoalOnRight() throws UnsupportedMethodException {
		throw new UnsupportedMethodException();
	}

	@Override
	public int distanceToObstacleOnRight() throws UnsupportedMethodException {
		throw new UnsupportedMethodException();
	}

	@Override
	public int distanceToObstacleBehind() throws UnsupportedMethodException {
		throw new UnsupportedMethodException();
	}
}

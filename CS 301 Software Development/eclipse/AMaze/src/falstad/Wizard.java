package falstad;

public class Wizard implements RobotDriver {
	public int turns = 1;
	public int walks = 1;
	public int sights = 1;
	public Robot bot;
	public boolean solved = false;
	
	/**
	 * Assigns a robot platform to the driver. Not all robot configurations may be suitable such that the method 
	 * will throw an exception if the robot does not match minimal configuration requirements, e.g. providing a sensor
	 * to measure the distance to an object in a particular direction. 
	 * @param r robot to operate
	 * @throws UnsuitableRobotException if driver cannot operate the given robot
	 */
	public void setRobot(Robot r) throws UnsuitableRobotException {
		bot = r;
	}

	/**
	 * Drives the robot towards the exit given it exists and given the robot's energy supply lasts long enough. 
	 * @return true if driver successfully reaches the exit, false otherwise
	 * @throws exception if robot stopped due to an accident
	 */
	public boolean drive2Exit() throws Exception {
		Maze maze = bot.getMaze();
		maze.solveStep();
		return true;
	}
	
	/**
	 * Returns the total energy consumption of the journey
	 */
	public float getEnergyConsumption() {
		return (walks * 5) + (turns * 3) + sights;
	}
	
	/**
	 * Returns the total length of the journey in number of cells traversed. The initial position counts as 0. 
	 */
	public int getPathLength() {
		return walks;
	}

	@Override
	public boolean getSolved() {
		return solved;
	}
}

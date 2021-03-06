package falstad;

public class LosPreferences {

	public int difficulty = 0;
	boolean showWholeMaze;
	boolean showSolution;
	boolean showWalls;
	String mazeType;
	public String solverBot;
	
	public LosPreferences() {
		difficulty = 0;
		showWholeMaze = false;
		showSolution = false;
		showWalls = false;
		mazeType = "Falstad";
		solverBot = "Manual";
	}
	
	public void changeDifficulty(int hardness) {
		difficulty = hardness;
		System.out.println("difficulty changed to: " + hardness);
	}
	
	public void changeWholeMaze(boolean showMaze) {
		showWholeMaze = showMaze;
		System.out.println("ShowMaze set to: " + showMaze);
	}
	
	public void changeShowSolution(boolean showYellow) {
		showSolution = showYellow;
		System.out.println("ShowSolution set to: " + showYellow);
	}
	
	public void changeShowWalls(boolean showTheWalls) {
		showWalls = showTheWalls;
		System.out.println("ShowWalls set to: " + showTheWalls);
	}
	
	public void changeMazeType(String typeOfMaze) {
		mazeType = typeOfMaze;
		System.out.println("Type of maze set to: " + typeOfMaze);
	}
	
	public void changeSolverType(String botType) {
		solverBot = botType;
		System.out.println("Bot type set to: " + botType);
	}
}

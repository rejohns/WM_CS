package falstad;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JComboBox;

public class SolverListener implements ActionListener {
	public String solver;
	public Maze maze;
	
	public SolverListener(Maze maze) {
		this.maze = maze;
	}
	
	@Override
	public void actionPerformed(ActionEvent e) {
		JComboBox cb = (JComboBox)e.getSource();
	    solver = (String)cb.getSelectedItem();
	    if (solver == "Manual") {
	    	maze.botName = "Manual";
	    }
	    else if (solver == "Gambler") {
	    	maze.botName = "Gambler";
	    }
	    else if (solver == "Curious Gambler") {
	    	maze.botName = "Curious Gambler";
	    }
	    else if (solver == "Wall Follower") {
	    	maze.botName = "Wall Follower";
	    }
	    else {
	    	maze.botName = "Wizard";
	    }
	}

}

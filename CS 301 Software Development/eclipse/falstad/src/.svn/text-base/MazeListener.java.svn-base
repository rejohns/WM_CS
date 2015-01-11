package falstad;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JComboBox;

public class MazeListener implements ActionListener {
	public String mazeName;
	public Maze maze;

	public MazeListener(Maze maze) {
		this.maze = maze;
	}
	
	@Override
	public void actionPerformed(ActionEvent arg0) {
		  JComboBox cb = (JComboBox)arg0.getSource();
	      mazeName = (String)cb.getSelectedItem();
	      if (mazeName == "Falstad") {
	    	  maze.mazeName = "Falstad";
	      }
	      else {
	    	  maze.mazeName = "Prim";
	      }
	}

}

/**
 * 
 */
package falstad;

import java.awt.BorderLayout;
import java.awt.event.KeyListener;

import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JPanel;


/**
 * This class is a wrapper class to startup the Maze as a Java application
 * 
 *
 * This code is refactored code from Maze.java by Paul Falstad, www.falstad.com, Copyright (C) 1998, all rights reserved
 * Paul Falstad granted permission to modify and use code for teaching purposes.
 * Refactored by Peter Kemper
 */
public class MazeApplication extends JFrame {

	Maze maze ;
	KeyListener kl ;
	JComboBox mazeList;
	JComboBox solverList;
	
	/**
	 * Constructor
	 */
	public MazeApplication() {
		System.out.println("MazeApplication:init started, object is displayable? " + this.isDisplayable());

		
		this.setLayout(new BorderLayout());
		
		maze = new Maze() ;
		add(maze, BorderLayout.CENTER) ;
		
		kl = new SimpleKeyListener(this, maze) ;
		addKeyListener(kl) ;

		
		String[] mazeStrings = {"Falstad", "Prim"};
		String[] solverStrings = {"Manual", "Gambler", "Curious Gambler", "Wall Follower", "Wizard"};
		mazeList = new JComboBox(mazeStrings);
		mazeList.addActionListener(new MazeListener(maze));
		solverList = new JComboBox(solverStrings);
		solverList.addActionListener(new SolverListener(maze));
		
		JPanel panel = new JPanel();
		panel.setLayout(new BorderLayout());
		panel.add(mazeList, BorderLayout.EAST);
		panel.add(solverList, BorderLayout.CENTER);
		add(panel, BorderLayout.NORTH);
		
		setVisible(true) ;
		maze.init();
		System.out.println("Maze displayable?" + maze.isDisplayable());
		// pack() ;
		this.setSize(400, 420) ;
		maze.redraw() ;
		//repaint() ;
	}
	
	


	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		/*
		// quick test
		int[][] twoDimArray = { {1,2,3}, {4,5,6}, {7,8,9} };
		for (int i = 0 ; i < 3 ; i++)
		{
			for (int j = 0 ; j < 3 ; j++)
				System.out.println("(i,j,value):" + i + " " + j + " " + twoDimArray[i][j]);
		}
		*/
		// TODO Auto-generated method stub
		MazeApplication a = new MazeApplication() ;
		a.repaint() ;
	}

}

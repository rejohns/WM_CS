/**
 * 
 */
package falstad;


import java.awt.event.KeyListener;

import javax.swing.JFrame;


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

	/**
	 * Constructor
	 */
	public MazeApplication() {
		System.out.println("MazeApplication:init started, object is displayable? " + this.isDisplayable());

		maze = new Maze() ;
		add(maze) ;
		
		kl = new SimpleKeyListener(this, maze) ;
		addKeyListener(kl) ;
		
		setVisible(true) ;
		maze.init();
		System.out.println("Maze displayable?" + maze.isDisplayable());
		// pack() ;
		this.setSize(400, 400) ;
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

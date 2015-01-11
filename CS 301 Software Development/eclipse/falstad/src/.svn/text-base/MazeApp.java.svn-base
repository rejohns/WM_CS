/**
 * 
 */
package falstad;

import java.applet.Applet;
import java.awt.Graphics;
import java.awt.event.KeyListener;

/**
 * A wrapper class to operate Maze in an Applet
 * 
 * This code is refactored code from Maze.java by Paul Falstad, www.falstad.com, Copyright (C) 1998, all rights reserved
 * Paul Falstad granted permission to modify and use code for teaching purposes.
 * Refactored by Peter Kemper
 */
public class MazeApp extends Applet {

	Maze maze ;
	KeyListener kl ;

	/**
	 * init method
	 */
	public void init() {
		
		System.out.println("MazeApp:init started, object is displayable? " + this.isDisplayable());
		maze = new Maze() ;
		this.add(maze) ;

		kl = new SimpleKeyListener(this, maze) ;
		addKeyListener(kl) ;

		maze.init();
		this.setSize(400, 400) ;
		maze.redraw() ;
		System.out.println("Maze displayable?" + maze.isDisplayable());
	}
	/**
	 * start method
	 */
	public void start() {
		maze.start();
	}

	/**
	 * stop
	 */
	public void stop() {
	}
	
	/**
	 * overwrites Applet method
	 */
	public void update(Graphics g) {
		maze.update(g) ;
	} 

	/**
	 * overwrites Applet method
	 */
	public void paint(Graphics g) {
		maze.paint(g) ;
	}
	
	/**
	 * automatically generated version id
	 */
	private static final long serialVersionUID = -4387558742380364689L;

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		System.out.println("MazeApp: main: Not implemented yet");
	}


}

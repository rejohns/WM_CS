package falstad;

import java.awt.Container;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

/**
 * Class implements a wrapper for the user input handled by the Maze class. 
 * It is shared by MazeApplication and MazeApp to talk to the Maze.
 *
 * This code is refactored code from Maze.java by Paul Falstad, www.falstad.com, Copyright (C) 1998, all rights reserved
 * Paul Falstad granted permission to modify and use code for teaching purposes.
 * Refactored by Peter Kemper
 */
public class SimpleKeyListener implements KeyListener {

	Container parent ;
	Maze maze ;
	
	SimpleKeyListener(Container parent, Maze maze){
		this.parent = parent ;
		this.maze = maze ;
	}
	/**
	 * Translates keyboard input to the corresponding characters for the Maze.keyDown method
	 */
	@Override
	public void keyPressed(KeyEvent arg0) {
		// TODO Auto-generated method stub
		//System.out.println("1: Communicating key: " + arg0.getKeyText(arg0.getKeyCode()) + " with key char: " + arg0.getKeyChar() + " with code: " + arg0.getKeyCode());
		int key = arg0.getKeyChar() ;
		int code = arg0.getKeyCode() ;

		if (KeyEvent.CHAR_UNDEFINED == key)
		{
			if ((KeyEvent.VK_0 <= code && code <= KeyEvent.VK_9)||(KeyEvent.VK_A <= code && code <= KeyEvent.VK_Z))
				key = code ;
			if (KeyEvent.VK_ESCAPE == code)
				key = maze.ESCAPE ;
			if (KeyEvent.VK_UP == code)
				key = 'k' ;
			if (KeyEvent.VK_DOWN == code)
				key = 'j' ;
			if (KeyEvent.VK_LEFT == code)
				key = 'h' ;
			if (KeyEvent.VK_RIGHT == code)
				key = 'l' ;
		}
		//System.out.println("Calling keydown with " + key) ;
		maze.keyDown(null, key) ;
		parent.repaint() ;
	
	}
	@Override
	public void keyReleased(KeyEvent arg0) {
		// nothing to do
		
	}
	@Override
	public void keyTyped(KeyEvent arg0) {
		// NOTE FOR THIS TYPE OF EVENT IS getKeyCode always 0, so Escape etc is not recognized	
		// this is why we work with keyPressed
	}

}

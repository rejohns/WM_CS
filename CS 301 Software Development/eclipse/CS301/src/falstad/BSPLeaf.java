/**
 * 
 */
package falstad;

import java.util.Vector;

/**
 * 
 * This code is refactored code from Maze.java by Paul Falstad, www.falstad.com, Copyright (C) 1998, all rights reserved
 * Paul Falstad granted permission to modify and use code for teaching purposes.
 * Refactored by Peter Kemper
 */
public class BSPLeaf extends BSPNode {
	Vector<Seg> slist;

	void fix_bounds(int x, int y) {
		xl = Math.min(xl, x);
		yl = Math.min(yl, y);
		xu = Math.max(xu, x);
		yu = Math.max(yu, y);
	}

	/**
	 * Constructor
	 */
	BSPLeaf(Vector<Seg> sl) {
		slist = sl;
		xl = yl =  1000000;
		xu = yu = -1000000;
		isleaf = true;
		for (int i = 0; i != sl.size(); i++) {
			Seg se = (Seg) slist.elementAt(i);
			fix_bounds(se.x, se.y);
			fix_bounds(se.x + se.dx, se.y + se.dy);
		}
	}

}


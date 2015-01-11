package falstad;

import java.util.Vector;

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

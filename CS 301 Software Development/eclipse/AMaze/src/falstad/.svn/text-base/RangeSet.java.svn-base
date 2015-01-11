package falstad;

import java.util.Vector;

public class RangeSet {
private Vector<RangeSetElement> ranges;
	
	/**
	 * Constructor
	 */
	RangeSet() {
		ranges = new Vector<RangeSetElement>();
	}

	/**
	 * Tells if the set is empty.
	 * @return true if the set is empty, false otherwise
	 */
	public boolean isEmpty() {
		return ranges.isEmpty();
	}

	/**
	 * Clears the set and fills it with a single new element as specified
	 * @param mn minimum value for new element
	 * @param mx maximum value for new element
	 */
	public void set(int mn, int mx) {
		ranges.removeAllElements();
		ranges.addElement(new RangeSetElement(mn, mx));
	}

	/**
	 * Removes interval [fx,tx] from existing set such that none of its elements intersects with it anymore.
	 * Existing intervals are reduced if they intersect, split into two or fully removed if they are contained in [fx,tx]
	 * @param fx is the lower bound of the interval
	 * @param tx is the upper bound of the interval
	 */
	public void remove(int fx, int tx) {
		// make sure fx <= tx
		if (tx < fx) {
			int jj = tx;
			tx = fx;
			fx = jj;
		}
		// check all elements of the set for an overlap with interval [fx,tx]
		// assume that set is sorted in increasing order
		for (int i = 0; i != ranges.size(); i++) {
			RangeSetElement rse =
				(RangeSetElement) ranges.elementAt(i);
			if (rse.max < fx)
				continue; // rse is below [fx,tx]
			if (rse.min > tx)
				return;   // rse is above [fx,tx]
			if (fx <= rse.min) { 
				if (rse.max <= tx) { // rse is inside [fx,tx]
					ranges.removeElementAt(i--); // remove element, adjust index for iterating reduced set
					continue;
				}
				rse.min = tx+1; // else, rse and [fx,tx] overlap, truncate intersection from rse from below
				return;
			}
			if (fx <= rse.max && tx >= rse.max) { // rse.min < fx <= rse.max <= tx
				rse.max = fx-1; // rse and [fx,tx] overlap, truncate intersection from rse from above
				continue;
			}
			// if all conditions fail, then [fx,tx] lies inside the current interval
			// split interval into two: [rse.min,fx-1] and [tx+1,rse.max]
			RangeSetElement nrse = new RangeSetElement(rse.min, fx-1);
			ranges.insertElementAt(nrse, i);
			rse.min = tx+1;
			return;
		}
	}
	/**
	 * Tells if there is at least one interval that intersects with [p.x,p.y]. It modifies attributes of p in case of an intersection. 
	 * A point is used as a quick hack to communicate two integers and being able to return modified values in a boolean method.
	 * @param p describes interval [p.x,p.y], its entries are modified to represent the intersection if there is one
	 * @return true if there is an intersection, false otherwise
	 */
	public boolean intersect(int[] p) {
		// "p" isn't (strictly speaking) a point, but I need to return two
		// values here, and can't find a nicer way to do it.
		int min = p[0];
		int max = p[1];
		// assume that set is sorted in increasing order
		for (int i = 0; i != ranges.size(); i++) {
			RangeSetElement rse =
				(RangeSetElement) ranges.elementAt(i);
			if (rse.max < min) // current interval is below
				continue; 
			if (rse.min > max) // current interval is above
				return false;
			// at this point: min < rse.max and rse.min < max 
			if (rse.min > min) // current interval overlaps
				p[0] = rse.min;
			if (rse.max < max) // current interval is contained
				p[1] = rse.max;
			return true;
		}
		return false;
	}
}

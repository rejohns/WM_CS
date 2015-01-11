package falstad;

import java.util.Random;
import java.awt.Point;
import java.util.ArrayList;

public class MazeBuilderPrim extends MazeBuilder {

	public MazeBuilderPrim() {}
	
	public MazeBuilderPrim(boolean deterministic){
		if (true == deterministic)
		{
			random.setSeed(seed);
			// TODO: implement code that makes sure that if MazeBuilder.build is called for same skill level twice, same results
			// HINT: check http://download.oracle.com/javase/6/docs/api/java/util/Random.html
		}
	}
	
	public void generate() {
		cells.initialize();
		Random randomGenerator = new Random();
		int x = randomGenerator.nextInt(width);
		int firstX = x;
		int y = randomGenerator.nextInt(height);
		int firstY = y;
		int randInt;
		cells.setVirginToZero(x,y);
		ArrayList<Point> frontierArray = new ArrayList();
		ArrayList<Point> inArray = new ArrayList();
		Point nextPoint;
		int dx;
		int dy;
		inArray.add(new Point(firstX, firstY));
		if (firstX + 1 < width) {
			frontierArray.add(new Point(firstX + 1, firstY));
		}
		if (firstX - 1 > 0) {
			frontierArray.add(new Point(firstX - 1, firstY));
		}
		if (firstY + 1 < height) {
			frontierArray.add(new Point(firstX, firstY + 1));
		}
		if (firstY - 1 > 0) {
			frontierArray.add(new Point(firstX, firstY + 1));
		}
		while (frontierArray.size() > 0) {
			randInt = randomGenerator.nextInt(frontierArray.size());
			nextPoint = frontierArray.get(randInt);
			inArray.add(nextPoint);
			frontierArray.remove(randInt);
			if (nextPoint.x + 1 < width && ! inArray.contains(new Point(nextPoint.x + 1, nextPoint.y))) {
				frontierArray.add(new Point(nextPoint.x + 1, nextPoint.y));
			}
			if (nextPoint.x - 1 >= 0 && ! inArray.contains(new Point(nextPoint.x - 1, nextPoint.y))) {
				frontierArray.add(new Point(nextPoint.x - 1, nextPoint.y));
			}
			if (nextPoint.y + 1 < height && ! inArray.contains(new Point (nextPoint.x, nextPoint.y + 1))) {
				frontierArray.add(new Point(nextPoint.x, nextPoint.y + 1));
			}
			if (nextPoint.y - 1 >= 0 && ! inArray.contains(new Point (nextPoint.x, nextPoint.y - 1))) {
				frontierArray.add(new Point(nextPoint.x, nextPoint.y - 1));
			}
			for (int index = 0; index < inArray.size(); index ++) {
				if (nextPoint.x == inArray.get(index).x && nextPoint.y + 1 == inArray.get(index).y) {
					dx = 0;
					dy = 1;
					cells.deleteWall(inArray.get(index).x, inArray.get(index).y, -dx, -dy);
					cells.deleteWall(nextPoint.x, nextPoint.y, dx, dy);
					break;
				}
				else if (nextPoint.x == inArray.get(index).x && nextPoint.y - 1 == inArray.get(index).y) {
					dx = 0;
					dy = -1;
					cells.deleteWall(inArray.get(index).x, inArray.get(index).y, -dx, -dy);
					cells.deleteWall(nextPoint.x, nextPoint.y, dx, dy);
					break;
				}
				else if (nextPoint.x + 1 == inArray.get(index).x && nextPoint.y == inArray.get(index).y) {
					dx = 1;
					dy = 0;
					cells.deleteWall(inArray.get(index).x, inArray.get(index).y, -dx, -dy);
					cells.deleteWall(nextPoint.x, nextPoint.y, dx, dy);
					break;
				}
				else if (nextPoint.x - 1 == inArray.get(index).x && nextPoint.y == inArray.get(index).y) {
					dx = -1;
					dy = 0;
					cells.deleteWall(inArray.get(index).x, inArray.get(index).y, -dx, -dy);
					cells.deleteWall(nextPoint.x, nextPoint.y, dx, dy);
					break;
				}
			}
		}
		computeDists(width/2, height/2);
		
		// find most remote point in maze somewhere on the border
		int remotex = -1, remotey = -1, remotedist = 0;
		for (x = 0; x != width; x++) {
			if (dists[x][0] > remotedist) {
				remotex = x;
				remotey = 0;
				remotedist = dists[x][0];
			}
			if (dists[x][height-1] > remotedist) {
				remotex = x;
				remotey = height-1;
				remotedist = dists[x][height-1];
			}
		}
		for (y = 0; y != height; y++) {
			if (dists[0][y] > remotedist) {
				remotex = 0;
				remotey = y;
				remotedist = dists[0][y];
			}
			if (dists[width-1][y] > remotedist) {
				remotex = width-1;
				remotey = y;
				remotedist = dists[width-1][y];
			}
		}
	
		// recompute distances for an exit point (x,y) = (remotex,remotey)
		computeDists(remotex, remotey);
	
		// identify cell with the greatest distance
		setStartPositionToCellWithMaxDistance();
	
		// make exit position at true exit 
		setExitPosition(remotex, remotey);
	}
}

